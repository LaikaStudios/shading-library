/*
 *  Copyright(c) 2009 Laika, developed under contract by mitchpraterLLC.
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
 *  Fractal functions using Noise basis.
 *  Polymorphic in return type and arguments.
 *  2D, 3D, and 4D domains.
 *  
 * Return Type      [float | color | point | vector] fracnoise (...)
 *  Return values for float, color, and point types are in the 0 to 1 range.
 *  The vector component values are each in the -1 to +1 range.
 *
 * Domain Dimension fracnoise (float,float | point | float,point, ...)
 *  Two, three, or four dimensions.
 *
 * Arguments    fracnoise (..., iterations [,exponent [,exponRange [,lacunarity [,samplesize [,limit]]]]])
 *  All float values:
 *  iterations  The number of noise layers that will be combined.
 *  exponent    The frequency exponent: 1/f^exponent. Used for scaling the magnitude
 *          of the contribution of the given frequency layer. Related to fractal
 *          dimension: 3-2*dimension = exponent. Defaults to 1.
 *  exponRange  Adjusts the crossover scale. Produces erosion effects.
 *          0 = no effect. Negative values will smooth the lower
 *          valued areas; positive values will smooth the higher
 *          valued areas. Defaults to 0.
 *  lacunarity  The frequency increment multiplier. Defaults to 2,
 *          which doubles the frequency of each successive iteration.
 *  samplesize  The filter width. Generally a function of the uPolygon size.
 *          Determines how much a given layer is attenuated for antialiasing
 *          purposes. Default is derived from filter functions of the
 *          domain value(s).
 *  limit[2]    Two dimensional array containing the attenuation limits. When
 *          samplesize is smaller than the first value, no attenuation will 
 *          occur. When samplesize is larger than the second value, full
 *          attenuation will occur. A smoothstep() response is used between
 *          the two limits. Defaults to {.25, 1}.
 */
#ifndef FRACNOISE_H
#define FRACNOISE_H

#ifndef FRACNOISE_LIMITS
#define FRACNOISE_LIMITS {.25, 1}
#endif

#include <sl.h>
#include <filter.h>


/***************************************************************
 *  Fractal functions defined here.
 ***************************************************************/
/*********************************
 *  4D domain functions.
 *********************************/
/*
 *  Float return type.
 */
float fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity; float drQ, limit[2])
{
    float   d, sum, scale;
    float   i, f;

    d = 0;
    sum = 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        scale = 1/pow (f, expon + mix (-expRange, expRange, d/sum));
        d += scale*mix (
#pragma nolint
            smoothstep (.2, .8, float noise(f*Q + vector i*3.17, f*r)),
            .5,
            smoothstep (limit[0], limit[1], f*drQ)
        );
        sum += scale;
    }
    return (d/sum);
}

/*
 *  Point return type.
 */
point fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity; float drQ, limit[2])
{
    point   d, sum, scale;
    float   i, f;

    d = point 0;
    sum = point 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        setxcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, xcomp(d)/xcomp(sum))));
        setycomp (scale, 1/pow (f, expon + mix (-expRange, expRange, ycomp(d)/ycomp(sum))));
        setzcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, zcomp(d)/zcomp(sum))));
        d += vector scale*mix (
#pragma nolint
            _smoothstep (.2, .8, point noise(f*Q + vector i*3.17, f*r)),
            point .5,
            smoothstep (limit[0], limit[1], f*drQ)
        );
        sum += vector scale;
    }
    return (d/sum);
}

/*
 *  Vector return type.
 */
