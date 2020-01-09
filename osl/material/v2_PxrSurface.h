/*
 *  Copyright 2018-2020 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
/*
    Defines two components of the material layering system.
    The first is the set of user interface parameters that control the material.
    The second is the data struct passed from one layer to another.
 */
#ifndef MATERIAL_V2_PXRSURFACE_H
#define MATERIAL_V2_PXRSURFACE_H


/**************************************************************************************************
 *  Material UI
 **************************************************************************************************/
//
// Default values.
//
#define EnableUIDefault                             1
#define MaskUIDefault                               1.0
#define MaskBxdfSharpenUIDefault                    0.0
#define MaskDispSharpenUIDefault                    0.0
#define ThicknessUIDefault                          0.0
#define CoveringHeightUIDefault                     0.0
#define DispMagSpaceUIDefault                       "world"
#define DispMagScaleUIDefault                       1.0
#define Displacement_BumpPercentUIDefault           1.0

#define Global_ShadingNormalUIDefault               normal(0.0)
#define Global_UseShadingNormalUIDefault            1.0
#define Global_ShadowColorUIDefault                 color(0.0)

#define commonEnableUIDefault                       0
#define commonGainUIDefault                         1.0
#define commonColorUIDefault                        color(0.5)
#define commonEdgeColorUIDefault                    color(0.5)
#define commonFaceColorUIDefault                    color(0.0)
#define commonFresnelExponentUIDefault              5.0
#define commonRefractionIndexUIDefault              1.52
#define commonRefractionColorUIDefault              color(1.0)
#define commonExtinctionCoeffUIDefault              0.0
#define commonExtinctionColorUIDefault              color(1.0)
#define commonAnisotropyUIDefault                   0.0
#define commonAnisotropyDirectionUIDefault          vector(0.0)
#define commonShadingNormalUIDefault                normal(0.0)
#define commonUseShadingNormalUIDefault             1.0

#define ClearCoat_EnableUIDefault                   commonEnableUIDefault
#define ClearCoat_GainUIDefault                     commonGainUIDefault
#define ClearCoat_ColorUIDefault                    commonColorUIDefault
#define ClearCoat_FaceColorUIDefault                commonFaceColorUIDefault
#define ClearCoat_FresnelExponentUIDefault          commonFresnelExponentUIDefault
#define ClearCoat_RefractionIndexUIDefault          commonRefractionIndexUIDefault
#define ClearCoat_RefractionColorUIDefault          commonRefractionColorUIDefault
#define ClearCoat_LayerThicknessUIDefault           0.0
#define ClearCoat_LayerColorUIDefault               color(0.0)
#define ClearCoat_RoughnessUIDefault                0.0
#define ClearCoat_AnisotropyUIDefault               commonAnisotropyUIDefault
#define ClearCoat_AnisotropyDirectionUIDefault      commonAnisotropyDirectionUIDefault
#define ClearCoat_ShadingNormalUIDefault            commonShadingNormalUIDefault
#define ClearCoat_UseShadingNormalUIDefault         commonUseShadingNormalUIDefault

#define PrimarySpecular_EnableUIDefault             commonEnableUIDefault
#define PrimarySpecular_GainUIDefault               commonGainUIDefault
#define PrimarySpecular_ColorUIDefault              commonColorUIDefault
#define PrimarySpecular_FaceColorUIDefault          commonFaceColorUIDefault
#define PrimarySpecular_FresnelExponentUIDefault    commonFresnelExponentUIDefault
#define PrimarySpecular_RefractionIndexUIDefault    commonRefractionIndexUIDefault
#define PrimarySpecular_RefractionColorUIDefault    commonRefractionColorUIDefault
#define PrimarySpecular_ExtinctionCoeffUIDefault    commonExtinctionCoeffUIDefault
#define PrimarySpecular_ExtinctionColorUIDefault    commonExtinctionColorUIDefault
#define PrimarySpecular_RoughnessUIDefault          0.2
#define PrimarySpecular_AnisotropyUIDefault         commonAnisotropyUIDefault
#define PrimarySpecular_AnisotropyDirectionUIDefault   commonAnisotropyDirectionUIDefault
#define PrimarySpecular_ShadingNormalUIDefault      commonShadingNormalUIDefault
#define PrimarySpecular_UseShadingNormalUIDefault   commonUseShadingNormalUIDefault

#define SecondarySpecular_EnableUIDefault           commonEnableUIDefault
#define SecondarySpecular_GainUIDefault             commonGainUIDefault
#define SecondarySpecular_ColorUIDefault            commonColorUIDefault
#define SecondarySpecular_FaceColorUIDefault        commonFaceColorUIDefault
#define SecondarySpecular_FresnelExponentUIDefault  commonFresnelExponentUIDefault
#define SecondarySpecular_RefractionIndexUIDefault  commonRefractionIndexUIDefault
#define SecondarySpecular_RefractionColorUIDefault  commonRefractionColorUIDefault
#define SecondarySpecular_ExtinctionCoeffUIDefault  commonExtinctionCoeffUIDefault
#define SecondarySpecular_ExtinctionColorUIDefault  commonExtinctionColorUIDefault
#define SecondarySpecular_RoughnessUIDefault        0.6
#define SecondarySpecular_AnisotropyUIDefault       commonAnisotropyUIDefault
#define SecondarySpecular_AnisotropyDirectionUIDefault commonAnisotropyDirectionUIDefault
#define SecondarySpecular_ShadingNormalUIDefault    commonShadingNormalUIDefault
#define SecondarySpecular_UseShadingNormalUIDefault commonUseShadingNormalUIDefault

#define Iridescence_EnableUIDefault                 commonEnableUIDefault
#define Iridescence_GainUIDefault                   commonGainUIDefault
#define Iridescence_RoughnessUIDefault              0.2
#define Iridescence_FaceColorUIDefault              color(1,0,0)
#define Iridescence_EdgeColorUIDefault              color(0,0,1)
#define Iridescence_ExponentUIDefault               1.0
#define Iridescence_RepetitionsUIDefault            1.0
#define Iridescence_ThicknessUIDefault              800.0
#define Iridescence_AnisotropyUIDefault             commonAnisotropyUIDefault
#define Iridescence_AnisotropyDirectionUIDefault    commonAnisotropyDirectionUIDefault
#define Iridescence_ShadingNormalUIDefault          commonShadingNormalUIDefault
#define Iridescence_UseShadingNormalUIDefault       commonUseShadingNormalUIDefault

