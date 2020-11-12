/*
 *  Copyright(c) 2009-2011 Laika, developed under contract by mitchpraterLLC.
 *  All rights reserved unless otherwise granted. This program or documentation
 *  contains proprietary, confidential information and trade secrets of Laika
 *  and/or mitchpraterLLC. Use of copyright notice does not imply publication.
 *  
 *  By possessing this software, you agree to the following terms and conditions.
 *  You may not resell this software, but you may freely distribute it to others.
 *  You may use it for whatever purposes you wish. There are no warranties of any
 *  kind. You may make modifications, however any derivation may only be distributed
 *  to others under the same freedoms granted to you herein, it must retain this
 *  notice and agreement, and the modifications noted. We also ask that you make
 *  such modifications available to Laika, mitchpraterLLC, or to the computer
 *  graphics community as a whole through a public forum.
 */

/*
 *  Brdf Functions
 */
#ifndef BRDF_H
#define BRDF_H

#include <sl.h>
#include <math.h>
#include <color.h>

/*
 *  The basic 1970's era diffuse response function.
 *  Not much use these days, but included for completeness.
 */
float brdfLambert (float NdotL)
{
    return max(0, NdotL);
}

float brdfLambert (
    vector  Nn;     // normalized surface normal
    vector  Ln      // normalized light direction
)
{
    return brdfLambert (Nn.Ln);
}


/*
 *  Simple Cosine Lobe function.
 *
 *  The lobe shaping exponent: 0 < expon
 *  expon > 1 more tightly focuses the lobe shape, producing a more
 *  specular type of response.
 *  expon < 1 broadens the response, increasing the brightness and
 *  flattening the center of the lobe shape, steepening its edges.
 */
float brdfCosineLobe
(
    float   RdotL;  // R dot L
    float   expon   // lobe shaping exponent
                    // equivalent to above with range = 1
)
{
    float   response;

    response = max (0, RdotL);
    response = pow (response, expon);

    return response;
}

float brdfCosineLobe
(
    vector  Rn;     // normalized surface reflection direction
    vector  Ln;     // normalized light direction
    float   expon   // lobe shaping exponent
)
{
    return brdfCosineLobe (Rn.Ln, expon);
}


/*
 *  Use correct step location in mathEdge based on RENDER_LINEAR.
 */
float brdfEdge (float x, t)
{
#ifdef RENDER_LINEAR
    return mathEdge (x, .218, t);
#else
    return mathEdge (x, .5, t);
#endif
}


/*
 *  Generalized Cosine Lobe functions corrected for RENDER_LINEAR.
 */
float brdfGCL (float dot, Exponent)
{
    return colorSRGBToLinear (mathGCL (dot, Exponent));
}

float brdfGCL (float dot, Range, Exponent)
{
    return colorSRGBToLinear (mathGCL (dot, Range, Exponent));
}

float brdfGCL (float dot, Size, Range, Exponent)
{
    return colorSRGBToLinear (mathGCL (dot, Size, Range, Exponent));
}

float brdfGCL (vector S, L; float Exponent)
{
    return colorSRGBToLinear (mathGCL (S, L, Exponent));
}

float brdfGCL (vector S, L; float Range, Exponent)
{
    return colorSRGBToLinear (mathGCL (S, L, Range, Exponent));
}

float brdfGCL (vector S, L; float Size, Range, Exponent)
{
    return colorSRGBToLinear (mathGCL (S, L, Size, Range, Exponent));
}


/*
 *  Oren-Nayar brdf functions. Oren and Nayar's generalization of Lambert's
 *  reflection model. The roughness parameter gives the standard deviation of
 *  angle orientations of the presumed surface grooves.  When roughness=0, the
 *  model is identical to Lambertian reflection. This model is useful for very
 *  rough surfaces such as clay. It's main characteristic is a retro-reflective
 *  diffuse lobe.
 */
struct BrdfOrenNayarCtx
{
    varying float A = 0;
    varying float B = 0;
    varying float theta_r = 0; // Angle between V and N
    varying vector VperpN = vector(0); // Part of V perpendicular to N
};

void brdfOrenNayarPrep(float rough; vector Vn; normal Nn; output BrdfOrenNayarCtx ctx)
{
    // Surface roughness coefficients for Oren/Nayar's formula
    float sigma2 = rough * rough;
    ctx->A = 1 - 0.5 * sigma2 / (sigma2 + 0.33);
    ctx->B = 0.45 * sigma2 / (sigma2 + 0.09);

    // Useful precomputed quantities
    ctx->theta_r = acos(Vn . Nn);
    ctx->VperpN = normalize(Vn-Nn*(Vn.Nn));
}

float brdfOrenNayar(float rough; vector Vn; normal Nn; vector Ln; BrdfOrenNayarCtx ctx)
{
    float cos_theta_i = Ln . Nn;
    float cos_phi_diff = ctx->VperpN . normalize(Ln - Nn*cos_theta_i);
    float theta_i = acos(cos_theta_i);
    float alpha = max(theta_i, ctx->theta_r);
    float beta = min(theta_i, ctx->theta_r);
    return cos_theta_i * (ctx->A + ctx->B * max(0, cos_phi_diff) * sin(alpha) * tan(beta));
}


