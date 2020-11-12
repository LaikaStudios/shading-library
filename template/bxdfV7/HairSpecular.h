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

#ifndef bxdfV7HairSpecular_h
#define bxdfV7HairSpecular_h

#include <stdrsl/RadianceSample.h>

#include <coreV7/sampleSize.h>
#include <coreV7/response.h>
#include <coreV7/fresnel.h>
#include <coreV7/blend.h>
#include <coreV7/bxdf.h>

#include <sl.h>
#include <math.h>

struct  bxdfV7HairSpecular_s : bxdf_s
{
    public  varying float   HighlightShift = 0;
    public  uniform float   HighlightWidth = 0;
    public  varying color   TransmitColor = color 0;
    public  varying float   ReflectIntensity = 0;
    public  varying float   Seed = 0;
    public  varying vector  Orientation = vector 0;
    public  varying color   DirectTint = color 1;
    public  varying color   IndirectTint = color 1;
    public  uniform float   SampleDensity = 1;
    
    private uniform float   m_numSamples = 0;

    private varying float   m_alphaR = 0, m_alphaTT = 0, m_alphaTRT = 0;
    private uniform float   m_betaR = 0, m_betaTT = 0, m_betaTRT = 0;
    private uniform float   m_gammaG =  PI * 15 / 180;  // 15 degrees.
    private uniform float   m_gammaTT = PI * 30 / 180;  // 30 degree transmission lobe phi width.

    private uniform float   m_kG = 2.0; // glint strength [.5, 5]
    private varying float   m_deltaG = 0;
    private varying vector  m_B0n = vector 0, m_B1n = vector 0;

    private varying vector  m_tangent = vector 0;


    // utility convert a vector to two angles
    private void getThetaPhi( varying vector tn; varying vector dir; output varying float theta, phi )
    {
        theta = PI * .5 - acos( dir.tn );   // [-Pi/2, Pi/2]
        phi = atan( dir.m_B0n, dir.m_B1n ); // [-Pi, Pi]
    }

    // Utility  Add two angles in [-Pi,Pi] and get a result in [-Pi,Pi]
    private varying float addPhi( varying float phi1, phi2 )
    {
        varying float   phiRet = phi1 + phi2; // [-2Pi, 2Pi]

        if (phiRet < -PI) phiRet += MATH_TWOPI;
        if (phiRet > PI)  phiRet -= MATH_TWOPI;

        return phiRet;
    }

    // utility, normalized (area == 1) gaussian
    private varying float gaussian( varying float mean; varying float dev; varying float t )
    {
        varying float   tn = t - mean;
        return (1 / (dev * MATH_SQRTTWOPI)) * exp( -tn*tn / (2*dev*dev) );
    }

    private varying color hspecular( varying color _Cbxdf; varying float thetaV, thetaL, phi )
    {
        varying float   phiD = abs(phi);
        varying float   thetaH = (thetaL  + thetaV) / 2;

        varying float   NR = cos( phiD/2 );

        varying color   R = ReflectIntensity * NR * gaussian( m_alphaR, m_betaR, thetaH );


        varying color   TRT = ReflectIntensity * TransmitColor * TransmitColor;
        varying float   glint = m_kG * gaussian( m_deltaG, m_gammaG, phiD ) * m_gammaG;

        TRT *= NR + glint;
        TRT *= gaussian( m_alphaTRT, m_betaTRT, thetaH );

        // Disney Artist Friendly Hair.
        // Didn't use a normalized gaussian for the gamma term.
        varying color   TT = TransmitColor;
        varying float   NTT = gaussian( 0, m_gammaTT, phiD - PI ) * m_gammaTT;

        TT *= NTT * gaussian( m_alphaTT, m_betaTT, thetaH );

        varying float   thetaD = (thetaL - thetaV) / 2;
        varying float   cosThetaD2 = cos(thetaD);

        cosThetaD2 *= cosThetaD2;

        return (R + TRT + TT) * _Cbxdf * cos(thetaL) / (cosThetaD2 * MATH_TWOPI);
    }

    // generate a random phi in [-Pi, Pi]
    private varying float genPhi( varying float e0; output varying float pdf )
    {
        pdf = 1.0 / MATH_TWOPI;
        return (e0 - .5) * MATH_TWOPI; //[-Pi, Pi]
    }