#define Fuzz_EnableUIDefault                        commonEnableUIDefault
#define Fuzz_GainUIDefault                          commonGainUIDefault
#define Fuzz_ColorUIDefault                         commonColorUIDefault
#define Fuzz_ConeAngleUIDefault                     15.0
#define Fuzz_ShadingNormalUIDefault                 commonShadingNormalUIDefault
#define Fuzz_UseShadingNormalUIDefault              commonUseShadingNormalUIDefault

#define Diffuse_EnableUIDefault                     commonEnableUIDefault
#define Diffuse_GainUIDefault                       commonGainUIDefault
#define Diffuse_ColorUIDefault                      commonColorUIDefault
#define Diffuse_RoughnessUIDefault                  0.0
#define Diffuse_ExponentUIDefault                   1.0
#define Diffuse_BackfaceGainUIDefault               0.0
#define Diffuse_BackfaceColorUIDefault              commonColorUIDefault
#define Diffuse_TransmitGainUIDefault               0.0
#define Diffuse_TransmitColorUIDefault              commonColorUIDefault
#define Diffuse_ShadingNormalUIDefault              commonShadingNormalUIDefault
#define Diffuse_UseShadingNormalUIDefault           commonUseShadingNormalUIDefault

#define Scatter_EnableUIDefault                     commonEnableUIDefault
#define Scatter_GainUIDefault                       commonGainUIDefault
#define Scatter_ColorUIDefault                      color(0.830,0.791,0.753)
#define Scatter_PathLengthUIDefault                 10.0
#define Scatter_PathColorUIDefault                  color(0.851,0.557,0.395)
#define Scatter_DirectionUIDefault                  0.0
#define Scatter_RoughnessUIDefault                  0.0
#define Scatter_RefractionIndexUIDefault            1.3
#define Scatter_IlluminationGainUIDefault           0.0
#define Scatter_IlluminationTintUIDefault           color(1.0)

#define Subsurface_EnableUIDefault                  commonEnableUIDefault
#define Subsurface_GainUIDefault                    commonGainUIDefault
#define Subsurface_ColorUIDefault                   color(0.830,0.791,0.753)
#define Subsurface_PathLengthUIDefault              10.0
#define Subsurface_PathColorUIDefault               color(0.851,0.557,0.395)
#define Subsurface_PostTintUIDefault                color(1.0)
#define Subsurface_TransmitGainUIDefault            0.0
#define Subsurface_ShortGainUIDefault               0.0
#define Subsurface_ShortLengthUIDefault             1.0
#define Subsurface_ShortColorUIDefault              color(0.9,0.9,0.9)
#define Subsurface_LongGainUIDefault                0.0
#define Subsurface_LongLengthUIDefault              100.0
#define Subsurface_LongColorUIDefault               color(0.8,0.0,0.0)
#define Subsurface_DirectionalityUIDefault          0.0
#define Subsurface_DiffuseBlendUIDefault            0.0
#define Subsurface_BleedUIDefault                   0.0

#define ScatterAndSubsurface_IlluminationTintUIDefault  color(1.0)
#define ScatterAndSubsurface_RoughnessUIDefault         0.0

#define Glass_EnableUIDefault                       commonEnableUIDefault
#define Glass_ReflectionGainUIDefault               commonGainUIDefault
#define Glass_RoughnessUIDefault                    0.1
#define Glass_AnisotropyUIDefault                   commonAnisotropyUIDefault
#define Glass_AnisotropyDirectionUIDefault          commonAnisotropyDirectionUIDefault
#define Glass_ShadingNormalUIDefault                commonShadingNormalUIDefault
#define Glass_UseShadingNormalUIDefault             commonUseShadingNormalUIDefault
#define Glass_RefractionGainUIDefault               commonGainUIDefault
#define Glass_RefractionColorUIDefault              commonRefractionColorUIDefault
#define Glass_RefractionIndexUIDefault              commonRefractionIndexUIDefault
#define Glass_ExtinctionCoeffUIDefault              commonExtinctionCoeffUIDefault
#define Glass_ExtinctionColorUIDefault              color(0.5)
#define Glass_ScatterColorUIDefault                 color(0.0)
#define Glass_ManifoldExplorationIORDefault         -1.0

#define Glow_EnableUIDefault                        commonEnableUIDefault
#define Glow_GainUIDefault                          commonGainUIDefault
#define Glow_ColorUIDefault                         commonColorUIDefault


//
//  Generic user parameter naming.
//
#define UI_PARAMETER(PREFIX,NAME) PREFIX##_##NAME

//
//  Enable, Gain, and Color parameter declarations.
//
#define ENABLE_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    int PREFIX##_Enable = PREFIX##_EnableUIDefault \
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

#define GAIN_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX##_##Gain = PREFIX##_##GainUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 2.0, \
        string help = \
            "Controls the intensity of this response. " \
    ]]

#define COLOR_UI_DECLARE(PAGE,PREFIX) \
    color PREFIX##_##Color = PREFIX##_##ColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Color", \
        string help = \
            "Controls the color of this response. " \
    ]]

#define EG_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    ENABLE_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    GAIN_UI_DECLARE( PAGE, PREFIX )

#define EGC_UI_DECLARE(PAGE,PREFIX,DESCRIPTION) \
    ENABLE_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    GAIN_UI_DECLARE( PAGE, PREFIX ), \
    COLOR_UI_DECLARE( PAGE, PREFIX )

