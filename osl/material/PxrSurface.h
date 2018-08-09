/*
 *  Copyright 2018 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License, Version 2.0, <LICENSE-APACHE or
 *  http://apache.org/licenses/LICENSE-2.0> or the MIT license <LICENSE-MIT or
 *  http://opensource.org/licenses/MIT>, at your option. This file may not be
 *  copied, modified, or distributed except according to those terms.
 */
/*
    Defines two components of the material layering system.
    The first is the set of user interface parameters that control the material.
    The second is the data struct passed from one layer to another.
*/
#ifndef PXRSURFACE_MATERIAL_H
#define PXRSURFACE_MATERIAL_H


/**************************************************************************************************
 *  Material UI
 **************************************************************************************************/
//
// Default values.
//
#define EnableUIDefault                         1
#define MaskUIDefault                           1.0
#define MaskBxdfSharpenUIDefault                0.0
#define MaskDispSharpenUIDefault                0.0
#define ThicknessUIDefault                      0.0
#define CoveringHeightUIDefault                 0.0
#define DispMagSpaceUIDefault                   "world"
#define DispMagScaleUIDefault                   1.0
#define BumpPercentUIDefault                    1.0

#define globalShadingNormalUIDefault            normal(0.0)
#define globalUseShadingNormalUIDefault         1.0
#define globalShadowColorUIDefault              color(0.0)

#define commonEnableUIDefault                   0
#define commonGainUIDefault                     1.0
#define commonColorUIDefault                    color(0.5)
#define commonEdgeColorUIDefault                color(0.5)
#define commonFaceColorUIDefault                color(0.0)
#define commonFresnelExponentUIDefault          5.0
#define commonRefractionIndexUIDefault          1.52
#define commonRefractionColorUIDefault          color(1.0)
#define commonExtinctionCoeffUIDefault          0.0
#define commonExtinctionColorUIDefault          color(1.0)
#define commonAnisotropyUIDefault               0.0
#define commonShadingTangentUIDefault           vector(0.0)
#define commonShadingNormalUIDefault            normal(0.0)
#define commonUseShadingNormalUIDefault         1.0

#define clearcoatEnableUIDefault                commonEnableUIDefault
#define clearcoatGainUIDefault                  commonGainUIDefault
#define clearcoatEdgeColorUIDefault             commonEdgeColorUIDefault
#define clearcoatFaceColorUIDefault             commonFaceColorUIDefault
#define clearcoatFresnelExponentUIDefault       commonFresnelExponentUIDefault
#define clearcoatRefractionIndexUIDefault       commonRefractionIndexUIDefault
#define clearcoatRefractionColorUIDefault       commonRefractionColorUIDefault
#define clearcoatLayerThicknessUIDefault        0.0
#define clearcoatLayerColorUIDefault            color(0.0)
#define clearcoatRoughnessUIDefault             0.0
#define clearcoatAnisotropyUIDefault            commonAnisotropyUIDefault
#define clearcoatShadingTangentUIDefault        commonShadingTangentUIDefault
#define clearcoatShadingNormalUIDefault         commonShadingNormalUIDefault
#define clearcoatUseShadingNormalUIDefault      commonUseShadingNormalUIDefault

#define specPrimaryEnableUIDefault              commonEnableUIDefault
#define specPrimaryGainUIDefault                commonGainUIDefault
#define specPrimaryEdgeColorUIDefault           commonEdgeColorUIDefault
#define specPrimaryFaceColorUIDefault           commonFaceColorUIDefault
#define specPrimaryFresnelExponentUIDefault     commonFresnelExponentUIDefault
#define specPrimaryRefractionIndexUIDefault     commonRefractionIndexUIDefault
#define specPrimaryRefractionColorUIDefault     commonRefractionColorUIDefault
#define specPrimaryExtinctionCoeffUIDefault     commonExtinctionCoeffUIDefault
#define specPrimaryExtinctionColorUIDefault     commonExtinctionColorUIDefault
#define specPrimaryRoughnessUIDefault           0.2
#define specPrimaryAnisotropyUIDefault          commonAnisotropyUIDefault
#define specPrimaryShadingTangentUIDefault      commonShadingTangentUIDefault
#define specPrimaryShadingNormalUIDefault       commonShadingNormalUIDefault
#define specPrimaryUseShadingNormalUIDefault    commonUseShadingNormalUIDefault

#define specSecondaryEnableUIDefault            commonEnableUIDefault
#define specSecondaryGainUIDefault              commonGainUIDefault
#define specSecondaryEdgeColorUIDefault         commonEdgeColorUIDefault
#define specSecondaryFaceColorUIDefault         commonFaceColorUIDefault
#define specSecondaryFresnelExponentUIDefault   commonFresnelExponentUIDefault
#define specSecondaryRefractionIndexUIDefault   commonRefractionIndexUIDefault
#define specSecondaryRefractionColorUIDefault   commonRefractionColorUIDefault
#define specSecondaryExtinctionCoeffUIDefault   commonExtinctionCoeffUIDefault
#define specSecondaryExtinctionColorUIDefault   commonExtinctionColorUIDefault
#define specSecondaryRoughnessUIDefault         0.6
#define specSecondaryAnisotropyUIDefault        commonAnisotropyUIDefault
#define specSecondaryShadingTangentUIDefault    commonShadingTangentUIDefault
#define specSecondaryShadingNormalUIDefault     commonShadingNormalUIDefault
#define specSecondaryUseShadingNormalUIDefault  commonUseShadingNormalUIDefault

