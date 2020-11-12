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
/*
    Declares a standard set of bxdf template parameters and methods.
 */
#ifndef coreV7bxdf_h
#define coreV7bxdf_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>
#include <color.h>

#define bxdfUPPER  0
#define bxdfMIDDLE 1
#define bxdfLOWER  2


// Commonly used in response function computation.
#define bxdfZeroResponse coreV7bxdfZeroResponse
void coreV7bxdfZeroResponse( output __radiancesample sample[] )
{
    uniform float   i, alen = arraylength(sample);

    for (i=0; i < alen; i+=1) sample[i]->setMaterialResponse( color 0, 0 );
}

#define bxdf_s coreV7bxdf_s
struct  coreV7bxdf_s
{
    public  varying float   OnOff = 1;              // Include the response in the result: -1=Kill, 0=Bypass, 1=On.
    public  varying float   NormOnOff = 1;          // Include the response in intensity normalization or not.
    public  varying float   Intensity = 1;          // How intense is the response? (sRGB)
    public  varying color   Color = color 1;        // What color is the response? (sRGB)
    public  varying float   ColorMix = 1;           // How much of the color is used in the response?
    public  varying vector  Ns = vector (0,0,0);    // An alternative shading normal (normalized, current space).
    public  varying float   NsInfluence = 0;        // How much of the alternative shading normal to use.
    public  varying color   FresnelValue = 0;       // An alternative Fresnel value for this response.
    public  varying float   FresnelInfluence = 0;   // How much of the alternative Fresnel value to use.

    public  uniform float   Layer = 0;  // Upper=0, Middle=1, Lower=2.

    public  varying color   Cbxdf = color 0;    // The response color*intensity*opacity (linear).
    public  varying color   Obxdf = color 0;    // The response opacity (linear).
    public  varying point   Pbxdf = point 0;    // This response's displaced point.
    public  varying vector  Nbxdf = vector 0;   // This response's bumped normal.

    public  uniform float   shortCircuit = 0;   // Does this response need to be computed or not?
    public  varying float   ShortCircuitThreshold = 0;  // color*intensity*opacity*onoff execution threshold.

    public  varying color   IndirectCbxdf = color 0;    // The color to use in ComputeIndirectLambert.
                                                        // Cbxdf * indirect diffuse shaping.
                                                        // Apply shaping in prelighting().

    // Initialize the response colorintensity.
    public void begin()
    {
        Cbxdf = max( 0, OnOff ) * colorSRGBToLinear( Intensity * mix( color(0.5), Color, ColorMix ));
    }

    // Set the Layer so the response knows what material layer it's in and initialize its colorintensity.
    public void begin( uniform float layer )
    {
        Layer = layer;
        begin();
    }

    // Shading surface setup: surface point and shading normal of the response.
    public void displacement( varying point _P; varying normal _N )
    {
        Pbxdf = _P;
        Nbxdf = slerp( vector _N, Ns, NsInfluence );    // We assume the input vectors are pre-normalized.
    }

    // Set the response opacity and scale the response colorintensity by opacity.
    public void opacity( varying color _O )
    {
        Obxdf = _O;
        Cbxdf *= _O;
    }

    // Don't compute the response if its colorintensity(*opacity) is near 0.
    public void initShortCircuit()
    {
        shortCircuit = gridnever( abs(colorRGBToValue(Cbxdf)) > ShortCircuitThreshold );
    }

    // Illumination computation setup.
    public void prelighting(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
    ) {
        IndirectCbxdf = Cbxdf;
    }

    // Get the Fresnel K value according to the layer the response is in and its computational context
    // and apply the Fresnel Override controls.
    // This can be used in a response's evaluate/generateSamples() methods (where Eye is known).
    public varying color GetK( uniform float _Eye; response_s _response; fresnel_s _fresnel; uniform string _context )
    {
        varying color   K;

        // Kr
        if (Layer == 0)
        {
            if (_context != "diffuse")
            {
                K = _fresnel->GetKr( _Eye );
                K = mix( K, FresnelValue, FresnelInfluence );
            }
            else
            {
                // XXX Untested.
                // XXX does the diffuselighting() pathway know what Eye is? Should this be GetAvgKr()?
                K = mix( _fresnel->GetKr( _Eye ), FresnelValue, FresnelInfluence );
                K /= _response->GetKt( _Eye );  // diffuse cache scaled by Kt in specularlighting().
            }
        }

        // Kt
        else
        {
            if (_context != "diffuse")
            {
                K = _fresnel->GetKt( _Eye );
                K = mix( K, FresnelValue, FresnelInfluence );
            }
            else
            {
                // XXX does the diffuselighting() pathway know what Eye is? Should this be GetAvgKt()?
                K = _response->GetKt( _Eye );
                K = mix( color 1, FresnelValue / K, FresnelInfluence );
            }
        }

        return K;
    }

    // Get the Blend B value according to the layer the response is in and its computational context.
    public varying color GetB( blend_s _blend; uniform string _context )
    {
        varying color   B = color(1);

        if (_context != "diffuse")
        {
            if (Layer == 1)
            {
                B = _blend->GetBr();
            }

            else if (Layer == 2)
            {
                B = _blend->GetBt();
            }
        }

        return B;
    }

    // Manage sample arrays.
    public void AddSamples(
        uniform float           _addSampleNum;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        if (_addSampleNum < 1)
        {
            push( _nsamplesPerComponent, 0 );
            return;
        }

        uniform float   oldSampleNum = arraylength( _samples );
        uniform float   newSampleNum = oldSampleNum + _addSampleNum;

        resize( _samples, newSampleNum );

        // New resize() entries are not initialized to 0.
        uniform float   i;
        for (i=oldSampleNum; i < newSampleNum; i+=1)
        {
            _samples[i]->setMaterialResponse( color 0, 0 );
        }

        // Allocate new nsamplesPerComponent array entry.
        push( _nsamplesPerComponent, _addSampleNum );
    }

    public void AddSamples(
        uniform float           _addSampleNum;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
        output uniform float    _oldSampleNum;
        output uniform float    _newSampleNum;
    ) {
        _oldSampleNum = arraylength( _samples );

        if (_addSampleNum < 1)
        {
            push( _nsamplesPerComponent, 0 );
            _newSampleNum = _oldSampleNum;
            return;
        }

        _newSampleNum = _oldSampleNum + _addSampleNum;

        resize( _samples, _newSampleNum );

        // New resize() entries are not initialized to 0.
        uniform float   i;
        for (i=_oldSampleNum; i < _newSampleNum; i+=1)
        {
            _samples[i]->setMaterialResponse( color 0, 0 );
        }

        // Allocate new nsamplesPerComponent array entry.
        push( _nsamplesPerComponent, _addSampleNum );
    }


    /*
     * Method prototypes.
     */

    // Plausible integration methods.
    public void evaluateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
    ) { }

    public void generateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) { }

    // Post-illumination.
    public void postlighting( output varying color C, O )
    { }
}

#endif
