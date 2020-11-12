/*
 *  Copyright(c) 2011 Laika, developed under contract by mitchpraterLLC.
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
#define ifever_h
/*
 *  Don't #ifndef.
 *  Declaration must occur in any code block where ifever.h is included.
 */
uniform float   _ifever;

#define ifever(CONDITION) \
    _ifever = 0; if (CONDITION) _ifever = 1; \
    if (_ifever)

#define ifnever(CONDITION) \
    _ifever = 1; if (CONDITION) _ifever = 0; \
    if (_ifever)
