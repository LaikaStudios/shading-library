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
#ifndef bxdfV7Fuzz_h
#define bxdfV7Fuzz_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>

struct  bxdfV7Fuzz_s : bxdf_s
{
    public  varying float   Range = 1;
    public  varying float   Exponent = 1;
    public  varying float   MinIntensity = 0;

    private varying float   asperity = 1;


    /*
     *  Set up for ComputeIndirectLambert() call.
     */
    public void prelighting(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
    ) {

        extern uniform float    Eye;

        varying vector  Ic = _response->GetI( Eye );
        varying float   asperity = pow( mix( 1, MinIntensity, abs(Nbxdf.Ic) ), 0.5*Exponent );

        IndirectCbxdf = Cbxdf * asperity;
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
        if (shortCircuit)
        {
            bxdfZeroResponse( _samples );   // Don't respond to any samples.
            return;
        }

        extern uniform float    Eye;

        varying vector  Ic = _response->GetI( Eye );
        varying color   K = GetK( Eye, _response, _fresnel, _context );

        varying float   asperity = pow( mix( 1, MinIntensity, abs(Nbxdf.Ic) ), Exponent );
        varying float   Kpdf = (Exponent + 1) / MATH_TWOPI * asperity;

        varying color   responseColor;
        varying float   responsePdf;

        uniform float   i, alen = arraylength( _samples );

        for (i=0; i < alen; i+=1)
        {
            responsePdf = Kpdf * mathGCL( Nbxdf, _samples[i]->direction, Range, 1 );
            responseColor = responsePdf * K * Cbxdf;

            _samples[i]->setMaterialResponse( responseColor, responsePdf );
        }

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
        if (shortCircuit) return;   // Don't generate any samples if this response is off.

        if (_context != "diffuse")
        {
            // specular sample generation.
            AddSamples( 0, _samples, _nsamplesPerComponent );
        }
        else
        {
            // diffuse MIS sample generation.
            AddSamples( 0, _samples, _nsamplesPerComponent );
        }
    }
}

#endif
