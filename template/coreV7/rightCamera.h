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
#ifndef coreV7rightCamera_h
#define coreV7rightCamera_h

#include <slim.h>

#define rightCamera_s coreV7rightCamera_s
struct  coreV7rightCamera_s
{
    private uniform string  space = "";         // current space right camera coordinate system.
    private uniform point   rightCam = point 0; // current space right camera location.
    private uniform point   midCam = point 0;   // Mid-way between left and right cameras: average camera position.
    private varying vector  rightI = vector 0;  // current space right camera eye vector.
    private varying vector  midI = vector 0;    // current space mid-way camera eye vector.

    private uniform float   checked = 0;
    private uniform float   exists = 0;

    public  void Init()
    {
        if (checked) return;

        if (option ("user:right_camera", space) != 0)
        {
            if (space != "")
            {
                rightCam = transform( space, "current", point 0 );
                rightI = normalize( SLIM_P - rightCam );

                midCam = point(0.5) * rightCam;
                midI = normalize( SLIM_P - midCam );

                exists = 1;
            }
        }

        checked = 1;
    }

    public  uniform float Exists()
    {
        Init();
        return exists;
    }

    public  uniform string Space()
    {
        Init();
        return space;
    }

    public  varying vector I()
    {
        Init();
        return rightI;
    }

    public  varying vector MidI()
    {
        Init();
        return rightI;
    }

    public  varying point Cam()
    {
        Init();
        return rightCam;
    }

    public  varying point MidCam()
    {
        Init();
        return midCam;
    }
}

#endif