#define iridescenceEnableUIDefault              commonEnableUIDefault
#define iridescenceGainUIDefault                commonGainUIDefault
#define iridescenceRoughnessUIDefault           0.2
#define iridescenceFaceColorUIDefault           color(1,0,0)
#define iridescenceEdgeColorUIDefault           color(0,0,1)
#define iridescenceExponentUIDefault            1.0
#define iridescenceRepetitionsUIDefault         1.0
#define iridescenceThicknessUIDefault           800.0
#define iridescenceAnisotropyUIDefault          commonAnisotropyUIDefault
#define iridescenceShadingTangentUIDefault      commonShadingTangentUIDefault

#define fuzzEnableUIDefault                     commonEnableUIDefault
#define fuzzGainUIDefault                       commonGainUIDefault
#define fuzzColorUIDefault                      commonColorUIDefault
#define fuzzConeAngleUIDefault                  15.0
#define fuzzShadingNormalUIDefault              commonShadingNormalUIDefault
#define fuzzUseShadingNormalUIDefault           commonUseShadingNormalUIDefault

#define diffuseEnableUIDefault                  commonEnableUIDefault
#define diffuseGainUIDefault                    commonGainUIDefault
#define diffuseColorUIDefault                   commonColorUIDefault
#define diffuseRoughnessUIDefault               0.0
#define diffuseFalloffExponentUIDefault         1.0
#define diffuseBackfaceGainUIDefault            0.0
#define diffuseBackfaceColorUIDefault           commonColorUIDefault
#define diffuseTransmitGainUIDefault            0.0
#define diffuseTransmitColorUIDefault           commonColorUIDefault
#define diffuseShadingNormalUIDefault           commonShadingNormalUIDefault
#define diffuseUseShadingNormalUIDefault        commonUseShadingNormalUIDefault

#define scatterEnableUIDefault                  commonEnableUIDefault
#define scatterGainUIDefault                    commonGainUIDefault
#define scatterColorUIDefault                   color(0.830,0.791,0.753)
#define scatterPathLengthUIDefault              10.0
#define scatterPathColorUIDefault               color(0.851,0.557,0.395)
#define scatterDirectionUIDefault               0.0
#define scatterRoughnessUIDefault               0.0
#define scatterRefractionIndexUIDefault         1.3
#define scatterBacksideGainUIDefault            0.0
#define scatterBacksideTintUIDefault            color(1.0)

#define subsurfaceEnableUIDefault               commonEnableUIDefault
#define subsurfaceGainUIDefault                 commonGainUIDefault
#define subsurfaceColorUIDefault                color(0.830,0.791,0.753)
#define subsurfacePathLengthUIDefault           10.0
#define subsurfacePathColorUIDefault            color(0.851,0.557,0.395)
#define subsurfacePostTintUIDefault             color(1.0)
#define subsurfaceTransmitGainUIDefault         0.0
#define subsurfaceShortGainUIDefault            0.0
#define subsurfaceShortLengthUIDefault          1.0
#define subsurfaceShortColorUIDefault           color(0.9,0.9,0.9)
#define subsurfaceLongGainUIDefault             0.0
#define subsurfaceLongLengthUIDefault           100.0
#define subsurfaceLongColorUIDefault            color(0.8,0.0,0.0)
#define subsurfaceDirectionalityUIDefault       0.0
#define subsurfaceDiffuseBlendUIDefault         0.0
#define subsurfaceBleedUIDefault                0.0

#define ssIlluminationTintUIDefault             color(1.0)
#define ssRoughnessUIDefault                    0.0

#define glassEnableUIDefault                    commonEnableUIDefault
#define glassReflectionGainUIDefault            commonGainUIDefault
#define glassRefractionGainUIDefault            commonGainUIDefault
#define glassRefractionColorUIDefault           commonColorUIDefault
#define glassRefractionIndexUIDefault           commonRefractionIndexUIDefault
#define glassRoughnessUIDefault                 0.1
#define glassAnisotropyUIDefault                commonAnisotropyUIDefault
#define glassShadingTangentUIDefault            commonShadingTangentUIDefault

#define glowEnableUIDefault                     commonEnableUIDefault
#define glowGainUIDefault                       commonGainUIDefault
#define glowColorUIDefault                      commonColorUIDefault


//
//  General-purpose response parameter macros.
//
#define ENABLE_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    int PREFIX ## Enable = PREFIX ## EnableUIDefault \
    [[ \
        int connectable = 0, \
        string page = #PAGE, \
        string label = "Enable", \
        string widget = "checkBox", \
        string help = \
            "Turns the use of this response On/Off. " \
            "<br/><br/>" \
            #DESCRIPTION \
    ]]

#define GAIN_UI_DECLARE(PAGE,PREFIX,DESCRIPTION,DEFAULT) \
    float PREFIX ## Gain = DEFAULT \
    [[ \
        string page = #PAGE, \
        string label = "Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 2.0, \
        string help = \
            "Controls the intensity of this response. " \
            "In light linear space. " \
            "<br/><br/>" \
            #DESCRIPTION \
    ]]

#define COLOR_UI_DECLARE(PAGE,PREFIX,DESCRIPTION,DEFAULT) \
    color PREFIX ## Color = DEFAULT \
    [[ \
        string page = #PAGE, \
        string label = "Color", \
        string help = \
            #DESCRIPTION \
    ]]


//
//  General-purpose response parameter sets.
//
#define EG_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    int PREFIX ## Enable = PREFIX ## EnableUIDefault \
    [[ \
        int connectable = 0, \
        string page = #PAGE, \
        string label = "Enable", \
        string widget = "checkBox", \
        string help = \
            "Turns the use of this response On/Off. " \
            "<br/><br/>" \
            #DESCRIPTION \
    ]], \
    float PREFIX ## Gain = PREFIX ## GainUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 2.0, \
        string help = \
            "Controls the intensity of this response. " \
            "In light linear space. " \
    ]]

#define EGC_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EG_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    color PREFIX ## Color = PREFIX ## ColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Color", \
        string help = \
            "Controls the color of this response. " \
            "In light linear space. " \
    ]]

