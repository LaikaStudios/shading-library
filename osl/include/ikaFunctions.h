/*
 *  Copyright 2018-2019 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
#ifndef IKA_FUNCTIONS_H
#define IKA_FUNCTIONS_H

#include "ikaConstants.h"

//----------------------------------------------------------------------
//  Macros
//----------------------------------------------------------------------
#define IKA_DEGREES(R) degrees(R)
#define IKA_RADIANS(D) radians(D)


//----------------------------------------------------------------------
//  bumped normal compensation
//
//  If a bump makes a front facing N back facing, those surfaces would not
//  be visible in a displaced surface; but they are with a bumped normal,
//  which typically produces a black response.
//  This function prevents the normal from becoming backfacing along I,
//  but allows it to rotate around I, maintaining its curvature
//  perpendicular to I, thus preserving its appearance.
//  Based on RixAdjustNormal().
//  Note: back facing normals that becoming front facing are not visible.
//----------------------------------------------------------------------
normal ikaBumpCompensation( normal n )
{
    normal  Nb = n;

    if( dot( I,N ) < 0.0 ) // Front facing surface.
    {
        vector In = normalize( I );
        float  IdotN = dot( In, n );
        if( IdotN > 0.0 )
        {
            Nb -= 1.001*IdotN * In; // Make Nb roughly perpendicular to I.
            Nb = normalize( Nb );
        }
    }

    return Nb;
}


//----------------------------------------------------------------------
//  CreateOrthonormalBasis from RiTypesHelper.h
//  Based on the paper Building an Orthonormal Basis, Revisited in
//  the Journal of Computer Graphics Techniques Vol. 6, No. 1, 2017
//  http://jcgt.org/published/0006/01/01/
//----------------------------------------------------------------------
void ikaCreateOrthonormalBasis(
    vector  Nn,
    output vector  Tn,
    output vector  Bn
)
{
    float  sign = select( -1.0, 1.0, Nn[2] >= 0.0 );
    float  a = -1.0 / ( sign + Nn[2] );

    Bn = vector( Nn[0]*Nn[1]*a, sign + Nn[1]*Nn[1]*a, -Nn[1] );
    Tn = vector( 1.0 + sign*Nn[0]*Nn[0]*a, sign*Bn[0], -sign*Nn[0] );
}


//----------------------------------------------------------------------
//  max
//----------------------------------------------------------------------
color ikaMax( float a, color b )
{
    return max( color(a), b );
}

color ikaMax( color a, float b )
{
    return max( a, color(b) );
}

#endif
