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
    Contains data relevant to how the surface is being viewed:
        left camera, right camera, or traced ray.
 */

#ifndef coreV7camera_h
#define coreV7camera_h

#include <slim.h>

#define LEFT    0
#define RIGHT   1

#define camera_s coreV7camera_s
struct  coreV7camera_s
{
    // Does the right camera need to be computed or not?
    // This is based on the existence of a right camera and whether we're shading camera rays.
    // It will also incorporate the RightCamSpecular parameter and user:right_specular option.
    private uniform float   computeRight = 0;

    private uniform float   right_exists = 0;   // Is there a right camera?
    private uniform string  right_space = "";   // Right camera coordinate system.

    private varying vector  Ic[2] = {0};        // current space eye direction (camera to surface).
    private varying point   origin[2] = {0};    // current space camera location or ray origin.
    private varying float   worldLen[2] = {0};  // world space eye length.


    /*
     *  Init() must be called prior to calling any other methods.
     *  Init() will be called before we know what camera eye we're shading with.
     */
    public  void Init()
    {
        Ic[0] = normalize( I );
        origin[0] = SLIM_P - I; // Works for the left camera or a traced ray.
        worldLen[0] = length( vtransform( "current", "world", I ));
    }

    public  void Init( uniform float _depth; uniform float RightCamSpecular )
    {
        Ic[0] = normalize( I );
        origin[0] = SLIM_P - I; // Works for the left camera or a traced ray.
        worldLen[0] = length( vtransform( "current", "world", I ));

        if (option( "user:right_camera", right_space ) != 0)
        {
            if (right_space != "")
            {
                right_exists = 1;

                // There is no distinction between left and right cameras for non-camera rays;
                // there's just the incoming I direction.
                // Also, if we've turned off computing right camera specular responses, we can skip the rest
                // and leave computeRight = 0.
                if (_depth == 0 && RightCamSpecular == 1)
                {
                    origin[1] = transform( right_space, "current", point 0 );
                    Ic[1] = SLIM_P - origin[1];
                    worldLen[1] = length( vtransform( "current", "world", Ic[1] ));
                    Ic[1] = normalize( Ic[1] );

                    computeRight = 1;
                }
            }
        }
    }


    public  uniform float RightExists() { return right_exists; }
    public  uniform string RightSpace() { return right_space; }


    public  varying vector GetI( uniform float Eye ) { return Ic[Eye]; }
    public  varying vector GetLeftI()  { return Ic[0]; }
    public  varying vector GetRightI() { return Ic[1]; }


    public  varying point GetOrigin( uniform float Eye ) { return origin[Eye]; }
    public  varying point GetLeftOrigin()  { return origin[0]; }
    public  varying point GetRightOrigin() { return origin[1]; }


    public  varying float GetIWorldLength( uniform float Eye ) { return worldLen[Eye]; }
    public  varying float GetLeftIWorldLength()  { return worldLen[0]; }
    public  varying float GetRightIWorldLength() { return worldLen[1]; }

    public  uniform float ComputeRight() { return computeRight; }
}

#endif
