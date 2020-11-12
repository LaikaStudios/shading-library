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
#ifndef rgba_h
#define rgba_h

#include <color.h>

struct rgba_s {
    public  color   rgb = color 0;
    public  color   a = color 0;

    public  float GetR() { return rgb[0]; }
    public  float GetG() { return rgb[1]; }
    public  float GetB() { return rgb[2]; }
    public  color GetRGB() { return rgb; }
    public  float GetAvg() { return colorRGBToAvg( rgb ); }
    public  color GetAlpha() { return a; }
    public  float GetAlpha() { return a[0]; }
}

#endif
