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

/*
 *  Return a value, average value, and alpha from the texture.
 */

#ifndef texture_h
#define texture_h

#include <color.h>

/*
 *  texture() access function.
 */
void textureRead (
    uniform string  _file;
    uniform float   _channel;
    varying float   _s, _t;
    uniform float   _usefilterregions;
    uniform float   _ignoremissing;
    uniform string  _filter;
    varying float   _blur;
    uniform float   _lerp;

    output varying color    outValue;
    output varying color    outAverage;
    output varying float    outAlpha;
) {
    if (_ignoremissing)
    {
        uniform float   dummy, exists;
        exists = textureinfo( _file, "exists", dummy );

        if (exists == 0) return;
    }

    varying float   one = 1.0;

    if (_usefilterregions)
    {
        filterregion    fr;
        fr->calculate2d( _s, _t );

        if (_channel < 4)
        {
            outAverage = float texture( _file[_channel], fr, "blur", one );
            outValue = float texture( _file[_channel], fr, "filter", _filter, "blur", _blur, "lerp", _lerp );
        }
        else
        {
            outAverage = color texture( _file, fr, "blur", one );
            outValue = color texture( _file, fr, "filter", _filter, "blur", _blur, "lerp", _lerp );
        }

        outAlpha = float texture( _file[3], fr, "filter", _filter, "blur", _blur, "lerp", _lerp, "fill", 1.0 );
    }

    else
    {
        if (_channel < 4)
        {
            outAverage = float texture( _file[_channel], _s, _t, "blur", one );
            outValue = float texture( _file[_channel], _s, _t, "filter", _filter, "blur", _blur, "lerp", _lerp );
        }
        else
        {
            outAverage = color texture( _file, _s, _t, "blur", one );
            outValue = color texture( _file, _s, _t, "filter", _filter, "blur", _blur, "lerp", _lerp );
        }

        outAlpha = float texture( _file[3], _s, _t, "filter", _filter, "blur", _blur, "lerp", _lerp, "fill", 1.0 );
    }
}


/*
 *  textureatlas() access function.
 */
void textureatlasRead (
    uniform string  _file;
    uniform string  _type;
    uniform float   _channel;
    varying float   _s, _t;
    uniform float   _usefilterregions;
    uniform float   _ignoremissing;
    uniform string  _filter;
    varying float   _blur;

    output varying color    outValue;
    output varying color    outAverage;
    output varying float    outAlpha;
) {
    varying float   one = 1.0;

    outAverage = color textureatlas( _file, _type, _s, _t,
        "usefilterregions", _usefilterregions,
        "blur", one,
        "sparse", _ignoremissing
    );
    outValue = color textureatlas( _file, _type, _s, _t,
        "usefilterregions", _usefilterregions,
        "filter", _filter,
        "blur", _blur,
        "sparse", _ignoremissing
    );

    if (_channel < 3)
    {
        outAverage = outAverage[_channel];
        outValue = outValue[_channel];
    }
}


/*
 *  ptexture() access function.
 */
void ptextureRead (
    uniform string  _file;
    uniform float   _channel;
    uniform float   _ignoremissing;
    uniform string  _filter;
    varying float   _blur;
    uniform float   _lerp;

    output varying color    outValue;
    output varying color    outAverage;
    output varying float    outAlpha;
) {
    if (_ignoremissing)
    {
        uniform float   dummy, exists;
        exists = textureinfo( _file, "exists", dummy );

        if (exists == 0) return;
    }

    varying float   one = 1.0;
    varying float   faceindex = 0;
    readprimvar( "__faceindex", faceindex );

    if (_channel < 4)
    {
        outAverage = float ptexture( _file, _channel, faceindex, "blur", one );
        outValue = float ptexture( _file, _channel, faceindex, "filter", _filter, "blur", _blur, "lerp", _lerp );
    }
    else
    {
        outAverage = color ptexture( _file, 0, faceindex, "blur", one );
        outValue = color ptexture( _file, 0, faceindex, "filter", _filter, "blur", _blur, "lerp", _lerp );
    }

    outAlpha = float ptexture( _file, 3, faceindex, "filter", _filter, "blur", _blur, "lerp", _lerp, "fill", 1.0 );
}

#endif
