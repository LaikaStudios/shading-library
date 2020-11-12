/*
 *  Copyright(c) 2014 Laika, developed under contract by mitchpraterLLC.
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
#ifndef bxdfV7Translucence_h
#define bxdfV7Translucence_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>
#include <ray.h>
#include <color.h>

/* Maximum power 10 can be raised to before we get values bigger than can be computed. */
/* Experimentally determined to be 38, but using 35 just to be safe. */
#define MAX_EXPONENT 35

struct  bxdfV7Translucence_s : bxdf_s
{
    public  varying color   LightTint = color 1;
    public  varying float   Density = 0;
    public  varying float   Exponent = 1;
    public  varying float   pow10Exponent = 10;
    public  varying float   Anisotropy = 0;
    public  varying vector  Tangent = vector 0;
    public  uniform float   SampleDensity = 0;
    public  uniform float   RayDepth = -1;
    public  varying color   FillColor = color 0;
    public  uniform float   MaxDist = 0;

    private varying float   minpow10Exponent = 1;
    private varying color   Cmatl = color 0;    // Need a non-opacity scaled material color.
    private uniform float   sampleNumSqrt = 0;
    private varying color   kt[2] = {1};


    /*
     *  Initialization.
     */

    // Override bxdfV7 begin() method: need non-opacity scaled material color in prelighting().
    public void begin()
    {
        Cmatl = colorSRGBToLinear( mix( color(0.5), Color, ColorMix ));
/*         XXX Should probably be this, since these are attenuation factors rather than an actual color. */
/*         But don't want to mess with creating a legacy version. */
/*         Cmatl = mix( color(0.5), Color, ColorMix ); */
    }

    // This is called by Material.
    public void begin( uniform float layer )
    {
        Layer = layer;

        begin();    // Local begin().
    }

    public void prelighting( output response_s _response; output fresnel_s _fresnel; output blend_s _blend )
    {
        // Set max Ray Depth.
        if (RayDepth < 0) RayDepth = _response->maxspeculardepth;

        uniform float   raydepth = _response->Depth();

        // Set Fresnel Kt and Middle/Lower blend scaling as appropriate.
        varying color   Kt = color 1;
        varying color   fresnelScale = color 1;
        varying color   B = color 1;

        if (raydepth == 0 || raydepth > RayDepth)
        {
            Kt = _fresnel->GetKt( LEFT );
            fresnelScale = mix( color 1, FresnelValue/Kt, FresnelInfluence );

            B = GetB( _blend, "specular" );
        }

        kt[LEFT] = fresnelScale * Kt * B;

        varying float   depth = _response->GetIWorldLength( LEFT );

        // Beer's law color computation goes into Cbxdf.
        varying color   transmittance = (color 1 - Cmatl) * depth * Density;
        transmittance = min( transmittance, color(MAX_EXPONENT) );

        varying color   Ctran = color 1 / _pow( 10, transmittance );

        Cbxdf = mix( Ctran, color 1, _response->Outside() );

        if (_response->ComputeRight())
        {
            if (raydepth == 0 || raydepth > RayDepth)
            {
                Kt = _fresnel->GetKt( RIGHT );
                fresnelScale = mix( color 1, FresnelValue/Kt, FresnelInfluence );
            }

            kt[RIGHT] = fresnelScale * Kt * B;

            depth = _response->GetIWorldLength( RIGHT );

            transmittance = (color 1 - Cmatl) * depth * Density;
            transmittance = min( transmittance, color(MAX_EXPONENT) );

            Ctran = color 1 / _pow( 10, transmittance );

            // Average of left and right.
            Cbxdf = mix( Cbxdf, mix( Ctran, color 1, _response->Outside() ), 0.5 );
        }

        // Scale by Intensity*OnOff.
        Cbxdf *= Intensity * max( 0, OnOff );

        // Re-scale Cbxdf by opacity;
        Cbxdf *= Obxdf;

        // Set the response's flag to skip computation if Cbxdf < threshold.
        bxdf_s::initShortCircuit();

        // Determine the number of samples.
        minpow10Exponent = _pow( pow10Exponent, 1/_pow( 10,Anisotropy ));

        sampleSize_s    sampleSize;
        uniform float   responseSize;

        // 2*responseSize to account for dual-spread.
        responseSize = sampleSize->ExponentToHemisphere( pow10Exponent, minpow10Exponent );
        uniform float   sampleNum = sampleSize->HemisphereToSampleNum( SampleDensity, 2*responseSize, _response );

        // Samples increase geometrically since we'll generate sampleNum samples at two ray depths.
        sampleNumSqrt = sqrt( sampleNum );  // sampleNum is a perfect square from the sampleSize function.
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
        extern uniform float    Eye;

        if (shortCircuit)
        {
            bxdfZeroResponse( _samples );
            return;
        }

        // "Fill" response when ray depth is exceeded.
        if (_response->Depth() > RayDepth)
        {
            varying color   Cfill = FillColor * kt[Eye];

            uniform float   i, alen = arraylength( _samples );
            for (i=0; i < alen; i+=1)
            {
                _samples[i]->setMaterialResponse( Cfill * max( 0, _samples[i]->direction.Nbxdf ), 1 );
            }

            accumulateMaterialResponse( _samples );

            return;
        }

        // Direct light source response.
        // XXX should correspond to "specular" context in generateSamples().
        // My assumption is this is the root of the multiple response blooming,
        // but messing with the pdf values and colors doesn't change that fundamentally;
        // due to the mutiple ray scattering through the material perhaps.
#if 1
        bxdfZeroResponse( _samples );
#else
        uniform float   i, alen = arraylength( _samples );
        varying float   pdf;
        for (i=0; i < alen; i+=1)
        {
            pdf = pow( max( 0, - _samples[i]->direction.normalize(I) ), pow10Exponent );
            _samples[i]->setMaterialResponse( Cbxdf * pdf, pdf );
        }

        accumulateMaterialResponse( _samples );
#endif
    }