//
//  Refraction parameters.
//
#define REFRACTION_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX ## RefractionIndex = PREFIX ## RefractionIndexUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Refraction Index", \
        int slider = 1, float slidermin = 1.0, float slidermax = 4.0, float slidercenter = 1.5, \
        string help = \
            "This is actually the ratio of the refractive index of the material, " \
            "over the refractive index of the medium the camera is in. " \
            "If the camera is in air (refractive index = 1.0) this value " \
            "simply becomes the refractive index of the material. " \
            "<br/>" \
            "<br/>ice = 1.31" \
            "<br/>water = 1.333" \
            "<br/>alcohol = 1.358" \
            "<br/>human eye = 1.386" \
            "<br/>kerosene = 1.39" \
            "<br/>rock salt = 1.5" \
            "<br/>glass = 1.52" \
            "<br/>amber = 1.55" \
            "<br/>flint = 1.61" \
            "<br/>gem stones = 1.775" \
            "<br/>cubic zirconia = 2.155" \
            "<br/>diamond = 2.417" \
            "<br/>silicon = 3.46" \
            "<br/>germanium = 4.03" \
    ]], \
    color PREFIX ## RefractionColor = PREFIX ## RefractionColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Refraction Color", \
        string color_enableFilmlookVis = "False", \
        string help = \
            "Allows you to reproduce spectral Fresnel effects. " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Refraction Index</strong> to produce the " \
            "values used in the response computation. " \
    ]]

//
//  Extinction parameters.
//
#define EXTINCTION_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX ## ExtinctionCoeff = PREFIX ## ExtinctionCoeffUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Extinction Coeff", \
        int slider = 1, float slidermin = 0.0, float slidermax = 7.0, \
        string help = \
            "The metallic <strong>Extinction Coefficient</strong> is a refractive property of conductive " \
            "materials. It primarily influences their response intensity when viewed perpendicularly to " \
            "the surface (higher coefficient = brighter response). " \
            "Non-conductors (dielectrics) have an extinction coefficient of 0. " \
            "<br/><br/>" \
            "Some values for polished (non-oxidized) materials:" \
            "<br/>silicon = 0.004" \
            "<br/>germanium = 1.68" \
            "<br/>gold = 2.78" \
            "<br/>copper = 2.97" \
            "<br/>iron = 3.0" \
            "<br/>brass = 3.14" \
            "<br/>lead = 3.44" \
            "<br/>silver = 3.47" \
            "<br/>titanium = 3.6" \
            "<br/>platinum = 3.93" \
            "<br/>mercury = 4.92" \
            "<br/>aluminium = 6.79" \
    ]], \
    color PREFIX ## ExtinctionColor = PREFIX ## ExtinctionColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Extinction Color", \
        string color_enableFilmlookVis = "False", \
        string help = \
            "Determines the spectral characteristics of the extinction component of the response " \
            "(i.e. the color). " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Extinction Coefficient</strong> to produce the " \
            "values used in the response computation. " \
    ]]

//
//  Anisotropy parameters.
//
#define ANISOTROPY_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX ## Anisotropy = PREFIX ## AnisotropyUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Anisotropy", \
        int slider = 1, float slidermin = -1.0, float slidermax = 1.0, \
        string help = \
            "At 0, the response is round, and is completely determined by the <strong>Roughness</strong> " \
            "value. Deviations from 0 will <em>decrease</em> the applied Roughness along one axis, " \
            "sharpening the response in that direction. " \
            "When <strong>Anisotropy</strong> is -1 or +1, the applied Roughness value becomes 0. " \
            "As such, <strong>Roughness</strong> must be reasonably large for this to produce any effect. " \
            "Positive and negative <strong>Anisotropy</strong> values will decrease the Roughness, " \
            "and therefore the response size, along axes that are perpendicular to each other. " \
    ]], \
    vector PREFIX ## ShadingTangent = PREFIX ## ShadingTangentUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Anisotropy Direction", \
        string help = \
            "When connected, this (surface tangent) vector will determine the orientation of the " \
            "<strong>Anisotropy</strong> axes." \
            "<br/><br/>" \
            "Note: though it is intended to be, it is not <em>strictly</em> necessary that this vector " \
            "actually be tangent to the surface. " \
    ]]

//
//  Shading normal parameters.
//
#define SHADINGNORMAL_UI_DECLARE(PAGE,PREFIX) \
    normal PREFIX ## ShadingNormal = PREFIX ## ShadingNormalUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Shading Normal", \
        string help = \
            "An alternate shading normal to use for this response. " \
            "Supersedes the Global Shading Normal if one is specified. " \
    ]], \
    float PREFIX ## UseShadingNormal = PREFIX ## UseShadingNormalUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Use Shading Normal", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "How much of the connected <strong>Shading Normal</strong> is used? " \
            "This is a mix between the regular (potentially displaced) shading normal <strong>N</strong>, " \
            "or the Global Shading Normal if one is specified, " \
            "and this response's <strong>Shading Normal</strong> value. " \
            "<br/><br/>" \
            "Note: this has no effect if this response's <strong>Shading Normal</strong> is unconnected. " \
    ]]

//
//  Specular response parameter set:
//      Enable, Gain, EdgeColor, FaceColor, FresnelExponent, RefractionIndex, and RefractionColor.
//
#define EGEFFRR_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EG_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    color PREFIX ## EdgeColor = PREFIX ## EdgeColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Edge Color", \
        string help = \
            "Controls the color of this response at the silhouette edge. " \
            "In light linear space. " \
    ]], \
    color PREFIX ## FaceColor = PREFIX ## FaceColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Artistic) Face Color", \
        string help = \
            "Controls the color of this response perpendicular to the surface. " \
            "In light linear space. " \
    ]], \
    float PREFIX ## FresnelExponent = PREFIX ## FresnelExponentUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Artistic) Fresnel Exponent", \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, \
        string help = \
            "Shapes the Fresnel curve: the intensity falloff from the Edge Color to the Face Color. " \
            "<br/><br/>" \
            "Note: this really doesn't do anything noticable." \
    ]], \
    REFRACTION_UI_DECLARE( PAGE, PREFIX )

