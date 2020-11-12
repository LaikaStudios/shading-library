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
#ifndef volumeCommon_h
#define volumeCommon_h

#include <stdrsl/RadianceSample.h>

#include <color.h>

struct  volumeCommon_s
{
    public  varying float   OnOff = 1;
    public  uniform float   u_OnOff = 0;
    public  varying float   Intensity = 0;
    public  varying color   Color = color(0);
    public  varying float   UseColor = 1;

    public  varying color   Cbxdf = color 0;
    public  varying vector  Nbxdf = vector 0;

    public  varying color   IndiffCbxdf = color 0;  // Indirect diffuse surface color.
                                                    // Cbxdf * indirect diffuse shaping.
                                                    // Apply shaping in prelighting().
    public void begin()
    {
        u_OnOff = max( 0, gridmax( OnOff ));
        Cbxdf = u_OnOff * colorSRGBToLinear( Intensity * mix( color(0.5), Color, UseColor ));
    }

    // Set IndiffCbxdf in the response's prelighting() method if it has an indirectdiffuse() response.
    public void prelighting()
    {
    }
}


void zeroMaterialResponse( output __radiancesample sample[] )
{
    uniform float   i, alen = arraylength( sample );
    for (i=0; i < alen; i+=1) sample[i]->setMaterialResponse( color 0, 0 );
}

void setConstantResponse( output __radiancesample sample[]; varying color Cpdf; varying float pdf )
{
    uniform float   i, alen = arraylength( sample );
    for (i=0; i < alen; i+=1) sample[i]->setMaterialResponse( Cpdf, pdf );
}

void addSamples( uniform float _addSampleNum;
    output __radiancesample _samples[];
    output uniform float    _nsamplesPerComponent[];
    output uniform float    _oldSampleNum;
    output uniform float    _newSampleNum;
) {
    _oldSampleNum = arraylength( _samples );

    if (_addSampleNum < 1)
    {
        push( _nsamplesPerComponent, 0 );
        _newSampleNum = _oldSampleNum;
        return;
    }

    _newSampleNum = _oldSampleNum + _addSampleNum;

    resize( _samples, _newSampleNum );

    // New resize() entries are not initialized to 0.
    uniform float   i;
    for (i=_oldSampleNum; i < _newSampleNum; i+=1)
    {
        _samples[i]->setMaterialResponse( color 0, 0 );
    }

    // Allocate new nsamplesPerComponent array entry.
    push( _nsamplesPerComponent, _addSampleNum );
}

// Don't return new sample indicies.
void addSamples( uniform float _addSampleNum;
    output __radiancesample _samples[];
    output uniform float    _nsamplesPerComponent[];
) {
    uniform float   oldSampleNum, newSampleNum;

    addSamples(
        _addSampleNum,
        _samples,
        _nsamplesPerComponent,
        oldSampleNum,
        newSampleNum
    );
}

#endif
