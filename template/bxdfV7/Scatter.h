/*
 *  Copyright(c) 2013-2014 Laika, developed under contract by mitchpraterLLC.
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
#ifndef bxdfV7Scatter_h
#define bxdfV7Scatter_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>
#include <slim.h>

struct  bxdfV7Scatter_s : bxdf_s
{
    public  varying float   ScatterDirection = 1;
    public  varying float   Exponent = 1;


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

        varying float   scatter = mathGCL( Ic, Nbxdf, 2, 1 );
        scatter = mix( 1-scatter, scatter, ScatterDirection );
        scatter = pow( scatter, 0.5*Exponent );

        IndirectCbxdf = Cbxdf * scatter;
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

        varying float   Kpdf = (Exponent + 1) / MATH_TWOPI;

        varying vector  dir;
        varying float   scatter, view;

        varying color   responseColor;
        varying float   responsePdf, cosine;

        uniform float   i, alen = arraylength( _samples );

        for (i=0; i < alen; i+=1)
        {
            dir =  _samples[i]->direction;

            scatter = mathGCL( Ic, dir, 2, 1 );
            scatter = mix( 1-scatter, scatter, ScatterDirection );
            scatter = pow( scatter, Exponent );

            view = mathGCL( Nbxdf, dir, 1, 1 );

            responsePdf = Kpdf * scatter;
            responseColor = responsePdf * view * K * Cbxdf;

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