//
//  Face Color and Fresnel parameter declarations.
//
#define FACEFRESNEL_UI_DECLARE(PAGE,PREFIX) \
    color PREFIX##_##FaceColor = PREFIX##_##FaceColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Face Color (Artistic)", \
        string help = \
            "Controls the color of this response perpendicular to the surface. " \
    ]], \
    float PREFIX##_##FresnelExponent = PREFIX##_##FresnelExponentUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Fresnel Exponent (Artistic)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, \
        string help = \
            "Shapes the Fresnel curve: the intensity falloff from the Edge Color to the Face Color. " \
            "<br/><br/>" \
            "Note: this really doesn't do anything noticable." \
    ]]

//
//  Shading Normal parameter declarations.
//
#define SHADINGNORMAL_UI_DECLARE(PAGE,PREFIX) \
    normal PREFIX##_##ShadingNormal = PREFIX##_##ShadingNormalUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Shading Normal", \
        string help = \
            "An alternate shading normal to use for this response. " \
            "Supersedes the Global Shading Normal if one is specified. " \
    ]], \
    float PREFIX##_##UseShadingNormal = PREFIX##_##UseShadingNormalUIDefault \
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
//  Anisotropy parameter declarations.
//
#define ANISOTROPY_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX##_##Anisotropy = PREFIX##_##AnisotropyUIDefault \
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
            "<br/><br/>" \
            "NOTE: <em>Negative</em> <strong>Anisotropy</strong> will produce the correct response when " \
            "the <strong>Anisotropy Direction</strong> indicates the surface &quot;brushing&quot; " \
            "direction: when it is parallel to any fibers or scratching of the surface. " \
    ]], \
    vector PREFIX##_##AnisotropyDirection = PREFIX##_##AnisotropyDirectionUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Anisotropy Direction", \
        string help = \
            "When connected, this (surface tangent) vector will determine the orientation of the " \
            "<strong>Anisotropy</strong> axes." \
            "<br/><br/>" \
            "Note: though it is intended to be, it is not <em>strictly</em> necessary that this vector " \
            "actually be tangent to the surface. " \
            "<br/>" \
            "ALSO: <em>Negative</em> <strong>Anisotropy</strong> will produce the correct response when " \
            "the <strong>Anisotropy Direction</strong> indicates the surface &quot;brushing&quot; " \
            "direction: when it is parallel to any fibers or scratching of the surface. " \
    ]]

//
//  Refraction parameter declarations.
//
#define REFRACTION_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX##_##RefractionIndex = PREFIX##_##RefractionIndexUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Refraction Index (Physical)", \
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
            "<br/>vero clear = 1.47" \
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
    color PREFIX##_##RefractionColor = PREFIX##_##RefractionColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Refraction Color (Physical)", \
        string help = \
            "Allows you to reproduce spectral Fresnel effects. " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Refraction Index</strong> to produce the " \
            "values used in the response computation. " \
    ]]

//
//  Extinction parameter declarations.
//
#define EXTINCTION_UI_DECLARE(PAGE,PREFIX) \
    float PREFIX##_##ExtinctionCoeff = PREFIX##_##ExtinctionCoeffUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Extinction Coeff (Physical)", \
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
    color PREFIX##_##ExtinctionColor = PREFIX##_##ExtinctionColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Extinction Color (Physical)", \
        string help = \
            "Determines the spectral characteristics of the extinction component of the response " \
            "(i.e. the color). " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Extinction Coefficient</strong> to produce the " \
            "values used in the response computation. " \
    ]]

//
//  Specular parameter declaration set:
//      Roughness, Anisotropy, and ShadingNormal parameters.
//
#define RAS_UI_DECLARE(PAGE,PREFIX,ROUGH) \
    float PREFIX##_##Roughness = ROUGH \
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



/**************************************************************************************************
 *  Reponse parameter declaration macros.
 **************************************************************************************************/
//
// Clearcoat parameter declarations.
//
#define CLEARCOAT_UI_DECLARE(PAGE,PREFIX,DESCRIPTION,ROUGH) \
    EGC_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    FACEFRESNEL_UI_DECLARE( PAGE, PREFIX ), \
    REFRACTION_UI_DECLARE( PAGE, PREFIX ), \
    \
    float PREFIX##_##LayerThickness = PREFIX##_##LayerThicknessUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Layer Thickness (Physical)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "The thickness of the " #PAGE " layer. " \
            "Coupled with the <strong>Layer Color</strong>, determines how much light " \
            "from the underlying responses is absorbed, and therefore colored by, the " #PAGE ". " \
            "Greater thickness produces more coloration. " \
            "<br/><br/>" \
            "Note: this only affects other responses in this material node - not other layered materials. " \
    ]], \
    color PREFIX##_##LayerColor = PREFIX##_##LayerColorUIDefault \
    [[ \
        string page = #PAGE, \
        string label = "Layer Color (Physical)", \
        string help = \
            "Determines the spectral characteristics of the " #PAGE " layer i.e. the color). " \
            "<br/><br/>" \
            "Note: this only affects other responses in this material node - not other layered materials. " \
    ]], \
    \
    RAS_UI_DECLARE( PAGE, PREFIX, ROUGH )


//
// Specular parameter declarations.
//
#define SPECULAR_UI_DECLARE(PAGE,PREFIX,DESCRIPTION,ROUGH) \
    EGC_UI_DECLARE( PAGE, PREFIX, DESCRIPTION ), \
    FACEFRESNEL_UI_DECLARE( PAGE, PREFIX ), \
    EXTINCTION_UI_DECLARE( PAGE, PREFIX ), \
    REFRACTION_UI_DECLARE( PAGE, PREFIX ), \
    RAS_UI_DECLARE( PAGE, PREFIX, ROUGH )