//
//  Specular response parameter set:
//      Roughness, Anisotropy, ShadingTangent, ShadingNormal, and UseShadingNormal.
//
#define RASSU_UI_DECLARE(PAGE,PREFIX,ROUGH) \
    float PREFIX ## Roughness = ROUGH \
    [[ \
        string page = #PAGE, \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, float slidercenter = 0.25, \
        string help = \
            "Represents the statistical deviation of the specular micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "specular blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = blurrier, smaller = sharper. Only a value from 0 to 1 is allowed. " \
    ]], \
\
    ANISOTROPY_UI_DECLARE( PAGE, PREFIX ), \
    SHADINGNORMAL_UI_DECLARE( PAGE, PREFIX )


//
// Declare Clearcoat response parameters.
//
#define CLEARCOAT_UI_DECLARE(PAGE,PREFIX,DESCRIPTION,ROUGH) \
    EGEFFRR_UI_DECLARE(PAGE,PREFIX,DESCRIPTION), \
    float PREFIX ## LayerThickness = PREFIX ## LayerThicknessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Layer Thickness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "The thickness of the " #PAGE " layer. " \
            "Coupled with the <strong>Layer Color</strong>, determines how much light " \
            "from the underlying responses is absorbed, and therefore colored by, the " #PAGE ". " \
            "Greater thickness produces more coloration. " \
    ]], \
    color PREFIX ## LayerColor = PREFIX ## LayerColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Layer Color", \
        string color_enableFilmlookVis = "False", \
        string help = \
            "Determines the spectral characteristics of the " #PAGE " layer i.e. the color). " \
            "In light linear space. " \
    ]], \
    RASSU_UI_DECLARE(PAGE,PREFIX,ROUGH)


//
// Declare Specular response parameters.
//
#define SPECULAR_UI_DECLARE(PAGE,PREFIX,DESCRIPTION,ROUGH) \
    EGEFFRR_UI_DECLARE(PAGE,PREFIX,DESCRIPTION), \
    EXTINCTION_UI_DECLARE(PAGE,PREFIX), \
    RASSU_UI_DECLARE(PAGE,PREFIX,ROUGH)


//
// Declare Iridescence response parameters.
//
#define IRIDESCENCE_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EG_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
\
    color PREFIX ## FaceColor = PREFIX ## FaceColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Artistic) Face Color", \
        string help = \
            "Artistic Iridescence simply rotates around the color wheel in hue from the " \
            "<strong>Face Color</strong> to the <strong>Edge Color</strong> as a function " \
            "of the view direction and the surface normal (<strong>V.N</strong>). " \
            "In light linear space. " \
    ]], \
\
    color PREFIX ## EdgeColor = PREFIX ## EdgeColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Artistic) Edge Color", \
        string help = \
            "Artistic Iridescence simply rotates around the color wheel in hue from the " \
            "<strong>Face Color</strong> to the <strong>Edge Color</strong> as a function " \
            "of the view direction and the surface normal (<strong>V.N</strong>). " \
            "In light linear space. " \
    ]], \
\
    float PREFIX ## Exponent = PREFIX ## ExponentUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Artistic) Exponent", \
        int slider = 1, float slidermin = 0.25, float slidermax = 4.0, float slidercenter = 1.0, \
        string help = \
            "Raises the <strong>V.N</strong> value to this exponent before using the result as the " \
            "index into the <strong>Face Color</strong> to <strong>Edge Color</strong> hue lookup. " \
    ]], \
\
    float PREFIX ## Repetitions = PREFIX ## RepetitionsUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Artistic) Repetitions", \
        int slider = 1, float slidermin = 0.0, float slidermax = 3.0, \
        string help = \
            "Determines how many cycles from the <strong>Face Color</strong> to the " \
            "<strong>Edge Color</strong> are produced. " \
    ]], \
\
    float PREFIX ## Thickness = PREFIX ## ThicknessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Physical) Thickness", \
        int slider = 1, float slidermin = 100.0, float slidermax = 1200.0, \
        string help = \
            "The (clear) thin-film thickness in nanometers. The resulting coloration is produced by " \
            "physically based thin-film interference. " \
    ]], \
\
    float PREFIX ## Roughness = PREFIX ## RoughnessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, float slidercenter = 0.25, \
        string help = \
            "Represents the statistical deviation of the specular micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "specular blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = blurrier, smaller = sharper. Only a value from 0 to 1 is allowed. " \
    ]], \
\
    ANISOTROPY_UI_DECLARE( PAGE, PREFIX )


//
// Declare Fuzz response parameters.
//
#define FUZZ_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EGC_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    float PREFIX ## ConeAngle = PREFIX ## ConeAngleUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Cone Angle", \
        int slider = 1, float slidermin = 5.0, float slidermax = 30.0, \
        string help = \
            "Specifies the width of the response in degrees. " \
    ]], \
    SHADINGNORMAL_UI_DECLARE( PAGE, PREFIX )


