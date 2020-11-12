/*
 *  Copyright(c) 2013-2014 Laika, developed under contract by mitchpraterLLC.
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
#ifndef meV7BumpDispSocket_h
#define meV7BumpDispSocket_h

struct meV7BumpDispSocket_s
{
    private varying point   P = point 0;
    private varying vector  N = vector 0;

    // Both.
    public void Get( output point _P; output vector _N )
    {
        _P = P;
        _N = N;
    }

    public void Set( point _P; vector _N )
    {
        P = _P;
        N = _N;
    }

    // P.
    public point GetP()
    {
        return P;
    }
    public void GetP( output point _P )
    {
        _P = P;
    }

    public void SetP( point _P )
    {
        P = _P;
    }

    // N.
    public vector GetN()
    {
        return N;
    }
    public void GetN( output vector _N )
    {
        _N = N;
    }

    public void SetN( vector _N )
    {
        N = _N;
    }
}

#endif
