/*
 *  Copyright 2018 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
/*
    Defines the vstruct connection made with the PxrSurface bxdf.
*/
#ifndef PXRSURFACE_ADAPTER_H
#define PXRSURFACE_ADAPTER_H

#include "material/PxrSurface.h"


/**************************************************************************************************
 *  vstruct Output Parameters
 **************************************************************************************************/

//
//  These macros examine the vstruct Enable parameters to determine what is connected
//  to the bxdf node.
//
#define OUTPUT_VSTRUCT_ENABLE(TYPE,PREFIX,INPUT,PARAM,OUTPUT,NAME,ENABLE) \
    output TYPE PREFIX##PARAM = TYPE(0.0) \
    [[ \
        string widget = "null", \
        string vstructmember = #OUTPUT "." #NAME, \
        string vstructConditionalExpr = "connect if " #INPUT "_" #PREFIX #ENABLE " == 1" \
    ]]

// The vstructmember uses the same prefix as the input material struct.
#define OUTPUT_VSTRUCT_IMPLICIT(TYPE,PREFIX,INPUT,PARAM,OUTPUT,NAME) \
    output TYPE PREFIX##PARAM = TYPE(0.0) \
    [[ \
        string widget = "null", \
        string vstructmember = #OUTPUT "." #PREFIX #NAME, \
        string vstructConditionalExpr = "connect if " #INPUT "_" #PREFIX "Enable == 1" \
    ]]

// The vstructmember name does not use the same prefix as the input material struct.
#define OUTPUT_VSTRUCT_EXPLICIT(TYPE,PREFIX,INPUT,PARAM,OUTPUT,NAME) \
    output TYPE PREFIX##PARAM = TYPE(0.0) \
    [[ \
        string widget = "null", \
        string vstructmember = #OUTPUT "." #NAME, \
        string vstructConditionalExpr = "connect if " #INPUT "_" #PREFIX "Enable == 1" \
    ]]

//
// Gain parameters are set to 1 or 0 depending on whether the response is enabled or not.
// Their corresponding color value is used to control the actual response intensity.
// This is done to avoid hue aliasing when two material layers are blended and one has
// a low gain value.
//
#define OUTPUT_VSTRUCT_SET_GAIN(INPUT,OUTPUT,PREFIX,NAME) \
    output float PREFIX##NAME = 0.0 \
    [[ \
        string widget = "null", \
        string vstructmember = #OUTPUT "." #PREFIX #NAME, \
        string vstructConditionalExpr = "set 1.0 if " #INPUT "_" #PREFIX "Enable == 1 else set 0.0" \
    ]]

// Some vstructmember names do not follow the proper naming conventions.
#define OUTPUT_VSTRUCT_SET_GAIN_EXPLICIT(INPUT,OUTPUT,PREFIX,NAME) \
    output float PREFIX##NAME = 0.0 \
    [[ \
        string widget = "null", \
        string vstructmember = #OUTPUT "." #NAME, \
        string vstructConditionalExpr = "set 1.0 if " #INPUT "_" #PREFIX "Enable == 1 else set 0.0" \
    ]]


#define CLEARCOAT_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, EdgeCG,             OUTPUT, EdgeColor ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, FaceCG,             OUTPUT, FaceColor ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, FresnelExponent,    OUTPUT, FresnelShape ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, RefractionCI,       OUTPUT, Ior ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, LayerThickness,     OUTPUT, Thickness ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, LayerColor,         OUTPUT, AborptionTint ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, Roughness ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Anisotropy,         OUTPUT, Anisotropy ), \
    OUTPUT_VSTRUCT_ENABLE( normal, PREFIX, INPUT, ShadingNormal,  OUTPUT, clearcoatBumpNormal, ShadingNormalEnable ), \
    OUTPUT_VSTRUCT_ENABLE( vector, PREFIX, INPUT, ShadingTangent, OUTPUT, clearcoatAnisotropyDirection, ShadingTangentEnable )

#define SPECPRIMARY_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, EdgeCG,             OUTPUT, specularEdgeColor ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, FaceCG,             OUTPUT, specularFaceColor ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, FresnelExponent,    OUTPUT, specularFresnelShape ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, RefractionCI,       OUTPUT, specularIor ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, ExtinctionCC,       OUTPUT, specularExtinctionCoeff ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, specularRoughness ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, Anisotropy,         OUTPUT, specularAnisotropy ), \
    OUTPUT_VSTRUCT_ENABLE( normal, PREFIX, INPUT, ShadingNormal,  OUTPUT, specularBumpNormal, ShadingNormalEnable ), \
    OUTPUT_VSTRUCT_ENABLE( vector, PREFIX, INPUT, ShadingTangent, OUTPUT, specularAnisotropyDirection, ShadingTangentEnable )

#define SPECSECONDARY_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, EdgeCG,             OUTPUT, roughSpecularEdgeColor ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, FaceCG,             OUTPUT, roughSpecularFaceColor ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, FresnelExponent,    OUTPUT, roughSpecularFresnelShape ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, RefractionCI,       OUTPUT, roughSpecularIor ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, ExtinctionCC,       OUTPUT, roughSpecularExtinctionCoeff ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, roughSpecularRoughness ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, Anisotropy,         OUTPUT, roughSpecularAnisotropy ), \
    OUTPUT_VSTRUCT_ENABLE( normal, PREFIX, INPUT, ShadingNormal,  OUTPUT, roughSpecularBumpNormal, ShadingNormalEnable ), \
    OUTPUT_VSTRUCT_ENABLE( vector, PREFIX, INPUT, ShadingTangent, OUTPUT, roughSpecularAnisotropyDirection, ShadingTangentEnable )