//
// Declare Diffuse response parameters.
//
#define DIFFUSE_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EGC_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    float PREFIX ## Roughness = PREFIX ## RoughnessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Represents the statistical deviation of the diffuse micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "diffuse blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = broader, smaller = narrower. Only a value from 0 to 1 is allowed. " \
            "At 0, produces a shaped Lambert response using the <strong>Falloff Exponent</strong>. " \
    ]], \
    float PREFIX ## FalloffExponent = PREFIX ## FalloffExponentUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Falloff Exponent", \
        int slider = 1, float slidermin = 0.25, float slidermax = 4.0, float slidercenter = 1.0, \
        string conditionalVisOp = "equalTo", \
        string conditionalVisPath = "../" #PREFIX "Roughness", \
        string conditionalVisValue = "0.0", \
        string help = \
            "Raises the <strong>L.N</strong> value to this exponent for use in the " \
            "Lambert response. Only used when <strong>Roughness</strong> = 0. " \
    ]], \
    \
    SHADINGNORMAL_UI_DECLARE( PAGE, PREFIX ), \
    \
    float PREFIX ## BackfaceGain = PREFIX ## BackfaceGainUIDefault \
    [[ \
        string page = #PAGE ".Double Sided", \
        string label = "Backface Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Combined with <strong>Backface Color</strong>, determines the alternate coloration " \
            "to use on the back faces of the geometry. " \
            "In light linear space. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]], \
    color PREFIX ## BackfaceColor = PREFIX ## BackfaceColorUIDefault \
    [[ \
        string page = #PAGE ".Double Sided", \
        string label = "Backface Color", \
        string help = \
            "Combined with <strong>Backface Gain</strong>, determines the alternate coloration " \
            "to use on the back faces of the geometry. " \
            "In light linear space. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]], \
    \
    float PREFIX ## TransmitGain = PREFIX ## TransmitGainUIDefault \
    [[ \
        string page = #PAGE ".Double Sided", \
        string label = "Transmit Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the 'backside diffuse' response. " \
            "This does not simulate scattering through a solid object, " \
            "but is useful for open surfaces such as a sheet of fabric. " \
            "In light linear space. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]], \
    color PREFIX ## TransmitColor = PREFIX ## TransmitColorUIDefault \
    [[ \
        string page = #PAGE ".Double Sided", \
        string label = "Transmit Color", \
        string help = \
            "Controls the 'backside diffuse' response. " \
            "This does not simulate scattering through a solid object, " \
            "but is useful for open surfaces such as a sheet of fabric. " \
            "In light linear space. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]]


//
// Declare Scatter response parameters.
//
#define SCATTER_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EG_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
\
    color PREFIX ## Color = PREFIX ## ColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Color", \
        string help = \
            "Controls the surface coloration (reflectivity) of this response: the surface albedo. " \
            "In addition, this has the effect of tinting the illumination with the opposite hue " \
            "before it penetrates into the interior of the object. " \
            "In light linear space. " \
    ]], \
 \
    color PREFIX ## PathColor = PREFIX ## PathColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Path Color", \
        string help = \
            "Allows you to control the spectral qualities of the internal scattering. " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Path Length</strong> to produce the " \
            "values used in the response computation. " \
            "A lower value means that particular color component will be absorbed more quickly " \
            "by the material and so will not travel as far through it. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## PathLength = PREFIX ## PathLengthUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Path Length", \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, \
        string help = \
            "Sets the average distance through the material a light ray will travel before being absorbed: " \
            "the material's <strong><em>Mean Free Path</em></strong> distance. " \
            "This length is multiplied by the <strong>Path Color</strong> before being used in " \
            "the response computation. " \
            "Typically specified in millimeters, its relative scale compared to the modeling units " \
            "is definitively set by the PxrSurface_Controls " \
            "<strong>Scatter and Subsurface : Unit Length</strong> parameter. " \
    ]], \
 \
    float PREFIX ## Direction = PREFIX ## DirectionUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Direction", \
        int slider = 1, float slidermin = -1.0, float slidermax = 1.0, \
        string help = \
            "This controls the degree of forward to backward scattering. " \
            "Negative values produce back scattering (brighter when viewed from the same side as the light). " \
            "Positive values produce forward scattering (brighter when looking toward the light: i.e. back-lit). " \
            "When 0, scattering is isotropic. " \
    ]], \
 \
    float PREFIX ## Roughness = PREFIX ## RoughnessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Represents the statistical deviation of the response's micro-facet normals " \
            "from the geometric normal. " \
            "The end result is that for higher <strong>Roughness</strong> values, " \
            "the silhouette view of the surface will become darker. " \
    ]], \
 \
    float PREFIX ## RefractionIndex = PREFIX ## RefractionIndexUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Refraction Index", \
        int slider = 1, float slidermin = 1.0, float slidermax = 4.0, float slidercenter = 1.5, \
        string help = \
            "Scattering is modeled as particles suspended in a medium. " \
            "As light enters or leaves a particle, its path is refractively bent. " \
            "It is the accumulation of many such refractive bending events that scatters " \
            "light in a scattering material. " \
            "The ratio of the refractive index of the particles, over the refractive " \
            "index of the medium, determines the <strong>Refraction Index</strong> value. " \
    ]], \
 \
    float PREFIX ## BacksideGain = PREFIX ## BacksideGainUIDefault \
    [[ \
        string page = #PAGE ".Backside", \
        string label = "Illumination Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the degree to which direct illumination striking the back side of the object " \
            "will be included in the result. " \
            "In light linear space. " \
    ]], \
 \
    color PREFIX ## BacksideTint = PREFIX ## BacksideTintUIDefault \
    [[ \
        string page = #PAGE ".Backside", \
        string label = "Illumination Tint", \
        string help = \
            "Adjusts the coloration (tint) of the back side illumination. " \
            "In light linear space. " \
    ]], \


