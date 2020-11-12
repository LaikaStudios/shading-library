/*
 *  Copyright(c) 2012-2014 Laika, developed under contract by mitchpraterLLC.
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
#ifndef coreV7bumpdisp_h
#define coreV7bumpdisp_h

#define bumpdisp_s coreV7bumpdisp_s
struct  coreV7bumpdisp_s
{
    public  varying point    Pin = point  0;
    public  varying vector   Nin = vector 0;

    public  varying point    Pcovered = point 0;
    public  varying vector   Ncovered = vector 0;

    public  varying point    Pout = point  0;
    public  varying vector   Nout = vector 0;
}

#endif
