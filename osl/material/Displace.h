/*
 *  Copyright 2018 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License, Version 2.0, <LICENSE-APACHE or
 *  http://apache.org/licenses/LICENSE-2.0> or the MIT license <LICENSE-MIT or
 *  http://opensource.org/licenses/MIT>, at your option. This file may not be
 *  copied, modified, or distributed except according to those terms.
 */
#ifndef DISPLACE_MATERIAL_H
#define DISPLACE_MATERIAL_H

#define DISPLACE_FLOAT_UI_DECLARE(NAME) \
    int float_On_ ## NAME = 0 \
    [[ \
        int connectable = 0, \
        string page = "Displacement.Float", \
        string label = "On " #NAME, \
        string widget = "checkBox", \
        string help = \
            "Turns the use of this displacement <strong>Input</strong> On/Off. " \
    ]], \
    float float_Value_ ## NAME = 0.5 \
    [[ \
        string page = "Displacement.Float", \
        string label = "Input " #NAME, \
        string help = \
            "A (0,1) ranged value that determines this displacement's pattern. " \
            "Combined with the <strong>Position</strong> and <strong>Magnitude</strong> " \
            "(and optionally <strong>Mask</strong>), " \
            "it determines the location and amount of displacement along the surface normal. " \
    ]], \
    float float_Position_ ## NAME = 0.0 \
    [[ \
        string page = "Displacement.Float", \
        string label = "Position " #NAME, \
        int slider = 1, float slidermin = -1.0, float slidermax = 1.0, \
        string help = \
            "<strong>Position</strong> sets the vertical location of this displacement relative to the " \
            "existing surface. This will allow it to produce only peaks, a combination of peaks and valleys, " \
            "or just valleys. " \
            "<br/><br/>" \
            "With a <strong>Position</strong> of 1, this will result in the <strong>Input</strong> (0,1) " \
            "values creating displacement only above the current surface. " \
            "<br/><br/>" \
            "A <strong>Position</strong> of 0 will result in the <strong>Input</strong> " \
            "values (adjusted to the range -.5,+.5) producing displacement peaks above, and valleys below, " \
            "the current surface. " \
            "<br/><br/>" \
            "A <strong>Position</strong> of -1 will shift the <strong>Input</strong> values (to -1,0) " \
            "so they will only create valleys below the current surface. " \
    ]], \
    float float_Magnitude_ ## NAME = 1.0 \
    [[ \
        string page = "Displacement.Float", \
        string label = "Magnitude " #NAME, \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, float slidercenter = 1.0, \
        string help = \
            "The peak-to-valley distance produced by this displacement. " \
            "<br/><br/>" \
            "The assumption is that the <strong>Input</strong> pattern will range from 0 to 1; " \
            "so this parameter controls the actual displacement amount (in displacement " \
            "<strong>Mag Space</strong> units). " \
            "<br/><br/>" \
            "Note: you can use a negative value here to invert the <strong>Input</strong> pattern " \
            "and displacement direction (to -N). " \
    ]], \
    float float_Mask_ ## NAME = 1.0 \
    [[ \
        string page = "Displacement.Float", \
        string label = "Mask " #NAME, \
        int slider = 1, float slidermin = 0, float slidermax = 1, \
        string help = \
            "Controls how this displacement combines/composites " \
            "with the other <strong>Input</strong> displacements. " \
            "<br/><br/>" \
            "For example, if you want this displacement to exist only in the peaks " \
            "or valleys of another, you can connect a function of the other displacement's " \
            "<strong>Input</strong> here. " \
            "<br/><br/>" \
            "Alternatively, <strong>Mask</strong> values > 1 can be used to accentuate this " \
            "displacement in the desired areas. " \
            "Negative <strong>Mask</strong> values will invert this displacement. " \
            "<br/><br/>" \
            "Note: <strong>Mask</strong> values > 1 may result in displacements larger than " \
            "the <strong>Magnitude</strong> value. " \
    ]]