//
//  Subsurface response parameters.
//
#define SUBSURFACE_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    EG_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
 \
    color PREFIX ## Color = PREFIX ## ColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Color", \
        string help = \
            "Controls the surface (albedo) and subsurface Path <strong>Color</strong> of this response: " \
            "allows you to control the spectral qualities of the surface reflection and internal scattering. " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Path Length</strong> to produce the " \
            "values used in the subsurface computation. " \
            "A lower value means that particular color component will be absorbed more quickly " \
            "by the material, and so will not travel as far through it (and therefore will not be as " \
            "prevalent in the response). " \
            "In light linear space. " \
    ]], \
 \
    color PREFIX ## PathColor = PREFIX ## PathColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(not Multiple MFP) Path Tint", \
        string help = \
            "Tints the Path <strong>Color</strong> used in the subsurface scattering computation. " \
            "It is not used by the <em>Multiple Mean Free Paths</em> <strong>Model</strong>. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## PathLength = PREFIX ## PathLengthUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Path Length", \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, \
        string help = \
            "Sets the average distance through the material a light ray will travel before being absorbed: " \
            "the material's <strong><em>Mean Free Path</em></strong> distance. " \
            "This length is multiplied by the Path <strong>Color</strong>, and possibly the " \
            "<strong>Path Tint</strong>, before being used in the response computation. " \
            "Typically specified in millimeters, its relative scale compared to the modeling units " \
            "is definitively set by the PxrSurface_Controls " \
            "<strong>Scatter and Subsurface : Unit Length</strong> parameter. " \
    ]], \
 \
    color PREFIX ## PostTint = PREFIX ## PostTintUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Post Tint", \
        string help = \
            "Modifies the resulting coloration. This is a completely non-physical control " \
            "you can use to force the result into the color you want. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## TransmitGain = PREFIX ## TransmitGainUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Double Sided) Transmit Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Adjusts the gain of the response to light transmitted through the material. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## Directionality = PREFIX ## DirectionalityUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Path Traced) Directionality", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Increasing <strong>Directionality</strong> intensifies the amount of forward scattering " \
            "(when light rays tend to continue to travel in their original direction) that occurs within " \
            "the material. The end result is that hard corners will be accentuated and the response " \
            "will appear more diffuse overall. " \
    ]], \
 \
    float PREFIX ## DiffuseBlend = PREFIX ## DiffuseBlendUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Path Traced) Diffuse Blend", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Blends the Subsurface response with a Diffuse one: makes the response appear more diffuse. " \
            "However, this can help when dark artifacts are seen on very sharp edges. " \
    ]], \
 \
    float PREFIX ## Bleed = PREFIX ## BleedUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "(Non-Expon Path Traced) Bleed", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Effectively increases the <strong>Path Length</strong> (i.e. <em>Mean Free Path</em>), " \
            "but without the reduction in detail that would accompany an increase in the " \
            "<strong>Path Length</strong> directly. " \
    ]], \
 \
    float PREFIX ## ShortGain = PREFIX ## ShortGainUIDefault \
    [[ \
        string page = #PAGE ".Multiple MFP", \
        string label = "Short Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the <strong>Short</strong> path length component of the response. " \
            "In light linear space. " \
    ]], \
 \
    color PREFIX ## ShortColor = PREFIX ## ShortColorUIDefault \
    [[ \
        string page = #PAGE ".Multiple MFP", \
        string label = "Short Color", \
        string help = \
            "Controls the spectral qualities of the <strong>Short</strong> path length component of the response. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## ShortLength = PREFIX ## ShortLengthUIDefault \
    [[ \
        string page = #PAGE ".Multiple MFP", \
        string label = "Short Length", \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, \
        string help = \
            "Controls the <em>Mean Free Path</em> scattering distance used in the " \
            "<strong>Short</strong> path length component of the response. " \
    ]], \
 \
    float PREFIX ## LongGain = PREFIX ## LongGainUIDefault \
    [[ \
        string page = #PAGE ".Multiple MFP", \
        string label = "Long Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the <strong>Long</strong> path length component of the response. " \
            "In light linear space. " \
    ]], \
 \
    color PREFIX ## LongColor = PREFIX ## LongColorUIDefault \
    [[ \
        string page = #PAGE ".Multiple MFP", \
        string label = "Long Color", \
        string help = \
            "Controls the spectral qualities of the <strong>Long</strong> path length component of the response. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## LongLength = PREFIX ## LongLengthUIDefault \
    [[ \
        string page = #PAGE ".Multiple MFP", \
        string label = "Long Length", \
        int slider = 1, float slidermin = 0.0, float slidermax = 100.0, \
        string help = \
            "Controls the <em>Mean Free Path</em> scattering distance used in the " \
            "<strong>Long</strong> path length component of the response. " \
    ]]


//
//  Glass response parameters.
//
#define GLASS_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    ENABLE_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
 \
    float PREFIX ## ReflectionGain = PREFIX ## ReflectionGainUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Reflection Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the glass <strong>Reflection</strong>. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## RefractionGain = PREFIX ## RefractionGainUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Refraction Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the glass <strong>Refraction</strong> response. " \
            "In light linear space. " \
    ]], \
 \
    color PREFIX ## RefractionColor = PREFIX ## RefractionColorUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Refraction Color", \
        string help = \
            "Sets the color of the glass. Affects the <strong>Refraction</strong> response by " \
            "tinting the light that passes through the glass. " \
            "In light linear space. " \
    ]], \
 \
    float PREFIX ## RefractionIndex = PREFIX ## RefractionIndexUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Refraction Index", \
        int slider = 1, float slidermin = 1.0, float slidermax = 4.0, float slidercenter = 1.5, \
        string help = \
            "This is actually the ratio of the refractive index of the material, " \
            "over the refractive index of the medium the camera is in. " \
            "If the camera is in air (refractive index = 1.0) this value " \
            "simply becomes the refractive index of the material. " \
            "<br/>" \
            "<br/>ice = 1.31" \
            "<br/>water = 1.333" \
            "<br/>alcohol = 1.358" \
            "<br/>human eye = 1.386" \
            "<br/>kerosene = 1.39" \
            "<br/>rock salt = 1.5" \
            "<br/>glass = 1.52" \
            "<br/>amber = 1.55" \
            "<br/>flint = 1.61" \
            "<br/>gem stones = 1.775" \
            "<br/>cubic zirconia = 2.155" \
            "<br/>diamond = 2.417" \
            "<br/>silicon = 3.46" \
            "<br/>germanium = 4.03" \
    ]], \
 \
    float PREFIX ## Roughness = PREFIX ## RoughnessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, float slidercenter = 0.25, \
        string help = \
            "Represents the statistical deviation of the specular micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "specular blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = blurrier, smaller = sharper. Only a value from 0 to 1 is allowed. " \
    ]], \
\
    ANISOTROPY_UI_DECLARE( PAGE, PREFIX )


