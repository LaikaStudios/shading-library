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
#ifndef coreV7rayinfo_h
#define coreV7rayinfo_h

#define rayinfo_s coreV7rayinfo_s
struct  coreV7rayinfo_s
{
    public  uniform string  type = "";

    // At depth = 0: label = __reyes or __camera (indicating hidden or raytrace hider).
    // At depth > 0: label has the user-supplied value.
    public  uniform string  label = "";

    // Raytracing info.
    public  uniform float   depth = 0;
    public  varying float   importance = 1;
    public  uniform float   shadowdepth = 0;
    public  uniform float   diffusedepth = 0;
    public  uniform float   speculardepth = 0;
    public  uniform float   subsurfacedepth = 0;

    public  uniform float   maxdepth = 1;
    public  uniform float   maxdiffusedepth = 1;
    public  uniform float   maxspeculardepth = 1;

    // Integration info.
    public  uniform string  shadingintent = "shadegrid";
    public  uniform float   integrationcounter = 0;
    public  uniform float   integrationlod = 0;
    public  uniform string  integrationmode = "";


    // Initialization.
    public void Init()
    {
        rayinfo (
#if __PRMAN_RELEASE__ >= 1800
            "shadingintent", shadingintent,
            "integrationcounter", integrationcounter,
            "integrationlod", integrationlod,
#endif
            "type", type,
            "label", label,
            "depth", depth,
            "importance", importance,
            "shadowdepth", shadowdepth,
            "diffusedepth", diffusedepth,
            "speculardepth", speculardepth,
            "subsurfacedepth", subsurfacedepth
        );

#if __PRMAN_RELEASE__ >= 1800
        option( "RiHider:integrationmode", integrationmode );
#endif

        option   ( "trace:maxdepth", maxdepth );
        attribute( "trace:maxdiffusedepth", maxdiffusedepth );
        attribute( "trace:maxspeculardepth", maxspeculardepth );
    }

    public  uniform float Depth()
    {
        return depth;
    }

    public  uniform string Type()
    {
        return type;
    }

    public  uniform string Label()
    {
        return label;
    }

    public  uniform float Camera()
    {
        return (type == "camera") ? 1 : 0;
    }

    public  uniform float Light()
    {
        return (type == "light") ? 1 : 0;
    }

    public  uniform float Specular()
    {
        return (type == "specular") ? 1 : 0;
    }

    public  uniform float Diffuse()
    {
        return (type == "diffuse") ? 1 : 0;
    }

    public  uniform float Transmission()
    {
        return (type == "transmission") ? 1 : 0;
    }
}

#endif
