/*
 *  Copyright(c) 2012 Laika, developed under contract by mitchpraterLLC.
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

#ifndef coshader_h
#define coshader_h

void coshaderBegin(uniform string category)
{
    shader coshaders[] = getshaders("category", category);
    uniform float numCoshaders = arraylength(coshaders);
    uniform float i;
    for (i=0; i < numCoshaders; i+=1) {
        if (hasmethod(coshaders[i], "begin") == 1) {
            coshaders[i]->begin();
        }
    }
}

void coshaderPostlighting(uniform string category;
                          output color Ci; output color Oi)
{
    shader coshaders[] = getshaders("category", category);
    uniform float numCoshaders = arraylength(coshaders);
    uniform float i;
    for (i=0; i < numCoshaders; i+=1) {
        if (hasmethod(coshaders[i], "postlighting") == 1) {
            coshaders[i]->postlighting(Ci, Oi);
        }
    }
}

#endif // coshader_h