//
// Iridescence parameters.
//
#define IRIDESCENCE_UI_DECLARE(DESCRIPTION) \
    EG_UI_DECLARE( Iridescence, Iridescence, DESCRIPTION ), \
    \
    color Iridescence_FaceColor = Iridescence_FaceColorUIDefault \
    [[ \
        string page = "Iridescence", \
        string label = "Face Color (Artistic)", \
        string help = \
            "Artistic Iridescence simply rotates around the color wheel in hue from the " \
            "<strong>Face Color</strong> to the <strong>Edge Color</strong> as a function " \
            "of the view direction and the surface normal (<strong>V.N</strong>). " \
    ]], \
    color Iridescence_EdgeColor = Iridescence_EdgeColorUIDefault \
    [[ \
        string page = "Iridescence", \
        string label = "Edge Color (Artistic)", \
        string help = \
            "Artistic Iridescence simply rotates around the color wheel in hue from the " \
            "<strong>Face Color</strong> to the <strong>Edge Color</strong> as a function " \
            "of the view direction and the surface normal (<strong>V.N</strong>). " \
    ]], \
    float Iridescence_Exponent = Iridescence_ExponentUIDefault \
    [[ \
        string page = "Iridescence", \
        string label = "Exponent (Artistic)", \
        int slider = 1, float slidermin = 0.25, float slidermax = 4.0, float slidercenter = 1.0, \
        string help = \
            "Raises the <strong>V.N</strong> value to this exponent before using the result as the " \
            "index into the <strong>Face Color</strong> to <strong>Edge Color</strong> hue lookup. " \
    ]], \
    float Iridescence_Repetitions = Iridescence_RepetitionsUIDefault \
    [[ \
        string page = "Iridescence", \
        string label = "Repetitions (Artistic)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 3.0, \
        string help = \
            "Determines how many cycles from the <strong>Face Color</strong> to the " \
            "<strong>Edge Color</strong> are produced. " \
    ]], \
    float Iridescence_Thickness = Iridescence_ThicknessUIDefault \
    [[ \
        string page = "Iridescence", \
        string label = "Thickness (Physical)", \
        int slider = 1, float slidermin = 100.0, float slidermax = 1200.0, \
        string help = \
            "The (clear) thin-film thickness in nanometers. The resulting coloration is produced by " \
            "physically based thin-film interference. " \
    ]], \
    float Iridescence_Roughness = Iridescence_RoughnessUIDefault \
    [[ \
        string page = "Iridescence", \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, float slidercenter = 0.25, \
        string help = \
            "Represents the statistical deviation of the specular micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "specular blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = blurrier, smaller = sharper. Only a value from 0 to 1 is allowed. " \
    ]], \
    \
    ANISOTROPY_UI_DECLARE( Iridescence, Iridescence ), \
    SHADINGNORMAL_UI_DECLARE( Iridescence, Iridescence )


//
// Fuzz parameters.
//
#define FUZZ_UI_DECLARE(DESCRIPTION) \
    EGC_UI_DECLARE( Fuzz, Fuzz, DESCRIPTION ), \
    \
    float Fuzz_ConeAngle = Fuzz_ConeAngleUIDefault \
    [[ \
        string page = "Fuzz", \
        string label = "Cone Angle", \
        int slider = 1, float slidermin = 5.0, float slidermax = 30.0, \
        string help = \
            "Specifies the width of the response in degrees. " \
    ]], \
    \
    SHADINGNORMAL_UI_DECLARE( Fuzz, Fuzz )


