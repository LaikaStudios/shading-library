/*
 *  Copyright(c) 2012-2015 Laika, developed under contract by mitchpraterLLC.
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
 *  Just a collection of functions.
 *  Computes the number of samples for the given light shape or surface response and size
 *  based on the desired sample density. Sample density is specified as the number of
 *  uniform samples per hemisphere.
 */
#ifndef sampleSizeV7_h
#define sampleSizeV7_h

#include <coreV7/rayinfo.h>
#include <coreV7/response.h>

#include <sl.h>

#define sampleSize_s sampleSizeV7_s 
struct  sampleSizeV7_s
{
    // Perfect square sample numbers make for better noise reduction.
    public uniform float squareSamples( uniform float samples )
    {
        uniform float   sqrtsamples = round( sqrt( samples ));
        return max( 1, sqrtsamples*sqrtsamples );
    }

    // For isotropic cosine-lobe responses, where exponent is the power the cosine is raised to.
    // The angle is set to the .001 threshold of the resulting response intensity.
    public varying float ExponentToCone( varying float exponent )
    {
        varying float   spline_index, cone;

        spline_index = spline( "solvecatmull-rom", exponent, 1, 1, 10, 100, 1000, 10000, 100000, 1000000, 1000000 );
        cone = spline( "catmull-rom", spline_index, HALFPI, HALFPI, .88903, .36844, .11784, .03926, .01312, .0044, .0044 );

        cone = clamp( cone, 0, HALFPI );

        return cone;
    }

    // Returns the % of a hemisphere covered by a response with the given (half) cone angle.
    // 1-cos(cone) is the true %, but reduces the number of samples too quickly for small angles.
    // gridavg() of each component produces smoother/better results than applying it to the result.
    public uniform float ConeToHemisphere( varying float cone )
    {
        return sqrt( gridavg(cone/HALFPI) * gridavg(1-cos(cone)) );
    }

    // For isotropic cosine-lobe responses, where exponent is the power the cosine is raised to.
    public uniform float ExponentToHemisphere( varying float exponent )
    {
        return ConeToHemisphere( ExponentToCone( exponent ));
    }

    // For Exponent controlled anisotropic responses.
    public uniform float ExponentToHemisphere( varying float exp0, exp1 )
    {
        uniform float   size0 = ExponentToHemisphere( exp0 );
        uniform float   size1 = ExponentToHemisphere( exp1 );

        return sqrt( size0*size1 );
    }

    public uniform float HemisphereToSampleNum(
        uniform float       _SampleDensity; // sqrt( samples per hemisphere )
        uniform float       _response_size; // % of hemisphere.
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        uniform float   sampleDensity = (_SampleDensity < 0) ? _response->surf_density : _SampleDensity;
        uniform float   sampleNum = sampleDensity*sampleDensity * _response_size;

        // On specular responses, scaling by importance can cause grid artifacts.
        // Note that Pixar code only scales by importance for diffuse responses.
        // sqrt( importance ) reduces the artifacts, but does not completely eliminate them.
        // Scaling by importance does improve speed significantly, so this is still being used.
        sampleNum *= gridmax( sqrt( _response->importance ));

        return squareSamples( sampleNum );  // Nearest perfect square.
    }


    // Power depth reduction sample number computation.
    public uniform float PowerDepthReduction(
        uniform float       _sampleNum;
        uniform float       _reduction_factor;  // < 1.
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        // Ray depth and importance sample number reduction.
        uniform float   sampleNum = _sampleNum * pow( _reduction_factor, max( 0, _response->depth - 1 ));
        sampleNum *= gridmax( _response->importance );

        return squareSamples( sampleNum );  // Nearest perfect square.
    }

    // response_size is the % of a hemisphere.
    public uniform float PowerDepthReduction(
        uniform float       _SampleDensity; // sqrt( samples per hemisphere )
        uniform float       _response_size; // % of hemisphere.
        uniform float       _reduction_factor;  // < 1.
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        uniform float   sampleDensity = (_SampleDensity < 0) ? _response->surf_density : _SampleDensity;
        uniform float   sampleNum = sampleDensity*sampleDensity * _response_size;

        return PowerDepthReduction( sampleNum, _reduction_factor, _response );
    }


