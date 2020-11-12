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
#ifndef bxdfV7Lambert_h
#define bxdfV7Lambert_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>

struct  bxdfV7Lambert_s : bxdf_s
{

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

        varying color   ColorOverPI = Cbxdf / PI;
        varying color   K = GetK( Eye, _response, _fresnel, _context );

        varying color   responseColor;

        uniform float   i, alen = arraylength( _samples );
        for (i=0; i < alen; i+=1)
        {
            responseColor = K * Cbxdf * max( 0, _samples[i]->direction.Nbxdf );

            _samples[i]->setMaterialResponse( responseColor, 0 );
        }

        accumulateMaterialResponse( _samples );
    }
}

#endif
