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
    Functions that set the results are any type of view-independent energy splitting function.

    Results contain the division of light radiance between two response layers: the
    "reflected" and the "transmitted".
 */
#ifndef coreV7blend_h
#define coreV7blend_h

#define blend_s coreV7blend_s
struct  coreV7blend_s
{
    private varying color   Br = color 1;
    private varying color   Bt = color 0;


    public  void Set( varying color _Br; varying color _Bt )
    {
        Br = _Br;
        Bt = _Bt;
    }

    public  void SetBr( varying color _Br ) { Br = _Br; }
    public  void SetBt( varying color _Bt ) { Bt = _Bt; }

    public  color GetBr() { return Br; }
    public  color GetBt() { return Bt; }

    public  void Set( varying float _Br; varying float _Bt )
    {
        Br = color(_Br);
        Bt = color(_Bt);
    }

    public  void Set( varying color _Br; varying color _Bt )
    {
        Br = _Br;
        Bt = _Bt;
    }
}

#endif