//
// Diffuse parameters.
//
#define DIFFUSE_UI_DECLARE(DESCRIPTION) \
    EGC_UI_DECLARE( Diffuse, Diffuse, DESCRIPTION ), \
    \
    float Diffuse_Roughness = Diffuse_RoughnessUIDefault \
    [[ \
        string page = "Diffuse", \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Represents the statistical deviation of the diffuse micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "diffuse blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = broader, smaller = narrower. Only a value from 0 to 1 is allowed. " \
            "<br/><br/> " \
            "At 0, produces a Lambert response that is shaped using the <strong>Exponent</strong> parameter. " \
    ]], \
    float Diffuse_Exponent = Diffuse_ExponentUIDefault \
    [[ \
        string page = "Diffuse", \
        string label = "Exponent", \
        int slider = 1, float slidermin = 0.0, float slidermax = 4.0, \
        string conditionalVisPath = "../Diffuse_Roughness", \
        string conditionalVisOp = "equalTo", \
        string conditionalVisValue = "0", \
        string help = \
            "Raises the <strong>L.N</strong> value to this <strong>Exponent</strong> for " \
            "use in the Lambert response. " \
            "<br/><br/> " \
            "Only used when <strong>Roughness</strong> = 0. " \
    ]], \
    \
    SHADINGNORMAL_UI_DECLARE( Diffuse, Diffuse ), \
    \
    float Diffuse_TransmitGain = Diffuse_TransmitGainUIDefault \
    [[ \
        string page = "Diffuse", \
        string label = "Transmit Gain (Double Sided)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the front side response to light hitting the back side of the surface. " \
            "This does not simulate scattering through a solid object, " \
            "but is useful for open surfaces such as a sheet of fabric. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]], \
    color Diffuse_TransmitColor = Diffuse_TransmitColorUIDefault \
    [[ \
        string page = "Diffuse", \
        string label = "Transmit Color (Double Sided)", \
        string help = \
            "Controls the front side response to light hitting the back side of the surface. " \
            "This does not simulate scattering through a solid object, " \
            "but is useful for open surfaces such as a sheet of fabric. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]], \
    float Diffuse_BackfaceGain = Diffuse_BackfaceGainUIDefault \
    [[ \
        string page = "Diffuse", \
        string label = "Backface Gain (Double Sided)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Combined with <strong>Backface Color</strong>, determines the alternate coloration " \
            "to use on the back faces of the geometry. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]], \
    color Diffuse_BackfaceColor = Diffuse_BackfaceColorUIDefault \
    [[ \
        string page = "Diffuse", \
        string label = "Backface Color (Double Sided)", \
        string help = \
            "Combined with <strong>Backface Gain</strong>, determines the alternate coloration " \
            "to use on the back faces of the geometry. " \
            "Only used when <strong>Double Sided</strong> is <em>On</em> " \
            "and <strong>Use Diffuse Color</strong> is <em>Off</em>. " \
    ]]


//
// Scatter parameter declarations.
//
#define SCATTER_UI_DECLARE(DESCRIPTION) \
    EG_UI_DECLARE( Scatter, Scatter, DESCRIPTION ), \
    \
    color Scatter_Color = Scatter_ColorUIDefault \
    [[ \
        string page = "Scatter", \
        string label = "Color", \
        string help = \
            "Controls the surface coloration (reflectivity) of this response: the surface albedo. " \
            "In addition, this has the effect of tinting the illumination with the opposite hue " \
            "before it penetrates into the interior of the object. " \
    ]], \
    color Scatter_PathColor = Scatter_PathColorUIDefault \
    [[ \
        string page = "Scatter", \
        string label = "Path Color", \
        string help = \
            "Allows you to control the spectral qualities of the internal scattering. " \
            "This should typically have an HSV color value of 1 (a full intensity color) " \
            "as it is multiplied with the <strong>Path Length</strong> to produce the " \
            "values used in the response computation. " \
            "A lower value means that particular color component will be absorbed more quickly " \
            "by the material and so will not travel as far through it. " \
    ]], \
    float Scatter_PathLength = Scatter_PathLengthUIDefault \
    [[ \
        string page = "Scatter", \
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
    float Scatter_Direction = Scatter_DirectionUIDefault \
    [[ \
        string page = "Scatter", \
        string label = "Direction", \
        int slider = 1, float slidermin = -1.0, float slidermax = 1.0, \
        string help = \
            "This controls the degree of forward to backward scattering. " \
            "Negative values produce back scattering " \
            "(brighter when viewed from the same side as the light). " \
            "Positive values produce forward scattering " \
            "(brighter when looking toward the light: i.e. back-lit). " \
            "When 0, scattering is isotropic. " \
    ]], \
    float Scatter_Roughness = Scatter_RoughnessUIDefault \
    [[ \
        string page = "Scatter", \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Represents the statistical deviation of the response's micro-facet normals " \
            "from the geometric normal. " \
            "The end result is that for higher <strong>Roughness</strong> values, " \
            "the silhouette view of the surface will become darker. " \
    ]], \
    float Scatter_RefractionIndex = Scatter_RefractionIndexUIDefault \
    [[ \
        string page = "Scatter", \
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
    float Scatter_IlluminationGain = Scatter_IlluminationGainUIDefault \
    [[ \
        string page = "Scatter", \
        string label = "Illumination Gain (Backside)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the degree to which direct illumination striking the back side of the object " \
            "will be included in the result. " \
    ]], \
    color Scatter_IlluminationTint = Scatter_IlluminationTintUIDefault \
    [[ \
        string page = "Scatter", \
        string label = "Illumination Tint (Backside)", \
        string help = \
            "Adjusts the coloration (tint) of the direct illumination striking the back side of the object. " \
    ]]


//
//  Subsurface parameter declarations.
//
#define SUBSURFACE_UI_DECLARE(DESCRIPTION) \
    EG_UI_DECLARE( Subsurface, Subsurface, DESCRIPTION ), \
    \
    color Subsurface_Color = Subsurface_ColorUIDefault \
    [[ \
        string page = "Subsurface", \
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
    ]], \
    color Subsurface_PathColor = Subsurface_PathColorUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Path Tint (not Multiple MFP)", \
        string help = \
            "Tints the Path <strong>Color</strong> used in the subsurface scattering computation. " \
            "It is not used by the <em>Multiple Mean Free Paths</em> <strong>Model</strong>. " \
    ]], \
    float Subsurface_PathLength = Subsurface_PathLengthUIDefault \
    [[ \
        string page = "Subsurface", \
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
    color Subsurface_PostTint = Subsurface_PostTintUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Post Tint", \
        string help = \
            "Modifies the resulting coloration. This is a completely non-physical control " \
            "you can use to force the result into the color you want. " \
    ]], \
    float Subsurface_TransmitGain = Subsurface_TransmitGainUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Transmit Gain (Double Sided)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Adjusts the gain of the response to light transmitted through the material. " \
    ]], \
    float Subsurface_Directionality = Subsurface_DirectionalityUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Directionality (Path Traced)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Increasing <strong>Directionality</strong> intensifies the amount of forward scattering " \
            "(when light rays tend to continue to travel in their original direction) that occurs within " \
            "the material. The end result is that hard corners will be accentuated and the response " \
            "will appear more diffuse overall. " \
    ]], \
    /* XXX Cannot make a connection to PxrSurface subsurfaceDiffuseBlend parameter \
    float Subsurface_DiffuseBlend = Subsurface_DiffuseBlendUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "(Path Traced) Diffuse Blend", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Blends the Subsurface response with a Diffuse one: makes the response appear more diffuse. " \
            "However, this can help when dark artifacts are seen on very sharp edges. " \
    ]], \
    */ \
    float Subsurface_Bleed = Subsurface_BleedUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Bleed (Non-Expon Path Traced)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Effectively increases the <strong>Path Length</strong> (i.e. <em>Mean Free Path</em>), " \
            "but without the reduction in detail that would accompany an increase in the " \
            "<strong>Path Length</strong> directly. " \
    ]], \
    float Subsurface_ShortGain = Subsurface_ShortGainUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Short Gain (Multiple MFP)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the <strong>Short</strong> " \
            "path length component of the response. " \
    ]], \
    color Subsurface_ShortColor = Subsurface_ShortColorUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Short Color (Multiple MFP)", \
        string help = \
            "Controls the spectral qualities of the <strong>Short</strong> " \
            "path length component of the response. " \
    ]], \
    float Subsurface_ShortLength = Subsurface_ShortLengthUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Short Length (Multiple MFP)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, \
        string help = \
            "Controls the <em>Mean Free Path</em> scattering distance used in the " \
            "<strong>Short</strong> path length component of the response. " \
    ]], \
    float Subsurface_LongGain = Subsurface_LongGainUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Long Gain (Multiple MFP)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the <strong>Long</strong> " \
            "path length component of the response. " \
    ]], \
    color Subsurface_LongColor = Subsurface_LongColorUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Long Color (Multiple MFP)", \
        string help = \
            "Controls the spectral qualities of the <strong>Long</strong> " \
            "path length component of the response. " \
    ]], \
    float Subsurface_LongLength = Subsurface_LongLengthUIDefault \
    [[ \
        string page = "Subsurface", \
        string label = "Long Length (Multiple MFP)", \
        int slider = 1, float slidermin = 0.0, float slidermax = 100.0, \
        string help = \
            "Controls the <em>Mean Free Path</em> scattering distance used in the " \
            "<strong>Long</strong> path length component of the response. " \
    ]]


