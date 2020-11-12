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
#ifndef effectorV7Cube_h
#define effectorV7Cube_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7Cube_s : effector_s
{
    public  uniform string  BlurType = "Centered";
    public  varying float   BlurWidth = 1;
    public  uniform string  SpaceList = "shader";

    private uniform string  _spaces[] = {};

    public  void begin()
    {
        effector_s::begin();

        _spaces = split( SpaceList, "," );
    }

    public  void radianceEffect( output light_s _light )
    {
        varying point   Q;
        varying float   xwidth, ywidth, zwidth, field;

        uniform float   i;
        uniform string  space;

        varying float   total = 1;
        for (i=0; i < arraylength(_spaces); i+=1)
        {
            space = _spaces[i];
            if (space == "") space = "shader";

            Q = transform( space, _light->Ps );

            xwidth = BlurWidth / length (vtransform (space, "world", vector(1,0,0)));
            ywidth = BlurWidth / length (vtransform (space, "world", vector(0,1,0)));
            zwidth = BlurWidth / length (vtransform (space, "world", vector(0,0,1)));

            if (BlurType == "Centered") {
                xwidth /= 2;
                ywidth /= 2;
                zwidth /= 2;

                field = smoothstep (-1-xwidth, -1+xwidth, xcomp(Q)) -
                    smoothstep (1-xwidth, 1+xwidth, xcomp(Q));
                field *= smoothstep (-1-ywidth, -1+ywidth, ycomp(Q)) -
                    smoothstep (1-ywidth, 1+ywidth, ycomp(Q));
                field *= smoothstep (-1-zwidth, -1+zwidth, zcomp(Q)) -
                    smoothstep (1-zwidth, 1+zwidth, zcomp(Q));

            } else if (BlurType == "Inside") {

                field = smoothstep (-1, -1+xwidth, xcomp(Q)) -
                    smoothstep (1-xwidth, 1, xcomp(Q));
                field *= smoothstep (-1, -1+ywidth, ycomp(Q)) -
                    smoothstep (1-ywidth, 1, ycomp(Q));
                field *= smoothstep (-1, -1+zwidth, zcomp(Q)) -
                    smoothstep (1-zwidth, 1, zcomp(Q));

            } else { // Outside			
                field = smoothstep (-1-xwidth, -1, xcomp(Q)) -
                    smoothstep (1, 1+xwidth, xcomp(Q));
                field *= smoothstep (-1-ywidth, -1, ycomp(Q)) -
                    smoothstep (1, 1+ywidth, ycomp(Q));
                field *= smoothstep (-1-zwidth, -1, zcomp(Q)) -
                    smoothstep (1, 1+zwidth, zcomp(Q));
            }
            
            total *= 1-field;
        }

        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( 1-total );
    }
}

#endif
