/*
 *  Copyright(c) 2010-2013 Laika, developed under contract by mitchpraterLLC.
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
#ifndef FILTER_H
#define FILTER_H

/*
 *  filterregion based filtering.
 *  This object builds on the prman filterregion object
 *  to provide more filtering information and capability.
 */
#if 0
struct filterregion_s
{
    private filterregion    _frx;
    private filterregion    _fry;
    private filterregion    _frz;

    private uniform float   _dim = 0;

    public  void calculate1d( varying float x )
    {
        _frx->calculate1d( x );
        _dim = 1;
    }

    public  void calculate2d( varying float x, y )
    {
        _frx->calculate1d( x );
        _fry->calculate1d( y );
        _dim = 2;
    }

    public  void calculate2d( varying point x )
    {
        _frx->calculate1d( x[0] );
        _fry->calculate1d( x[1] );
        _dim = 2;
    }

    public  void calculate2d( varying vector x )
    {
        _frx->calculate1d( x[0] );
        _fry->calculate1d( x[1] );
        _dim = 2;
    }

    public  void calculate2d( varying color x )
    {
        _frx->calculate1d( x[0] );
        _fry->calculate1d( x[1] );
        _dim = 2;
    }

    public  void calculate3d( varying point x )
    {
        _frx->calculate1d( x[0] );
        _fry->calculate1d( x[1] );
        _frz->calculate1d( x[2] );
        _dim = 3;
    }

    public  void calculate3d( varying vector x )
    {
        _frx->calculate1d( x[0] );
        _fry->calculate1d( x[1] );
        _frz->calculate1d( x[2] );
        _dim = 3;
    }

    public  void calculate3d( varying color x )
    {
        _frx->calculate1d( x[0] );
        _fry->calculate1d( x[1] );
        _frz->calculate1d( x[2] );
        _dim = 3;
    }

    public  float maxsize()
    {
        float   result = 0.0;

        if (_dim > 0) result = _frx->maxsize();
        if (_dim > 1) result = max( result, _fry->maxsize() );
        if (_dim > 2) result = max( result, _frx->maxsize() );

        return result;
    }

    public  float filtersize_x()
    {
        return _frx->maxsize();
    }

    public  float filtersize_y()
    {
        return _fry->maxsize();
    }

    public  float filtersize_z()
    {
        return _frz->maxsize();
    }

    public  float minsize()
    {
        float   result = 0.0;

        if (_dim > 0) result = _frx->minsize();
        if (_dim > 1) result = min( result, _fry->minsize() );
        if (_dim > 2) result = min( result, _frx->minsize() );

        return result;
    }

    public  float filtersize()
    {
        float   result = 0.0;

        if (_dim == 1) return _frx->maxsize();
        if (_dim == 2) return sqrt( _frx->maxsize() * _fry->maxsize() );
        if (_dim == 3) return pow( _frx->maxsize() * _fry->maxsize() * _frz->maxsize(), 1.0/3.0 );

        return result;
    }

    public  vector gradient()
    {
        vector  result = vector(0,0,0);

        if (_dim == 1) return vector( _frx->maxsize(), 0,0 );
        if (_dim == 2) return vector( _frx->maxsize(), _fry->maxsize(), 0 );
        if (_dim == 3) return vector( _frx->maxsize(), _fry->maxsize(), _frz->maxsize() );

        return result;
    }

    public  vector ngradient()
    {
        vector  result = gradient();

        return normalize( result );
    }

    public  float filtersize( varying vector v )
    {
        varying vector  grad = gradient();

        varying float   result = abs( grad.v );

        return result;
    }
}
#endif

/*
 *  Geometric mean sample size calculations.
 *  These area based values are smoother and generally produce
 *  better results than diagonal distance measures and should
 *  should be used instead of those; unless there's a specific
 *  reason not to.
 */

/*
 *  Minimum filter width.
 *  Most functions that will rely on these values won't like
 *  dealing with a zero width, so this defines the smallest value
 *  these functions will return. This is scene dimension dependent,
 *  but this number is small enough that it's probably okay.
 */
#define FILTER_MINWIDTH (2e-7)

/*
 *  Float version.
 *  Length of an edge of a square of equivalent size
 *  (rather than the diagonal distance measure). This is smaller
 *  than the actual maximum (diagonal) sampling distance, but
 *  may be smoother.
 */
float filterWidth (float x)
{
    return  max(sqrt(Du(x)*du * Dv(x)*dv), FILTER_MINWIDTH);
}

/*  Calculate the size if you already have the derivatives available. */
float filterWidth (float dux, dvx)
{
    return  max(sqrt(dux*dvx), FILTER_MINWIDTH);
}

/*
 *  Point and vector versions.
 *  Length of an edge of a cubic volume of equivalent size
 *  (rather than the diagonal distance measure). This is smaller
 *  than the actual maximum sampling distance, but may be smoother.
 */
float filterWidth (point Q)
{
    return  max(sqrt(area(Q)), FILTER_MINWIDTH);
}

float filterWidth (vector Q)
{
    return  max(sqrt(area(point Q)), FILTER_MINWIDTH);
}

float filterWidth (vector dQu, dQv)
{
    return  max(sqrt(length(dQu^dQv)), FILTER_MINWIDTH);
}

#endif
