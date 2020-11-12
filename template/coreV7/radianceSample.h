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

#ifndef radianceSample_h
#define radianceSample_h

#include <stdrsl/RadianceSample.h>


void setLightVisibility( varying color visibility; output __radiancesample _samples[] )
{
    uniform float   i, alen = arraylength(_samples);
    for (i=0; i < alen; i+=1)
    {
        _samples[i]->lightVisibility = visibility;
    }
}

void mixLightVisibility( varying color visibility; output __radiancesample _samples[]; varying float mixAmount )
{
    uniform float   i, alen = arraylength(_samples);
    for (i=0; i < alen; i+=1)
    {
        _samples[i]->lightVisibility = mix( visibility, _samples[i]->lightVisibility, mixAmount );
    }
}

#endif
