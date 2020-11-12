/*
 *  Copyright(c) 2010-2011 Laika, developed under contract by mitchpraterLLC.
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
 *  Some of these functions are re-created from ideas developed while at Pixar.
 */

//
//  Math Functions and Constants
//
#ifndef MATH_H
#define MATH_H

#include <filter.h>

//---------------------------------------
//  Constants
//---------------------------------------
#define MATH_PI         3.141592653589793
#define MATH_INVPI      0.318309886183791
#define MATH_INVPISQ    0.101321183642
#define MATH_TWOPI      6.283185307179586
#define MATH_INVTWOPI   0.159154943091895
#define MATH_HALFPI     1.570796326794897
#define MATH_SQRT2      1.414213562373095
#define MATH_INVSQRT2   0.707106781186547
#define MATH_SQRTTWOPI  2.506628274631

//---------------------------------------
//  Exponentiation
//---------------------------------------
float   mathSquare (float x) { return (x*x); }

//---------------------------------------
//  Exterior Reflection function.
//  Unlike the rsl reflect() function, this
//  doesn't produce a forward-facing reflection
//  vector on the inside of surfaces:
//  exterior (toward N) reflection only.
//---------------------------------------
vector  mathExteriorReflect (vector In, Nn) {
    return (In - 2*min(In.Nn,0)*Nn);
}

/*
 *  Edge function.
 *
 *  Used to continuously vary the hardening of
 *  the transition between 0 and 1 in a varying function.
 *
 *  x contains the value to be hardened: a 0 to 1 ranged function.
 *  e is the edge step location: typically .5.
 *  0 <= t <= 1 determines how much of the Edge to apply to x.
 */
float mathEdge (float x, e, t)
{
    float   dx = max (1 - mix(.3, 1, t), filterWidth(x));
    float   edge = filterstep (e, x, x+dx);

    return x * mix (1, edge, smoothstep (0, .1, t));
}

/*
 *  Generalized Cosine Lobe function.
 *  GCL controls the shape of a cosine response, allowing
 *  area light effects to be created by simple non-area light sources.
 *
 *  dot     - the original cosine lobe: usually the dot product of the light and surface response directions.
 *  Size    - the size of the light source (0 <= Size <= 2).
 *  Range   - the surface area that will respond to illumination (0 <= Range <= 2).
 *  Exponent - the "focusing" exponent (0 < Exponent).
 *
 *  Size = 1 produces a distant light response.
 *  Values less than one produce more focused responses.
 *  Values greater than one produce more broad, area light responses.
 *  Size = 2 equates to a hemispherical area light source.
 *
 *  Range = 1 produces a Lambert surface response.
 *  Values less than one contract that range.
 *  Values greater than one expand it.
 *
 *  Vector values must be normalized.
 *  Note that Size and Range are interchangable.
 */
float mathGCL (float dot, Exponent)
{
    return pow (max (0, dot), Exponent);
}

float mathGCL (float dot, Range, Exponent)
{
    return pow (max (0, (dot-1)/Range + 1), Exponent);
}

float mathGCL (float dot, Size, Range, Exponent)
{
    return pow (max (0, (dot-1)/(Size+Range-1) + 1), Exponent);
}

float mathGCL (vector S, L; float Exponent)
{
    return pow (max (0, S.L), Exponent);
}

float mathGCL (vector S, L; float Range, Exponent)
{
    return pow (max (0, (S.L-1)/Range + 1), Exponent);
}

float mathGCL (vector S, L; float Size, Range, Exponent)
{
    return pow (max (0, (S.L-1)/(Size+Range-1) + 1), Exponent);
}

/*
 *  Rational Spline function.
 *
 *  The spline shape is controlled with the x parameter: 0 < x < 1.
 *  The spline produces a value of 0 when t<=0, .5 when t=x, and 1 when t>=1.
 *  The spline has 0 slope at t<=0 and t>=1.
 */
float mathRationalSpline (float x, t)
{
    if (t <= 0) return 0;
    if (t >= 1) return 1;

    float   tt = t*t;
    float   xx = x*x;

    return  tt*xx/2 / ((xx - x + .5)*tt - (t - .5)*xx);
}

/*
 *  Linear Spline function.
 *
 *  Produces a linear spline with parabolic shaped shoulders at the ends.
 *  The shoulder widths are controlled by a and b, which define the
 *  percentage each shoulder occupies: a+b <= 1.
 *  The spline produces a value of 0 when t<=0, and 1 when t>=1.
 *  The spline has 0 slope at t<=0 and t>=1.
 */
float mathLinearSpline (float a, b, t)
{
    if (t <= 0) return 0;
    if (t >= 1) return 1;

    float   x = 2 - a - b;
    if (t < a) return t*t / (a*x);
    if (t > 1-b) return 1 - mathSquare(1-t)/(b*x);

    return (2*t - a) / x;
}

