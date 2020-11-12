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
#ifndef override_h
#define override_h

#include <sl.h>
#include <manifold.h>

uniform float overrideU( uniform shader _coshader; uniform float _x )
{
    return (_coshader != null) ? gridmax( float _coshader->GetFloat() ) : _x;
}
varying float overrideV( uniform shader _coshader; varying float _x )
{
    return (_coshader != null) ? float _coshader->GetFloat() : _x;
}
varying float override( uniform shader _coshader; varying float _x )
{
    return overrideV( _coshader, _x );
}


uniform color overrideU( uniform shader _coshader; uniform color _x )
{
    return (_coshader != null) ? color( _gridmax( color _coshader->GetColor() )) : _x;
}
varying color overrideV( uniform shader _coshader; varying color _x )
{
    return (_coshader != null) ? color _coshader->GetColor() : _x;
}
varying color override( uniform shader _coshader; varying color _x )
{
    return overrideV( _coshader, _x );
}


uniform point overrideU( uniform shader _coshader; uniform point _x )
{
    return (_coshader != null) ? point( _gridmax( point _coshader->GetPoint() )) : _x;
}
varying point overrideV( uniform shader _coshader; varying point _x )
{
    return (_coshader != null) ? point _coshader->GetPoint() : _x;
}
varying point override( uniform shader _coshader; varying point _x )
{
    return overrideV( _coshader, _x );
}


uniform vector overrideU( uniform shader _coshader; uniform vector _x )
{
    return (_coshader != null) ? vector( _gridmax( vector _coshader->GetVector() )) : _x;
}
varying vector overrideV( uniform shader _coshader; varying vector _x )
{
    return (_coshader != null) ? vector _coshader->GetVector() : _x;
}
varying vector override( uniform shader _coshader; varying vector _x )
{
    return overrideV( _coshader, _x );
}


uniform normal overrideU( uniform shader _coshader; uniform normal _x )
{
    return (_coshader != null) ? normal( _gridmax( normal _coshader->GetNormal() )) : _x;
}
varying normal overrideV( uniform shader _coshader; varying normal _x )
{
    return (_coshader != null) ? normal _coshader->GetNormal() : _x;
}
varying normal override( uniform shader _coshader; varying normal _x )
{
    return overrideV( _coshader, _x );
}


uniform string overrideU( uniform shader _coshader; uniform string _x )
{
    if (_coshader != null)
    {
        uniform string  override = _coshader->GetString();
        return override;
    }
    else
    {
        return _x;
    }
}
uniform string override( uniform shader _coshader; uniform string _x )
{
    return overrideU( _coshader, _x );
}


manifold_s override( uniform shader _coshader; manifold_s _x )
{
    manifold_s  x;

    if (_coshader != null)
    {
        x = _coshader->GetManifold();
        return x;
    }

    return _x;
}

#endif