//
//  Glass parameter declarations.
//
#define GLASS_UI_DECLARE(DESCRIPTION) \
    ENABLE_UI_DECLARE( Glass, Glass, DESCRIPTION ), \
    \
    float Glass_ReflectionGain = Glass_ReflectionGainUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Reflection Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the glass <strong>Reflection</strong>. " \
    ]], \
    float Glass_Roughness = Glass_RoughnessUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Roughness", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, float slidercenter = 0.25, \
        string help = \
            "Represents the statistical deviation of the specular micro-facet normals " \
            "from the surface normal. As such, it does not produce linear changes in the perceived " \
            "specular blurriness with uniform increments of the <strong>Roughness</strong> value. " \
            "But, bigger = blurrier, smaller = sharper. Only a value from 0 to 1 is allowed. " \
    ]], \
    \
    ANISOTROPY_UI_DECLARE( Glass, Glass ), \
    SHADINGNORMAL_UI_DECLARE( Glass, Glass ), \
    \
    float Glass_RefractionGain = Glass_RefractionGainUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Refraction Gain", \
        int slider = 1, float slidermin = 0.0, float slidermax = 1.0, \
        string help = \
            "Controls the intensity of the <strong>Refracted</strong> glass responses. " \
    ]], \
    float Glass_RefractionIndex = Glass_RefractionIndexUIDefault \
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
            "<br/>vero clear = 1.47" \
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
    color Glass_RefractionColor = Glass_RefractionColorUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Refraction Color", \
        string help = \
            "Sets the color of the glass. Affects the <strong>Refraction</strong> response by " \
            "tinting the light that passes into the glass. " \
    ]], \
    float Glass_ExtinctionCoeff = Glass_ExtinctionCoeffUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Extinction Coeff", \
        int slider = 1, float slidermin = 0.0, float slidermax = 5.0, \
        string help = \
            "Controls the light's <strong>Extinction</strong> within the object's interior. " \
            "Larger values attenuate more rapidly. " \
            "<br/><br/>" \
            "Note: this <em>must</em> be > 0 for <strong>Extinction Color</strong> and " \
            "<strong>Scatter Color</strong> to work. " \
    ]], \
    color Glass_ExtinctionColor = Glass_ExtinctionColorUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Extinction Color", \
        string conditionalVisPath = "../Glass_ExtinctionCoeff", \
        string conditionalVisOp = "greaterThan", \
        string conditionalVisValue = "0.0", \
        string help = \
            "Controls the light's <strong>Extinction</strong> within the object's interior. " \
            "Larger values attenuate more, so the visible color will have the opposite hue. " \
            "<br/><br/>" \
            "Note: this <em>must</em> be something other than black for " \
            "<strong>Scatter Color</strong> to work. " \
    ]], \
    color Glass_ScatterColor = Glass_ScatterColorUIDefault \
    [[ \
        string page = "Glass", \
        string label = "Scatter Color", \
        string conditionalVisPath = "../Glass_ExtinctionCoeff", \
        string conditionalVisOp = "greaterThan", \
        string conditionalVisValue = "0.0", \
        string help = \
            "<strong>Extinction Color</strong> <em>must</em> be something other than black for this " \
            "to work. " \
            "<br/><br/>" \
            "Light rays Refracted into the object's interior normally pass straight through " \
            "it and are only tinted by the <strong>Refraction Color.</strong> " \
            "<br/><br/>" \
            "<strong>Scatter Color</strong> will cause light to also be scattered " \
            "within the object's interior, as would happen if the glass had microscopic " \
            "particulates or bubbles within it. " \
    ]], \
    float Glass_ManifoldExplorationIOR = Glass_ManifoldExplorationIORDefault \
    [[ \
        string page = "Glass", \
        string label = "Manifold Exploration IOR", \
        int slider = 1, float slidermin = 1.0, float slidermax = 4.0, float slidercenter = 1.5, \
        string help = \
            "The index of refraction used for <strong>Manifold Exploration</strong>. " \
            "When set to -1, the <strong>Refraction Index</strong> will be used. " \
    ]]


/**************************************************************************************************
 *  Connection Struct and vstruct parameters
 **************************************************************************************************/
//
//  Composited values for a v2_PxrSurface material.
//
struct material_v2_PxrSurface_s
{
    // Dummy Socket variable.
    int    socket;

    // Layer values.
    vector coverV; // This material's covering height vector.

    // Accumulated Displacement values.
    float  dispMask;
    float  thickness;
    vector deltaP;

    // Accumulated Bxdf values.
    float  bxdfMask;
    float  maxMask;

    float  Displacement_BumpPercent;

    normal Global_ShadingNormal;
    float  Global_UseShadingNormal;
    color  Global_ShadowColor;
    color  Global_UserColor;

    color  ClearCoat_CG;
    color  ClearCoat_FaceCG;
    float  ClearCoat_FresnelExponent;
    color  ClearCoat_RefractionCI;
    color  ClearCoat_ExtinctionCC;
    float  ClearCoat_LayerThickness;
    color  ClearCoat_LayerColor;
    float  ClearCoat_Roughness;
    float  ClearCoat_Anisotropy;
    vector ClearCoat_AnisotropyDirection;
    float  ClearCoat_UseAnisotropyDirection;
    normal ClearCoat_ShadingNormal;
    float  ClearCoat_UseShadingNormal;

    color  PrimarySpecular_CG;
    color  PrimarySpecular_FaceCG;
    float  PrimarySpecular_FresnelExponent;
    color  PrimarySpecular_RefractionCI;
    color  PrimarySpecular_ExtinctionCC;
    float  PrimarySpecular_Roughness;
    float  PrimarySpecular_Anisotropy;
    vector PrimarySpecular_AnisotropyDirection;
    float  PrimarySpecular_UseAnisotropyDirection;
    normal PrimarySpecular_ShadingNormal;
    float  PrimarySpecular_UseShadingNormal;

