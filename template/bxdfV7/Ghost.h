/*
 *  Copyright(c) 2013-2015 Laika, developed under contract by mitchpraterLLC.
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
#ifndef bxdfV7Ghost_h
#define bxdfV7Ghost_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>
#include <ray.h>
#include <color.h>

struct  bxdfV7Ghost_s : bxdf_s
{
    public  varying color   LightTint = color 1;
    public  varying float   Exponent = 1;
    public  varying float   pow10Exponent = 10;
    public  varying float   Anisotropy = 0;
    public  varying vector  Tangent = vector 0;
    public  uniform float   SampleDensity = 1;
    public  uniform float   RayDepth = 1;
    public  varying color   FillColor = color 0;
    public  varying float   RI = 1;
    public  varying float   TrueKt = 1;

    private varying float   minpow10Exponent = 1;
    private varying color   Cmatl = color 0;    // Need a non-opacity scaled material color.
    private uniform float   sampleNum = 0;      // Number of samples to use for this response.
    private varying color   kr[2] = {1};        // Scaled material Fresnel values.
    private varying color   kt[2] = {1};
    private varying float   lightScale = 1;
    private varying vector  R = vector(0);
    private varying vector  T = vector(0);


    /*
     *  Initialization.
     */

    // Override bxdfV7 begin() method: need non-opacity scaled material color.
    public void begin()
    {
        Cmatl = colorSRGBToLinear( mix( color(0.5), Color, ColorMix ));

        lightScale = colorRGBToAvg( LightTint );
    }

    // This is called by Material.
    public void begin( uniform float layer )
    {
        Layer = layer;

        begin();    // Local begin().
    }

    public void prelighting( output response_s _response; output fresnel_s _fresnel; output blend_s _blend )
    {
        // Undo global Material Fresnel affect.
        varying color   Kr = _fresnel->GetKr( LEFT );
        varying color   Kt = _fresnel->GetKt( LEFT );
        varying color   fresnelScale = color(1) / Kt;

        kr[LEFT] = fresnelScale * Kr;
        kt[LEFT] = fresnelScale * Kt;

        // Local Fresnel computation.
        varying float   eta;
        varying vector  In, Nf;

        eta = 1.0/RI;

        // Left camera.
        In = _response->GetLeftI();
        Nf = _response->GetNg();

        {
            varying float   Kr, KT;

            fresnel( In, Nf, eta, Kr, KT, R, T );

            kr[LEFT] *= mix( Kr, 1.0, FresnelInfluence );
            kt[LEFT] *= mix( color( mix( 1.0-Kr, KT, TrueKt )), FresnelValue, FresnelInfluence );
        }

        Cbxdf = Cmatl;

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

        responseSize = sampleSize->ExponentToHemisphere( pow10Exponent, minpow10Exponent );
        sampleNum = sampleSize->SpecularDepthReduction( SampleDensity, 5*responseSize, _response );

        // Ray depth.
        if (RayDepth < 0) RayDepth = _response->maxspeculardepth;
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
        bxdfZeroResponse( _samples );    // Refraction doesn't have any direct response to lights.

        if (shortCircuit != 1) accumulateMaterialResponse( _samples );
    }


    private void genRefractionSamples
    (
        varying color           _DITint;
        varying float           _DIScale;
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        uniform float   eye;
        uniform float   sampleNum;
        uniform float   oldSampleNum;
        uniform float   newSampleNum;

        output __radiancesample _samples[];
    ) {
        // Tangent and Bitangent perpendicular to T.
        varying vector  Tbit = normalize( T^Tangent );
        varying vector  Ttan = Tbit^T;

        // Generate stratified samples for refraction.
        varying point   randomSamples[];
        randomstrat( oldSampleNum, oldSampleNum+sampleNum, randomSamples );

        varying vector  To;
        varying float   pdf;

        uniform float   i;
        for (i=oldSampleNum; i < oldSampleNum+sampleNum; i+=1) 
        {
            if (T != vector(0))
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
                    pdf * _DITint * kt[eye] * Cbxdf,
                    pdf * _DIScale,
                    To,
                    MAXDIST
                );
            }
#if 0
            // Not needed - new samples are all initialized to 0.
            else
            {
                _samples[i]->setMaterialResponse( color 0, 0 );
            }
