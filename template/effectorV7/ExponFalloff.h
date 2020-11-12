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
#ifndef effectorV7ExponFalloff_h
#define effectorV7ExponFalloff_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7ExponFalloff_s : effector_s
{
    public  varying float   Clamp = 1;
    public  varying float   Exponent = 2;
    public  varying float   Distance = 0;
    public  uniform string  Space = "shader";

    private uniform string  _space = "";

    public  void begin()
    {
        effector_s::begin();

        _space = (Space != "") ? Space : "shader";
    }

    public  void radianceEffect( output light_s _light )
    {
        varying point   Q = transform( _space, _light->Ps );

        // This (linear) field value is not constrained to the 0-1 range.
        varying float   maxField = pow( Distance+1, Exponent );
        varying float   field = maxField / pow( length(vector Q)+1, Exponent );

        field = mix( field, maxField-field, Invert );
        field = mix( field, clamp( field,0,1 ), Clamp );

        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( 1-field );
    }
}

#endif
