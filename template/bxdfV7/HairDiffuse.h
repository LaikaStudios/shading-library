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
#   2012-2014: Modified by pstuart to conform to Laika's bxdf Material response implementation.
#
*/

// Hair:
//
// a physically plausible specular component for hair. Models three
// specular transport paths:  
//      R (reflection), 
//      TRT (Transmission/Reflection/Transmission)
//      TT (Transmission/Transmission)
//
// Based on MARSCHNER, S. R., JENSEN, H. W., CAMMARANO, M., WORLEY, S., 
// AND HANRAHAN, P. 2003. Light Scattering from Human Hair Fibers. 
// ACM Transactions on Graphics 22, 3, 780– 791.
//
// Also: simplified according to Sedeghi, Pritchett, Jensen, Tamstorf 2010.
// An Artist Friendly Hair Shading System. ACM Trans. Graph. 29, 4.
// 
// sampled based on Cauchy distribution described by
// Ou, Xie, Krishnamachari, Pellacini,
// ISHair: Importance Sampling for Hair Scattering,  EGSR 2012.
// 
// Diffuse component based on:
// GOLDMAN, D. B. 1997. Fake Fur Rendering. In Proceedings of 
// SIGGRAPH 97, ACM Press/Addison-Wesley Publishing Co., New York, NY, 
// USA, 127–134.

#ifndef bxdfV7HairDiffuse_h
#define bxdfV7HairDiffuse_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <math.h>

struct  bxdfV7HairDiffuse_s : bxdf_s
{
    public  varying float   ScatterDirection = .5;

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

        varying vector  Tn = normalize(dPdv);
        varying vector  TcrossE = Tn ^ -Ic;

        varying vector  TcrossL;
        varying float   k, fdir, TdotL, kajiyaDiffuse;
        varying color   responseColor;

        uniform float   i, alen = arraylength( _samples );

        for (i=0; i < alen; i+=1)
        {
            TcrossL = Tn ^ _samples[i]->direction;

            k = linear( -1, 1, TcrossL . TcrossE );
            fdir = mix( k, 1-k, ScatterDirection );

            TdotL = Tn . _samples[i]->direction;
            kajiyaDiffuse = sqrt( 1-TdotL*TdotL );  // sin = 1 - cos*cos

            responseColor = Cbxdf * K * fdir * kajiyaDiffuse;
            _samples[i]->setMaterialResponse( responseColor, kajiyaDiffuse*MATH_INVPISQ );
        }

        accumulateMaterialResponse( _samples );
    }
}

#endif
