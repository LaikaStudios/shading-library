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
#ifndef bxdfV7Subsurface_h
#define bxdfV7Subsurface_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

struct  bxdfV7Subsurface_s : bxdf_s
{
    public  uniform float   SampleDensity = 1;
    public  uniform string  Label = "";

    public  varying color   Tint = color 1;
    public  varying color   Albedo = color .9;
    public  varying color   DMFP = color 1;
    public  uniform string  Type = "ssdiffusion";
    public  uniform float   IOR = 1.3;
    public  uniform float   FollowTopology = 0;
    public  varying float   Offset = -1;
    public  uniform float   LambertSize = 0;
    public  uniform float   IgnoreOcclusion = 0;
    public  uniform float   Smooth = 0;
    public  varying float   MaxDist = MAXDIST;
    public  varying color   Weight = color 1;

    public  uniform float   SampleNum = 1;

    /*
     *  Initialization.
     */
    public void prelighting( output response_s _response; output fresnel_s _fresnel; output blend_s _blend )
    {
        sampleSize_s    sampleSize;

        uniform float   responseSize = sampleSize->ExponentToHemisphere(4);
        SampleNum = sampleSize->SpecularDepthReduction( SampleDensity, responseSize, _response );
    }
}

#endif