    public void generateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        if (shortCircuit == 1 ) return; // Don't generate any samples.

        varying color   lightTint = color 1;
        varying float   lightScale = 1;

        // For direct light rays, set the light tint and pdf scale.
        if (_context == "specular")
        {
            lightTint = LightTint;
            lightScale = colorRGBToAvg( LightTint );
        }

        // Material global variable.
        extern uniform float    Eye;

        varying vector  Ic = _response->GetI( Eye );
        uniform float   raydepth = _response->Depth();

        uniform float   i;
        uniform float   pdf = 1e6;  // For single continuation rays.

        // Determine current and new sample numbers and array sizes.
        uniform float   oldSampleNum = arraylength( _samples );
        uniform float   addSampleNum = sampleNumSqrt;
        uniform float   newSampleNum = oldSampleNum + addSampleNum;


        // "Fill" response.
        // Create a dummy response lobe (with 0 samples in it) to make the
        // generated number of lobes and accumulateMaterialResponse() calls balance.
        if (raydepth > RayDepth)
        {
            push( _nsamplesPerComponent, 0 );
            return;
        }


        // Direct and indirect specular ray hits on the surface, including camera rays.
        if (_context != "diffuse")
        {
            // First and second ray hits spread the samples.
            if (raydepth < 2)
            {
                // Generate stratified samples for the spread.
                varying point   randomSamples[];
                randomstrat( oldSampleNum, newSampleNum, randomSamples );

                resize( _samples, newSampleNum );
                push( _nsamplesPerComponent, addSampleNum );    // +1 new lobe with addSampleNum samples.

                varying vector  Nc = faceforward( Nbxdf, Ic, -Nbxdf );  // Nc.Ic > 0
                varying float   bend = (1 - Ic.Nc) * mix( 0.25, 1.0, raydepth );
                varying vector  T = slerp( Ic, Nc, bend);

                varying vector  Tbit = normalize( T^Tangent );
                varying vector  Ttan = Tbit^T;

                varying vector  To;
                varying float   pdf;

                for (i=oldSampleNum; i < newSampleNum; i+=1) 
                {
                    rayGenAS(
                        T,  // Don't want reflection samples.
                        T,  // Just use the AS sample spread around T.
                        Ttan,
                        Tbit,
                        pow10Exponent,
                        minpow10Exponent,
                        randomSamples[i],

                        To,
                        pdf
                    );

                    _samples[i]->setMaterialSample( 
                        pdf * Cbxdf * lightTint * kt[Eye],
                        pdf * lightScale,
                        To,
                        MaxDist
                    );
                }
            }

            // Continuation rays: just go through subsequent surfaces without changing direction.
            else
            {
                resize( _samples, oldSampleNum + 1 );   // One sample per (incoming) ray hit.
                push( _nsamplesPerComponent, 1 );       // +1 new lobe with 1 sample.

                _samples[oldSampleNum]->setMaterialSample(
                    pdf * Cbxdf * lightTint,
                    pdf * lightScale,
                    Ic,
                    MaxDist
                );
            }
        }
    }
}

#endif
