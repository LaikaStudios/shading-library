/*
 *  Copyright(c) 2009-2014 Laika, developed under contract by mitchpraterLLC.
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
 *  Ray generation functions for ray traced scene sampling.
 */

#ifndef ray_h
#define ray_h

#include <sl.h>
#include <math.h>

/*
 *  Non-stratified sample distributions.
 */
void rayGenCosineSamples (
    vector  axis;
    float   coneangle;
    uniform float   nsamples;
    output vector   R[];
) {
    float   theta, phi, sinphi, angle;
    vector  rotAxis;

    rotAxis = vector(0,0,1)^axis;
    angle = acos(vector(0,0,1).axis);

    uniform float   i;
    for (i=0; i<nsamples; i+=1) {
        theta = MATH_TWOPI*random();
        phi = coneangle*random();
        sinphi = sin(phi);
        R[i] = vector (sinphi*cos(theta), sinphi*sin(theta), cos(phi));
        R[i] = _rotate (R[i], angle, rotAxis);
    }
}

void rayGenUniformSamples (
    vector  axis;
    float   coneangle;
    uniform float   nsamples;
    output vector   R[];
) {
    float   zmin, z, scale, theta, angle;
    vector  rotAxis;

    rotAxis = vector(0,0,1)^axis;
    angle = acos(vector(0,0,1).axis);
    zmin = cos(coneangle);

    uniform float   i;
    for (i=0; i<nsamples; i+=1) {
        theta = MATH_TWOPI*random();
        z = mix (zmin, 1, random());
        scale = sqrt (1 - z*z);
        R[i] = vector (scale*cos(theta), scale*sin(theta), z);
        R[i] = _rotate (R[i], angle, rotAxis);
    }
}


/*
 *  Gaussian distributed samples. Used for Ward-glossy reflections.
 *  ***Not tested***
 */
void rayGenGaussianSamples (
    float samples, alpha;
    vector I;
    vector axis, dPdu, dPdv;
    output vector R[];
) {
    vector ndir = vector(axis), udir = normalize(dPdu), vdir;
    vector In = normalize(I), IinPlane, hdir;
    uniform float s;
    float delta, phi, r; // (delta is angle from the "up" axis)
    float u, v, n;

    // Create an orthonormal basis.
    udir = udir - (axis.udir) * axis; // project udir onto plane perp. to normal.
    normalize(udir);
    vdir = axis ^ udir;

    // Compute directions.
    for (s = 0; s < samples; s += 1) {
        // Compute (delta,phi) specifying the half vector h.
        delta = atan(alpha * sqrt(-log(random())));
        phi = MATH_TWOPI * random();

        // Compute half vector h.
        n = cos(delta);
        r = sin(delta);
        u = r * cos(phi);
        v = r * sin(phi);
        hdir = u * udir + v * vdir + n * ndir;

    // Convert to a reflection direction.
        R[s] = reflect(In, hdir);
    }
}

/*
 *  Ashikhmin-Shirley Anisotropic reflection.
 */

// Generate quadrant stratified polar angles.
void rayGenASQuadrant(
        varying float   pow10TanExp;
        varying float   pow10BitExp;
        varying vector  rn;

        output varying float    cosTheta;
        output varying float    cosPhi;
        output varying float    sinTheta;
        output varying float    sinPhi;
) {
    varying float   phi;
    varying float   expon;

    phi = atan( sqrt(pow10TanExp / pow10BitExp) * tan( MATH_HALFPI*rn[0] ));  // Good for any values of Exp >= 1.

    cosPhi = cos( phi );
    sinPhi = sin( phi );

    expon = 1.0 / ( pow10TanExp * cosPhi*cosPhi + pow10BitExp * sinPhi*sinPhi );

    cosTheta = pow( 1.0 - rn[1], expon );
    sinTheta = sqrt( 1.0 - cosTheta*cosTheta );
}