vector fracnoise (float r; point Q; float vectorMag; float iterations, expon, expRange, lacunarity; float drQ, limit[2])
{
    point   d, sum, scale;
    float   i, f;

    d = point 0;
    sum = point 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        setxcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, xcomp(d)/xcomp(sum))));
        setycomp (scale, 1/pow (f, expon + mix (-expRange, expRange, ycomp(d)/ycomp(sum))));
        setzcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, zcomp(d)/zcomp(sum))));
        d += vector scale*mix (
#pragma nolint
            2*_smoothstep (.2, .8, point noise(f*(Q + vectorMag*vector d), f*r)) - vector 1,
            point 0,
            smoothstep (limit[0], limit[1], f*drQ)
        );
        sum += vector scale;
    }
    return vector (d/sum);
}

/*
 *  Color return type.
 */
color fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity; float drQ, limit[2])
{
    return color (point fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}


/*********************************
 *  3D domain functions.
 *********************************/
/*
 *  Float return type.
 */
float fracnoise (point Q; float iterations, expon, expRange, lacunarity; float dQ, limit[2])
{
    float   d, sum, scale;
    float   i, f;

    d = 0;
    sum = 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        scale = 1/pow (f, expon + mix (-expRange, expRange, d/sum));
        d += scale*mix (
#pragma nolint
            smoothstep (.2, .8, float noise(f*Q + vector i*3.17)),
            .5,
            smoothstep (limit[0], limit[1], f*dQ)
        );
        sum += scale;
    }
    return (d/sum);
}

/*
 *  Point return type.
 */
point fracnoise (point Q; float iterations, expon, expRange, lacunarity; float dQ, limit[2])
{
    point   d, sum, scale;
    float   i, f;

    d = point 0;
    sum = point 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        setxcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, xcomp(d)/xcomp(sum))));
        setycomp (scale, 1/pow (f, expon + mix (-expRange, expRange, ycomp(d)/ycomp(sum))));
        setzcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, zcomp(d)/zcomp(sum))));
        d += vector scale*mix (
#pragma nolint
            _smoothstep (.2, .8, point noise(f*Q + vector i*3.17)),
            point .5,
            smoothstep (limit[0], limit[1], f*dQ)
        );
        sum += vector scale;
    }
    return (d/sum);
}

/*
 *  Vector return type.
 */
vector fracnoise (point Q; float vectorMag; float iterations, expon, expRange, lacunarity; float dQ, limit[2])
{
    point   d, sum, scale;
    float   i, f;

    d = point 0;
    sum = point 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        setxcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, xcomp(d)/xcomp(sum))));
        setycomp (scale, 1/pow (f, expon + mix (-expRange, expRange, ycomp(d)/ycomp(sum))));
        setzcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, zcomp(d)/zcomp(sum))));
        d += vector scale*mix (
#pragma nolint
            2*_smoothstep (.2, .8, point noise(f*(Q + vectorMag*vector d))) - vector 1,
            point 0,
            smoothstep (limit[0], limit[1], f*dQ)
        );
        sum += vector scale;
    }
    return vector (d/sum);
}

/*
 *  Color return type.
 */
color fracnoise (point Q; float iterations, expon, expRange, lacunarity; float dQ, limit[2])
{
    return color (point fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

/*********************************
 *  2D domain functions.
 *********************************/
/*
 *  Float return type.
 */
float fracnoise (float x, y; float iterations, expon, expRange, lacunarity; float dxy, limit[2])
{
    float   d, sum, scale;
    float   i, f;

    d = 0;
    sum = 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        scale = 1/pow (f, expon + mix (-expRange, expRange, d/sum));
        d += scale*mix (
#pragma nolint
            smoothstep (.2, .8, float noise(f*x + i*1.73, f*y + i*3.17)),
            .5,
            smoothstep (limit[0], limit[1], dxy)
        );
        sum += scale;
    }
    return (d/sum);
}

/*
 *  Point return type.
 */
point fracnoise (float x, y; float iterations, expon, expRange, lacunarity; float dxy, limit[2])
{
    point   d, sum, scale;
    float   i, f;

    d = point 0;
    sum = point 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        setxcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, xcomp(d)/xcomp(sum))));
        setycomp (scale, 1/pow (f, expon + mix (-expRange, expRange, ycomp(d)/ycomp(sum))));
        setzcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, zcomp(d)/zcomp(sum))));
        d += vector scale*mix (
#pragma nolint
            _smoothstep (.2, .8, point noise(f*x + i*1.73, f*y + i*3.17)),
            point .5,
            smoothstep (limit[0], limit[1], f*dxy)
        );
        sum += vector scale;
    }
    return (d/sum);
}