/**************************************************************************************************
 *  Connection Struct and Vstruct parameters
 **************************************************************************************************/

//
//  Connection struct for a PxrSurface material layer.
//
struct PxrSurface_material_s
{
    int    layer; // Material layering depth. Top = 0.

    //
    // Material layering and displacement parameters.
    //
    float  bxdfMask;
    float  thickness;
    vector deltaP;  // Layer accumulated displacement vector.

    //
    // Bxdf parameters start here.
    //
    float  BumpPercent;

    normal globalShadingNormal;
    float  globalUseShadingNormal;
    color  globalShadowColor;

    color  clearcoatEdgeCG;
    color  clearcoatFaceCG;
    float  clearcoatFresnelExponent;
    color  clearcoatRefractionCI;
    color  clearcoatExtinctionCC;
    float  clearcoatLayerThickness;
    color  clearcoatLayerColor;
    float  clearcoatRoughness;
    float  clearcoatAnisotropy;
    vector clearcoatShadingTangent;
    float  clearcoatUseShadingTangent;
    normal clearcoatShadingNormal;
    float  clearcoatUseShadingNormal;

    color  specPrimaryEdgeCG;
    color  specPrimaryFaceCG;
    float  specPrimaryFresnelExponent;
    color  specPrimaryRefractionCI;
    color  specPrimaryExtinctionCC;
    float  specPrimaryRoughness;
    float  specPrimaryAnisotropy;
    vector specPrimaryShadingTangent;
    float  specPrimaryUseShadingTangent;
    normal specPrimaryShadingNormal;
    float  specPrimaryUseShadingNormal;

    color  specSecondaryEdgeCG;
    color  specSecondaryFaceCG;
    float  specSecondaryFresnelExponent;
    color  specSecondaryRefractionCI;
    color  specSecondaryExtinctionCC;
    float  specSecondaryRoughness;
    float  specSecondaryAnisotropy;
    vector specSecondaryShadingTangent;
    float  specSecondaryUseShadingTangent;
    normal specSecondaryShadingNormal;
    float  specSecondaryUseShadingNormal;

    float  iridescenceGain;
    color  iridescenceFaceColor;
    color  iridescenceEdgeColor;
    float  iridescenceExponent;
    float  iridescenceRepetitions;
    float  iridescenceThickness;
    float  iridescenceRoughness;
    float  iridescenceAnisotropy;
    vector iridescenceShadingTangent;
    float  iridescenceUseShadingTangent;

    color  fuzzCG;
    float  fuzzConeAngle;
    normal fuzzShadingNormal;
    float  fuzzUseShadingNormal;

    color  diffuseCG;
    float  diffuseRoughness;
    float  diffuseFalloffExponent;
    color  diffuseBackfaceCG;
    color  diffuseTransmitCG;
    normal diffuseShadingNormal;
    float  diffuseUseShadingNormal;

    color  scatterCG;
    float  scatterPathLength;
    color  scatterPathColor;
    float  scatterDirection;
    float  scatterRoughness;
    float  scatterRefractionIndex;
    color  scatterBacksideCG;

    color  subsurfaceCG;
    float  subsurfacePathLength;
    color  subsurfacePathColor;
    color  subsurfacePostTint;
    float  subsurfaceTransmitGain;
    float  subsurfaceShortLength;
    color  subsurfaceShortCG;
    float  subsurfaceLongLength;
    color  subsurfaceLongCG;
    float  subsurfaceDirectionality;
    float  subsurfaceDiffuseBlend;
    float  subsurfaceBleed;

    // scatter and subsurface.
    color  ssIlluminationTint;
    float  ssRoughness;

    float  glassReflectionGain;
    color  glassRefractionCG;
    float  glassRefractionIndex;
    float  glassRoughness;
    float  glassAnisotropy;
    vector glassShadingTangent;
    float  glassUseShadingTangent;

    color  glowCG;
};

//
//  vstruct naming conventions.
//
#define VSTRUCT_PARAM_NAME(VSTRUCT,MEMBER) VSTRUCT##_##MEMBER
#define VSTRUCT_REF_STRING(VSTRUCT,MEMBER) #VSTRUCT "." #MEMBER

//
//  Struct parameter.
//  Default layer = 0.
//  All other struct values are initially pre-multiplied by alpha = 0 for proper compositing.
//
#define PXRSURFACE_MATERIAL_STRUCT(VSTRUCT,MEMBER) \
    PxrSurface_material_s  VSTRUCT_PARAM_NAME( VSTRUCT, MEMBER ) = { 0, 0.0 } \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string type = "struct", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, MEMBER ) \
    ]]

//
//  The vstruct Enable parameters are only used to determine what ultimately needs
//  to be connected to the bxdf node using the vstructConditionalExpr mechanism.
//  They are not used for any other purpose.
//
//  Response Enable parameter.
//  These are used to determine whether a response is used
//  anywhere in the material layer stack.
//
#define RESPONSE_ENABLE_PARAM(VSTRUCT,PREFIX) \
    int VSTRUCT##_##PREFIX##Enable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, PREFIX ) "Enable" \
    ]]

