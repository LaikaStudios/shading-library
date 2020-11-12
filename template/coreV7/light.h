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
#ifndef coreV7light_h
#define coreV7light_h

#include <stdrsl/RadianceSample.h>

#include <slim.h>

#define light_s coreV7light_s
struct  coreV7light_s
{
    public  varying point   Ps = point 0;   // Surface point being lit. P in a geolight is the light surface.
    public  varying point   Po = point 0;   // Light location (origin of light source coordinate system).
    public  varying point   Pl = point 0;   // The light surface in a geo (area) light.

    public  varying color   Color = color 1;    // linear.
    public  varying float   Intensity = 1;      // linear.
    public  varying float   Luma = 1;           // linear. Intensity*luma(Color).


    public void Init() {
        Ps = SLIM_P;            // Always the surface point.
        Po = point "shader" 0;  // Light coordinate system origin.
        Pl = P;                 // Light surface point.

        Color = color 1;
        Intensity = 1;
        Luma = 1;
    }

    public void Init( varying point _Po ) {
        Init();
        Po = _Po;
    }

    public void begin() {
        Init();
    }
}


/*
 *  Some helper functions for handling light effectors.
 */

/*
 *  Returns light, coshader, and bound array effectors that are of the desired __category type:
 *  Laika shading system compatible.
 */
void getTypeEffectorList
(
        uniform string  typeCategory;   // What __category shaders to return.
        uniform shader  boundCoshaders[];

        output uniform shader   effectors[];
) {
    uniform float   i;

    effectors = getlights( "category", typeCategory );
    uniform float   effectorCount = arraylength( effectors );

    uniform shader  shadereffectors[] = getshaders( "category", typeCategory );
    uniform float   shadereffectorCount = arraylength( shadereffectors );

    if (shadereffectorCount > 0)
    {
        effectorCount += shadereffectorCount;
        reserve( effectors, effectorCount );

        for (i=0; i < shadereffectorCount; i+=1)
        {
            push( effectors, shadereffectors[i] );
        }
    }

    /* boundCoshaders array shaders have to be filtered for __category type. */
    uniform float   boundCount = arraylength( boundCoshaders );

    if (boundCount > 0)
    {
        uniform string  currentCategory;

        for (i=0; i < boundCount; i+=1)
        {
            getvar( boundCoshaders[i], "__category", currentCategory );
            if (match( typeCategory, currentCategory ))
            {
                push( effectors, boundCoshaders[i] );
            }
        }
        effectorCount = arraylength( effectors );
    }
}

/*
 *  Prunes the set of effectors to those whose user categories match the light's user categories.
 */
void getUserEffectorList
(
        uniform string          lightCategory[];
        output uniform shader   lightEffector[];
) {
    uniform string  effectorCategories; // Comma-separated list.
    uniform float   lightCategoryNum, lightEffectorNum, i, j, k;

    lightCategoryNum = arraylength( lightCategory );
    lightEffectorNum = arraylength( lightEffector );

    k = 0;
    for (i=0; i < lightEffectorNum; i+=1)
    {
        effectorCategories = lightEffector[i]->__categoryUser;

        for (j=0; j < lightCategoryNum; j+=1)
        {
            if (match( lightCategory[j], effectorCategories ))
            {
                lightEffector[k] = lightEffector[i];
                k += 1;
                break;
            }
        }
    }

    resize( lightEffector, k );
}

/*  Both in one. */
void getEffectorList
(
        uniform string  typeCategory;
        uniform string  userCategory[];
        uniform shader  boundCoshaders[];

        output uniform shader   effectors[];
) {
    getTypeEffectorList( typeCategory, boundCoshaders, effectors );
    getUserEffectorList( userCategory, effectors );
}


/*
 *  Radiance effects are multiplicative on the light color.
 *  Their effect is accumulated once, and then used to scale the light sample radiances.
 *  They do not vary by sample.
 */
varying color accumulateRadianceEffects( uniform shader effectors[]; output light_s _light )
{
    uniform float   i, numEffectors = arraylength( effectors );

    for (i=0; i < numEffectors; i+=1)
    {
        effectors[i]->radianceEffect( _light );
    }

    return _light->Color;
}

void applyRadianceEffects(
        uniform shader  effectors[];
        output light_s  _light;

        output __radiancesample samples[];
) {
    varying color   radianceEffects = accumulateRadianceEffects( effectors, _light );

    uniform float   i, numSamples = arraylength( samples );

    for (i=0; i < numSamples; i+=1)
    {
        samples[i]->radiance *= radianceEffects;
    }
}

/*
 *  Shadow effects are applied to each shadow sample, as they can vary per sample.
 */
void applyShadowEffects(
        uniform shader  effectors[];
        output light_s _light;

        output __radiancesample samples[];
) {
    uniform float   i, numEffectors = arraylength( effectors );

    for (i=0; i < numEffectors; i+=1)
    {
        effectors[i]->shadowEffect( _light, samples );
    }
}

#endif