//  Given a surface normal coordinate frame and a viewer direction,
//  generates a quadrant-stratified, Ashikhmin-Shirley reflection sample.
void rayGenAS(
        varying vector  V;  // Observer: direction toward camera.
        varying vector  Normal;
        varying vector  Tangent;
        varying vector  Bitangent;
        varying float   pow10TanExp;
        varying float   pow10BitExp;

        output varying vector   L;      // Sample direction (toward the light).
        output varying float    pdf;    // Sample pdf.
) {
    // Random numbers.
    varying vector  rn = random();

    // Generate quadrant-stratified anisotropic sample angles.
    float   cosTheta, sinTheta, cosPhi, sinPhi;

    if (rn[0] < 0.25)
    {
        rn[0] *= 4.0;
        rayGenASQuadrant( pow10TanExp, pow10BitExp, rn, cosTheta, cosPhi, sinTheta, sinPhi );
    }
    else if (rn[0] < 0.5)
    {
        rn[0] = 4.0 * (0.5 - rn[0]);
        rayGenASQuadrant( pow10TanExp, pow10BitExp, rn, cosTheta, cosPhi, sinTheta, sinPhi );
        cosPhi = -cosPhi;
    }
    else if (rn[0] < 0.75)
    {
        rn[0] = 4.0 * (rn[0] - 0.5);
        rayGenASQuadrant( pow10TanExp, pow10BitExp, rn, cosTheta, cosPhi, sinTheta, sinPhi );
        cosPhi = -cosPhi;
        sinPhi = -sinPhi;
    }
    else
    {
        rn[0] = 4.0 * (1.0 - rn[0]);
        rayGenASQuadrant( pow10TanExp, pow10BitExp, rn, cosTheta, cosPhi, sinTheta, sinPhi );
        sinPhi = -sinPhi;
    }

    // Perturbed surface normal: reflection half-angle vector H (between Observer V and Incident light L).
    varying vector  H = cosPhi * sinTheta*Tangent + sinPhi * sinTheta*Bitangent + cosTheta*Normal;

    L = reflect( -V, H );   // Incident vector: direction toward light.

    // PDF
    varying float   HdotV = H.V;

    varying float   NdotH = cosTheta;
    varying float   NdotL = Normal.L;

    varying float   expon = pow10TanExp * cosPhi*cosPhi + pow10BitExp * sinPhi*sinPhi;

    pdf = sqrt( pow10TanExp * pow10BitExp ) * pow( NdotH, expon ) / ( 4.0*MATH_TWOPI * HdotV );

    // -pdf due to HdotV < 0: H normal points away from V and therefore can't reflect it.
    pdf = max( 0.0, pdf );

    varying float   NdotV = Normal.V;

    pdf /= max( NdotV, NdotL );
}

//  Given a surface normal coordinate frame and a viewer direction,
//  generates an Ashikhmin-Shirley distributed reflection sample
//  based on the given random number values.
void rayGenAS(
        varying vector  V;      // Observer: direction toward camera.
        varying vector  Normal;
        varying vector  Tangent;
        varying vector  Bitangent;
        varying float   pow10TanExp;
        varying float   pow10BitExp;
        varying point   srn;    // Ideally, this is from randomstrat().

        output varying vector   L;      // Sample direction (toward the light).
        output varying float    pdf;    // Sample pdf.
) {
    // Generate sample direction polar angles.
    varying float   phi, cosPhi, sinPhi;
    varying float   expon, cosTheta, sinTheta;

    phi = atan( sqrt(pow10TanExp / pow10BitExp) * tan( MATH_TWOPI*srn[0] ));  // Good for any values of Exp >= 1.

    cosPhi = cos( phi );
    sinPhi = sin( phi );

    expon = pow10TanExp * cosPhi*cosPhi + pow10BitExp * sinPhi*sinPhi;

    cosTheta = pow( 1.0 - srn[1], 1.0/expon );
    sinTheta = sqrt( 1.0 - cosTheta*cosTheta );

    // Perturbed surface normal: reflection half-angle vector H (between Observer V and Incident light L).
    varying vector  H = cosPhi * sinTheta*Tangent + sinPhi * sinTheta*Bitangent + cosTheta*Normal;

    L = reflect( -V, H );   // Incident vector: direction toward light.

    // PDF
    varying float   HdotV = H.V;
    varying float   HdotN = cosTheta;

    pdf = sqrt( pow10TanExp * pow10BitExp ) * pow( HdotN, expon ) / ( 4.0*MATH_TWOPI * HdotV );
    pdf = max( 0.0, pdf );  // -pdf due to HdotV < 0: H normal points away from V and therefore can't reflect it.

    varying float   NdotV = Normal.V;
    varying float   NdotL = Normal.L;

    pdf /= max( NdotV, NdotL );
}

void rayEvalAS(
        varying vector  V;      // Observer: direction toward camera.
        varying vector  Normal;
        varying vector  Tangent;
        varying vector  Bitangent;
        varying float   pow10TanExp;
        varying float   pow10BitExp;

        varying vector   L;      // Sample direction (toward the light).
        output varying float    pdf;    // Sample pdf.
) {
}


/*
 *  Derived from the example provided Pixar's application note:
 *  A Tour of Ray-Traced Shading in PRMan.
 */
