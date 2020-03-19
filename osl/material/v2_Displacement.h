/*
 *  Copyright 2018-2019 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
#ifndef MATERIAL_V2_DISPLACEMENT_H
#define MATERIAL_V2_DISPLACEMENT_H

#define DISPLACE_FLOAT_UI_DECLARE(NAME) \
    int Displacement_Float_On ## NAME = 0 \
    [[ \
        int connectable = 0, \
        string page = "Displacement.Float", \
        string label = "On " #NAME, \
        string widget = "mapper", \
        string options = "Off:0|On:1|Invert Input:-1", \
        string help = \
            "Turns the use of the <strong>Input " #NAME "</strong> displacement <em>On, " \
            "Off,</em>  or can <em>Invert</em> the Input value before " \
            "it's used: Input -> 1-Input. " \
    ]], \
    float Displacement_Float_Input ## NAME = 0.5 \
    [[ \
        string page = "Displacement.Float", \
        string label = "Input " #NAME, \
        string help = \
            "A (0,1) ranged value that determines this displacement's pattern. " \
            "Combined with the <strong>Position</strong> and <strong>Magnitude</strong> " \
            "(and optionally <strong>Mask</strong>), " \
            "it determines the location and amount of displacement along the surface normal. " \
    ]], \
    float Displacement_Float_Position ## NAME = 0.0 \
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
    float Displacement_Float_Magnitude ## NAME = 1.0 \
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
    float Displacement_Float_Mask ## NAME = 1.0 \
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
    int Displacement_Vector_On ## NAME = 0 \
    [[ \
        int connectable = 0, \
        string page = "Displacement.Vector", \
        string label = "On " #NAME, \
        string widget = "checkBox", \
        string help = \
            "Turns the use of the <strong>Input " #NAME "</strong> displacement <em>On</em> or <em>Off.</em> " \
    ]], \
    vector Displacement_Vector_Input ## NAME = vector(0.0) \
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
    int Displacement_Vector_Mode ## NAME = 1 \
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
    float Displacement_Vector_Magnitude ## NAME = 1.0 \
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
    float Displacement_Vector_Mask ## NAME = 1.0 \
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


float displace_float_pattern(
    int    mode, // 0=Off, 1=On, -1=Invert
    float  input,
    float  magnitude,
    float  mask
    )
{
    float  result = 0.0;

    if( mode == 1 ) result = input*magnitude*mask;
    else if( mode == -1 ) result = ( 1.0-input )*magnitude*mask;

    return result;
}

float displace_float_amount(
    int    mode, // 0=Off, 1=On, -1=Invert
    float  input,
    float  position,
    float  magnitude,
    float  mask
    )
{
    float  result = 0.0;

    if( mode == 1 ) result = ( input + mix( -0.5, 0.0, position ))*magnitude*mask;
    else if( mode == -1 ) result = (( 1.0-input ) + mix( -0.5, 0.0, position ))*magnitude*mask;

    return result;
}

#define DISPLACE_FLOAT_MAG(NAME) \
    (( Displacement_Float_On ## NAME ) ? abs( Displacement_Float_Magnitude ## NAME ) : 0.0 )

#define DISPLACE_FLOAT_PATTERN(NAME) \
    displace_float_pattern( \
        Displacement_Float_On ## NAME, \
        Displacement_Float_Input ## NAME, \
        Displacement_Float_Magnitude ## NAME, \
        Displacement_Float_Mask ## NAME \
        )

#define DISPLACE_FLOAT_AMOUNT(NAME) \
    displace_float_amount( \
        Displacement_Float_On ## NAME, \
        Displacement_Float_Input ## NAME, \
        Displacement_Float_Position ## NAME, \
        Displacement_Float_Magnitude ## NAME, \
        Displacement_Float_Mask ## NAME \
        )


// Computes a common space displacement vector with
// the given orientation and directional magnitude.
vector displace_vector(
    vector input,
    int    mode, // -1=Negate, 1=None, 0=Expand
    string directionSpace,
    string magnitudeSpace,
    float  magnitude,
    float  mask
    )
{
    vector  in = input;

    // Remapping modes.
    // in = select( in, mix( vector(-1.0), vector(1.0), in ), mode > 1 ); // Expand.
    // in = select( in, -in, mode == 1 || mode == 3 ); // Negate.
    if( mode == -1 )
    {
        in = -in;
    }
    else if( mode == 0 )
    {
        in = mix( vector(-1.0), vector(1.0), in );
    }

    // Determine the Magnitude Space scale relative to a common space vector.
    // Note: must use a vector transform to get the proper orienation,
    // and a normal transform to get the proper scale.
    normal Vmag = normalize( transform( directionSpace, "common", in )); // vector
    float  magScale = length( transform( "common", magnitudeSpace, Vmag )); // normal

    // Common space displacement vector with the original vector length.
    vector Vdir = Vmag * length( in );

    // Apply the Magnitude Space scaling, plus the unitless UI controls.
    vector  result = Vdir * magScale * magnitude * mask;

    // Done.
    return result;
}


#define DISPLACE_VECTOR_MAG(NAME) \
    (( Displacement_Vector_On ## NAME ) ? length( Displacement_Vector_Input ## NAME ) * Displacement_Vector_Magnitude ## NAME * Displacement_Vector_Mask ## NAME : 0.0 )

#define DISPLACE_VECTOR_PATTERN(NAME) \
    (( Displacement_Vector_On ## NAME ) ? 0.5*(dot( N, Displacement_Vector_Input ## NAME )+1.0) * Displacement_Vector_Magnitude ## NAME * Displacement_Vector_Mask ## NAME : 0.0 )

#define DISPLACE_VECTOR_AMOUNT(NAME,DIRSPACE,MAGSPACE) ( \
    Displacement_Vector_On ## NAME \
        ? displace_vector( \
            Displacement_Vector_Input ## NAME, \
            Displacement_Vector_Mode ## NAME, \
            DIRSPACE, \
            MAGSPACE, \
            Displacement_Vector_Magnitude ## NAME, \
            Displacement_Vector_Mask ## NAME \
            ) \
        : vector(0.0) \
    )

#endif
