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
#ifndef bxdfV7Velvet_h
#define bxdfV7Velvet_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>
#include <math.h>

#define MINVALUE 0.1

struct  bxdfV7Velvet_s : bxdf_s
{
    public  varying float   Thickness = 1;
    public  varying float   ScatteringDistance = 1;
    public  varying color   DirectTint = color 1;
    public  varying color   IndirectTint = color 1;
    public  uniform float   SampleDensity = 1;

    private varying float   ScatteringRatio = 1;
    private varying vector  Tn = vector 0;
    private varying vector  Bn = vector 0;

    private uniform float   sampleNum = 0;


    /*
     *  Initialization.
     */
    public void prelighting( output response_s _response; output fresnel_s _fresnel; output blend_s _blend )
    {
        if (shortCircuit) return;

        sampleSize_s    sampleSize;

        uniform float   responseSize = sampleSize->ExponentToHemisphere(4);
        sampleNum = sampleSize->SpecularDepthReduction( SampleDensity, responseSize, _response );

        Tn = normalize( dPdu^Nbxdf );
        Bn = Nbxdf^Tn;

        ScatteringRatio = Thickness / ScatteringDistance;
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

        varying vector  Ic = _response->GetI( Eye );
        varying color   K = GetK( Eye, _response, _fresnel, _context );

        varying float   pdf = ScatteringRatio / (4*PI * max (MINVALUE, (Ic.Nbxdf)));

        varying color   Cvelvet;
        varying float   NdotL;
        varying float   NdotV;

        uniform float   i, alen = arraylength( _samples );

        for (i=0; i < alen; i+=1)
        {
            NdotL = max( MINVALUE, _samples[i]->direction.Nbxdf );
            NdotV = max( MINVALUE, _samples[i]->direction.Ic );
            Cvelvet = DITint * Cbxdf * K * ScatteringRatio / (NdotL * NdotV * 4*PI);

            _samples[i]->setMaterialResponse( Cvelvet, pdf );
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

        if (_context != "diffuse")
        {
            // specular sample generation.
            extern uniform float    Eye;

            varying vector  Ic = _response->GetI( Eye );
            varying color   K = GetK( Eye, _response, _fresnel, _context );

            varying float   pdf = ScatteringRatio / (4*PI * max(MINVALUE, (Ic.Nbxdf)));

            uniform float   start, size;
            AddSamples( sampleNum, _samples, _nsamplesPerComponent, start, size );

            varying float   cosTheta, sinTheta, phi, asperity;
            varying vector  dir;

            uniform float   i;
            for (i=start; i < size; i+=1)
            {
                cosTheta = 1 - random();
                sinTheta = sqrt( 1 - cosTheta*cosTheta );
                phi = random() * MATH_TWOPI;
                asperity = pdf / max( MINVALUE, cosTheta );

                dir = Nbxdf*cosTheta + Tn*sinTheta*cos(phi) + Bn*sinTheta*sin(phi);

                _samples[i]->setMaterialSample( DITint * Cbxdf * K * asperity, pdf, dir, MAXDIST );
            }
        }
        else
        {
            // diffuse MIS sample generation.
            AddSamples( 0, _samples, _nsamplesPerComponent );
        }
    }
}

#endif
