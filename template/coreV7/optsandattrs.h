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
#ifndef optsandattrs_h
#define optsandattrs_h

#define optsandattrs_s coreV7optsandattrs_s
struct  coreV7optsandattrs_s
{
    public  uniform string  pass_class = "";    // Set by RMS.
    public  uniform float   right_specular = 1; // User option: compute right camera specular(1) or re-use left(0)?
    public  uniform float   areashadowbias = 0;

    // Global sample density control for all shader types.
    // Used in the absence of any type-based density attribute.
    public  uniform float   sample_density = 1; // sqrt( samples per hemisphere ).

    // Derived from sample_density.
    public  uniform float   samples_per_hemisphere = 1;
    public  uniform float   samples_per_steradian = 1;

    // Shader type-based sample densities.
    public  uniform float   surf_density = 1;   // sqrt( samples per hemisphere ).
    public  uniform float   light_density = 1;  // sqrt( samples per hemisphere ).
    public  uniform float   env_density = 1;    // Explicit number of samples.

    public  uniform string  hider_integrationmode = "";


    // Initialization.
    public void Init()
    {
        option( "user:pass_class", pass_class );

        option( "user:right_specular", right_specular );

#if __PRMAN_RELEASE__ >= 1800
        option( "RiHider:integrationmode", hider_integrationmode );
#endif

        // XXX tracebias ought to be set in rayinfo.h.
        uniform float   tracebias = 0.01;
        attribute( "trace:bias", tracebias );

        if (attribute( "user:areashadowbias", areashadowbias ) == 0) areashadowbias = tracebias;

        if (attribute( "user:sample_density", sample_density ) == 0) {
            sample_density = 4;
        }
        if (attribute( "user:surf_density", surf_density ) == 0) {
            surf_density = sample_density;
        }
        if (attribute( "user:light_density", light_density ) == 0) {
            light_density = sample_density;
        }
        if (attribute( "user:env_density", env_density ) == 0) {
            env_density = sample_density;
        }

        samples_per_hemisphere = sample_density*sample_density;
        samples_per_steradian = samples_per_hemisphere / TWOPI;
    }
}

#endif
