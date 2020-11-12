/* $Revision: #1 $  $Date: 2012/09/17 $ */
/*
# ------------------------------------------------------------------------------
#
# Copyright (c) 2012 Pixar Animation Studios. All rights reserved.
#
# The information in this file is provided for the exclusive use of the
# software licensees of Pixar.  It is UNPUBLISHED PROPRIETARY SOURCE CODE
# of Pixar Animation Studios; the contents of this file may not be disclosed
# to third parties, copied or duplicated in any form, in whole or in part,
# without the prior written permission of Pixar Animation Studios.
# Use of copyright notice is precautionary and does not imply publication.
#
# PIXAR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
# ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL PIXAR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES
# OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
# ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
# SOFTWARE.
#
# Pixar
# 1200 Park Ave
# Emeryville CA 94608
#
# ------------------------------------------------------------------------------
#
#   2014: Modified to conform to Laika's bxdfV7 Material response implementation.
#
*/
#ifndef bxdfV7HG_h
#define bxdfV7HG_h

#include <stdrsl/RadianceSample.h>
#include <stdrsl/Math.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>

// HG.h
//   A simple BSDFComponent: an anisotropic scattering volumetric
//   specular component parameterized by a color and a
//   scattering parameter "g", which is the mean cosine of scattering
//   interactions. g is between -1 and 1. A value of g > 0 indicates
//   the volume is primarily forward-scattering while g < 0 indicates
//   the volume is backwards-scattering. g = 0 means the volume is
//   isotropic.
//

struct  bxdfV7HG_s : bxdf_s
{
    public  varying float   G = 0;

    private varying float   absG = 0;
    private varying float   m_one_plus_g2 = 1.0;
    private varying float   m_one_minus_g2 = 1.0;
    private varying float   m_one_over_2g = 1.0;

    private uniform float   sampleNum = 1;
    
    public void prelighting(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
    ) {
        if (shortCircuit) return;

        absG = abs(G);

        m_one_plus_g2 = 1 + G*G;
        m_one_minus_g2 = 1 - G*G;

        if (absG >= .0001)
        {
            m_one_over_2g = .5 / G;
        }
#if 0
        sampleSize_s    sampleSize;

        uniform float   responseSize = mix( 1/4, 1, cos( HALFPI*gridmax(absG) ));
        sampleNum = sampleSize->SpecularDepthReduction( SampleDensity, responseSize, _response );
#endif
    }

    private float pdf( float costheta )
    {
        return M_INVFOURPI*m_one_minus_g2 / pow( m_one_plus_g2 - 2*G*costheta, 1.5 );        
    }

    // Assumes G is not close to zero, since it divides by G
    private float invertcdf( float xi )
    {
        float   sqrterm = m_one_minus_g2 / (1 - G + 2*G*xi);
        return m_one_over_2g * (m_one_plus_g2 - sqrterm * sqrterm);
    }

    public void evaluateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
    ) {
        if (shortCircuit)
        {
            bxdfZeroResponse( _samples );   // Don't respond to any samples.
            return;
        }

        extern uniform float    Eye;

        varying vector  Ic = _response->GetI( Eye );
        varying color   K = GetK( Eye, _response, _fresnel, _context );

        uniform float   i, alen = arraylength( _samples );
        varying float   mpdf;

        if (absG < 0.0001)
        {
            for (i=0; i < alen; i+=1)
            {
                _samples[i]->setMaterialResponse( K * Cbxdf * M_INVFOURPI, M_INVFOURPI );
            }
        }
        else
        {
            for (i=0; i < alen; i+=1)
            {
                mpdf = pdf( Ic . _samples[i]->direction );

                _samples[i]->setMaterialResponse( K * Cbxdf * mpdf, mpdf );                
            }
        }

        accumulateMaterialResponse( _samples );
    }

    public void generateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        if (shortCircuit) return;   // Don't generate any samples if this response is off.

#if 1
        AddSamples( 0, _samples, _nsamplesPerComponent );
#else
        // Material global variable.
        extern uniform float    Eye;

        varying float   xi1, xi2, costheta, sintheta, phi, sinphi, cosphi, mpdf;
        varying vector  dir;

        uniform float   i, start, size;
        AddSamples( sampleNum, _samples, _nsamplesPerComponent, start, size );

        varying vector  Ic = _response->GetI( Eye );

        varying float   xdir = xcomp(Ic);
        varying float   ydir = ycomp(Ic);
        varying float   zdir = zcomp(Ic);
        varying float   omz2 = sqrt( 1 - zdir*zdir );

        // Special case for g = 0 (otherwise we divide by zero).
        // This is just isotropic scattering, so pick random points on
        // a sphere for directions.
        if (absG < 0.0001)
        {
            for (i=start; i < size; i+=1)
            {
                xi1 = random();
                xi2 = random();
                costheta = 1 - 2 * xi1;
                sintheta = sqrt( 1 - costheta * costheta );
                phi = M_TWOPI * xi2;
                sincos( phi, sinphi, cosphi );
                setxcomp( dir, sintheta * cosphi );
                setycomp( dir, sintheta * sinphi );
                setzcomp( dir, costheta );

                mpdf = M_INVFOURPI;
                _samples[i]->setMaterialSample( Cbxdf * mpdf, mpdf, dir, MAXDIST );
            }
        }
        else
        {
            // Compute new set of directional cosines based on I.
            for (i=start; i < size; i+=1)
            {
                xi1 = random();
                xi2 = random();
                costheta = invertcdf( xi1 );
                sintheta = sqrt( 1 - costheta*costheta );
                phi = M_TWOPI * xi2;
                sincos( phi, sinphi, cosphi );

                if (zdir > 0.9999)
                {
                    setxcomp(dir, sintheta * cosphi);
                    setycomp(dir, sintheta * sinphi);
                    setzcomp(dir, costheta);
                }
                else if (zdir < -0.9999)
                {
                    setxcomp(dir, sintheta * cosphi);
                    setycomp(dir, -sintheta * sinphi);
                    setzcomp(dir, -costheta);
                }
                else
                {
                    setxcomp( dir, sintheta * (xdir*zdir*cosphi - ydir*sinphi) / omz2 + xdir * costheta );
                    setycomp( dir, sintheta * (ydir*zdir*sinphi + xdir*sinphi) / omz2 + ydir * costheta );
                    setzcomp( dir, -omz2 * sintheta * cosphi + zdir * costheta );
                }

                mpdf = pdf( costheta );
                _samples[i]->setMaterialSample( Cbxdf * mpdf, mpdf, dir, MAXDIST );
            }
        }
#endif
    }
}

#endif
