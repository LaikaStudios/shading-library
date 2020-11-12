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
#ifndef matlattrs_h
#define matlattrs_h

#define matlattrs_s bxdfV7matlattrs_s
struct  bxdfV7matlattrs_s
{
    public  uniform string  SpecTraceSet = "";
    public  uniform string  SpecExcludeSet = "";
    public  uniform string  DiffTraceSet = "";
    public  uniform string  DiffExcludeSet = "";
    public  uniform string  SubsTraceSet = "";
    public  uniform string  SubsExcludeSet = "";

    public  uniform float   SpecIndirectOn = 1;
    public  uniform float   SpecDirectIntensity = 1;
    public  uniform float   SpecDirectSaturation = 1;
    public  uniform color   SpecDirectTint = color 1;
    public  uniform float   SpecIndirectIntensity = 1;
    public  uniform float   SpecIndirectSaturation = 1;
    public  uniform color   SpecIndirectTint = color 1;
    public  uniform float   SpecMaxDist = -1;

    public  uniform float   DiffIndirectOn = 1;
    public  uniform float   DiffDirectIntensity = 1;
    public  uniform float   DiffDirectSaturation = 1;
    public  uniform color   DiffDirectTint = color 1;
    public  uniform float   DiffIndirectIntensity = 1;
    public  uniform float   DiffIndirectSaturation = 1;
    public  uniform color   DiffIndirectTint = color 1;
    public  uniform float   DiffMaxDist = -1;

    // Initialization.
    public void Init()
    {
        attribute( "user:SpecTraceSet", SpecTraceSet );
        attribute( "user:SpecExcludeSet", SpecExcludeSet );
        attribute( "user:DiffTraceSet", DiffTraceSet );
        attribute( "user:DiffExcludeSet", DiffExcludeSet );
        attribute( "user:SubsTraceSet", SubsTraceSet );
        attribute( "user:SubsExcludeSet", SubsExcludeSet );

        attribute( "user:SpecMaxDist", SpecMaxDist );
        if (SpecMaxDist < 0) SpecMaxDist = MAXDIST;

        attribute( "user:SpecIndirectOn", SpecIndirectOn );
        attribute( "user:SpecDirectIntensity", SpecDirectIntensity );
        attribute( "user:SpecDirectSaturation", SpecDirectSaturation );
        attribute( "user:SpecDirectTint", SpecDirectTint );
        attribute( "user:SpecIndirectIntensity", SpecIndirectIntensity );
        attribute( "user:SpecIndirectSaturation", SpecIndirectSaturation );
        attribute( "user:SpecIndirectTint", SpecIndirectTint );

        attribute( "user:DiffMaxDist", DiffMaxDist );
        if (DiffMaxDist < 0) DiffMaxDist = MAXDIST;

        attribute( "user:DiffIndirectOn", DiffIndirectOn );
        attribute( "user:DiffDirectIntensity", DiffDirectIntensity );
        attribute( "user:DiffDirectSaturation", DiffDirectSaturation );
        attribute( "user:DiffDirectTint", DiffDirectTint );
        attribute( "user:DiffIndirectIntensity", DiffIndirectIntensity );
        attribute( "user:DiffIndirectSaturation", DiffIndirectSaturation );
        attribute( "user:DiffIndirectTint", DiffIndirectTint );
    }
}

#endif