void rayGenStratCosineSamples (
    vector          axis;
    float           coneangle;
    uniform float   nsamples;

    output vector   R[];
) {
    uniform float   i, rs, ps;
    uniform float   rstrata, phistrata, samples;
    varying float   r, phi;

    float   angle, sinecone;
    vector  rotAxis;

    rotAxis = vector(0,0,1)^axis;
    angle = acos(vector(0,0,1).axis);
    sinecone = sin(coneangle);

    // Compute the number of strata in r and phi directions.
    // There are roughly four times as many phistrata as rstrata.
    rstrata = floor(sqrt(nsamples)/2);
    phistrata = floor(nsamples/rstrata);

    // Generate stratified directions.
    // Strata are uniform in r and phi,
    // which will produce cosine distributed strata on the sphere.
    i = 0;
    for (rs = 0; rs < rstrata; rs += 1) {
    for (ps = 0; ps < phistrata; ps += 1) {
        r = sinecone*((rs + random()) / rstrata);
        phi = MATH_TWOPI*((ps + random()) / phistrata);

        R[i] = vector (r*cos(phi),r*sin(phi),sqrt(1-r*r));
        R[i] = _rotate (R[i], angle, rotAxis);
        i += 1;
    }
    }

    // Generate stratified directions only in phi for the remaining samples.
    samples = nsamples - rstrata*phistrata;
    for (ps = 0; ps < samples; ps += 1) {
        r = sinecone*random();
        phi = MATH_TWOPI*((ps + random()) / samples);

        R[i] = vector (r*cos(phi),r*sin(phi),sqrt(1-r*r));
        R[i] = _rotate (R[i], angle, rotAxis);
        i += 1;
    }
}

/*
 *  A stratified, uniform distribution.
 *  Basic method is from http://eqsp.sourceforge.net
 *  modified to produce rays within the specified cone.
 */
void rayGenStratUniformSamples (
    vector          axis;
    float           coneangle;
    uniform float   nsamples;

    output vector   R[];
) {
    vector  rotAxis = vector(0,0,1)^axis;
    float   angle = acos(vector(0,0,1).axis);

    float   coneArea = MATH_TWOPI*(1-cos(coneangle));
    float   sampleArea = coneArea / nsamples;
    float   sampleEdge = sqrt(sampleArea);
    float   capAngle = acos(1 - sampleArea/MATH_TWOPI); // capArea == sampleArea.
    float   collarAngle = coneangle - capAngle;
    float   nCollars = max(1, round(collarAngle / sampleEdge));

    collarAngle = collarAngle / nCollars;

    float   phiTop, phiBot; // co-latitude collar angles.
    float   hTop, hBot;     // z heights of the co-latitudes (measured from the pole).
    float   collarHeight, collarArea;
    float   nRegions, regionAngle;
    float   theta, thetaR, z, scale;
    uniform float   i, j, k;

    k = 0;
    theta = 0;
    phiTop = capAngle;
    hTop = 1 - cos(phiTop);

    z = 1 - hTop*random();
    scale = sqrt(1-z*z);
    thetaR = MATH_TWOPI*random();
    R[k] = vector(scale*cos(thetaR),scale*sin(thetaR),z);
    R[k] = _rotate (R[k], angle, rotAxis);
    k += 1;

    for (i=0; i < nCollars; i+=1) {
        phiBot = phiTop + collarAngle;
        hBot = 1 - cos(phiBot);

        collarHeight = hBot - hTop;
        collarArea = MATH_TWOPI*collarHeight;
        nRegions = max(1, round(collarArea / sampleArea));
        regionAngle = MATH_TWOPI / nRegions;
        for (j=0; j < nRegions; j+=1) {
            z = 1 - (hTop + collarHeight*random());
            scale = sqrt(1-z*z);
            thetaR = theta + regionAngle*random();
            R[k] = vector(scale*cos(thetaR),scale*sin(thetaR),z);
            R[k] = _rotate (R[k], angle, rotAxis);
            k += 1;
            theta += regionAngle;
        }

        theta += regionAngle/2;
        phiTop = phiBot;
        hTop = hBot;
    }

    resize(R,k);
}


/*
 *  Clip a ray to a hemisphere.
 */
void rayClipToHemisphere (
    vector          Ns;
    output float    nsamples;
    output vector   R[];
) {
    float   j = 0;
    uniform float   i;
    for (i=0; i<nsamples; i+=1) {
        if (R[i].Ns > 0) {
            R[j] = R[i];
            j += 1;
        }
    }
    nsamples = j;
}


/*
 *  New transmission() function that removes self-occclusion.
 *  ***Not tested***
 */
color rayTransmission (
    point   Psrc, Pdst;
    vector  Nsrc;
    uniform float   samples;
    float   samplecone;
    float   samplebase;
    uniform float   bias;
    uniform string  label;
    uniform string  subset;
    uniform string  hitmode;
    uniform string  hitsides;
) {
    vector  R[] = {}; resize(R,samples+4);
    rayGenStratUniformSamples (Pdst-Psrc, samplecone, samples, R);

    float   len = length(Pdst-Psrc);

    color   Ct = color 0;
    float   nsamples = 0;
    uniform float   i;
    for (i=0; i < arraylength(R); i+=1) {
        if (Nsrc.R[i] > 0) {
            Ct += transmission (Psrc, Psrc+len*R[i],
                "subset", subset,
                "label", label,
                "hitmode", hitmode,
                "hitsides", hitsides,
                "samples", 1,
                "samplebase", samplebase,
                "samplecone", 0,
                "bias", bias
            );
            nsamples += 1;
        }
    }
    Ct /= nsamples;
}

#endif
