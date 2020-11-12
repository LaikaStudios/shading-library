/*
 *  Copyright(c) 2012-2014 Laika, developed under contract by mitchpraterLLC.
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
#ifndef bxdfV7AS_h
#define bxdfV7AS_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>

struct  bxdfV7AS_s : bxdf_s
{
    public  varying float   Direction = 1;      // Determines response lobe direction.
    public  varying float   Exponent = 1;
    public  varying float   pow10Exponent = 10;
    public  varying float   Anisotropy = 0;
    public  varying vector  Tangent = vector 0;
    public  varying color   DirectTint = color 1;
    public  varying color   IndirectTint = color 1;
    public  uniform float   SampleDensity = 1;

    private uniform float   sampleNum = 0;
    private varying float   minpow10Exponent = 1;
    private varying color   B = color 1;


    /*
     *  Initialization.
     */
    public void prelighting( output response_s _response; output fresnel_s _fresnel; output blend_s _blend )
    {
        minpow10Exponent = _pow( pow10Exponent, 1/_pow( 10,Anisotropy ));

        // Determine the number of samples.
        sampleSize_s    sampleSize;
        uniform float   responseSize;

        responseSize = sampleSize->ExponentToHemisphere( pow10Exponent, minpow10Exponent ); // % of hemisphere.
        sampleNum = sampleSize->SpecularDepthReduction( SampleDensity, responseSize, _response );

        // Blend value when this is used as a Middle layer response.
        B = GetB( _blend, "specular" );
    }


    /*
     *  Plausible Integration Methods.
     */
    public void evaluateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
    ) {
        varying color   DITint = color 1;

        if (_context == "specular") DITint = DirectTint;
        if (_context == "indirectspecular") DITint = IndirectTint;

        // Don't respond to any samples.
        if (shortCircuit == 1 || gridmax( colorRGBToAvg(DITint) ) == 0)
        {
            bxdfZeroResponse( _samples );
            return;
        }

        extern uniform float    Eye;

        varying vector  V = - _response->GetI( Eye );
        varying color   K = GetK( Eye, _response, _fresnel, _context );
        varying vector  S = slerp( V, Nbxdf, mix( 0.5, 1.0, Direction ));

        evaluateSamplesAS( K * B * DITint * Cbxdf, pow10Exponent, minpow10Exponent, S, Tangent, V, _samples );

        accumulateMaterialResponse( _samples );
    }

    public void generateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        varying color   DITint = color 1;

        if (_context == "specular") DITint = DirectTint;
        if (_context == "indirectspecular") DITint = IndirectTint;

        // Don't generate any samples.
        if (shortCircuit == 1 || gridmax( colorRGBToAvg(DITint) ) == 0) return;

        if (SampleDensity == 0)
        {
            AddSamples( 0, _samples, _nsamplesPerComponent );
            return;
        }

        extern uniform float    Eye;

        varying vector  V = - _response->GetI( Eye );
        varying color   K = GetK( Eye, _response, _fresnel, _context );
        varying vector  S = slerp( V, Nbxdf, mix( 0.5, 1.0, Direction ));

        uniform float   oldLength = arraylength( _samples );

        generateSamplesAS( K * B * DITint * Cbxdf, sampleNum, pow10Exponent,  minpow10Exponent, S, Tangent, V, _samples );

        uniform float   newLength = arraylength( _samples );
        push( _nsamplesPerComponent, newLength-oldLength );
    }
}

#endif
