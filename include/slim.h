/*
 *  Copyright(c) 2010-2012 Laika, developed under contract by mitchpraterLLC.
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
#ifndef SLIM_H
#define SLIM_H

#ifndef SLIM_SHADERTYPEID
    #define SLIM_SHADERTYPEID -1
#endif

#if SLIM_SHADERTYPEID == SLIM_TYPEID_volume
    #define SLIM_SHADERTYPE_P   Pv
    #define SLIM_P              Pv
    #define SLIM_P_STRING       "Pv"
#elif SLIM_SHADERTYPEID == SLIM_TYPEID_light
    #define SLIM_SHADERTYPE_P   Ps
    #define SLIM_P              Ps
    #define SLIM_P_STRING       "Ps"
#else
    #define SLIM_SHADERTYPE_P   P
    #define SLIM_P              P
    #define SLIM_P_STRING       "P"
#endif

#endif
