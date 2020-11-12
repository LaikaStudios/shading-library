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
#ifndef effectorV7Smoothstep_h
#define effectorV7Smoothstep_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7Smoothstep_s : effector_s
{
    public  uniform string  CombineMethod = "Accumulate";
    public  uniform string  SpaceList = "shader";

    private uniform string  _spaces[] = {};

    public  void begin()
    {
        effector_s::begin();

        _spaces = split( SpaceList, "," );
    }

    public  void radianceEffect( output light_s _light )
    {
        varying float   Qz;
        varying float   field;

        uniform float   i;
        uniform string  space;
                    
        uniform float   cycle = 0;
        varying float   total = 1;
        for (i=0; i < arraylength(_spaces); i+=1)
        {
            cycle = 1 - cycle;

            space = _spaces[i];
            if (space == "") space = "shader";

            Qz = zcomp( transform( space, _light->Ps ));

            field = smoothstep( -1, 1, Qz );
            total *= field;

            if (CombineMethod == "Alternate") total = 1-total;
        }
        if (CombineMethod == "Accumulate" || cycle == 0) total = 1-total;
        
        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( total );
    }
}

#endif
