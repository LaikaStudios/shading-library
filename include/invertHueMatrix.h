/*
 *  Copyright(c) 2011 Laika, developed under contract by mitchpraterLLC.
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
 *  The derivation of a luma-preserving hue inversion matrix based on
 *  http://www.graficaobscura.com/matrix/index.html
 *
 *  lumaCoefficients are based on the RGB to luma linear transformation.
 *  These are typically based either on the BT.601 or BT.709 definitions:
 *      BT.601 = (.298939, .586625, .114436)
 *      BT.709 = (.212671, .715160, .072169)
 *
 *  The resulting matrix is used to invert an RGB color's hue:
 *      cHueInverted = color transform (invertHueMatrix(...), point c);
 */
#ifndef invertHueMatrix_H
#define invertHueMatrix_H

uniform matrix  invertHueMatrix (
    uniform point   lumaCoefficients;
) {
    uniform matrix  invertHue = matrix 1;

    // Rotate the r=g=b diagonal onto the z axis.
    invertHue = rotate (invertHue, atan(sqrt(2),1), vector(1,-1,0));

    // Shear in x and y to put the luma vector onto the z axis.
    uniform point   lumaShear = transform (invertHue, lumaCoefficients);
    uniform float   zx = lumaShear[0] / lumaShear[2];
    uniform float   zy = lumaShear[1] / lumaShear[2];
    uniform matrix  zShear = (1, 0, zx, 0, 0, 1, zy, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    uniform matrix  zUnShear = (1, 0, -zx, 0, 0, 1, -zy, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    invertHue = invertHue * zShear;

    // Invert the hue.
    invertHue = rotate (invertHue, PI, vector(0,0,1));

    // Undo the setup transformations.
    invertHue = invertHue * zUnShear;
    invertHue = rotate (invertHue, atan(sqrt(2),1), vector(-1,1,0));

    return invertHue;
}

#endif