/*
 *  Vector return type.
 */
vector fracnoise (float x, y; float vectorMag; float iterations, expon, expRange, lacunarity; float dxy, limit[2])
{
    point   d, sum, scale;
    float   i, f;

    d = point 0;
    sum = point 0;
    for (i=0; i < iterations; i+=1) {
        f = pow (lacunarity, i);
        setxcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, xcomp(d)/xcomp(sum))));
        setycomp (scale, 1/pow (f, expon + mix (-expRange, expRange, ycomp(d)/ycomp(sum))));
        setzcomp (scale, 1/pow (f, expon + mix (-expRange, expRange, zcomp(d)/zcomp(sum))));
        d += vector scale*mix (
#pragma nolint
            2*_smoothstep (.2, .8, point noise(f*(x + vectorMag*xcomp(d)), f*(y + vectorMag*ycomp(d)))) - vector 1,
            point 0,
            smoothstep (limit[0], limit[1], f*dxy)
        );
        sum += vector scale;
    }
    return vector (d/sum);
}

/*
 *  Color return type.
 */
color fracnoise (float x, y; float iterations, expon, expRange, lacunarity; float dxy, limit[2])
{
    return color (point fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}



/***************************************************************
 *  Polymorphic argument lists defined here
 *  for each return type and domain dimension.
 ***************************************************************/

float fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity; float drQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (float fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

float fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    return (float fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

float fracnoise (float r; point Q; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    return (float fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

float fracnoise (float r; point Q; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    return (float fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

float fracnoise (float r; point Q; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (float fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

float fracnoise (point Q; float iterations, expon, expRange, lacunarity; float dQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (float fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

float fracnoise (point Q; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    return (float fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

float fracnoise (point Q; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    return (float fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

float fracnoise (point Q; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    return (float fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

float fracnoise (point Q; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (float fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

float fracnoise (float x, y; float iterations, expon, expRange, lacunarity; float dxy)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (float fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

float fracnoise (float x, y; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    return (float fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

float fracnoise (float x, y; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    return (float fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

float fracnoise (float x, y; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    return (float fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

float fracnoise (float x, y; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (float fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

point fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity; float drQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (point fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

point fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    return (point fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

point fracnoise (float r; point Q; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    return (point fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

point fracnoise (float r; point Q; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    return (point fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

point fracnoise (float r; point Q; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (point fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

point fracnoise (point Q; float iterations, expon, expRange, lacunarity; float dQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (point fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

point fracnoise (point Q; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    return (point fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

point fracnoise (point Q; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    return (point fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

point fracnoise (point Q; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    return (point fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

point fracnoise (point Q; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (point fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

point fracnoise (float x, y; float iterations, expon, expRange, lacunarity; float dxy)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (point fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

point fracnoise (float x, y; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    return (point fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

point fracnoise (float x, y; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    return (point fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

point fracnoise (float x, y; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    return (point fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

point fracnoise (float x, y; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (point fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

color fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity; float drQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (color fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

color fracnoise (float r; point Q; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    return (color fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

color fracnoise (float r; point Q; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    return (color fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

color fracnoise (float r; point Q; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    return (color fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

color fracnoise (float r; point Q; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (color fracnoise (r, Q, iterations, expon, expRange, lacunarity, drQ, limit));
}

color fracnoise (point Q; float iterations, expon, expRange, lacunarity; float dQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (color fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

color fracnoise (point Q; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    return (color fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

color fracnoise (point Q; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    return (color fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

color fracnoise (point Q; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    return (color fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

color fracnoise (point Q; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (color fracnoise (Q, iterations, expon, expRange, lacunarity, dQ, limit));
}

color fracnoise (float x, y; float iterations, expon, expRange, lacunarity; float dxy)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (color fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

color fracnoise (float x, y; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    return (color fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

color fracnoise (float x, y; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    return (color fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

color fracnoise (float x, y; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    return (color fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

color fracnoise (float x, y; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (color fracnoise (x, y, iterations, expon, expRange, lacunarity, dxy, limit));
}

vector fracnoise (float r; point Q; float vectorMag; float iterations, expon, expRange, lacunarity; float drQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (vector fracnoise (r, Q, vectorMag, iterations, expon, expRange, lacunarity, drQ, limit));
}

vector fracnoise (float r; point Q; float vectorMag; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    return (vector fracnoise (r, Q, vectorMag, iterations, expon, expRange, lacunarity, drQ, limit));
}

vector fracnoise (float r; point Q; float vectorMag; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    return (vector fracnoise (r, Q, vectorMag, iterations, expon, expRange, lacunarity, drQ, limit));
}

vector fracnoise (float r; point Q; float vectorMag; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    return (vector fracnoise (r, Q, vectorMag, iterations, expon, expRange, lacunarity, drQ, limit));
}

vector fracnoise (float r; point Q; float vectorMag; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   drQ = max (filterWidth(r), filterWidth(Q));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (vector fracnoise (r, Q, vectorMag, iterations, expon, expRange, lacunarity, drQ, limit));
}

vector fracnoise (point Q; float vectorMag; float iterations, expon, expRange, lacunarity; float dQ)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (vector fracnoise (Q, vectorMag, iterations, expon, expRange, lacunarity, dQ, limit));
}

vector fracnoise (point Q; float vectorMag; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    return (vector fracnoise (Q, vectorMag, iterations, expon, expRange, lacunarity, dQ, limit));
}

vector fracnoise (point Q; float vectorMag; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    return (vector fracnoise (Q, vectorMag, iterations, expon, expRange, lacunarity, dQ, limit));
}

vector fracnoise (point Q; float vectorMag; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    return (vector fracnoise (Q, vectorMag, iterations, expon, expRange, lacunarity, dQ, limit));
}

vector fracnoise (point Q; float vectorMag; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dQ = filterWidth(Q);
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (vector fracnoise (Q, vectorMag, iterations, expon, expRange, lacunarity, dQ, limit));
}

vector fracnoise (float x, y; float vectorMag; float iterations, expon, expRange, lacunarity; float dxy)
{
    float   limit[2] = FRACNOISE_LIMITS;
    return (vector fracnoise (x, y, vectorMag, iterations, expon, expRange, lacunarity, dxy, limit));
}

vector fracnoise (float x, y; float vectorMag; float iterations, expon, expRange, lacunarity)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    return (vector fracnoise (x, y, vectorMag, iterations, expon, expRange, lacunarity, dxy, limit));
}

vector fracnoise (float x, y; float vectorMag; float iterations, expon, expRange)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    return (vector fracnoise (x, y, vectorMag, iterations, expon, expRange, lacunarity, dxy, limit));
}

vector fracnoise (float x, y; float vectorMag; float iterations, expon)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    return (vector fracnoise (x, y, vectorMag, iterations, expon, expRange, lacunarity, dxy, limit));
}

vector fracnoise (float x, y; float vectorMag; float iterations)
{
    float   limit[2] = FRACNOISE_LIMITS;
    float   dxy = max (filterWidth(x), filterWidth(y));
    float   lacunarity = 2;
    float   expRange = 0;
    float   expon = 1;
    return (vector fracnoise (x, y, vectorMag, iterations, expon, expRange, lacunarity, dxy, limit));
}

#endif