    // Diffuse depth reduction sample number computation.
    public uniform float DiffuseDepthReduction(
        uniform float       _sampleNum;
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        // Ray depth sample number reduction.
        uniform float   sampleNum = pow( _sampleNum, 1/(_response->diffusedepth + 1) );
        sampleNum *= gridmax( _response->importance );

        return squareSamples( sampleNum );  // Nearest perfect square.
    }

    // response_size is the % of a hemisphere.
    public uniform float DiffuseDepthReduction(
        uniform float       _SampleDensity; // sqrt( samples per hemisphere )
        uniform float       _response_size; // % of hemisphere.
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        uniform float   sampleDensity = (_SampleDensity < 0) ? _response->surf_density : _SampleDensity;
        uniform float   sampleNum = sampleDensity*sampleDensity * _response_size;

        return DiffuseDepthReduction( sampleNum, _response );
    }


    // Specular depth reduction sample number computation.
    public uniform float SpecularDepthReduction(
        uniform float       _sampleNum;
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        // Ray depth sample number reduction.
        uniform float   specdepth = _response->speculardepth;
        if (specdepth > 0) return 1;    // One secondary ray per initial ray,

        uniform float   sampleNum = _sampleNum * gridmax( _response->importance );

        return squareSamples( sampleNum );  // Nearest perfect square.
    }

    public uniform float SpecularDepthReduction(
        uniform float       _SampleDensity; // sqrt( samples per hemisphere )
        uniform float       _response_size; // % of hemisphere.
        output response_s   _response;
    ) {
        if (_response->integrationmode == "path") return 1;

        uniform float   sampleDensity = (_SampleDensity < 0) ? _response->surf_density : _SampleDensity;
        uniform float   sampleNum = sampleDensity*sampleDensity * _response_size;

        return SpecularDepthReduction( sampleNum, _response );
    }


    // Light source sample number computation.
    public uniform float Light (
        uniform float   samples_per_hemisphere;
        uniform string  Shape;  // disk, rect, sphere, or env.
        uniform float   reduction_factor;
        varying point   Ps;
        varying vector  Ns;
    ) {
// XXX Won't be set in the light. Needs to come from the surface.
        extern rayinfo_s    rayinfoData;

        if (rayinfoData->integrationmode == "path") return 1;

        if (reduction_factor == 0 && rayinfoData->depth > 0) return 1;

        // Ray depth and importance based sample count reduction.
        uniform float   countScale;
        {
            countScale = pow( reduction_factor, max( 0, rayinfoData->depth - 1 ));
            countScale *= gridmax( rayinfoData->importance );
        }

        // Environment light.
        if (Shape == "env")
        {
            return squareSamples( 2*samples_per_hemisphere * countScale );
        }

        // Area lights.
        varying vector  L = point "shader" 0 - Ps;
        varying float   Llen = length (vtransform ("world", L));

        L = normalize(L);

        varying float   area;   // world space light source projected area.
        {
            varying float   surfDot = abs(L.Ns);

            // disk or rect
            if (Shape != "sphere")
            {
                area = length (vtransform ("world", vector "shader" (1,0,0)))
                     * length (vtransform ("world", vector "shader" (0,1,0)));

                if (Shape != "rect") area *= PI; // disk
                else area *= 4;  // rect

                varying float   lightDot = abs( L.normalize( vector "shader" (0,0,-1) ));

                area *= surfDot * lightDot;
            }

            // sphere (based on average radius of ellipsoid)
            else
            {
                area = length( vtransform( "world", vector "shader" (1,1,1)/SQRT3 ));
                area *= area * PI * surfDot;
            }
        }

        varying float   cone = atan( sqrt(area) / SQRTPI, Llen );

        uniform float samples = samples_per_hemisphere * ConeToHemisphere(cone) * countScale;

        return squareSamples( samples );
    }
}

#endif
