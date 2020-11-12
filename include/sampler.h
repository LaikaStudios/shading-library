/*
 *  Copyright(c) 2012 Laika, developed under contract by mitchpraterLLC.
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
#ifndef sampler_h
#define sampler_h

#include <stdrsl/RadianceSample.h>
#include <stdrsl/Math.h>

// Generate 2d stratified samples through jittering. This does not give
// as nicely stratified result as randomstrat but is pretty good.
//
void randomJitteredSamples(uniform float offset; uniform float maxnsamples;
                           output __radiancesample samples[])
{
    uniform float xsamples = floor(sqrt(maxnsamples));
    uniform float ysamples = floor(maxnsamples/xsamples);
    uniform float realnsamples = xsamples * ysamples;
    uniform float index;
    varying float x, y;

    resize(samples, arraylength(samples) + realnsamples);

    uniform float i, j;
    for (i = 0; i < xsamples; i += 1) {
        for (j = 0; j < ysamples; j += 1) {
            index = offset + j * xsamples + i;
            x = (j + random()) / xsamples;
            y = (i + random()) / ysamples; 
            samples[index]->direction = vector(x,y,0);
        }
    }
}

struct StratifiedSampler
{
    uniform point origin = point 0;
    uniform vector xdir = vector 0;
    uniform vector ydir = vector 0;
    uniform vector zdir = vector 0;
    uniform float shapearea = 0;

    void begin()
    {
        origin = point "shader" 0;
        xdir = vector "shader" (1,0,0);
        ydir = vector "shader" (0,1,0);
        zdir = normalize(vector "shader" (0,0,-1));
        shapearea = length(xdir) * length(ydir);
    }

    void generateSquareSamples(varying point Ps;
                               uniform float maxnsamples;
                               output __radiancesample samples[]) 
    {
        //randomstrat(0, maxnsamples, samples);
        randomJitteredSamples(0, maxnsamples, samples);

        float r1, r2, lpdf, len, denom;
        point p;
        vector L, Ln;
        uniform float i;
        for (i = 0; i < maxnsamples; i += 1) {
            r1 = samples[i]->direction[0];
            r2 = samples[i]->direction[1];
            p = (r1 - 0.5) * xdir + (r2 - 0.5) * ydir + origin;

            L = p - Ps;
            len = max(length(L), .0001);
            Ln = L/len;

            // pdf of lightsample is 1 / shapearea
            // to convert to hemisphere domain we
            // apply light-to-hemisphere conversion of len*len/cos
            // To signal backside of light, we assign a negative
            // pdf value.
            // This can be negative, we leave it to client
            // to invert and assign energy to back-side
            lpdf = len*len / signedmax(M_MINDIVISOR, zdir.Ln * shapearea);

            // squirrel s,t into radiance, since caller owns radiance
            samples[i]->setLightSample(color(r1, r2, 0), lpdf, Ln, len);
        }
    }

    void generateDiskSamples(varying point Ps;
                             uniform float maxnsamples;
                             output __radiancesample samples[])
    {
        //randomstrat(0, maxnsamples, samples);
        randomJitteredSamples(0, maxnsamples, samples);

        float diskarea = MATH_PI / 4 * shapearea;

        float theta, r, x, y, lpdf, len, denom;
        point p;
        vector L, Ln;
        uniform float i;
        for (i = 0; i < maxnsamples; i += 1) {
            theta = MATH_TWOPI * samples[i]->direction[0];
            r = 0.5 * sqrt(samples[i]->direction[1]); // need sqrt to make uniform
            x = r * cos(theta);
            y = r * sin(theta);
            p = x * xdir + y * ydir + origin;

            L = p - Ps;
            len = max(length(L), .0001);
            Ln = L/len;

            // pdf of lightsample is 1 / shapearea
            // to convert to hemisphere domain we
            // apply light-to-hemisphere conversion of len*len/cos
            // To signal backside of light, we assign a negative
            // pdf value.
            // This can be negative, we leave it to client
            // to invert and assign energy to back-side
            lpdf = len*len / signedmax(M_MINDIVISOR, zdir.Ln * diskarea);

            // squirrel s,t into radiance, since caller owns radiance
            samples[i]->setLightSample(color(x, y, 0), lpdf, Ln, len);
        }
    }

    void generateSphereSamples(varying point Ps;
                               uniform float maxnsamples;
                               output __radiancesample samples[])
    {
        // FIXME:
        // instead of doing a spherical set of samples, generate a hemisphere
        // facing the point being illuminated. This should eliminate half of
        // the samples that would be facing away from the surface.

        //randomstrat(0, maxnsamples, samples);
        randomJitteredSamples(0, maxnsamples, samples);

        uniform vector realzdir = vector "shader" (0,0,-1);
        // Use same area as disk here since the sphere appears as a disk
        // from pov of surface. This is not quite right, but at least it
        // will be consistent with the disk sampler.
        uniform float sphereArea = MATH_PI / 4 * shapearea;

        float theta, phi, x, y, z, r, r1, r2, lpdf, len, denom;
        point p;
        vector L, Ln;
        uniform float i;
        for (i = 0; i < maxnsamples; i += 1) {
            r1 = samples[i]->direction[0];
            r2 = samples[i]->direction[1];

            z = 1 - 2 * r1;
            r = sqrt(max(0, 1 - z * z));
            phi = MATH_TWOPI * r2;
            theta = acos(z);
            x = r * cos(phi);
            y = r * sin(phi);
            p = 0.5 * x * xdir + 0.5 * y * ydir + 0.5 * z * realzdir + origin;

            L = p - Ps;
            len = max(length(L), .0001);
            Ln = L/len;

            // pdf of lightsample is 1 / shapearea
            lpdf = len*len / signedmax(M_MINDIVISOR, sphereArea);

            // squirrel s,t into radiance, since caller owns radiance
            samples[i]->setLightSample(color(x, y, 0), lpdf, Ln, len);
        }
 
    }

}

struct AngularSampler
{
    public uniform float AngularSize = 0.01; // % of sphere
    public uniform float Stratified = 0;

    private uniform vector _xdir = vector 0;
    private uniform vector _ydir = vector 0;
    private uniform vector _zdir = vector 0;
    private varying float _tan = 0;
    private varying float _spread = 0;
    private varying float _pdf = 0;
    private varying float _shapearea = 0;

    void begin()
    {
        _xdir = normalize(vector "shader" (1, 0, 0));
        _ydir = normalize(vector "shader" (0, 1, 0));
        _zdir = normalize(vector "shader" (0, 0, -1));

        uniform float angleInRadians = AngularSize * MATH_TWOPI / 2;
        _tan = tan(angleInRadians);
        _spread = cos(angleInRadians);
        _shapearea = angleInRadians * angleInRadians;
        varying float solidAngle = (1-_spread) * 2.0 * MATH_TWOPI;
        _pdf = 1 / solidAngle;
    }

    void generateSamples(varying point Ps;
                         uniform float maxnsamples;
                         output __radiancesample samples[])
    {
        //if (Stratified) {
        //    randomstrat(0, maxnsamples, samples);
        //} else {
            randomJitteredSamples(0, maxnsamples, samples);
        //}

        varying vector L, Ln;
        varying float r1, r2, len;

        uniform float i;
        uniform float alen = arraylength(samples);
        for (i = 0; i < alen; i += 1) {
            r1 = samples[i]->direction[0];
            r2 = samples[i]->direction[1];

            L = _zdir + _tan * r1 * _xdir + _tan * r2 * _ydir;
            len = max(length(L), .0001);
            Ln = L/len;

            samples[i]->setLightSample(color 0, _pdf, Ln, 1e6);
        }
    }

    void evaluateSamples(output __radiancesample samples[])
    {
        uniform float i, alen = arraylength(samples);
        for (i = 0; i < alen; i += 1) {
            float cosDir = samples[i]->direction._zdir;
            // if the sample vector is within the angular spread it is
            // considered a hit (as in plausibleSunlight)
            if (cosDir > _spread) {
                samples[i]->lightPdf = _pdf;
            } else {
                samples[i]->lightPdf = -1;
            }
        }
    }

}

#endif // sampler_h
