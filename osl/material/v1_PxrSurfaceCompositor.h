/*
 *  Copyright 2018 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
#ifndef MATERIAL_V1_PXRSURFACECOMPOSITOR_H
#define MATERIAL_V1_PXRSURFACECOMPOSITOR_H

#include "material/v1_PxrSurface.h"

/*
 *  Override the macros so they use two material inputs.
 */

//
//  Response Enable parameter.
//  Determines whether a given response is used by either of the input materials.
//
#undef  SET_RESPONSE_ENABLE_PARAM
#define SET_RESPONSE_ENABLE_PARAM(VSTRUCTOUT,VSTRUCTIN_A,VSTRUCTIN_B,PREFIX) \
        SET_RESPONSE_ENABLE_PARAM_CONDITION( VSTRUCTOUT, PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN_A##_##PREFIX##_##Enable == 1 \
                or \
                VSTRUCTIN_B##_##PREFIX##_##Enable == 1 \
                ) \
            else set 0 \
        )

//
//  ShadingNormal Enable parameter.
//  Determines whether a response's ShadingNormal is used by either of the input materials.
//
#undef  SET_SHADINGNORMAL_ENABLE_PARAM
#define SET_SHADINGNORMAL_ENABLE_PARAM(VSTRUCTOUT,VSTRUCTIN_A,VSTRUCTIN_B,PREFIX) \
        SET_SHADINGNORMAL_ENABLE_PARAM_CONDITION( VSTRUCTOUT, PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN_A##_##PREFIX##_##ShadingNormal_Enable == 1 \
                or \
                VSTRUCTIN_B##_##PREFIX##_##ShadingNormal_Enable == 1 \
                ) \
            else set 0 \
        )

//
//  AnisotropyDirection Enable parameter.
//  Determines whether a response's AnisotropyDirection is used by either of the input materials.
//
#undef  SET_ANISOTROPYDIRECTION_ENABLE_PARAM
#define SET_ANISOTROPYDIRECTION_ENABLE_PARAM(VSTRUCTOUT,VSTRUCTIN_A,VSTRUCTIN_B,PREFIX) \
        SET_ANISOTROPYDIRECTION_ENABLE_PARAM_CONDITION( VSTRUCTOUT, PREFIX, \
            set 1 \
            if ( \
                VSTRUCTIN_A##_##PREFIX##_##AnisotropyDirection_Enable == 1 \
                or \
                VSTRUCTIN_B##_##PREFIX##_##AnisotropyDirection_Enable == 1 \
                ) \
            else set 0 \
        )

//
//  All Enable parameters.
//
#undef  SET_ALL_ENABLE_PARAMS
#define SET_ALL_ENABLE_PARAMS(VSTRUCTOUT,VSTRUCTIN_A,VSTRUCTIN_B,PREFIX) \
    SET_RESPONSE_ENABLE_PARAM( VSTRUCTOUT, VSTRUCTIN_A, VSTRUCTIN_B, PREFIX ), \
    SET_SHADINGNORMAL_ENABLE_PARAM( VSTRUCTOUT, VSTRUCTIN_A, VSTRUCTIN_B, PREFIX ), \
    SET_ANISOTROPYDIRECTION_ENABLE_PARAM( VSTRUCTOUT, VSTRUCTIN_A, VSTRUCTIN_B, PREFIX )

/*
 *  Parameter compositing macros.
 */

//  Composites A over B.
#define A_OVER_B(OUTPUT,A,B,PREFIX,PARAM) \
    OUTPUT.PREFIX##_##PARAM = A.PREFIX##_##PARAM + ( B.PREFIX##_##PARAM )*( 1.0 - A.bxdfMask )

//  Composites A under B: B over A.
#define A_UNDER_B(OUTPUT,A,B,PREFIX,PARAM) \
    A_OVER_B(OUTPUT,B,A,PREFIX,PARAM)

#endif
