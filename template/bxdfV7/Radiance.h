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
#ifndef bxdfV7Radiance_h
#define bxdfV7Radiance_h

#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

struct  bxdfV7Radiance_s : bxdf_s
{
    public  uniform float   Visibility = 0;
    public  varying color   Occlusion = color 1;

    /*
     *  Initialization.
     */
    public void prelighting(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
    ) {
        Cbxdf *= Occlusion;

        uniform float   _depth = _response->depth;

        // Camera-only
        if( Visibility < 0 && _depth > 0 )
        {
            Cbxdf = color(0);
        }

        // Indirect-only
        if( Visibility > 0 && _depth == 0 )
        {
            Cbxdf = color(0);
        }
    }

}

#endif
