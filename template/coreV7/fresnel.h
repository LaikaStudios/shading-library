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
/*
    The name is a bit of a misnomer. Functions that set the results are any type
    of view-dependent energy splitting function, whether it's Fresnel-based or not.
    And they don't have to be view-dependent either; the Material shader will still
    treat them as such though.

    Results contain the division of light radiance between two response layers: the
    "reflected" and the "transmitted".
 */
#ifndef coreV7fresnel_h
#define coreV7fresnel_h

#include <coreV7/camera.h>
#include <coreV7/surface.h>

#define fresnel_s coreV7fresnel_s
struct  coreV7fresnel_s
{
    // Fresnel.slim template functions use these values to compute its results,
    // which are stored in the Outputs. Copies of data are necessary since this
    // struct provides the only communication to a Fresnel template's function call.
    private varying float   outside = 1;        // Copied from surface_s
    private uniform float   computeRight = 0;   // Copied from camera_s

    private varying vector  Ic[2] = {0};    // Copied from camera_s (ray origin to surface)
    private varying vector  Nf[2] = {0};    // faceforward() orients this so it points opposite I: Ic.Nf <= 0.

    // Outputs: results of Fresnel template computations.
    private varying color   Kr[2] = {0};
    private varying color   Kt[2] = {0};
    private varying vector  R[2] = {0};
    private varying vector  T[2] = {0};


    public  void Init (
        camera_s    _cameraData;
        surface_s   _surfaceData;
    ) {
        outside = _surfaceData->Outside();
        computeRight = _cameraData->ComputeRight();

        varying normal  Nn = _surfaceData->GetNg(); // Normalized geometric surface.

        Ic[0] = _cameraData->GetLeftI();
        Nf[0] = vector faceforward( Nn,Ic[0],Nn );

        Ic[1] = _cameraData->GetRightI();
        Nf[1] = vector faceforward( Nn,Ic[1],Nn );
    }

    // Init with your own values.
    public  void Init (
        varying normal  _Nn;
        varying float   _outside;
        varying vector  _l_I;
        varying vector  _r_I;
    ) {
        outside = _outside;

        Ic[0] = _l_I;
        Ic[1] = _r_I;

        Nf[0] = vector faceforward( _Nn,_l_I,_Nn );
        Nf[1] = vector faceforward( _Nn,_r_I,_Nn );
    }

    // Init() should be called before calling the following methods.

    // Get methods.
    public  varying float Outside() { return outside; }
    public  uniform float ComputeRight() { return computeRight; }

    public  varying vector GetI( uniform float Eye ) { return Ic[Eye]; }
    public  varying vector GetLeftI()  { return Ic[0]; }
    public  varying vector GetRightI() { return Ic[1]; }

    public  varying vector GetNf( uniform float Eye ) { return Nf[Eye]; }
    public  varying vector GetLeftNf()  { return Nf[0]; }
    public  varying vector GetRightNf() { return Nf[1]; }

    public  void GetLeft( output varying vector _I; output varying vector _Nf )
    {
        _I = Ic[0];
        _Nf = Nf[0];
    }

    public  void GetRight( output varying vector _I; output varying vector _Nf )
    {
        _I = Ic[1];
        _Nf = Nf[1];
    }

    public  varying color GetKr( uniform float Eye ) { return Kr[Eye]; }
    public  varying color GetLeftKr()  { return Kr[0]; }
    public  varying color GetRightKr() { return Kr[1]; }
    public  varying color GetAvgKr()
    {
        return (computeRight == 0) ? Kr[0] : mix( Kr[0], Kr[1], 0.5 );
    }

    public  varying color GetKt( uniform float Eye ) { return Kt[Eye]; }
    public  varying color GetLeftKt()  { return Kt[0]; }
    public  varying color GetRightKt() { return Kt[1]; }
    public  varying color GetAvgKt()
    {
        return (computeRight == 0) ? Kt[0] : mix( Kt[0], Kt[1], 0.5 );
    }

    public  varying vector GetR( uniform float Eye ) { return R[Eye]; }
    public  varying vector GetLeftR()  { return R[0]; }
    public  varying vector GetRightR() { return R[1]; }

    public  varying vector GetT( uniform float Eye ) { return T[Eye]; }
    public  varying vector GetLeftT()  { return T[0]; }
    public  varying vector GetRightT() { return T[1]; }


    // Set methods used by the Fresnel function to set the outputs.
    public  void SetLeft(
        varying float   _Kr;
        varying float   _Kt;
    ) {
        Kr[0] = _Kr;
        Kt[0] = _Kt;
        R[0] = reflect( Ic[0],Nf[0] );
        T[0] = Ic[0];
    }
    public  void SetLeft(
        varying color   _Kr;
        varying color   _Kt;
    ) {
        Kr[0] = _Kr;
        Kt[0] = _Kt;
        R[0] = reflect( Ic[0],Nf[0] );
        T[0] = Ic[0];
    }

    public  void SetLeft(
        varying float   _Kr;
        varying float   _Kt;
        varying vector  _R;
        varying vector  _T;
    ) {
        Kr[0] = _Kr;
        Kt[0] = _Kt;
        R[0]  = _R;
        T[0]  = _T;
    }
    public  void SetLeft(
        varying color   _Kr;
        varying color   _Kt;
        varying vector  _R;
        varying vector  _T;
    ) {
        Kr[0] = _Kr;
        Kt[0] = _Kt;
        R[0]  = _R;
        T[0]  = _T;
    }


    public  void SetRight(
        varying float   _Kr;
        varying float   _Kt;
    ) {
        Kr[1] = _Kr;
        Kt[1] = _Kt;
        R[1] = reflect( Ic[1],Nf[1] );
        T[1] = Ic[1];
    }
    public  void SetRight(
        varying color   _Kr;
        varying color   _Kt;
    ) {
        Kr[1] = _Kr;
        Kt[1] = _Kt;
        R[1] = reflect( Ic[1],Nf[1] );
        T[1] = Ic[1];
    }

    public  void SetRight(
        varying float   _Kr;
        varying float   _Kt;
        varying vector  _R;
        varying vector  _T;
    ) {
        Kr[1] = _Kr;
        Kt[1] = _Kt;
        R[1]  = _R;
        T[1]  = _T;
    }
    public  void SetRight(
        varying color   _Kr;
        varying color   _Kt;
        varying vector  _R;
        varying vector  _T;
    ) {
        Kr[1] = _Kr;
        Kt[1] = _Kt;
        R[1]  = _R;
        T[1]  = _T;
    }


    public  void Set(
        varying float   _Kr;
        varying float   _Kt;
    ) {
        SetLeft( _Kr, _Kt );
    }
    public  void Set(
        varying color   _Kr;
        varying color   _Kt;
    ) {
        SetLeft( _Kr, _Kt );
    }
    public  void Set(
        varying float   _Kr;
        varying float   _Kt;
        varying vector  _R;
        varying vector  _T;
    ) {
        SetLeft( _Kr, _Kt, _R, _T );
    }
    public  void Set(
        varying color   _Kr;
        varying color   _Kt;
        varying vector  _R;
        varying vector  _T;
    ) {
        SetLeft( _Kr, _Kt, _R, _T );
    }
}

#endif
