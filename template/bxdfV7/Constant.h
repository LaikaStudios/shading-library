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
#ifndef bxdfV7Constant_h
#define bxdfV7Constant_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/bxdf.h>

struct  bxdfV7Constant_s : bxdf_s
{
    public  varying color   Occlusion = color 1;
    public  varying float   Radiance = 1;

    // Just set the shader's color.
    public void postlighting( output varying color C, O )
    {
        extern rayinfo_s        rayinfoData;

        // camera rays are always affected.
        // non-camera (indirect) rays are determined by Radiance.
        varying float   affect = rayinfoData->depth > 0 ? Radiance : 1;
        varying color   Cout = mix( C, Cbxdf*Occlusion*O, max( 0, OnOff ));

        C = mix( C, Cout, affect );
    }
}

#endif