    color  SecondarySpecular_CG;
    color  SecondarySpecular_FaceCG;
    float  SecondarySpecular_FresnelExponent;
    color  SecondarySpecular_RefractionCI;
    color  SecondarySpecular_ExtinctionCC;
    float  SecondarySpecular_Roughness;
    float  SecondarySpecular_Anisotropy;
    vector SecondarySpecular_AnisotropyDirection;
    float  SecondarySpecular_UseAnisotropyDirection;
    normal SecondarySpecular_ShadingNormal;
    float  SecondarySpecular_UseShadingNormal;

    float  Iridescence_Gain;
    color  Iridescence_FaceColor;
    color  Iridescence_EdgeColor;
    float  Iridescence_Exponent;
    float  Iridescence_Repetitions;
    float  Iridescence_Thickness;
    float  Iridescence_Roughness;
    float  Iridescence_Anisotropy;
    vector Iridescence_AnisotropyDirection;
    float  Iridescence_UseAnisotropyDirection;
    normal Iridescence_ShadingNormal;
    float  Iridescence_UseShadingNormal;

    color  Fuzz_CG;
    float  Fuzz_ConeAngle;
    normal Fuzz_ShadingNormal;
    float  Fuzz_UseShadingNormal;

    color  Diffuse_CG;
    float  Diffuse_Roughness;
    float  Diffuse_Exponent;
    color  Diffuse_BackfaceCG;
    color  Diffuse_TransmitCG;
    normal Diffuse_ShadingNormal;
    float  Diffuse_UseShadingNormal;

    color  Scatter_CG;
    float  Scatter_PathLength;
    color  Scatter_PathColor;
    float  Scatter_Direction;
    float  Scatter_Roughness;
    float  Scatter_RefractionIndex;
    color  Scatter_BacksideCG; // (Backside) IlluminationGain * (Backside) IlluminationTint

    color  Subsurface_CG;
    float  Subsurface_PathLength;
    color  Subsurface_PathColor;
    color  Subsurface_PostTint;
    float  Subsurface_TransmitGain;
    float  Subsurface_ShortLength;
    color  Subsurface_ShortCG;
    float  Subsurface_LongLength;
    color  Subsurface_LongCG;
    float  Subsurface_Directionality;
    float  Subsurface_DiffuseBlend;
    float  Subsurface_Bleed;

    // Scatter and Subsurface.
    color  ScatterAndSubsurface_IlluminationTint;
    float  ScatterAndSubsurface_Roughness;

    float  Glass_ReflectionGain;
    float  Glass_Roughness;
    float  Glass_Anisotropy;
    vector Glass_AnisotropyDirection;
    float  Glass_UseAnisotropyDirection;
    normal Glass_ShadingNormal;
    float  Glass_UseShadingNormal;
    color  Glass_RefractionCG;
    float  Glass_RefractionIndex;
    color  Glass_ExtinctionCC;
    color  Glass_ScatterColor;
    float  Glass_ManifoldExplorationIOR;

    color  Glow_CG;

    // Expansion members.
    color  Expansion_color0;
    color  Expansion_color1;
    color  Expansion_color2;
    color  Expansion_color3;
    float  Expansion_float0;
    float  Expansion_float1;
    float  Expansion_float2;
    float  Expansion_float3;
    float  Expansion_float4;
    float  Expansion_float5;
    float  Expansion_float6;
    float  Expansion_float7;
    vector Expansion_vector0;
    vector Expansion_vector1;
    normal Expansion_normal0;
    normal Expansion_normal1;
};

//
//  struct and vstruct naming conventions.
//
#define STRUCT_MEMBER(PREFIX,NAME) PREFIX##_##NAME
#define STRUCT_REFERENCE(STRUCT,PREFIX,NAME) STRUCT##.##PREFIX##_##NAME

#define VSTRUCT_PARAMETER(VSTRUCT,PREFIX,NAME) VSTRUCT##_##PREFIX##_##NAME
#define VSTRUCT_REFERENCE(VSTRUCT,PREFIX,NAME) #VSTRUCT "." #PREFIX "_" #NAME

//
//  Vstruct parameter.
//  This single parameter is used to make connections between all its
//  vstructmember parameters when it is connected. It is not used for
//  any other purpose.
//
#define PXRSURFACE_MATERIAL_VSTRUCT(NAME,LABEL,DESCRIPTION) \
    int NAME = 0 \
    [[ \
        string label = #LABEL, \
        string readOnly = "true", \
        string tag = "material_v2_PxrSurface", \
        string help = #DESCRIPTION \
    ]]

//
//  Struct parameter.
//  This is also a vstructmember so it gets connected when the vstruct parameter is connected.
//  All struct values are initially pre-multiplied by alpha = 0 for proper compositing.
//
#define PXRSURFACE_MATERIAL_STRUCT(VSTRUCT,PARAM) \
    material_v2_PxrSurface_s PARAM = { 0, 0.0 } \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string type = "struct", \
        string vstructmember = #VSTRUCT "." "Struct" \
    ]]

//
//  The Response Enable parameters are only used to determine what ultimately needs
//  to be connected to the bxdf node using the vstructConditionalExpr mechanism.
//  They are not used for any other purpose.
//
//  These are used to determine whether a response is used anywhere in the material layer stack.
//

//  Declare a Response Enable parameter, and link it to its corresponding vstruct member.
#define RESPONSE_ENABLE_PARAM(VSTRUCT,PREFIX) \
    int VSTRUCT_PARAMETER( VSTRUCT, PREFIX, Enable ) = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REFERENCE( VSTRUCT, PREFIX, Enable ) \
    ]]

#define SET_RESPONSE_ENABLE_PARAM_CONDITION(VSTRUCTOUT,PREFIX,CONDITION) \
    output int VSTRUCT_PARAMETER( VSTRUCTOUT, PREFIX, Enable ) = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REFERENCE( VSTRUCTOUT, PREFIX, Enable ), \
        string vstructConditionalExpr = #CONDITION \
    ]]

