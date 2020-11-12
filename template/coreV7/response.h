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
#ifndef coreV7response_h
#define coreV7response_h

#include <coreV7/camera.h>
#include <coreV7/rayinfo.h>
#include <coreV7/surface.h>
#include <coreV7/fresnel.h>
#include <coreV7/optsandattrs.h>

#define response_s coreV7response_s
struct  coreV7response_s : camera_s, rayinfo_s, surface_s, optsandattrs_s
{
    private varying color   Kt[2] = {1};
    private varying color   AvgKt = color 1;

    public  void Init(
        camera_s        _cameraData;
        rayinfo_s       _rayinfoData;
        surface_s       _surfaceData;
        optsandattrs_s  _optsandattrsData;
    ) {
        camera_s  = _cameraData;
        rayinfo_s = _rayinfoData;
        surface_s = _surfaceData;
        optsandattrs_s = _optsandattrsData;
    }

    public  void SetKt( fresnel_s _fresnel )
    {
        Kt[0] = _fresnel->GetLeftKt();
        Kt[1] = _fresnel->GetRightKt();
        AvgKt = _fresnel->GetAvgKt();
    }
    public  varying color GetKt( uniform float _Eye ) { return Kt[_Eye]; }
    public  varying color GetAvgKt() { return AvgKt; }
}

#endif
