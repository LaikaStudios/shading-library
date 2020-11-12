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
#ifndef volumeRad_h
#define volumeRad_h

#include <volume/Common.h>

#include <sl.h>
#include <color.h>

struct  volumeRad_s : volumeCommon_s
{
    public  varying float   CameraIntensity = 1;
    public  varying color   CameraTint = color 1;
    public  varying float   DiffIntensity = 1;
    public  varying color   DiffTint = color 1;
    public  varying float   SpecIntensity = 1;
    public  varying color   SpecTint = color 1;

    public  varying color   cameraContribution = color 1;
    public  uniform color   diffContribution = color 1;
    public  uniform color   specContribution = color 1;

    public void prelighting()
    {
        cameraContribution = colorSRGBToLinear( CameraTint * CameraIntensity );
        diffContribution = gridavg( colorSRGBToLinear( DiffTint * DiffIntensity ));
        specContribution = gridavg( colorSRGBToLinear( SpecTint * SpecIntensity ));
    }


    public void evaluateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[]
    ) {
    }


    public void generateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
    }
}

#endif
