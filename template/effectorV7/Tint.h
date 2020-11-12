/*
 *  Copyright(c) 2010-2014 Laika, developed under contract by mitchpraterLLC.
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
#ifndef effectorV7Tint_h
#define effectorV7Tint_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7Tint_s : effector_s
{
    public  varying float   Effect = 1;

    public  void begin()
    {
        effector_s::begin();
    }

    public  void radianceEffect( output light_s _light )
    {
        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( Effect );
	}
}

#endif
