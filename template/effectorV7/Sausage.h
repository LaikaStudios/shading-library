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
#ifndef effectorV7Sausage_h
#define effectorV7Sausage_h

#include "coreV7/light.h"
#include "coreV7/effector.h"

struct  effectorV7Sausage_s : effector_s
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
        varying point   S;
        varying vector  V;
        uniform vector  vz;
        uniform float   rz, nz, pz, r;
        uniform point   ne, pe;

        uniform float   i;
        uniform string  space;
        uniform matrix  mspace;

        varying float   radius, width, field;

        varying float   total = 1;
        for (i=0; i < arraylength(_spaces); i+=1)
        {
            space = _spaces[i];
            if (space == "") space = "shader";

            // Unit length z in world space.
            vz = vtransform( space, "world", vector(0,0,1) );
            rz = length( vz );

            // Scale z to match world space.
            mspace = transform( space, matrix 1 );
            mspace = scale( mspace, point( 1,1, rz ));

            // Line segment end points.
            // Need to ensure they don't cross each other.
            nz = min( 0, 1-rz );
            pz = max( 0, rz-1 );

            ne = point( 0,0, nz );
            pe = point( 0,0, pz );

            // space location and ptlined() distance.
            S = transform( mspace, _light->Ps );

            radius = ptlined( ne, pe, S );

            // Width needs scaling based on space:world relationship.
            if (S[2] < nz)  // - end cap.
            {
                V = S - ne;
            }
            else if (S[2] > pz) // + end cap.
            {
                V = S - pe;
            }
            else    // center section.
            {
                V = vector S;
                V[2] = 0;
            }

            V = ntransform( "world", normalize( vtransform( matrix 1 / mspace, V )));

            width = BlurWidth * length( V );

            r = 1;

            if (BlurType == "Centered")
            {
                width /= 2;
                field = smoothstep( -r-width, -r+width, radius ) - smoothstep( r-width, r+width, radius );

            }
            else if (BlurType == "Inside")
            {
                field = smoothstep( -r, -r+width, radius ) - smoothstep( r-width, r, radius );
            }
            else // Outside
            {
                field = smoothstep( -r-width, -r, radius ) - smoothstep( r, r+width, radius );
            }

            total *= 1-field;
        }

        // Apply this effector's tint to the light.
        _light->Color *= effectorTint( 1-total );
    }
}

#endif
