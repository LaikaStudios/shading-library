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
#ifndef coreV7shadow_h
#define coreV7shadow_h

#include <coreV7/camera.h>
#include <coreV7/surface.h>

#define shadow_s coreV7shadow_s
struct  coreV7shadow_s
{
    // Copied from camera_s
    private varying vector  Ic = vector 0;
    private varying point   origin = point 0;
    private varying float   worldLen = 0;

    // Copied from surface_s
    private varying float   outside = 1;
    private varying vector  Ngc = vector 0;

    // Set by Shadow template functions.
    private varying color   oi = color 1;


    public  void Init (
        output camera_s     _cameraData;
        output surface_s    _surfaceData;
    ) {
        Ic = _cameraData->GetLeftI();
        origin = _cameraData->GetLeftOrigin();
        worldLen = _cameraData->GetLeftIWorldLength();

        outside = _surfaceData->Outside();
        Ngc = _surfaceData->GetNg();
    }


    public  varying vector GetI() { return Ic; }
    public  varying vector GetNg() { return Ngc; }
    public  varying normal GetNg() { return normal Ngc; }
    public  varying float  Outside() { return outside; }
    public  varying point  GetOrigin() { return origin; }
    public  varying float  GetIWorldLength() { return worldLen; }


    public  varying float GetNdotI()
    {
        return ( Ngc.Ic );
    }

    public  varying float GetAbsNdotI()
    {
        return abs( Ngc.Ic );
    }


    public  varying vector GetNf()
    {
        return vector faceforward( Ngc,Ic,Ngc );
    }


    public varying color GetOi()
    {
        return oi;
    }

    public varying color SetOi( varying color _oi; )
    {
        oi = _oi;
    }
}

#endif
