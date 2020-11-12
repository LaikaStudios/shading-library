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
#ifndef volumeScatter_h
#define volumeScatter_h

#include <stdrsl/RadianceSample.h>
#include <stdrsl/Math.h>

#include <volume/Common.h>

#include <sl.h>

struct  volumeScatter_s : volumeCommon_s
{
    public  varying float   Direction = 1;
    public  varying float   Exponent = 1;
    public  uniform float   SampleDensity = 1;


    public void prelighting( varying vector Nvol; varying vector _Ic )
    {
        Nbxdf = Nvol;

        varying float   scatter = mathGCL( _Ic, Nbxdf, 2, 1 );
        scatter = mix( 1-scatter, scatter, Direction );
        scatter = pow( scatter, 0.5*Exponent );

        IndiffCbxdf = Cbxdf * scatter;
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

        varying float   Kpdf = (Exponent + 1) * M_INVFOURPI;

        varying vector  dir;
        varying float   scatter, view;

        varying color   responseColor;
        varying float   responsePdf, cosine;

        uniform float   i, alen = arraylength( _samples );

        for (i=0; i < alen; i+=1)
        {
            dir =  _samples[i]->direction;

            scatter = mathGCL( _Ic, dir, 2, 1 );
            scatter = mix( 1-scatter, scatter, Direction );
            scatter = pow( scatter, Exponent );

            view = mathGCL( Nbxdf, dir, 1, 1 );

            responsePdf = Kpdf * scatter;
            responseColor = responsePdf * view * Cbxdf;

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