    // given a phi determine the probability that it would be picked
    private void evalPhi( varying float phi; output varying float pdf )
    {
        pdf =  1.0 / MATH_TWOPI;
    }

    // we sample based on a Cauchy distribution from
    // Importance Sampling for Hair Scattering, Ou et. al.
    // to avoid rejecting samples
    // we shift the single cauchy distribution around based on phi to
    // better target the distributions we are drawing from
    // put the mapping from phi to alpha/beta in one place
    private void getAlphaBeta( varying float phi; output varying float alpha; output varying float beta )
    {
        // distribute theta based on a (varying) gaussian
        varying float   cosPhi2 = cos(phi/2);

        // invert so 0 means r and trt terms and so things stay near 0 longer
        varying float   lerpTerm = 1 - cosPhi2;

        lerpTerm *= lerpTerm;
        lerpTerm *= lerpTerm; // ^4
        beta = (1 - lerpTerm) * .75 * m_betaTRT + lerpTerm * m_betaTT;
        alpha = (1 - lerpTerm) * m_alphaR + lerpTerm * m_alphaTT;
    }

    // given a phi and a random val, generate theta
    private varying float genTheta( varying float thetaV; varying float e1; varying float phi; output varying float pdf )
    {
        varying float   alpha, beta;
        getAlphaBeta( phi, alpha, beta );

        varying float   shiftedThetaV = (thetaV / 2) - alpha;
        varying float   invThetaHMax = atan((.25 * PI + shiftedThetaV) / beta);
        varying float   invThetaHMin = atan((-.25 * PI + shiftedThetaV) / beta);
        varying float   thetaH = beta * tan(e1 *(invThetaHMax - invThetaHMin) + invThetaHMin);

        // half angle to angle gives a 1/2, the rest is the windowed
        // Cauchy distribution
        pdf = beta;
        pdf /= 2 * (invThetaHMax-invThetaHMin) * (thetaH*thetaH + beta*beta);

        varying float   thetaNew = 2 * (thetaH + alpha) - thetaV;
        return clamp( thetaNew, -.4999*PI, .4999*PI );
    }

    // given a theta and a phi, determine the conditional probability for theta
    private void  evalTheta( varying float thetaV;  varying float theta;  varying float phi; output  varying float pdf )
    {
        varying float   alpha, beta;
        getAlphaBeta(phi, alpha, beta);

        varying float   thetaH = (theta + thetaV) * .5;

        thetaH -= alpha;
        varying float   shiftedThetaV = (thetaV / 2) - alpha;
        varying float   invThetaHMax = atan((.25 * PI + shiftedThetaV) / beta);
        varying float   invThetaHMin = atan((-.25 * PI + shiftedThetaV) / beta);

        // half angle to angle gives a 1/2, the rest is the windowed
        // Cauchy distribution
        pdf = beta;
        pdf /= 2 * (invThetaHMax-invThetaHMin) * (thetaH*thetaH + beta*beta);
    }

    public void prelighting( output response_s _response; output fresnel_s _fresnel; output blend_s _blend )
    {
        if (shortCircuit) return;

        m_tangent = normalize(dPdv);

        // B0n perpendicular to orientation and curve direction
        m_B0n = normalize( m_tangent^Orientation ); 

        // B1n in plane of Tn and orientation
        m_B1n = normalize( m_B0n^m_tangent );  

        // XXX do we expect users to pass in HighlightShift in the [-10, -5] range
        // which matches tables in papers, in which case we have an extra - .
        // or do we have a "friendly parameter that means amount of shift?
        m_betaR = PI * HighlightWidth / 180;    //[5,10]
        m_betaTT = m_betaR / 2;
        m_betaTRT = 3 * m_betaTT;

        m_alphaR = -PI * HighlightShift / 180;  //[-10, -5]
        m_alphaTT = -.5 * m_alphaR;
        m_alphaTRT = -1.5 * m_alphaR;

        // m_gammaTT is a constant

        varying float   glintLerp = noise( point(Seed, 13*Seed,  17*Seed) );
        m_deltaG = PI * mix( 30, 45, glintLerp ) / 180; // [30,45]

        // larger gammaG makes them visible more often
        // XXX we may want this (glint size) exposed as a parameter
        
        sampleSize_s    sampleSize;

        uniform float   responseSize = sampleSize->ConeToHemisphere( radians(HighlightWidth) );
        m_numSamples = sampleSize->SpecularDepthReduction( SampleDensity, responseSize, _response );
    }