#define RESPONSE_ENABLE_PARAM_CONDITIONAL(VSTRUCT,PREFIX,CONDITION) \
    int VSTRUCT##_##PREFIX##Enable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, PREFIX ) "Enable", \
        string vstructConditionalExpr = #CONDITION \
    ]]

#define RESPONSE_ENABLE_PARAM_SET(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
        RESPONSE_ENABLE_PARAM_CONDITIONAL(VSTRUCTOUT,PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN##_##PREFIX##Enable == 1 \
                or ( Enable == 1 and PREFIX##Enable == 1 ) \
                ) \
            else set 0 \
        )

//
//  ShadingNormal Enable parameter.
//  These are used to determine whether a response's ShadingNormal is used
//  anywhere in the material layer stack.
//
#define SHADINGNORMAL_ENABLE_PARAM(VSTRUCT,PREFIX) \
    int VSTRUCT##_##PREFIX##ShadingNormalEnable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, PREFIX ) "ShadingNormalEnable" \
    ]]

#define SHADINGNORMAL_ENABLE_PARAM_CONDITIONAL(VSTRUCT,PREFIX,CONDITION) \
    int VSTRUCT##_##PREFIX##ShadingNormalEnable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, PREFIX ) "ShadingNormalEnable", \
        string vstructConditionalExpr = #CONDITION \
    ]]

#define SHADINGNORMAL_ENABLE_PARAM_SET(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
        SHADINGNORMAL_ENABLE_PARAM_CONDITIONAL(VSTRUCTOUT,PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN##_##PREFIX##ShadingNormalEnable == 1 \
                or ( Enable == 1 and PREFIX##Enable == 1 and PREFIX##ShadingNormal is connected ) \
                ) \
            else set 0 \
        )

//
//  ShadingTangent Enable parameter.
//  These are used to determine whether a response's ShadingTangent is used
//  anywhere in the material layer stack.
//
#define SHADINGTANGENT_ENABLE_PARAM(VSTRUCT,PREFIX) \
    int VSTRUCT##_##PREFIX##ShadingTangentEnable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, PREFIX ) "ShadingTangentEnable" \
    ]]

#define SHADINGTANGENT_ENABLE_PARAM_CONDITIONAL(VSTRUCT,PREFIX,CONDITION) \
    int VSTRUCT##_##PREFIX##ShadingTangentEnable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REF_STRING( VSTRUCT, PREFIX ) "ShadingTangentEnable", \
        string vstructConditionalExpr = #CONDITION \
    ]]

#define SHADINGTANGENT_ENABLE_PARAM_SET(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
        SHADINGTANGENT_ENABLE_PARAM_CONDITIONAL(VSTRUCTOUT,PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN##_##PREFIX##ShadingTangentEnable == 1 \
                or ( Enable == 1 and PREFIX##Enable == 1 and PREFIX##ShadingTangent is connected ) \
                ) \
            else set 0 \
        )

//
//  Enable parameter sets.
//
//  Note: not all responses have ShadingNormal or ShadingTangent.
//  They'll simply be ignored in that case.
//  XXX
//  True, but the Preset Browser will become confused when it encouters
//  a vstructConditional that references a non-existent parameter.
//
#define ALL_ENABLE_PARAMS(VSTRUCT,PREFIX) \
    RESPONSE_ENABLE_PARAM( VSTRUCT, PREFIX ), \
    SHADINGNORMAL_ENABLE_PARAM( VSTRUCT, PREFIX ), \
    SHADINGTANGENT_ENABLE_PARAM( VSTRUCT, PREFIX )

#define ALL_ENABLE_PARAMS_SET(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
    RESPONSE_ENABLE_PARAM_SET( VSTRUCTOUT, VSTRUCTIN, PREFIX ), \
    SHADINGNORMAL_ENABLE_PARAM_SET( VSTRUCTOUT, VSTRUCTIN, PREFIX ), \
    SHADINGTANGENT_ENABLE_PARAM_SET( VSTRUCTOUT, VSTRUCTIN, PREFIX )


//
//  Struct member copy macros.
//  Used when the material doesn't produce the response itself,
//  so it simply passes the struct data from input to output.
//
#define PARAM_COPY(OUTPUT,INPUT,PREFIX,NAME) \
    OUTPUT.PREFIX##NAME = INPUT.PREFIX##NAME

#define CLEARCOAT_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, EdgeCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FaceCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FresnelExponent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionCI ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, LayerThickness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, LayerColor ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingTangent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingTangent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define SPECULAR_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, EdgeCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FaceCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FresnelExponent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionCI ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ExtinctionCC ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingTangent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingTangent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define IRIDESCENCE_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Gain ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FaceColor ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, EdgeColor ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Exponent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Repetitions ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Thickness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingTangent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingTangent )
 
#define FUZZ_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ConeAngle ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define DIFFUSE_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FalloffExponent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, BackfaceCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, TransmitCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define SCATTER_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, PathLength ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, PathColor ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Direction ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionIndex ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, BacksideCG )
 
#define SUBSURFACE_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, PathLength ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, PathColor ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, PostTint ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, TransmitGain ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShortLength ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShortCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, LongLength ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, LongCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Directionality ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, DiffuseBlend ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Bleed )
 
#define GLASS_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ReflectionGain ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionIndex ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingTangent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingTangent )
 
#define GLOW_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG )
 

//
//  Parameter compositing macro.
//  Used to composite the input material's response parameter over the current material's.
//
#define PARAM_OVER(OUTPUT,INPUT,PREFIX,PARAM,MASK) \
    OUTPUT.PREFIX##PARAM = INPUT.PREFIX##PARAM + ( PREFIX##PARAM * MASK )*( 1.0 - INPUT.##MASK )

#endif
