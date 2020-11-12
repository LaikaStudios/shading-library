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
#ifndef volumeGCL_h
#define volumeGCL_h

#include <stdrsl/RadianceSample.h>
#include <stdrsl/Math.h>

#include <volume/Common.h>

#include <sl.h>
#include <math.h>

struct  volumeGCL_s : volumeCommon_s
{
    public  varying float   Direction = 0;      // Determines response lobe direction.
    public  varying float   Exponent = 1;       // Response lobe exponent.

    private varying float   pow2Exponent = 1;   // 2^Response lobe exponent.


    public void begin()
    {
        volumeCommon_s::begin();

        pow2Exponent = _pow( 2.0, Exponent );
    }


    public void prelighting( varying vector Nvol )
    {
        Nbxdf = Nvol;

        IndiffCbxdf = Cbxdf;
    }


    public void evaluateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[]
    ) {
        if (_context != "diffuse")
        {
            return;
        }

        if (u_OnOff != 1)
        {
            zeroMaterialResponse( _samples );
            return;
        }

        varying vector  S = slerp( Nbxdf, reflect( _Ic, Nbxdf ), Direction );
        varying float   Kpdf = (pow2Exponent + 1) / MATH_TWOPI;

        varying color   responseColor;
        varying float   responsePdf, cosine;

        uniform float   i, alen = arraylength( _samples );

        for (i=0; i < alen; i+=1)
        {   
            cosine = mathGCL( S, _samples[i]->direction, pow2Exponent );

            responsePdf = Kpdf * cosine;
            responseColor = responsePdf * Cbxdf;

            _samples[i]->setMaterialResponse( responseColor, responsePdf );
        }

        accumulateMaterialResponse( _samples );
    }


    public void generateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        if (u_OnOff != 1) return;


        if (_context != "diffuse")
        {
            // specular sample generation.
            addSamples( 0, _samples, _nsamplesPerComponent );
        }
        else
        {
            // XXX diffuse MIS sample generation.
            addSamples( 0, _samples, _nsamplesPerComponent );
        }
    }
}

#endif