    public void evaluateSamples(
        output  response_s      _response;
        output  fresnel_s       _fresnel;
        output  blend_s         _blend;
        uniform string          _context;

        output __radiancesample _samples[];
    ) {
        varying color   DITint = color 1;

        if (_context == "specular") DITint = DirectTint;
        if (_context == "indirectspecular") DITint = IndirectTint;

        // Don't respond to any samples.
        if (shortCircuit == 1 || gridmax( colorRGBToAvg(DITint) ) == 0)
        {
            bxdfZeroResponse( _samples );
            return;
        }

        extern uniform float    Eye;

        varying vector  Ic = _response->GetI( Eye );
        varying vector  V = -Ic;
        varying color   K = GetK( Eye, _response, _fresnel, _context );

        varying float   thetaV, phiV;
        getThetaPhi( m_tangent, V, thetaV, phiV );

        varying vector  dir;
        varying float   thetaL, phiL;
        varying float   phi, phiPdf, thetaPdf;
        varying float   sinTheta, cosTheta;

        varying color   Cmatl;

        uniform float   i, alen = arraylength( _samples );
        for (i=0; i < alen; i+=1)
        {
            dir = _samples[i]->direction;

            getThetaPhi( m_tangent, dir, thetaL, phiL );

            phi = addPhi( phiL, -phiV );
            evalPhi( phi, phiPdf );
            evalTheta( thetaV, thetaL, phi, thetaPdf );

            // compression near the poles (+- ctx->m_Tangent) gives 1/cos(theta)
            sinTheta = dir.m_tangent;
            cosTheta = sqrt( 1.00001 - sinTheta * sinTheta );

            Cmatl = hspecular( DITint*Cbxdf, thetaV, thetaL, phi );
            _samples[i]->setMaterialResponse( Cmatl * K, phiPdf*thetaPdf / cosTheta );
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
        varying color   DITint = color 1;

        if (_context == "specular") DITint = DirectTint;
        if (_context == "indirectspecular") DITint = IndirectTint;

        // Don't generate any samples.
        if (shortCircuit == 1 || gridmax( colorRGBToAvg(DITint) ) == 0) return;

        if (SampleDensity == 0)
        {
            AddSamples( 0, _samples, _nsamplesPerComponent );
            return;
        }

        if (_context != "diffuse")
        {
            extern uniform float    Eye;

            varying vector  V =  - _response->GetI( Eye );
            varying color   K = GetK( Eye, _response, _fresnel, _context );

            varying color   Cmatl;

            uniform float   start, size;
            AddSamples( m_numSamples, _samples, _nsamplesPerComponent, start, size );

            varying float   thetaV, phiV;
            getThetaPhi( m_tangent, V, thetaV, phiV );

            varying vector rnd[m_numSamples];
            randomstrat( 0, m_numSamples, rnd );

            uniform float   i;
            for (i=start; i < size; i+=1)  
            {
                // generate phi unifomly around hair
                float phiPdf, thetaPdf;
                float phi = genPhi(rnd[i][0], phiPdf);
                float thetaL = genTheta(thetaV, rnd[i][1], phi, thetaPdf);

                // get sin's and cos's
                float sinThetaL, cosThetaL, sinPhiL, cosPhiL;
                sincos(thetaL, sinThetaL, cosThetaL);
                sincos(addPhi(phiV, phi), sinPhiL, cosPhiL);

                vector dir = (m_tangent * sinThetaL + m_B0n * cosThetaL * sinPhiL + m_B1n * cosThetaL * cosPhiL);

                Cmatl = hspecular( DITint*Cbxdf, thetaV, thetaL, phi );

                _samples[i]->setMaterialSample( Cmatl * K, phiPdf*thetaPdf / cosThetaL, dir, MAXDIST );
            }

        }
        else
        {
            // diffuse MIS sample generation.
            AddSamples( 0, _samples, _nsamplesPerComponent );
        }
    }
}

#endif