#define DISPLACE_VECTOR_UI_DECLARE(NAME) \
    int vector_On_ ## NAME = 0 \
    [[ \
        int connectable = 0, \
        string page = "Displacement.Vector", \
        string label = "On " #NAME, \
        string widget = "checkBox", \
        string help = \
            "Turns the use of this displacement <strong>Input</strong> On/Off. " \
    ]], \
    vector vector_Value_ ## NAME = vector(0.0) \
    [[ \
        string page = "Displacement.Vector", \
        string label = "Input " #NAME, \
        string help = \
            "A vector value that determines this displacement's direction and amount. " \
            "Typically, vectors have (-1,1) ranged component values. " \
            "Combined with the <strong>Mode</strong> and <strong>Magnitude</strong> " \
            "(and optionally <strong>Mask</strong>), " \
            "it determines how the surface is displaced. " \
    ]], \
    int vector_Mode_ ## NAME = 1 \
    [[ \
        int connectable = 0, \
        string page = "Displacement.Vector", \
        string label = "Mode " #NAME, \
        string widget = "mapper", \
        string options = "+:1|-:-1|+/-:0", \
        string help = \
            "<strong>+</strong> mode displaces in the <strong>Input</strong> vector direction. " \
            "<br/><br/>" \
            "<strong>-</strong> mode displaces in the opposite direction (<strong>-Input</strong>). " \
            "<br/><br/>" \
            "<strong>+/-</strong> takes an <strong>Input</strong> vector with (0,1) ranged values " \
            "and remaps them to (-1,+1). " \
            "<br/><br/>" \
            "Note: <strong>+/-</strong> Mode produces <em>double</em> the displacement range " \
            "of that produced by <strong>+</strong> or <strong>-</strong> for the same " \
            "<strong>Input</strong> data. " \
    ]], \
    float vector_Magnitude_ ## NAME = 1.0 \
    [[ \
        string page = "Displacement.Vector", \
        string label = "Magnitude " #NAME, \
        int slider = 1, float slidermin = 0.0, float slidermax = 10.0, float slidercenter = 1.0, \
        string help = \
            "The magnitude (vector length scale) of the displacement. " \
            "<br/><br/>" \
            "The assumption is that the <strong>Input</strong> vector will have a maximum length " \
            "of 1; so this parameter controls the actual displacement amount (in displacement " \
            "<strong>Mag Space</strong> units). " \
    ]], \
    float vector_Mask_ ## NAME = 1.0 \
    [[ \
        string page = "Displacement.Vector", \
        string label = "Mask " #NAME, \
        int slider = 1, float slidermin = 0, float slidermax = 1, \
        string help = \
            "Controls how this displacement combines/composites " \
            "with the other <strong>Input</strong> displacements. " \
            "<br/><br/>" \
            "For example, if you want this displacement to exist only in the peaks " \
            "or valleys of another, you can connect a function of the other displacement's " \
            "<strong>Input</strong> here. " \
            "<br/><br/>" \
            "Alternatively, <strong>Mask</strong> values > 1 can be used to accentuate this " \
            "displacement in the desired areas. " \
            "Negative <strong>Mask</strong> values will invert this displacement. " \
            "<br/><br/>" \
            "Note: <strong>Mask</strong> magnitudes > 1 may result in displacements larger than " \
            "the <strong>Magnitude</strong> value. " \
    ]]


#define DISPLACE_FLOAT_MAG(NAME) \
    (( float_On_ ## NAME == 0 ) ? 0.0 : float_Magnitude_ ## NAME )

#define DISPLACE_FLOAT_PATTERN(NAME) \
    (( float_On_ ## NAME == 0 ) ? 0.0 : float_Value_ ## NAME * float_Magnitude_ ## NAME * float_Mask_ ## NAME )

#define DISPLACE_FLOAT_AMOUNT(NAME) \
    (( float_On_ ## NAME == 0 ) ? 0.0 : ( float_Value_ ## NAME + mix( -0.5, 0.0, float_Position_ ## NAME )) * float_Magnitude_ ## NAME * float_Mask_ ## NAME )


vector displace_vector(
        vector value,
        int    mode,
        float  mag,
        float  mask
) {
    vector  result;

    if( mode != 0 ) result = value * mode;
    else result = mix( -1, 1, value );

    return result * mag * mask;
}

#define DISPLACE_VECTOR_MAG(NAME) \
    (( vector_On_ ## NAME == 0 ) ? 0.0 : length( vector_Value_ ## NAME ) * vector_Magnitude_ ## NAME )

#define DISPLACE_VECTOR_PATTERN(NAME) \
    (( vector_On_ ## NAME == 0 ) ? 0.0 : 0.5*(dot( N, vector_Value_ ## NAME )+1.0) * vector_Magnitude_ ## NAME * vector_Mask_ ## NAME )

#define DISPLACE_VECTOR_AMOUNT(NAME) \
    (( vector_On_ ## NAME == 0 ) ? vector(0.0) : displace_vector( vector_Value_ ## NAME, vector_Mode_ ## NAME, vector_Magnitude_ ## NAME, vector_Mask_ ## NAME ))

#endif