#endif
        }
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
        varying float   DIScale = 1;

        if (_context == "specular")
        {
            DITint = LightTint;
            DIScale = lightScale;
        }

        if (shortCircuit == 1) return; // Don't generate any samples.

        if (_response->depth > RayDepth) return;

        // Material global variable.
        extern uniform float    Eye;

        uniform float   i;
        uniform float   pdf = 1e6;  // For single continuation rays.

        // Manage the samples array.
        uniform float   addedSampleNum, oldSampleNum, newSampleNum;

        addedSampleNum = max( 2, sampleNum );
        oldSampleNum = arraylength( _samples );
        newSampleNum = oldSampleNum + addedSampleNum;

        resize( _samples, newSampleNum );

        // New resize() entries are not initialized to 0.
        for (i=oldSampleNum; i < newSampleNum; i+=1)
        {
            _samples[i]->setMaterialResponse( color 0, 0 );
        }

        // Allocate new nsamples array entries.
        uniform float   oldNsamplesLength = arraylength( _nsamplesPerComponent );

        resize( _nsamplesPerComponent, oldNsamplesLength + 3 );


        // Ray hits on the surface, including camera rays.
        if (_context == "indirectspecular") 
        {
            // Generate the refracted camera view samples transmitted into the material.
            if (_response->Depth() == 0)
            {
                genRefractionSamples( DITint, DIScale, _response, _fresnel, _blend, _context, Eye, sampleNum, oldSampleNum, newSampleNum, _samples );

                _nsamplesPerComponent[oldNsamplesLength] = sampleNum;
                _nsamplesPerComponent[oldNsamplesLength+1] = addedSampleNum - sampleNum;
                _nsamplesPerComponent[oldNsamplesLength+2] = 0;
            }

            // Path tracing: shading caused by secondary (i.e. non-camera) ray hits.
            // These rays just continue an original T (depth=0) ray's journey from above
            // as it splits into a reflection and refraction ray at each subsequent ray hit.
            else
            {
#if 1
                _samples[oldSampleNum]->setMaterialSample(
                    pdf * DITint * Intensity,
                    pdf * DIScale,
                    normalize(I),
                    MAXDIST
                );
#else
                // Reflection sample. Interior reflection: will intersect the inside of the material again.
                _samples[oldSampleNum]->setMaterialSample(
                    pdf * kr[0] * Cbxdf,
                    pdf,
                    R,
                    MAXDIST
                );

                // Refraction sample. Will exit the material.
                _samples[oldSampleNum+1]->setMaterialSample(
                    pdf * kt[0] * Cbxdf,
                    pdf,
                    T,
                    MAXDIST
                );

                // Manage nsamples array.
                _nsamplesPerComponent[oldNsamplesLength] = 1;
                _nsamplesPerComponent[oldNsamplesLength+1] = 1;
                _nsamplesPerComponent[oldNsamplesLength+2] = addedSampleNum - 2;
#endif
            }
        }

        // Light source evaluation: ray hits from a light source.
        // Response to directlighting() call in the specularlighting() method.
        else if (_context == "specular") 
        {
            varying float   outside = _response->Outside();
            varying float   inside = 1-outside;

            uniform float   numSamples;
#if 0
            // I is inside the material, so refract it out so it can integrate with the light.
            if (inside)
            {
                genRefractionSamples( DITint, DIScale, _response, _fresnel, _blend, _context, Eye, sampleNum, oldSampleNum, newSampleNum, _samples );

                _nsamplesPerComponent[oldNsamplesLength] = sampleNum;
                _nsamplesPerComponent[oldNsamplesLength+1] = addedSampleNum - sampleNum;
                _nsamplesPerComponent[oldNsamplesLength+2] = 0;
            }

            // I is already outside the material, so just continue it. This may not strictly be necessary.
            else
#else
            {
                _samples[oldSampleNum]->setMaterialSample(
                    pdf * DITint * Intensity,
                    pdf * DIScale,
                    normalize(I),
                    MAXDIST
                );

                _nsamplesPerComponent[oldNsamplesLength] = 1;
                _nsamplesPerComponent[oldNsamplesLength+1] = addedSampleNum - 1;
                _nsamplesPerComponent[oldNsamplesLength+2] = 0;
            }
#endif
        }
    }
}

#endif