#define IRIDESCENCE_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, FaceGain,           OUTPUT, FaceGain ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, EdgeGain,           OUTPUT, EdgeGain ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, FaceColor,          OUTPUT, PrimaryColor ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, EdgeColor,          OUTPUT, SecondaryColor ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Exponent,           OUTPUT, Curve ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Repetitions,        OUTPUT, Scale ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Thickness,          OUTPUT, Thickness ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, Roughness ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Anisotropy,         OUTPUT, Anisotropy ), \
    OUTPUT_VSTRUCT_ENABLE( vector, PREFIX, INPUT, ShadingTangent, OUTPUT, iridescenceAnisotropyDirection, ShadingTangentEnable )

#define FUZZ_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_SET_GAIN( INPUT, OUTPUT, PREFIX, Gain ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, CG,                 OUTPUT, Color ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, ConeAngle,          OUTPUT, ConeAngle ), \
    OUTPUT_VSTRUCT_ENABLE( normal, PREFIX, INPUT, ShadingNormal,  OUTPUT, fuzzBumpNormal, ShadingNormalEnable )

#define DIFFUSE_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_SET_GAIN( INPUT, OUTPUT, PREFIX, Gain ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, CG,                 OUTPUT, Color ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, Roughness ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, FalloffExponent,    OUTPUT, Exponent ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, BackfaceCG,         OUTPUT, BackColor ), \
    OUTPUT_VSTRUCT_SET_GAIN( INPUT, OUTPUT, PREFIX, TransmitGain ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, TransmitCG,         OUTPUT, TransmitColor ), \
    OUTPUT_VSTRUCT_ENABLE( normal, PREFIX, INPUT, ShadingNormal,  OUTPUT, diffuseBumpNormal, ShadingNormalEnable )

#define SCATTER_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_SET_GAIN_EXPLICIT( INPUT, OUTPUT, PREFIX, singlescatterGain ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, CG,                 OUTPUT, singlescatterColor ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, PathLength,         OUTPUT, singlescatterMfp ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, PathColor,          OUTPUT, singlescatterMfpColor ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, Direction,          OUTPUT, singlescatterDirectionality ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, singlescatterBlur ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, RefractionIndex,    OUTPUT, singlescatterIor ), \
    OUTPUT_VSTRUCT_SET_GAIN_EXPLICIT( INPUT, OUTPUT, PREFIX, singlescatterDirectGain ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, BacksideCG,         OUTPUT, singlescatterDirectGainTint )

#define SUBSURFACE_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_SET_GAIN( INPUT, OUTPUT, PREFIX, Gain ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, CG,                 OUTPUT, Color ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, PathLength,         OUTPUT, Dmfp ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, PathColor,          OUTPUT, DmfpColor ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, PostTint,           OUTPUT, PostTint ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, TransmitGain,       OUTPUT, TransmitGain ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, ShortLength,        OUTPUT, shortSubsurfaceDmfp ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, ShortCG,            OUTPUT, shortSubsurfaceColor ), \
    OUTPUT_VSTRUCT_SET_GAIN_EXPLICIT( INPUT, OUTPUT, PREFIX, shortSubsurfaceGain ), \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, LongLength,        OUTPUT, longSubsurfaceDmfp ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, LongCG,            OUTPUT, longSubsurfaceColor ), \
    OUTPUT_VSTRUCT_SET_GAIN_EXPLICIT( INPUT, OUTPUT, PREFIX, longSubsurfaceGain ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Directionality,     OUTPUT, Directionality ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, DiffuseBlend,       OUTPUT, DiffuseBlend ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Bleed,              OUTPUT, Bleed )

#define GLASS_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_EXPLICIT( float,  PREFIX, INPUT, ReflectionGain,     OUTPUT, reflectionGain ), \
    OUTPUT_VSTRUCT_SET_GAIN_EXPLICIT( INPUT, OUTPUT, PREFIX, refractionGain ), \
    OUTPUT_VSTRUCT_EXPLICIT( color,  PREFIX, INPUT, RefractionCG,       OUTPUT, refractionColor ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, RefractionIndex,    OUTPUT, Ior ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Roughness,          OUTPUT, Roughness ), \
    OUTPUT_VSTRUCT_IMPLICIT( float,  PREFIX, INPUT, Anisotropy,         OUTPUT, Anisotropy ), \
    OUTPUT_VSTRUCT_ENABLE( vector, PREFIX, INPUT, ShadingTangent, OUTPUT, glassAnisotropyDirection, ShadingTangentEnable )

#define GLOW_OUTPUT_VSTRUCT(INPUT,OUTPUT,PREFIX) \
    OUTPUT_VSTRUCT_SET_GAIN( INPUT, OUTPUT, PREFIX, Gain ), \
    OUTPUT_VSTRUCT_IMPLICIT( color,  PREFIX, INPUT, CG,                 OUTPUT, Color )

//  Copy input struct member value to output vstruct parameter.
#define COPY_PARAM(INPUT,PREFIX,NAME) \
        PREFIX##NAME = INPUT.PREFIX##NAME

#endif
