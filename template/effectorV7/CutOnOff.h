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
#ifndef effectorV7CutOnOff_h
#define effectorV7CutOnOff_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7CutOnOff_s : effector_s
{
    public  uniform float   CutOnMin = 0;
    public  uniform float   CutOnMax = 1;
    public  uniform float   CutOffMin = 999;
    public  uniform float   CutOffMax = 1000;
    public  uniform string  Space = "shader";

    private uniform string  _space = "";

    public  void begin()
    {
        effector_s::begin();

        _space = (Space != "") ? Space : "shader";
    }

    public  void radianceEffect( output light_s _light )
    {
        varying float   Qz = zcomp( transform( _space, _light->Ps ));

        varying float   field;

        field = smoothstep( CutOnMin, min( CutOnMax, CutOffMax ), Qz );
        field *= 1-smoothstep( max( CutOffMin, CutOnMin ), CutOffMax, Qz );

        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( field );
    }
}

#endif
