/*
 *  Copyright 2018-2020 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
/*
    Defines the vstruct connection made with the PxrSurface bxdf.
*/
#ifndef MATERIAL_V2_PXRSURFACEADAPTOR_H
#define MATERIAL_V2_PXRSURFACEADAPTOR_H

#include "material/v2_PxrSurface.h"


//
//  Copy input struct member to the corresponding output parameter.
//
#define COPY_PARAM(VSTRUCTIN,PREFIX,NAME) \
        PREFIX##_##NAME = VSTRUCTIN.PREFIX##_##NAME

/**************************************************************************************************
 *  vstruct Output Parameters
 **************************************************************************************************/

//
//  These macros examine the vstruct Enable parameters to determine what is connected
//  to the bxdf node.
//
#define PARAM_OUTPUT(VSTRUCTIN,VSTRUCTOUT,PREFIX,TYPE,PARAM,VSTRUCTMEMBER) \
    output TYPE PREFIX##_##PARAM = TYPE(0.0) \
    [[ \
        string widget = "null", \
        string vstructmember = #VSTRUCTOUT "." #VSTRUCTMEMBER, \
        string vstructConditionalExpr = "connect if " #VSTRUCTIN "_" #PREFIX "_Enable == 1" \
    ]]

#define PARAM_OUTPUT_ENABLE(VSTRUCTIN,VSTRUCTOUT,PREFIX,TYPE,PARAM,VSTRUCTMEMBER) \
    output TYPE PREFIX##_##PARAM = TYPE(0.0) \
    [[ \
        string widget = "null", \
        string vstructmember = #VSTRUCTOUT "." #VSTRUCTMEMBER, \
        string vstructConditionalExpr = "connect if " #VSTRUCTIN "_" #PREFIX "_" #PARAM "_Enable == 1" \
    ]]

//
// Gain parameters are set to 1 or 0 depending on whether the response is enabled or not.
// Their corresponding color value is used to control the actual response intensity.
// This is done to avoid hue aliasing when two material layers are blended and one has
// a low gain value.
//
#define PARAM_OUTPUT_SET_GAIN(VSTRUCTIN,VSTRUCTOUT,PREFIX,PARAM,VSTRUCTMEMBER) \
    output float PREFIX##_##PARAM = 0.0 \
    [[ \
        string widget = "null", \
        string vstructmember = #VSTRUCTOUT "." #VSTRUCTMEMBER, \
        string vstructConditionalExpr = "set 1.0 if " #VSTRUCTIN "_" #PREFIX "_Enable == 1 else set 0.0" \
    ]]


#define CLEARCOAT_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, clearcoatEdgeColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, FaceCG, clearcoatFaceColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, FresnelExponent, clearcoatFresnelShape ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, RefractionCI, clearcoatIor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, LayerThickness, clearcoatThickness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, LayerColor, clearcoatAborptionTint ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, clearcoatRoughness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Anisotropy, clearcoatAnisotropy ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, clearcoatBumpNormal ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, vector, AnisotropyDirection, clearcoatAnisotropyDirection )

#define PRIMARYSPECULAR_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, specularEdgeColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, FaceCG, specularFaceColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, FresnelExponent, specularFresnelShape ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, RefractionCI, specularIor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, ExtinctionCC, specularExtinctionCoeff ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, specularRoughness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Anisotropy, specularAnisotropy ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, specularBumpNormal ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, vector, AnisotropyDirection, specularAnisotropyDirection )

#define SECONDARYSPECULAR_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, roughSpecularEdgeColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, FaceCG, roughSpecularFaceColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, FresnelExponent, roughSpecularFresnelShape ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, RefractionCI, roughSpecularIor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, ExtinctionCC, roughSpecularExtinctionCoeff ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, roughSpecularRoughness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Anisotropy, roughSpecularAnisotropy ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, roughSpecularBumpNormal ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, vector, AnisotropyDirection, roughSpecularAnisotropyDirection )

#define IRIDESCENCE_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, FaceGain, iridescenceFaceGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, EdgeGain, iridescenceEdgeGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, FaceColor, iridescencePrimaryColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, EdgeColor, iridescenceSecondaryColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Exponent, iridescenceCurve ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Repetitions, iridescenceScale ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Thickness, iridescenceThickness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, iridescenceRoughness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Anisotropy, iridescenceAnisotropy ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, iridescenceBumpNormal ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, vector, AnisotropyDirection, iridescenceAnisotropyDirection )

#define FUZZ_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, Gain, fuzzGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, fuzzColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, ConeAngle, fuzzConeAngle ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, fuzzBumpNormal )

#define DIFFUSE_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, Gain, diffuseGain ), \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, TransmitGain, diffuseTransmitGain ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, diffuseBumpNormal ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, diffuseColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, diffuseRoughness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Exponent, diffuseExponent ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, BackfaceCG, diffuseBackColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, TransmitCG, diffuseTransmitColor )

#define SCATTER_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, Gain, singlescatterGain ), \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, DirectGain, singlescatterDirectGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, singlescatterColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, PathLength, singlescatterMfp ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, PathColor, singlescatterMfpColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Direction, singlescatterDirectionality ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, singlescatterBlur ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, RefractionIndex, singlescatterIor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, BacksideCG, singlescatterDirectGainTint )

#define SUBSURFACE_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, Gain, subsurfaceGain ), \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, ShortGain, shortSubsurfaceGain ), \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, LongGain, longSubsurfaceGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, subsurfaceColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, PathLength, subsurfaceDmfp ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, PathColor, subsurfaceDmfpColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, PostTint, subsurfacePostTint ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, TransmitGain, subsurfaceTransmitGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, ShortLength, shortSubsurfaceDmfp ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, ShortCG, shortSubsurfaceColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, LongLength, longSubsurfaceDmfp ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, LongCG, longSubsurfaceColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Directionality, subsurfaceDirectionality ), \
    /* XXX Not Really Connectable! PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, DiffuseBlend, subsurfaceDiffuseBlend ), */ \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Bleed, subsurfaceBleed )

#define GLASS_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, Gain, refractionGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, ReflectionGain, reflectionGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Roughness, glassRoughness ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, Anisotropy, glassAnisotropy ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, RefractionCG, refractionColor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, RefractionIndex, glassIor ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, float, ManifoldExplorationIOR, mwIor ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, normal, ShadingNormal, glassBumpNormal ), \
    PARAM_OUTPUT_ENABLE( VSTRUCTIN, VSTRUCTOUT, PREFIX, vector, AnisotropyDirection, glassAnisotropyDirection ), \
    \
    /* No vstructmember for ssAlbedo parameter in PxrSurface.args */ \
    output color PREFIX##_ssAlbedo = color(0.0) [[ string widget = "null" ]], \
    \
    /* No vstructmember for extinction parameter in PxrSurface.args */ \
    output color PREFIX##_extinction = color(0.0) [[ string widget = "null" ]]

#define GLOW_OUTPUT_PARAMS(VSTRUCTIN,VSTRUCTOUT,PREFIX) \
    PARAM_OUTPUT_SET_GAIN( VSTRUCTIN, VSTRUCTOUT, PREFIX, Gain, glowGain ), \
    PARAM_OUTPUT( VSTRUCTIN, VSTRUCTOUT, PREFIX, color, CG, glowColor )

#endif
