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

#ifndef coreV7surface_h
#define coreV7surface_h

#include <slim.h>
#include <prman.h>

#define surface_s coreV7surface_s
struct  coreV7surface_s
{
    private varying point   Pc = point 0;   // c suffix stands for current space.
    private varying normal  Nc = normal 0;
    private varying normal  Ngc = normal 0;
    private varying float   outside = 1;

    // Use the current prman surface state.
    public  void Init()
    {
        Pc = SLIM_P;
        Nc = normalize( N );
        Ngc = normalize( calculatenormal( Pc ));
        outside = prmanOutside();
    }

    // Set with your own values.
    public  void Init(
        varying point   _Pc;
        varying normal  _Nc;
        varying normal  _Ngc;
        varying float   _outside;
    ) {
        Pc = _Pc;
        Nc = _Nc;
        Ngc = _Ngc;
        outside = _outside;
    }

    public  void Init(
        varying point   _Pc;
        varying normal  _Nc;
        varying normal  _Ngc;
    ) {
        Pc = _Pc;
        Nc = _Nc;
        Ngc = _Ngc;
        outside = prmanOutside();
    }

    // coreV7 uses prman surface variables to contain surface state, so just grab what's in those.
    // We could just always use the prman variables and not use this struct at all; but using this
    // makes the code more explicit.
    public  void Update()
    {
        Pc = SLIM_P;
        Nc = normalize( N );
        Ngc = normalize( calculatenormal( Pc ));
    }

    public  void Update(
        varying point   _Pc;
        varying normal  _Nc;
    ) {
        Pc = _Pc;
        Nc = normalize( _Nc );
        Ngc = normalize( calculatenormal( Pc ));
    }

    // Get methods.
    public  varying point  GetP() { return Pc; }
    public  varying normal GetN() { return Nc; }
    public  varying vector GetN() { return vector( Nc ); }
    public  varying normal GetNg() { return Ngc; }
    public  varying vector GetNg() { return vector( Ngc ); }
    public  varying float  Outside() { return outside; }
}

#endif
