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
#ifndef effectorV7Sphere_h
#define effectorV7Sphere_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7Sphere_s : effector_s
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
        varying float   radius, width, field;

        uniform float   i;
        uniform string  space;

        varying float   total = 1;
        for (i=0; i < arraylength(_spaces); i+=1)
        {
            space = _spaces[i];
            if (space == "") space = "shader";

            Q = transform( space, _light->Ps );

            // This isn't exactly right. What you really want is to measure the width in
            // a direction perpendicular to the ellipsoid surface. But this is good enough
            // unless the space is highly non-uniform.
            width = BlurWidth / length( vtransform( space, "world", normalize(vector(Q)) ));
            radius = length( Q );

            if (BlurType == "Centered")
            {
                width /= 2;
                field = smoothstep( -1-width, -1+width, radius ) - smoothstep( 1-width, 1+width, radius );

            }
            else if (BlurType == "Inside")
            {
                field = smoothstep( -1, -1+width, radius ) - smoothstep( 1-width, 1, radius );
            }
            else // Outside
            {
                field = smoothstep( -1-width, -1, radius ) - smoothstep( 1, 1+width, radius );
            }
            
            total *= 1-field;
        }

        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( 1-total );
    }
}

#endif
