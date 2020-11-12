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
#ifndef effectorV7OrthoCard_h
#define effectorV7OrthoCard_h

#include <stdrsl/RadianceSample.h>

#include "coreV7/light.h"
#include "coreV7/effector.h"

#include <sl.h>
#include <raytrace.h>

struct  effectorV7OrthoCard_s : effector_s
{
    public  uniform float   TopAttenEnable = 1;
    public  varying float   TopAttenStart = 1;
    public  varying float   TopAttenEnd = 2;
    public  uniform float   BotAttenEnable = 1;
    public  varying float   BotAttenStart = 1;
    public  varying float   BotAttenEnd = 2;
    public  uniform float   LeftAttenEnable = 1;
    public  varying float   LeftAttenStart = 1;
    public  varying float   LeftAttenEnd = 2;
    public  uniform float   RightAttenEnable = 1;
    public  varying float   RightAttenStart = 1;
    public  varying float   RightAttenEnd = 2;
    public  uniform string  TextureMap = "";
    public  uniform string  TextureMode = "";
    public  varying float   TextureBlur = 0;
    public  uniform string  Space = "shader";

    private uniform string  _space = "";
    private uniform float   _topBotEnable = 1;
    private uniform float   _leftRightEnable = 1;
    private varying float   _width = 1;
    private varying float   _height = 1;


    public  void begin()
    {
        effector_s::begin();

        _space = (Space != "") ? Space : "shader";

        _leftRightEnable = max( LeftAttenEnable, RightAttenEnable );
        _topBotEnable = max( TopAttenEnable, BotAttenEnable );

        _width = LeftAttenEnd + RightAttenEnd;
        _height = TopAttenEnd + BotAttenEnd;
    }

    private color _sampleTint( varying point Q )
    {
        varying float   field = 1;
        {
            if (RightAttenEnable+LeftAttenEnable+TopAttenEnable+BotAttenEnable == 0)
            {
                field = 1;
            }
            else
            {
                varying float   lf, rf, tf, bf, lrf, tbf;

                lf = 1-smoothstep( LeftAttenStart, LeftAttenEnd, -xcomp(Q) );
                rf = 1-smoothstep( RightAttenStart, RightAttenEnd, xcomp(Q) );
                tf = 1-smoothstep( TopAttenStart, TopAttenEnd, ycomp(Q) );
                bf = 1-smoothstep( BotAttenStart, BotAttenEnd, -ycomp(Q) );

                lrf = mix( rf*RightAttenEnable, lf - (1-rf)*RightAttenEnable, LeftAttenEnable );
                tbf = mix( bf*BotAttenEnable, tf - (1-bf)*BotAttenEnable, TopAttenEnable );

                if      (_leftRightEnable == 1 && _topBotEnable == 0) field = lrf;
                else if (_leftRightEnable == 0 && _topBotEnable == 1) field = tbf;
                else    field = lrf * tbf;
            }
        }

        varying color   texColor = color 1;
        {
            if (TextureMap != "") {
                varying float   texAlpha;

                varying float   uval = (xcomp(Q)+1) / 2;
                varying float   vval = 1 - ((ycomp(Q)+1) / 2);

                texColor = texture( TextureMap, uval, vval, "blur", TextureBlur );
                if (TextureMode != "sRGB")
                {
                    texColor = colorLinearToSRGB( texColor );
                }
                texAlpha = texture( TextureMap[3], uval, vval, "blur", TextureBlur, "fill", 1.0 );

                texColor += color( 1-texAlpha );    // Set the surround to white.
            }
        }

        return effectorTint( field, texColor );
    }

    public  void radianceEffect( output light_s _light )
    {
        varying point   Q = transform( _space, _light->Ps );

        // Apply this effector's tint to the light.
        _light->Color *= _sampleTint( Q );
    }
}

#endif