color mathLinearSpline (float a, b; color t) {
    return color (
        mathLinearSpline(a,b,t[0]),
        mathLinearSpline(a,b,t[1]),
        mathLinearSpline(a,b,t[2])
    );
}

point mathLinearSpline (float a, b; point t) {
    return point (
        mathLinearSpline(a,b,t[0]),
        mathLinearSpline(a,b,t[1]),
        mathLinearSpline(a,b,t[2])
    );
}

vector mathLinearSpline (float a, b; vector t) {
    return vector (
        mathLinearSpline(a,b,t[0]),
        mathLinearSpline(a,b,t[1]),
        mathLinearSpline(a,b,t[2])
    );
}

/*
 *  Gaussian Curvature.
 *
 *  Computes the Gaussian curvature of the surface. The returned
 *  value is resolution and shadingrate independent and generally ranges
 *  between +/-100 or so. To see it as a grey value, do something like
 *
 *      Ci = smoothstep (-100, 100, mathGaussianCurvatureStd());
 *
 *  Note that the curvature returned for a cylinder will be 0.0.
 *  If you want non-zero for a cylinder, use Prater curvature.
 */
float   mathGaussianCurvature (vector dpdu, dpdv; normal n)
{
    varying vector  ndPdu, ndPdv;
    varying normal  Nn;

    varying vector  DudPdu, DvdPdv, DudPdv;
    varying float   K;


    ndPdu = normalize (dpdu);   /* resolution independence */
    ndPdv = normalize (dpdv);
    DudPdu = Du (ndPdu);        /* curvature */
    DvdPdv = Dv (ndPdv);
    DudPdv = Du (ndPdv);        /* twist? */
    Nn = normalize (n);         /* dPdu^dPdv */

    /* Gaussian curvature */
    K = (DudPdu.Nn*DvdPdv.Nn - mathSquare(DudPdv.Nn)) / (ndPdu.ndPdu*ndPdv.ndPdv - mathSquare(ndPdu.ndPdv));

    return K;
}

/*
 *  Standard function for determining Gaussian curvature of a surface.
 *  Takes no arguments because it always looks at dPdu, dPdv, and N.
 */
float   mathGaussianCurvatureStd()
{
    extern vector   dPdu, dPdv;
    extern normal   N;

    return mathGaussianCurvature (dPdu, dPdv, N);
}

/*
 *  Prater Curvature.
 *
 *  Computes the Prater curvature of the surface. Will give better
 *  results than Gaussian curvature in most cases. The returned
 *  value is resolution and shadingrate independent and generally ranges
 *  between +/-30 or so. To see it as a grey value, do something like
 *
 *      Ci = smoothstep (-30, 30, mathPraterCurvatureStd());
 *
 *  Produces 0 for a flat plane, as well as for saddle points.
 */
float   mathPraterCurvature (vector dpdu, dpdv; normal n)
{
    varying vector  ddPdu, ddPdv;
    varying float   K;


    ddPdv = Dv (normalize (dpdv));
    K = sign (ddPdv.n)*length (ddPdv);

    ddPdu = Du (normalize (dpdu));
    K += sign (ddPdu.n)*length (ddPdu);

    return -K;
}

/*
 *  Standard function for determining the Prater curvature of a surface.
 *  Takes no arguments because it always looks at dPdu, dPdv, and N.
 */
float   mathPraterCurvatureStd()
{
    extern vector   dPdu, dPdv;
    extern normal   N;

    return mathPraterCurvature (dPdu, dPdv, N);
}

/*
 *  Gaussian distribution function.
 */
float mathGaussian(float x; float sd)
{
    return 1/(MATH_SQRTTWOPI*sd) * exp(-(x*x)/(2*sd*sd));
}

/*
 * What's the volume of a cosine lobe raised to a power?
 * Exponent range .5 to 128.
 */
float mathCosineLobeVolume (float Exponent) {
    float   spline_index, result;

    spline_index = spline ("solvecatmull-rom", Exponent, .5, .5, 1, 2, 4, 8, 16, 32, 64, 128, 128);
    result = spline ("catmull-rom", spline_index, .8377581050, .8377581050, .5235987755, .2991993003, .1611073154, .08377580382, .04274275677, .02159170126, .01085178647, .005439984043, .005439984043);

/*     XXX It's not the volume we want, but the surface area: */
/*     result = math_TWOPI / (Exponent + 2.0); */
/*     But, this function works better to match samples with lobe size. Go figure. */

    return result;
}

/*
 * What's the normalized cosine lobe volume?
 * Normalized so Exponent=1 produces a unit volume.
 */
float mathNormalizedCosineLobeVolume (float Exponent) {
    return (mathCosineLobeVolume(Exponent) / 0.5235987755);
}

#endif
