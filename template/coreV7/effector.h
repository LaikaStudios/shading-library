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

#ifndef coreV7effector_h
#define coreV7effector_h

#include <stdrsl/RadianceSample.h>

#include <color.h>

#define effector_s coreV7effector_s
struct  coreV7effector_s
{
    // Standard Parameters
    public  varying float   Enable = 1;
    public  varying color   Color = color 0;
    public  varying float   Intensity = 1;
    public  varying color   DiffuseTint = color 1;
    public  varying float   DiffuseIntensity = 1;
    public  varying color   SpecularTint = color 1;
    public  varying float   SpecularIntensity = 1;
    public  uniform float   Invert = 0;

    private varying float   _enable = 1;


    public  void begin()
    {
        _enable = Enable;
    }

    // Returns a mix percentange: not light linear.
    private float _effectField( float field )
    {
        return ( _enable * mix( field, 1-field, Invert ));
    }

    // Input parameters are sRGB, so result is too.
    private color _effectColor()
    {
        varying color   effectColor = Color * Intensity;
        {
            uniform string  pipelinestage;
            shaderinfo( "pipelinestage", pipelinestage );

            if (pipelinestage == "diffuselighting")
            {
                effectColor *= DiffuseTint * DiffuseIntensity;
            }

            else if (pipelinestage == "specularlighting")
            {
                effectColor *= SpecularTint * SpecularIntensity;
            }
        }
        return effectColor;
    }

    public  color effectorTint( float field )
    {
        varying float   effectField = _effectField( field );
        varying color   effectColor = _effectColor();

        varying color   result = colorSRGBToLinear( mix( color 1, effectColor, effectField ));

        return result;
    }

    public  color effectorTint( float field; float enable )
    {
        varying float   effectField = _effectField( field );
        varying color   effectColor = _effectColor();

        varying color   result = colorSRGBToLinear( mix( color 1, effectColor, enable*effectField ));

        return result;
    }

    public  color effectorTint( float field; color tint )
    {
        varying float   effectField = _effectField( field );
        varying color   effectColor = _effectColor();

        varying color   result = colorSRGBToLinear( mix( color 1, tint*effectColor, effectField ));

        return result;
    }

    public  color effectorTint( float field; float enable; color tint )
    {
        varying float   effectField = _effectField( field );
        varying color   effectColor = _effectColor();

        varying color   result = colorSRGBToLinear( mix( color 1, tint*effectColor, enable*effectField ));

        return result;
    }
}

#endif
