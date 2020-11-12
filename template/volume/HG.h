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
#   Modified to conform to Laika's Volume shader implementation 2014.
#
*/
#ifndef volumeHG_h
#define volumeHG_h

#include <stdrsl/RadianceSample.h>
#include <stdrsl/Math.h>

#include <volume/Common.h>

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

struct  volumeHG_s : volumeCommon_s
{
    public  varying float   G = 0;
    public  uniform float   SampleDensity = 1;
    public  varying float   DirectIntensity = 1;
    public  varying color   DirectTint = color 1;
    public  varying float   IndirectIntensity = 1;
    public  varying color   IndirectTint = color 1;

    private varying float   absG = 0;
    private varying float   m_one_plus_g2 = 1.0;
    private varying float   m_one_minus_g2 = 1.0;
    private varying float   m_one_over_2g = 1.0;

    private uniform float   sampleNum = 1;


    public void begin( uniform float depth; varying float importance )
    {
        volumeCommon_s::begin();

        sampleNum = SampleDensity;
        if (depth > 0) sampleNum = 1;
    }

    public void prelighting()
    {
        absG = abs(G);

        m_one_plus_g2 = 1 + G*G;
        m_one_minus_g2 = 1 - G*G;

        if (absG >= 0.0001)
        {
            m_one_over_2g = 0.5 / G;
        }
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
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[]
    ) {
        varying color   DITint = color(1);

        if (_context == "specular") DITint = colorSRGBToLinear( DirectIntensity * DirectTint );
        if (_context == "indirectspecular") DITint = colorSRGBToLinear( IndirectIntensity * IndirectTint );

        if (_context == "diffuse" || u_OnOff != 1 || gridmax( colorRGBToAvg(DITint)) == 0)
        {
            zeroMaterialResponse( _samples );
            return;
        }

        uniform float   i, alen = arraylength( _samples );
        varying float   mpdf;

        if (absG < 0.0001)
        {
            setConstantResponse( _samples, DITint * Cbxdf * M_INVFOURPI, M_INVFOURPI );
        }
        else
        {
            for (i=0; i < alen; i+=1)
            {
                mpdf = pdf( _Ic . _samples[i]->direction );

                _samples[i]->setMaterialResponse( DITint * Cbxdf * mpdf, mpdf );                
            }
        }

        accumulateMaterialResponse( _samples );
    }

    public void generateSamples(
        uniform string          _context;
        varying vector          _Ic;

        output __radiancesample _samples[];
        output uniform float    _nsamplesPerComponent[];
    ) {
        varying color   DITint = color(1);

        if (_context == "specular") DITint = colorSRGBToLinear( DirectIntensity * DirectTint );
        if (_context == "indirectspecular") DITint = colorSRGBToLinear( IndirectIntensity * IndirectTint );

        if (_context == "diffuse" || u_OnOff != 1 || gridmax( colorRGBToAvg(DITint)) == 0) return;

        if (SampleDensity == 0)
        {
            addSamples( 0, _samples, _nsamplesPerComponent );
            return;
        }

        varying float   xi1, xi2, costheta, sintheta, phi, sinphi, cosphi, mpdf;
        varying vector  dir;

        uniform float   i, start, size;
        addSamples( sampleNum, _samples, _nsamplesPerComponent, start, size );

        varying float   xdir = xcomp(_Ic);
        varying float   ydir = ycomp(_Ic);
        varying float   zdir = zcomp(_Ic);
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
                _samples[i]->setMaterialSample( DITint * Cbxdf * mpdf, mpdf, dir, MAXDIST );
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
                _samples[i]->setMaterialSample( DITint * Cbxdf * mpdf, mpdf, dir, MAXDIST );
            }
        }
    }
}

#endif
