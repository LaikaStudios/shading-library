/*
 *  Copyright(c) 2014-2015 Laika, developed under contract by mitchpraterLLC.
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
#ifndef volumeAS_h
#define volumeAS_h

#include <stdrsl/RadianceSample.h>
#include <stdrsl/Math.h>

#include <coreV7/sampleSize.h>
#include <volume/Common.h>

#include <sl.h>

struct  volumeAS_s : volumeCommon_s
{
    public  varying float   Direction = 1;      // Determines response lobe direction.
    public  varying float   Exponent = 1;
    public  varying float   Anisotropy = 0;
    public  varying vector  Tangent = vector 0;
    public  varying float   DirectIntensity = 1;
    public  varying color   DirectTint = color 1;
    public  varying float   IndirectIntensity = 1;
    public  varying color   IndirectTint = color 1;
    public  uniform float   SampleDensity = 1;

    private varying float   pow10Exponent = 10;
    private varying float   minpow10Exponent = 1;

    private uniform float   sampleNum = 0;


    public void begin( uniform float depth; varying float importance )
    {
        volumeCommon_s::begin();

        pow10Exponent = _pow( 10.0, Exponent );
        minpow10Exponent = _pow( pow10Exponent, 1.0 / _pow( 10.0,Anisotropy ));

        if (depth > 0)
        {
            sampleNum = 1;
            return;
        }

        sampleSize_s    sampleSize;
        uniform float   responseArea = sampleSize->ExponentToHemisphere( pow10Exponent, minpow10Exponent );

        sampleNum = SampleDensity*SampleDensity * responseArea * gridmax( importance );
        sampleNum = sampleSize->squareSamples( sampleNum );
    }


    public void prelighting( varying vector Nvol )
    {
        Nbxdf = Nvol;
    }


    public void evaluateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[]
    ) {
        varying color   DITint = color(1);

        if (_context == "specular") DITint = colorSRGBToLinear( DirectIntensity * DirectTint );
        if (_context == "indirectspecular") DITint = colorSRGBToLinear( IndirectIntensity * IndirectTint );

        if (_context == "diffuse" || u_OnOff != 1 || gridmax( colorRGBToAvg(DITint)) == 0)
        {
            zeroMaterialResponse( _samples );
            return;
        }

        varying vector  S = slerp( normalize( Nbxdf - _Ic ), Nbxdf, Direction );
        evaluateSamplesAS( DITint * Cbxdf, pow10Exponent, minpow10Exponent, S, Tangent, -_Ic, _samples );

        accumulateMaterialResponse( _samples );
    }


    public void generateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        varying color   DITint = color(1);

        if (_context == "specular") DITint = colorSRGBToLinear( DirectIntensity * DirectTint );
        if (_context == "indirectspecular") DITint = colorSRGBToLinear( IndirectIntensity * IndirectTint );

        if (_context == "diffuse" || u_OnOff != 1 || gridmax( colorRGBToAvg(DITint)) == 0) return;

        if (SampleDensity == 0)
        {
            addSamples( 0, _samples, _nsamplesPerComponent );
            return;
        }

        uniform float   oldLength = arraylength( _samples );

        varying vector  S = slerp( normalize( Nbxdf - _Ic ), Nbxdf, Direction );
        generateSamplesAS( DITint * Cbxdf, sampleNum, pow10Exponent,  minpow10Exponent, S, Tangent, -_Ic, _samples );

        uniform float   newLength = arraylength( _samples );
        push( _nsamplesPerComponent, newLength-oldLength );
    }
}

#endif
