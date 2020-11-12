/*
 *  Copyright(c) 2009-2014 Laika, developed under contract by mitchpraterLLC.
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

/*
 *  prman surface shading related functions.
 *  XXX Using 1-geometry:frontfacing for orientation - haven't tested this.
 */
#ifndef prman_h
#define prman_h

#include <sl.h>


vector  prmanSurfaceNormal()
{
    return vector normalize(N);
}

vector  prmanGeometricNormal()
{
    return vector normalize(Ng);
}

// XXX Left camera or traced ray only.
normal  prmanShadingNormal(normal N)
{
    normal  Ns = normalize(N);
    uniform float   sides = 2;
    uniform float   raydepth;

    attribute( "Sides", sides );
    rayinfo( "depth", raydepth );

    if (sides == 2 || raydepth > 0) Ns = faceforward( Ns,I,Ns );

    return Ns;
}

normal  prmanShadingNormal()
{
    extern normal   N;

    return prmanShadingNormal(N);
}


uniform float   prmanTwosided()
{
    uniform float   sides = 2;

    attribute( "Sides", sides );

    return (sides - 1);
}

uniform float   prmanDoubleshaded()
{
    uniform float   doubleshaded = 0;

    attribute( "sides:doubleshaded", doubleshaded );

    return doubleshaded;
}

// 0 = not flipped. 1 = flipped.
// This actually depends on the handedness of the scene.
// In a standard prman (left handed) world, such as Slim, this is true.
// In a Maya/RMS (right handed) world, it's the opposite:
// 0 = flipped; 1 = not flipped.
// XXX used geometry:backfacing before. May not behave this way now.
uniform float   prmanOrientation()
{
    uniform float   orientation = 0;

    // Get the sign of the orientation bit.
    // 0 means we're not flipping the normal.
    // orientation = step ((dPdv^dPdu).Ng, 0);
    attribute( "geometry:frontfacing", orientation );
    orientation = 1 - orientation;

    return orientation;
}


// Which geometic face is this?
// 0 = back. 1 = front.
// XXX Left camera or traced ray only.
float   prmanFace()
{
    extern normal   Ng;
    extern vector   I;

    varying float   face;

    face = step( I.Ng, 0 ); // Not handedness dependent.
    return face;
}


// Which shading side of the surface is this?
// 0 = back. 1 = front.
// The sense of this is also flipped depending on the
// handedness of the world, just like prmanOrientation().
// XXX Note prmanOrientation() now uses 1-geometry:frontfacing - may not behave the same.
float   prmanSide()
{
    extern vector   dPdu, dPdv;
    extern normal   Ng;

    varying float   orientation, face;

    orientation = prmanOrientation();

    if (prmanDoubleshaded() != 0)
    {
        return orientation;
    }
    else
    {
        face = prmanFace();
        return linear( face, 1-face, orientation );
    }
}

// This attribute is explicitly varying, and allows one to consistently determine
// whether the point being shaded is intended to be used for the outside or inside
// of a surface. It works consistently whether or not ray tracing is being used,
// whether double-shading is used, or whether or not the current shading context
// is being used in a caching context. Moreover, it can be used without double-shading
// when the geometry is two sided.
varying float   prmanOutside()
{
    varying float   outside;
    attribute( "geometry:outside", outside );

    return outside;
}

#endif