//-----------------------------------------------------------------------
//  Fresnel response functions. These have to do with the behavior of
//  light as it transitions from one medium into another.
//
//  All materials have a characteristic index of refraction > 1.
//  The index of refraction of a vacuum is defined as 1.
//  Air has a slightly higher index, approx. 1.00029, but as a practical
//  matter is assumed to be 1.
//
//  n1 refers to the index of refraction (IOR, or refractive index, RI) of
//  the material the light comes from (e.g. the material the camera is in),
//  and n2 is the IOR of the material it transitions into. n1 is typically
//  air (n1=1).
//
//  eta is the ratio n1/n2.
//
//  Note that all this is wavelength and polarization dependent, and RI
//  is really a complex number, so even the "full" calculation done by
//  the fresnel() function makes significant simplifying assumptions.
//  Also, there are in fact, materials with RI < 1, since it's the phase
//  velocity of light in the material that matters, not the signal
//  velocity (which cannot exceed the speed of light). Not that any
//  of that matters. It's just to point out that we don't really care
//  all that much about absolute accuracy. It just needs to look
//  believable.
//
//  Also note that the fresnel() function returns a value for Kt that is
//  not equal to 1-Kr. Its value is based on the amount of beam spread that
//  takes place upon refracting into the second material. Not that that
//  is used here, it's just FYI in case you use fresnel() someplace else.
//  You'll probably almost always want to use 1-Kr for the Kt value.
//-----------------------------------------------------------------------

float brdfFresnelKr
(
    vector  In; // normalized incident direction
    normal  Nn; // normalized surface normal
    float   eta // index of refraction coefficient
)
{
    float   Kr, Kt;

    fresnel (In, Nn, eta, Kr, Kt);

    return Kr;
}

float brdfFresnelKr
(
    vector  In;     // normalized incident direction
    normal  Nn;     // normalized surface normal
    float   eta;    // index of refraction coefficient determines shape of response
    float   minKr   // minimum value to return for Kr, allowing for creative control
)
{
    float   Kr, Kt;
    float   intensity;

    fresnel (In, Nn, eta, Kr, Kt);

    if (minKr == 1)
        intensity = Kr;
    else
        intensity = mix (minKr, 1, linear (mathSquare ((1/eta - 1)/(1/eta + 1)), 1, Kr));

    return intensity;
}

//-----------------------------------------------------------------------
//  Potentially faster computational approximation to the Fresnel reflection
//  term. This is certainly faster for GPU shaders; but probably not for prman.
//
//  The higher the index of refraction of n2 relative to n1,
//  the better the approximation. n1 is typically 1 (for air).
//
//  This function simply rescales the pow5 cosine curve so its minimum
//  is a function of n1 and n2. A more direct means of control is to
//  simply specify the minimum value itself.
//-----------------------------------------------------------------------
float brdfFastKr
(
    float   VdotN;  // V dot N
    float   n1;     // index of refraction first material
    float   n2      // index of refraction second material
)
{
    float   r0;

    r0 = (n1 - n2)/(n1 + n2);
    r0 *= r0;

    float   pow5VdotN;

    pow5VdotN = pow (1-VdotN, 5);

    return (r0 + (1-r0)*pow5VdotN);
}

float brdfFastKr
(
    vector  Vn; // normalized view direction
    normal  Nn; // normalized surface normal
    float   n1; // index of refraction first material
    float   n2  // index of refraction second material
)
{
    return brdfFastKr (Vn.Nn, n1, n2);
}

float brdfFastKr
(
    float   VdotN;  // V dot N
    float   minKr   // minimum function value [0,1]
)
{
    float   pow5VdotN;

    pow5VdotN = pow (1-VdotN, 5);

    return (minKr + (1-minKr)*pow5VdotN);
}


float brdfFastKr
(
    vector  Vn;     // normalized view direction
    normal  Nn;     // normalized surface normal
    float   minKr   // minimum function value
)
{
    return brdfFastKr (Vn.Nn, minKr);
}

//-----------------------------------------------------------------------
//  Fresnel-like response based on facing ratio for easy control of transition
//-----------------------------------------------------------------------

float brdfFaceKr
(
    float VdotN;    // V dot N
    float min;      // value when facing normal
    float max;      // value when normal is perpendicular
    float exp       // exponent of falloff between min and max
)
{
    return mix(min, max, pow(1-max(0, VdotN), exp));
}

float brdfFaceKr
(
    vector Vn;      // normalized view vector
    normal Nn;      // normalized surface normal
    float min;      // value when facing normal
    float max;      // value when normal is perpendicular
    float exp       // exponent of falloff between min and max
)
{
    return brdfFaceKr(Vn.Nn, min, max, exp);
}

//-----------------------------------------------------------------------
//  Rim response functions.
//
//  brdfRim uses the cosine lobe response curve, but repositioned
//  to the surface's silhouette edge.
//-----------------------------------------------------------------------
float brdfRim
(
    float   VdotN;  // V dot N. +1 when surface is facing the camera, 0 when it's perpendicular.
    float   expon   // shaping exponent
)
{
    return pow (1-VdotN, expon);
}

float brdfRim
(
    vector  Vn;     // normalized view direction
    normal  Nn;     // normalized surface normal
    float   expon   // shaping exponent
)
{
    return brdfRim (Vn.Nn, expon);
}

//-----------------------------------------------------------------------
//  The range control provided here turns out to be fairly useless,
//  so the simpler expon only functions above should be used instead.
//
//  I include these here in spite of that, just to document the fact,
//  and so that no one would decide they might be useful and then waste
//  time creating them.
//-----------------------------------------------------------------------
float brdfRim
(
    float   VdotN;  // V dot N. +1 when surface is facing the camera, 0 when it's perpendicular.
    float   range;  // range (0,2]
    float   expon   // shaping exponent
)
{
    float   response;

    response = (VdotN - 1)/(2-range) + 1;
    response = pow (1-response, expon);

    return (response);
}

float brdfRim
(
    vector  Vn;     // normalized view direction
    normal  Nn;     // normalized surface normal
    float   range;  // range
    float   expon   // shaping exponent
)
{
    return brdfRim (Vn.Nn, range, expon);
}

#endif