#define SET_RESPONSE_ENABLE_PARAM(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
        SET_RESPONSE_ENABLE_PARAM_CONDITION( VSTRUCTOUT, PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN##_##PREFIX##_##Enable == 1 \
                or ( Enable == 1 and PREFIX##_##Enable == 1 ) \
                ) \
            else set 0 \
        )

//
//  The ShadingNormal Enable parameters are only used to determine what ultimately needs
//  to be connected to the bxdf node using the vstructConditionalExpr mechanism.
//  They are not used for any other purpose.
//
//  These are used to determine whether a response's ShadingNormal is used
//  anywhere in the material layer stack.
//
#define SHADINGNORMAL_ENABLE_PARAM(VSTRUCT,PREFIX) \
    int VSTRUCT##_##PREFIX##_ShadingNormal_Enable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REFERENCE( VSTRUCT, PREFIX, ShadingNormal_Enable ) \
    ]]

#define SET_SHADINGNORMAL_ENABLE_PARAM_CONDITION(VSTRUCTOUT,PREFIX,CONDITION) \
    output int VSTRUCT_PARAMETER( VSTRUCTOUT, PREFIX, ShadingNormal_Enable ) = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REFERENCE( VSTRUCTOUT, PREFIX, ShadingNormal_Enable ), \
        string vstructConditionalExpr = #CONDITION \
    ]]

#define SET_SHADINGNORMAL_ENABLE_PARAM(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
        SET_SHADINGNORMAL_ENABLE_PARAM_CONDITION( VSTRUCTOUT, PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN##_##PREFIX##_##ShadingNormal_Enable == 1 \
                or ( Enable == 1 and PREFIX##_##Enable == 1 and PREFIX##_##ShadingNormal is connected ) \
                ) \
            else set 0 \
        )

//
//  AnisotropyDirection Enable parameter.
//  These are used to determine whether a response's AnisotropyDirection is used
//  anywhere in the material layer stack.
//
#define ANISOTROPYDIRECTION_ENABLE_PARAM(VSTRUCT,PREFIX) \
    int VSTRUCT##_##PREFIX##_AnisotropyDirection_Enable = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REFERENCE( VSTRUCT, PREFIX, AnisotropyDirection_Enable ) \
    ]]

#define SET_ANISOTROPYDIRECTION_ENABLE_PARAM_CONDITION(VSTRUCTOUT,PREFIX,CONDITION) \
    output int VSTRUCT_PARAMETER( VSTRUCTOUT, PREFIX, AnisotropyDirection_Enable ) = 0 \
    [[ \
        int connectable = 0, \
        string widget = "null", \
        string vstructmember = VSTRUCT_REFERENCE( VSTRUCTOUT, PREFIX, AnisotropyDirection_Enable ), \
        string vstructConditionalExpr = #CONDITION \
    ]]

#define SET_ANISOTROPYDIRECTION_ENABLE_PARAM(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
        SET_ANISOTROPYDIRECTION_ENABLE_PARAM_CONDITION( VSTRUCTOUT, PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN##_##PREFIX##_##AnisotropyDirection_Enable == 1 \
                or ( Enable == 1 and PREFIX##_##Enable == 1 and PREFIX##_##AnisotropyDirection is connected ) \
                ) \
            else set 0 \
        )

//
//  Enable parameter sets.
//
#define ALL_ENABLE_PARAMS(VSTRUCT,PREFIX) \
    RESPONSE_ENABLE_PARAM( VSTRUCT, PREFIX ), \
    SHADINGNORMAL_ENABLE_PARAM( VSTRUCT, PREFIX ), \
    ANISOTROPYDIRECTION_ENABLE_PARAM( VSTRUCT, PREFIX )

#define SET_ALL_ENABLE_PARAMS(VSTRUCTOUT,VSTRUCTIN,PREFIX) \
    SET_RESPONSE_ENABLE_PARAM( VSTRUCTOUT, VSTRUCTIN, PREFIX ), \
    SET_SHADINGNORMAL_ENABLE_PARAM( VSTRUCTOUT, VSTRUCTIN, PREFIX ), \
    SET_ANISOTROPYDIRECTION_ENABLE_PARAM( VSTRUCTOUT, VSTRUCTIN, PREFIX )


//
//  Struct member copy macros.
//  Used when the material doesn't produce the response itself,
//  so it simply passes the struct data from input to output.
//
#define PARAM_COPY(OUTPUT,INPUT,PREFIX,NAME) \
    OUTPUT.PREFIX##_##NAME = INPUT.PREFIX##_##NAME

#define CLEARCOAT_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FaceCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FresnelExponent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionCI ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, LayerThickness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, LayerColor ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, AnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseAnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define SPECULAR_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FaceCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, FresnelExponent ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionCI ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ExtinctionCC ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, AnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseAnisotropyDirection ); \
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
    PARAM_COPY( OUTPUT, INPUT, PREFIX, AnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseAnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define FUZZ_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ConeAngle ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal )
 
#define DIFFUSE_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Exponent ); \
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
    /* PARAM_COPY( OUTPUT, INPUT, PREFIX, DiffuseBlend ); */ \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Bleed )
 
#define GLASS_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ReflectionGain ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Roughness ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, Anisotropy ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, AnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseAnisotropyDirection ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, UseShadingNormal ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionCG ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, RefractionIndex ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ExtinctionCC ); \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ScatterColor );  \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, ManifoldExplorationIOR )
 
#define GLOW_COPY(OUTPUT,INPUT,PREFIX) \
    PARAM_COPY( OUTPUT, INPUT, PREFIX, CG )
 

//
//  Parameter compositing macro.
//  Used to composite the input material's response parameter over the current material's.
//
#define PARAM_OVER(OUTPUT,INPUT,PREFIX,PARAM,MASK) \
    OUTPUT.PREFIX##_##PARAM = INPUT.PREFIX##_##PARAM + ( PREFIX##_##PARAM * MASK )*( 1.0 - INPUT.##MASK )
 
#endif
