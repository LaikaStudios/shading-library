/*
 *  Copyright(c) 2010-2014 Laika, developed under contract by mitchpraterLLC.
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

//
//  RSL Functions and Constants
//
//  Define some obvious polymorphic and missing sl functions.
//
#ifndef SL_H
#define SL_H

//---------------------------------------
//  Constants
//---------------------------------------
#define TWOPI       6.283185307179586
#define HALFPI      1.570796326794897
#define SQRTPI      1.772453850905516
#define SQRT2       1.414213562373095
#define SQRT3       1.732050807568877
#define INVSQRT2    0.7071067811865475

#define MAXDIST     1e30

//---------------------------------------
//  plot - fixes some bad behavior of rbug().
//---------------------------------------
void plot( uniform point x ) { rbug( x ); }
void plot( uniform vector x ) { uniform point p = point x; rbug( p ); }
void plot( uniform point x; uniform color c ) { rbug( x, c ); }
void plot( uniform vector x; uniform color c ) { uniform point p = point x; rbug( p, c ); }

void plot( varying point x ) { rbug( x ); }
void plot( varying vector x ) { varying point p = point x; rbug( p ); }
void plot( varying point x; varying color c ) { rbug( x, c ); }
void plot( varying vector x; varying color c ) { varying point p = point x; rbug( p, c ); }

//---------------------------------------
//  abs
//---------------------------------------
float   _abs (float x) { return abs (x); }
color   _abs (color x) { return color (abs(x[0]), abs(x[1]), abs(x[2])); }
point   _abs (point x) { return point (abs(x[0]), abs(x[1]), abs(x[2])); }
vector  _abs (vector x) { return vector (abs(x[0]), abs(x[1]), abs(x[2])); }
normal  _abs (normal x) { return normal (abs(x[0]), abs(x[1]), abs(x[2])); }

//---------------------------------------
//  bit-wise and() of the integer parts.
//---------------------------------------

plugin "and";

//---------------------------------------
//  average
//---------------------------------------
float   average( color a )  { return (a[0]+a[1]+a[2])/3.0; }
float   average( point a )  { return (a[0]+a[1]+a[2])/3.0; }
float   average( vector a ) { return (a[0]+a[1]+a[2])/3.0; }
float   average( normal a ) { return (a[0]+a[1]+a[2])/3.0; }

float   average( float a[] ) {
    uniform float   i, alen = arraylength(a);

    float   sum = 0;
    for (i=0; i < alen; i+=1) {
        sum += a[i];
    }

    return sum/alen;
}

//---------------------------------------
//  exp
//---------------------------------------
float   _exp (float x) { return exp (x); }
color   _exp (color x) { return color (exp(x[0]), exp(x[1]), exp(x[2])); }
point   _exp (point x) { return point (exp(x[0]), exp(x[1]), exp(x[2])); }
vector  _exp (vector x) { return vector (exp(x[0]), exp(x[1]), exp(x[2])); }
normal  _exp (normal x) { return normal (exp(x[0]), exp(x[1]), exp(x[2])); }

//---------------------------------------
//  log
//---------------------------------------
float   _log (float x) { return log (x); }
color   _log (color x) { return color (log(x[0]), log(x[1]), log(x[2])); }
point   _log (point x) { return point (log(x[0]), log(x[1]), log(x[2])); }
vector  _log (vector x) { return vector (log(x[0]), log(x[1]), log(x[2])); }
normal  _log (normal x) { return normal (log(x[0]), log(x[1]), log(x[2])); }

//---------------------------------------
//  floor
//---------------------------------------
float   _floor (float x) { return floor (x); }
color   _floor (color x) { return color (floor(x[0]), floor(x[1]), floor(x[2])); }
point   _floor (point x) { return point (floor(x[0]), floor(x[1]), floor(x[2])); }
vector  _floor (vector x) { return vector (floor(x[0]), floor(x[1]), floor(x[2])); }
normal  _floor (normal x) { return normal (floor(x[0]), floor(x[1]), floor(x[2])); }

//---------------------------------------
//  grid
//---------------------------------------
#define gridever(CONDITION)     gridmax((CONDITION) ? 1 : 0)
#define gridnever(CONDITION)    gridmin((CONDITION) ? 0 : 1)
#define gridalways(CONDITION)   gridmin((CONDITION) ? 1 : 0)
#define gridsometime(CONDITION) gridmax((CONDITION) ? 0 : 1)
#define gridsometimes(CONDITION) gridsometime(CONDITION)

uniform float   gridavg  (varying float x) { return (.5 * (gridmin(x) + gridmax(x))); }
uniform float   gridmean (varying float x) { return (sqrt (gridmin(x) * gridmax(x))); }

uniform color gridavg( varying color x )
{
    uniform color   result;
    
    result = .5 * color (
        (gridmin(x[0]) + gridmax(x[0])),
        (gridmin(x[1]) + gridmax(x[1])),
        (gridmin(x[2]) + gridmax(x[2]))
    );

    return result;
}

uniform color gridmean( varying color x )
{
    uniform color   result;
    
    result = color (
        sqrt((gridmin(x[0]) * gridmax(x[0]))),
        sqrt((gridmin(x[1]) * gridmax(x[1]))),
        sqrt((gridmin(x[2]) * gridmax(x[2])))
    );

    return result;
}

uniform float _gridmin( varying float x )
{
    return gridmin( x );
}

uniform color _gridmin( varying color x )
{
    return color( gridmin(x[0]), gridmin(x[1]), gridmin(x[2]) );
}

uniform point _gridmin( varying point x )
{
    return point( gridmin(x[0]), gridmin(x[1]), gridmin(x[2]) );
}

uniform vector _gridmin( varying vector x )
{
    return vector( gridmin(x[0]), gridmin(x[1]), gridmin(x[2]) );
}

uniform normal _gridmin( varying normal x )
{
    return normal( gridmin(x[0]), gridmin(x[1]), gridmin(x[2]) );
}

uniform float _gridmax( varying float x )
{
    return gridmax( x );
}

uniform color _gridmax( varying color x )
{
    return color( gridmax(x[0]), gridmax(x[1]), gridmax(x[2]) );
}

uniform point _gridmax( varying point x )
{
    return point( gridmax(x[0]), gridmax(x[1]), gridmax(x[2]) );
}

uniform vector _gridmax( varying vector x )
{
    return vector( gridmax(x[0]), gridmax(x[1]), gridmax(x[2]) );
}

uniform normal _gridmax( varying normal x )
{
    return normal( gridmax(x[0]), gridmax(x[1]), gridmax(x[2]) );
}

//---------------------------------------
//  clamp
//---------------------------------------
float   _clamp (float x; float a, b) { return clamp (x, a, b); }
color   _clamp (color x; float a, b) { return clamp (x, color a, color b); }
point   _clamp (point x; float a, b) { return clamp (x, point a, point b); }
vector  _clamp (vector x; float a, b) { return clamp (x, vector a, vector b); }
normal  _clamp (normal x; float a, b) { return clamp (x, normal a, normal b); }

//---------------------------------------
//  mix
//---------------------------------------
float   _mix (float a; float b; float x) { return mix (a, b, x); }

color   _mix (color a; color b; float x) { return mix (a, b, x); }
color   _mix (float a; float b; color x) { return color (mix(a,b,x[0]), mix(a,b,x[1]), mix(a,b,x[2])); }
color   _mix (color a; color b; color x) { return mix (a, b, x); }

point   _mix (point a; point b; float x) { return mix (a, b, x); }
point   _mix (float a; float b; point x) { return point (mix(a,b,x[0]), mix(a,b,x[1]), mix(a,b,x[2])); }
point   _mix (point a; point b; point x) { return mix (a, b, color x); }

vector  _mix (vector a; vector b; float x) { return mix (a, b, x); }
vector  _mix (float a; float b; vector x) { return vector (mix(a,b,x[0]), mix(a,b,x[1]), mix(a,b,x[2])); }
vector  _mix (vector a; vector b; vector x) { return mix (a, b, color x); }

normal  _mix (normal a; normal b; float x) { return mix (a, b, x); }
normal  _mix (float a; float b; normal x) { return normal (mix(a,b,x[0]), mix(a,b,x[1]), mix(a,b,x[2])); }
normal  _mix (normal a; normal b; normal x) { return mix (a, b, color x); }

//---------------------------------------
//  circularmix
//---------------------------------------
float   circularmix( float a; float b; float x )
{
    float   dir = sign( b-a );
    float   dist = abs( b-a );

    dist = mix( dist, 1-dist, step( 0.5, dist ));
    dir = mix( dir, -dir, step( 0.5, dist ));

    return mix( a, a + dir*dist, x );
}

//---------------------------------------
//  match
//---------------------------------------
uniform float   _match( string a; string b ) { return match( a,b ); }

uniform float   _match( string a; string b; uniform float invert )
{
    uniform float   m = match( a,b );
    return( (invert==0) ? m : 1-m );
}

uniform float   _match( string a[]; string b )
{
    uniform float   i, len = arraylength( a );
    if (len == 0) return 1;

    uniform float   m = 0;
    for (i=0; i < len; i+=1)
    {
        if (match( a[i],b ))
        {
            m = 1;
            break;
        }
    }
    return m;
}

uniform float   _match( string a[]; string b; uniform float invert )
{
    uniform float   m = _match( a,b );
    return( (invert==0) ? m : 1-m );
}

uniform float   _match( string a; string b[] )
{
    uniform float   i, len = arraylength( b );
    if (len == 0) return 1;

    uniform float   m = 0;
    for (i=0; i < len; i+=1)
    {
        if (match( a,b[i] ))
        {
            m = 1;
            break;
        }
    }
    return m;
}

uniform float   _match( string a; string b[]; uniform float invert )
{
    uniform float   m = _match( a,b );
    return( (invert==0) ? m : 1-m );
}

//---------------------------------------
//  cellnoisevariation
//---------------------------------------
float   cellnoisevariation( float x; float var )
{
#pragma nolint 2
    varying float   a = cellnoise( x, var );
    varying float   b = cellnoise( x, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
float   cellnoisevariation( float x; float y; float var )
{
#pragma nolint 2
    varying float   a = cellnoise( point( x, y, 0 ), var );
    varying float   b = cellnoise( point( x, y, 0 ), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
float   cellnoisevariation( point q; float var )
{
#pragma nolint 2
    varying float   a = cellnoise( q, var );
    varying float   b = cellnoise( q, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
float   cellnoisevariation( vector q; float var )
{
#pragma nolint 2
    varying float   a = cellnoise( point(q), var );
    varying float   b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
float   cellnoisevariation( normal q; float var )
{
#pragma nolint 2
    varying float   a = cellnoise( point(q), var );
    varying float   b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}

color   cellnoisevariation( float x; float var )
{
#pragma nolint 2
    varying color   a = cellnoise( x, var );
    varying color   b = cellnoise( x, var+1 );

    a = ctransform( "rgb", "hsl", a );
    b = ctransform( "rgb", "hsl", b );

    varying color   result = mix( a, b, mod( var, 1.0 ));
    result[0] = circularmix( a[0], b[0], mod( var, 1.0 ));

    return ctransform( "hsl", "rgb", result );
}
color   cellnoisevariation( float x; float y; float var )
{
#pragma nolint 2
    varying color   a = cellnoise( point( x, y, 0 ), var );
    varying color   b = cellnoise( point( x, y, 0 ), var+1 );

    a = ctransform( "rgb", "hsl", a );
    b = ctransform( "rgb", "hsl", b );

    varying color   result = mix( a, b, mod( var, 1.0 ));
    result[0] = circularmix( a[0], b[0], mod( var, 1.0 ));

    return ctransform( "hsl", "rgb", result );
}
color   cellnoisevariation( point q; float var )
{
#pragma nolint 2
    varying color   a = cellnoise( q, var );
    varying color   b = cellnoise( q, var+1 );

    a = ctransform( "rgb", "hsl", a );
    b = ctransform( "rgb", "hsl", b );

    varying color   result = mix( a, b, mod( var, 1.0 ));
    result[0] = circularmix( a[0], b[0], mod( var, 1.0 ));

    return ctransform( "hsl", "rgb", result );
}
color   cellnoisevariation( vector q; float var )
{
#pragma nolint 2
    varying color   a = cellnoise( point(q), var );
    varying color   b = cellnoise( point(q), var+1 );

    a = ctransform( "rgb", "hsl", a );
    b = ctransform( "rgb", "hsl", b );

    varying color   result = mix( a, b, mod( var, 1.0 ));
    result[0] = circularmix( a[0], b[0], mod( var, 1.0 ));

    return ctransform( "hsl", "rgb", result );
}
color   cellnoisevariation( normal q; float var )
{
#pragma nolint 2
    varying color   a = cellnoise( point(q), var );
    varying color   b = cellnoise( point(q), var+1 );

    a = ctransform( "rgb", "hsl", a );
    b = ctransform( "rgb", "hsl", b );

    varying color   result = mix( a, b, mod( var, 1.0 ));
    result[0] = circularmix( a[0], b[0], mod( var, 1.0 ));

    return ctransform( "hsl", "rgb", result );
}

point   cellnoisevariation( float x; float var )
{
#pragma nolint 2
    varying point   a = cellnoise( x, var );
    varying point   b = cellnoise( x, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
point   cellnoisevariation( float x; float y; float var )
{
#pragma nolint 2
    varying point   a = cellnoise( point( x, y, 0 ), var );
    varying point   b = cellnoise( point( x, y, 0 ), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
point   cellnoisevariation( point q; float var )
{
#pragma nolint 2
    varying point   a = cellnoise( q, var );
    varying point   b = cellnoise( q, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
point   cellnoisevariation( vector q; float var )
{
#pragma nolint 2
    varying point   a = cellnoise( point(q), var );
    varying point   b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
point   cellnoisevariation( normal q; float var )
{
#pragma nolint 2
    varying point   a = cellnoise( point(q), var );
    varying point   b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}

vector  cellnoisevariation( float x; float var )
{
#pragma nolint 2
    varying vector  a = cellnoise( x, var );
    varying vector  b = cellnoise( x, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
vector  cellnoisevariation( float x; float y; float var )
{
#pragma nolint 2
    varying vector  a = cellnoise( point( x, y, 0 ), var );
    varying vector  b = cellnoise( point( x, y, 0 ), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
vector  cellnoisevariation( point q; float var )
{
#pragma nolint 2
    varying vector  a = cellnoise( q, var );
    varying vector  b = cellnoise( q, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
vector  cellnoisevariation( vector q; float var )
{
#pragma nolint 2
    varying vector  a = cellnoise( point(q), var );
    varying vector  b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
vector  cellnoisevariation( normal q; float var )
{
#pragma nolint 2
    varying vector  a = cellnoise( point(q), var );
    varying vector  b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}

normal  cellnoisevariation( float x; float var )
{
#pragma nolint 2
    varying normal  a = cellnoise( x, var );
    varying normal  b = cellnoise( x, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
normal  cellnoisevariation( float x; float y; float var )
{
#pragma nolint 2
    varying normal  a = cellnoise( point( x, y, 0 ), var );
    varying normal  b = cellnoise( point( x, y, 0 ), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
normal  cellnoisevariation( point q; float var )
{
#pragma nolint 2
    varying normal  a = cellnoise( q, var );
    varying normal  b = cellnoise( q, var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
normal  cellnoisevariation( vector q; float var )
{
#pragma nolint 2
    varying normal  a = cellnoise( point(q), var );
    varying normal  b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}
normal  cellnoisevariation( normal q; float var )
{
#pragma nolint 2
    varying normal  a = cellnoise( point(q), var );
    varying normal  b = cellnoise( point(q), var+1 );

    return mix( a, b, mod( var, 1.0 ));
}

//---------------------------------------
//  linear
//---------------------------------------
float   linear (float a; float b; float x) {
    return (x-a)/(b-a);
}

color   linear (float a; float b; color x) {
    return color (linear(a,b,x[0]), linear(a,b,x[1]), linear(a,b,x[2]));
}
color   linear (color a; color b; float x) {
    return color (linear(a[0],b[0],x), linear(a[1],b[1],x), linear(a[2],b[2],x));
}
color   linear (color a; color b; color x) {
    return color (linear(a[0],b[0],x[0]), linear(a[1],b[1],x[1]), linear(a[2],b[2],x[2]));
}

point   linear (float a; float b; point x) {
    return point (linear(a,b,x[0]), linear(a,b,x[1]), linear(a,b,x[2]));
}
point   linear (point a; point b; float x) {
    return point (linear(a[0],b[0],x), linear(a[1],b[1],x), linear(a[2],b[2],x));
}
point   linear (point a; point b; point x) {
    return point (linear(a[0],b[0],x[0]), linear(a[1],b[1],x[1]), linear(a[2],b[2],x[2]));
}

vector  linear (float a; float b; vector x) {
    return vector (linear(a,b,x[0]), linear(a,b,x[1]), linear(a,b,x[2]));
}
vector  linear (vector a; vector b; float x) {
    return vector (linear(a[0],b[0],x), linear(a[1],b[1],x), linear(a[2],b[2],x));
}
vector  linear (vector a; vector b; vector x) {
    return vector (linear(a[0],b[0],x[0]), linear(a[1],b[1],x[1]), linear(a[2],b[2],x[2]));
}

normal  linear (float a; float b; normal x) {
    return normal (linear(a,b,x[0]), linear(a,b,x[1]), linear(a,b,x[2]));
}
normal  linear (normal a; normal b; float x) {
    return normal (linear(a[0],b[0],x), linear(a[1],b[1],x), linear(a[2],b[2],x));
}
normal  linear (normal a; normal b; normal x) {
    return normal (linear(a[0],b[0],x[0]), linear(a[1],b[1],x[1]), linear(a[2],b[2],x[2]));
}

//---------------------------------------
//  linearstep
//  Uses filtered transitions: not a simple clamp.
//---------------------------------------
float   linearstep (float a; float b; float x) {
    float   f = (x-a)/(b-a);
    float   half = (a+b)/2;

    if (a <= b)
        return ((x < half) ? mix (0, f, clamp(filterstep(a,x),0,1)) : mix (f, 1, clamp(filterstep(b,x),0,1)));
    else
        return ((x < half) ? mix (1, f, clamp(filterstep(b,x),0,1)) : mix (f, 0, clamp(filterstep(a,x),0,1)));
}

color   linearstep (float a; float b; color x) {
    return color (linearstep(a,b,x[0]), linearstep(a,b,x[1]), linearstep(a,b,x[2]));
}
color   linearstep (color a; color b; float x) {
    return color (linearstep(a[0],b[0],x), linearstep(a[1],b[1],x), linearstep(a[2],b[2],x));
}
color   linearstep (color a; color b; color x) {
    return color (linearstep(a[0],b[0],x[0]), linearstep(a[1],b[1],x[1]), linearstep(a[2],b[2],x[2]));
}

point   linearstep (float a; float b; point x) {
    return point (linearstep(a,b,x[0]), linearstep(a,b,x[1]), linearstep(a,b,x[2]));
}
point   linearstep (point a; point b; float x) {
    return point (linearstep(a[0],b[0],x), linearstep(a[1],b[1],x), linearstep(a[2],b[2],x));
}
point   linearstep (point a; point b; point x) {
    return point (linearstep(a[0],b[0],x[0]), linearstep(a[1],b[1],x[1]), linearstep(a[2],b[2],x[2]));
}

vector  linearstep (float a; float b; vector x) {
    return vector (linearstep(a,b,x[0]), linearstep(a,b,x[1]), linearstep(a,b,x[2]));
}
vector  linearstep (vector a; vector b; float x) {
    return vector (linearstep(a[0],b[0],x), linearstep(a[1],b[1],x), linearstep(a[2],b[2],x));
}
vector  linearstep (vector a; vector b; vector x) {
    return vector (linearstep(a[0],b[0],x[0]), linearstep(a[1],b[1],x[1]), linearstep(a[2],b[2],x[2]));
}

normal  linearstep (float a; float b; normal x) {
    return normal (linearstep(a,b,x[0]), linearstep(a,b,x[1]), linearstep(a,b,x[2]));
}
normal  linearstep (normal a; normal b; float x) {
    return normal (linearstep(a[0],b[0],x), linearstep(a[1],b[1],x), linearstep(a[2],b[2],x));
}
normal  linearstep (normal a; normal b; normal x) {
    return normal (linearstep(a[0],b[0],x[0]), linearstep(a[1],b[1],x[1]), linearstep(a[2],b[2],x[2]));
}

//---------------------------------------
//  vector rotate
//  A vector version of the rotate() function.
//  Rotates a by angle radians about r.
//---------------------------------------
vector vrotate (vector a; float angle; vector r) {
    return vector rotate (point a, angle, point 0, point r);
}

vector _rotate (vector a; float angle; vector r) {
    return vector rotate (point a, angle, point 0, point r);
}

point _rotate (point a; float angle; vector r) {
    return point rotate (point a, angle, point 0, point r);
}

//---------------------------------------
//  slerp - spherical linear interpolation
//  i.e. a rotational vector mix().
//
//  a and b are assumed to be normalized.
//  x=0 returns a; x=1 returns b.
//  x may be any value.
//---------------------------------------
vector  slerp (vector a; vector b; float x)
{
    varying point  cross = point(a^b);

    if (length(cross) > .000001) {
        return vector rotate (point a, mix(0, acos(a.b), x), point 0, cross);
    } else {
        return a;
    }
}

vector slerp (normal a; vector b; float x) {
    return slerp (vector a, b, x);
}

vector slerp (vector a; normal b; float x) {
    return slerp (a, vector b, x);
}

vector slerp (normal a; normal b; float x) {
    return slerp (vector a, vector b, x);
}

normal slerp (vector a; vector b; float x) {
    return normal slerp (a, b, x);
}

normal slerp (normal a; vector b; float x) {
    return normal slerp (vector a, b, x);
}

normal slerp (vector a; normal b; float x) {
    return normal slerp (a, vector b, x);
}

normal slerp (normal a; normal b; float x) {
    return normal slerp (vector a, vector b, x);
}

//---------------------------------------
//  max
//---------------------------------------
float   _max (float x; float y) { return float max (x,y); }

color   _max (color x; float y) { return color (max(x[0],y), max(x[1],y), max(x[2],y)); }
color   _max (float x; color y) { return color (max(x,y[0]), max(x,y[1]), max(x,y[2])); }
color   _max (color x; color y) { return color max(x,y); }

point   _max (point x; float y) { return point (max(x[0],y), max(x[1],y), max(x[2],y)); }
point   _max (float x; point y) { return point (max(x,y[0]), max(x,y[1]), max(x,y[2])); }
point   _max (point x; point y) { return point max(x,y); }

vector  _max (vector x; float y) { return vector (max(x[0],y), max(x[1],y), max(x[2],y)); }
vector  _max (float x; vector y) { return vector (max(x,y[0]), max(x,y[1]), max(x,y[2])); }
vector  _max (vector x; vector y) { return vector max(x,y); }

normal  _max (normal x; float y) { return normal (max(x[0],y), max(x[1],y), max(x[2],y)); }
normal  _max (float x; normal y) { return normal (max(x,y[0]), max(x,y[1]), max(x,y[2])); }
normal  _max (normal x; normal y) { return normal max(x,y); }

//---------------------------------------
//  min
//---------------------------------------
float   _min (float x; float y) { return float min (x,y); }

color   _min (color x; float y) { return color (min(x[0],y), min(x[1],y), min(x[2],y)); }
color   _min (float x; color y) { return color (min(x,y[0]), min(x,y[1]), min(x,y[2])); }
color   _min (color x; color y) { return color min(x,y); }

point   _min (point x; float y) { return point (min(x[0],y), min(x[1],y), min(x[2],y)); }
point   _min (float x; point y) { return point (min(x,y[0]), min(x,y[1]), min(x,y[2])); }
point   _min (point x; point y) { return point min(x,y); }

vector  _min (vector x; float y) { return vector (min(x[0],y), min(x[1],y), min(x[2],y)); }
vector  _min (float x; vector y) { return vector (min(x,y[0]), min(x,y[1]), min(x,y[2])); }
vector  _min (vector x; vector y) { return vector min(x,y); }

normal  _min (normal x; float y) { return normal (min(x[0],y), min(x[1],y), min(x[2],y)); }
normal  _min (float x; normal y) { return normal (min(x,y[0]), min(x,y[1]), min(x,y[2])); }
normal  _min (normal x; normal y) { return normal min(x,y); }

//---------------------------------------
//  mod
//---------------------------------------
float   _mod (float x; float y) { return float mod (x,y); }

color   _mod (color x; float y) { return color (mod(x[0],y), mod(x[1],y), mod(x[2],y)); }
color   _mod (float x; color y) { return color (mod(x,y[0]), mod(x,y[1]), mod(x,y[2])); }
color   _mod (color x; color y) { return color (mod(x[0],y[0]), mod(x[1],y[1]), mod(x[2],y[2])); }

point   _mod (point x; float y) { return point (mod(x[0],y), mod(x[1],y), mod(x[2],y)); }
point   _mod (float x; point y) { return point (mod(x,y[0]), mod(x,y[1]), mod(x,y[2])); }
point   _mod (point x; point y) { return point (mod(x[0],y[0]), mod(x[1],y[1]), mod(x[2],y[2])); }

vector  _mod (vector x; float y) { return vector (mod(x[0],y), mod(x[1],y), mod(x[2],y)); }
vector  _mod (float x; vector y) { return vector (mod(x,y[0]), mod(x,y[1]), mod(x,y[2])); }
vector  _mod (vector x; vector y) { return vector (mod(x[0],y[0]), mod(x[1],y[1]), mod(x[2],y[2])); }

normal  _mod (normal x; float y) { return normal (mod(x[0],y), mod(x[1],y), mod(x[2],y)); }
normal  _mod (float x; normal y) { return normal (mod(x,y[0]), mod(x,y[1]), mod(x,y[2])); }
normal  _mod (normal x; normal y) { return normal (mod(x[0],y[0]), mod(x[1],y[1]), mod(x[2],y[2])); }

//---------------------------------------
//  pow
//---------------------------------------

plugin "_pow";

//---------------------------------------
//  sign
//---------------------------------------
float   _sign (float x) { return sign (x); }
color   _sign (color x) { return color (sign(x[0]), sign(x[1]), sign(x[2])); }
point   _sign (point x) { return point (sign(x[0]), sign(x[1]), sign(x[2])); }
vector  _sign (vector x) { return vector (sign(x[0]), sign(x[1]), sign(x[2])); }
normal  _sign (normal x) { return normal (sign(x[0]), sign(x[1]), sign(x[2])); }

//---------------------------------------
//  step
//---------------------------------------
float   _step (float a; float x) { return step (a,x); }
color   _step (color a; color x) { return step (a,x); }

color   _step (color a; float x) { return  color (step(a[0],x), step(a[1],x), step(a[2],x)); }
color   _step (float a; color x) { return  color (step(a,x[0]), step(a,x[1]), step(a,x[2])); }

point   _step (point a; float x) { return  point (step(a[0],x), step(a[1],x), step(a[2],x)); }
point   _step (float a; point x) { return  point (step(a,x[0]), step(a,x[1]), step(a,x[2])); }
point   _step (point a; point x) { return  point (step(a[0],x[0]), step(a[1],x[1]), step(a[2],x[2])); }

vector  _step (vector a; float x) { return  vector (step(a[0],x), step(a[1],x), step(a[2],x)); }
vector  _step (float a; vector x) { return  vector (step(a,x[0]), step(a,x[1]), step(a,x[2])); }
vector  _step (vector a; vector x) { return  vector (step(a[0],x[0]), step(a[1],x[1]), step(a[2],x[2])); }

normal  _step (normal a; float x) { return  normal (step(a[0],x), step(a[1],x), step(a[2],x)); }
normal  _step (float a; normal x) { return  normal (step(a,x[0]), step(a,x[1]), step(a,x[2])); }
normal  _step (normal a; normal x) { return  normal (step(a[0],x[0]), step(a[1],x[1]), step(a[2],x[2])); }

//---------------------------------------
//  smoothstep
//---------------------------------------
float _smoothstep (float a; float b; float x) { return smoothstep (a, b, x); }

color _smoothstep (color a; color b; float x)
{
    return  color (
        smoothstep (a[0], b[0], x),
        smoothstep (a[1], b[1], x),
        smoothstep (a[2], b[2], x)
    );
}

color _smoothstep (float a; float b; color x)
{
    return  color (
        smoothstep (a, b, x[0]),
        smoothstep (a, b, x[1]),
        smoothstep (a, b, x[2])
    );
}

color _smoothstep (color a; color b; color x)
{
    return  color (
        smoothstep (a[0], b[0], x[0]),
        smoothstep (a[1], b[1], x[1]),
        smoothstep (a[2], b[2], x[2])
    );
}

point _smoothstep (point a; point b; point x)
{
    return  point (
        smoothstep (a[0], b[0], x[0]),
        smoothstep (a[1], b[1], x[1]),
        smoothstep (a[2], b[2], x[2])
    );
}

point _smoothstep (point a; point b; float x)
{
    return  point (
        smoothstep (a[0], b[0], x),
        smoothstep (a[1], b[1], x),
        smoothstep (a[2], b[2], x)
    );
}

point _smoothstep (float a; float b; point x)
{
    return  point (
        smoothstep (a, b, x[0]),
        smoothstep (a, b, x[1]),
        smoothstep (a, b, x[2])
    );
}

vector _smoothstep (vector a; vector b; vector x)
{
    return  vector (
        smoothstep (a[0], b[0], x[0]),
        smoothstep (a[1], b[1], x[1]),
        smoothstep (a[2], b[2], x[2])
    );
}

vector _smoothstep (vector a; vector b; float x)
{
    return  vector (
        smoothstep (a[0], b[0], x),
        smoothstep (a[1], b[1], x),
        smoothstep (a[2], b[2], x)
    );
}

vector _smoothstep (float a; float b; vector x)
{
    return  vector (
        smoothstep (a, b, x[0]),
        smoothstep (a, b, x[1]),
        smoothstep (a, b, x[2])
    );
}

normal _smoothstep (normal a; normal b; normal x)
{
    return  normal (
        smoothstep (a[0], b[0], x[0]),
        smoothstep (a[1], b[1], x[1]),
        smoothstep (a[2], b[2], x[2])
    );
}

normal _smoothstep (normal a; normal b; float x)
{
    return  normal (
        smoothstep (a[0], b[0], x),
        smoothstep (a[1], b[1], x),
        smoothstep (a[2], b[2], x)
    );
}

normal _smoothstep (float a; float b; normal x)
{
    return  normal (
        smoothstep (a, b, x[0]),
        smoothstep (a, b, x[1]),
        smoothstep (a, b, x[2])
    );
}

//---------------------------------------
//  split
//---------------------------------------

plugin "split";

//---------------------------------------
//  sqrt
//---------------------------------------
float   _sqrt (float x) { return sqrt (x); }
color   _sqrt (color x) { return color (sqrt(x[0]), sqrt(x[1]), sqrt(x[2])); }
point   _sqrt (point x) { return point (sqrt(x[0]), sqrt(x[1]), sqrt(x[2])); }
vector  _sqrt (vector x) { return vector (sqrt(x[0]), sqrt(x[1]), sqrt(x[2])); }
normal  _sqrt (normal x) { return normal (sqrt(x[0]), sqrt(x[1]), sqrt(x[2])); }

//---------------------------------------
//  filterstep
//---------------------------------------

#include <filter.h>

float _filterstep (float a; float x) {
    return filterstep (a, x);
}
// Caution: _filterstep() with three float arguments has a different
// meaning than filterstep() with three float arguments.
float _filterstep (float a; float x; float width) {
    float   dx = x + max (filterWidth(x), width);
    return filterstep (a, x, dx);
}
float _filterstep (float a; float x; string filter) {
    return filterstep (a, x, "filter", filter);
}
float _filterstep (float a; float x; float width; string filter) {
    float   dx = max (filterWidth(x), width);
    return filterstep (a, x, x+dx, "filter", filter);
}

color _filterstep (color a; float x)
{
    return  color (
        filterstep (a[0], x),
        filterstep (a[1], x),
        filterstep (a[2], x)
    );
}
color _filterstep (float a; color x)
{
    return  color (
        filterstep (a, x[0]),
        filterstep (a, x[1]),
        filterstep (a, x[2])
    );
}
color _filterstep (color a; color x)
{
    return  color (
        filterstep (a[0], x[0]),
        filterstep (a[1], x[1]),
        filterstep (a[2], x[2])
    );
}
color _filterstep (color a; float x; float width)
{
    float   dx = max (filterWidth(x), width);
    return  color (
        filterstep (a[0], x, x+dx),
        filterstep (a[1], x, x+dx),
        filterstep (a[2], x, x+dx)
    );
}

color _filterstep (float a; color x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  color (
        filterstep (a, x[0], x[0]+dx[0]),
        filterstep (a, x[1], x[1]+dx[1]),
        filterstep (a, x[2], x[2]+dx[2])
    );
}

color _filterstep (color a; color x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  color (
        filterstep (a[0], x[0], x[0]+dx[0]),
        filterstep (a[1], x[1], x[1]+dx[1]),
        filterstep (a[2], x[2], x[2]+dx[2])
    );
}
color _filterstep (color a; float x; string filter)
{
    return  color (
        filterstep (a[0], x, "filter", filter),
        filterstep (a[1], x, "filter", filter),
        filterstep (a[2], x, "filter", filter)
    );
}

color _filterstep (float a; color x; string filter)
{
    return  color (
        filterstep (a, x[0], "filter", filter),
        filterstep (a, x[1], "filter", filter),
        filterstep (a, x[2], "filter", filter)
    );
}

color _filterstep (color a; color x; string filter)
{
    return  color (
        filterstep (a[0], x[0], "filter", filter),
        filterstep (a[1], x[1], "filter", filter),
        filterstep (a[2], x[2], "filter", filter)
    );
}
color _filterstep (color a; float x; float width; string filter)
{
    float   dx = max (filterWidth(x), width);
    return  color (
        filterstep (a[0], x, x+dx, "filter", filter),
        filterstep (a[1], x, x+dx, "filter", filter),
        filterstep (a[2], x, x+dx, "filter", filter)
    );
}

color _filterstep (float a; color x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  color (
        filterstep (a, x[0], x[0]+dx[0], "filter", filter),
        filterstep (a, x[1], x[1]+dx[1], "filter", filter),
        filterstep (a, x[2], x[2]+dx[2], "filter", filter)
    );
}

color _filterstep (color a; color x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  color (
        filterstep (a[0], x[0], x[0]+dx[0], "filter", filter),
        filterstep (a[1], x[1], x[1]+dx[1], "filter", filter),
        filterstep (a[2], x[2], x[2]+dx[2], "filter", filter)
    );
}


point _filterstep (point a; point x)
{
    return  point (
        filterstep (a[0], x[0]),
        filterstep (a[1], x[1]),
        filterstep (a[2], x[2])
    );
}
point _filterstep (point a; float x)
{
    return  point (
        filterstep (a[0], x),
        filterstep (a[1], x),
        filterstep (a[2], x)
    );
}
point _filterstep (float a; point x)
{
    return  point (
        filterstep (a, x[0]),
        filterstep (a, x[1]),
        filterstep (a, x[2])
    );
}
point _filterstep (point a; float x; float width)
{
    float   dx = max (filterWidth(x), width);
    return  point (
        filterstep (a[0], x, x+dx),
        filterstep (a[1], x, x+dx),
        filterstep (a[2], x, x+dx)
    );
}

point _filterstep (float a; point x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  point (
        filterstep (a, x[0], x[0]+dx[0]),
        filterstep (a, x[1], x[1]+dx[1]),
        filterstep (a, x[2], x[2]+dx[2])
    );
}

point _filterstep (point a; point x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  point (
        filterstep (a[0], x[0], x[0]+dx[0]),
        filterstep (a[1], x[1], x[1]+dx[1]),
        filterstep (a[2], x[2], x[2]+dx[2])
    );
}
point _filterstep (point a; float x; string filter)
{
    return  point (
        filterstep (a[0], x, "filter", filter),
        filterstep (a[1], x, "filter", filter),
        filterstep (a[2], x, "filter", filter)
    );
}

point _filterstep (float a; point x; string filter)
{
    return  point (
        filterstep (a, x[0], "filter", filter),
        filterstep (a, x[1], "filter", filter),
        filterstep (a, x[2], "filter", filter)
    );
}

point _filterstep (point a; point x; string filter)
{
    return  point (
        filterstep (a[0], x[0], "filter", filter),
        filterstep (a[1], x[1], "filter", filter),
        filterstep (a[2], x[2], "filter", filter)
    );
}
point _filterstep (point a; float x; float width; string filter)
{
    float   dx = max (filterWidth(x), width);
    return  point (
        filterstep (a[0], x, x+dx, "filter", filter),
        filterstep (a[1], x, x+dx, "filter", filter),
        filterstep (a[2], x, x+dx, "filter", filter)
    );
}

point _filterstep (float a; point x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  point (
        filterstep (a, x[0], x[0]+dx[0], "filter", filter),
        filterstep (a, x[1], x[1]+dx[1], "filter", filter),
        filterstep (a, x[2], x[2]+dx[2], "filter", filter)
    );
}

point _filterstep (point a; point x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  point (
        filterstep (a[0], x[0], x[0]+dx[0], "filter", filter),
        filterstep (a[1], x[1], x[1]+dx[1], "filter", filter),
        filterstep (a[2], x[2], x[2]+dx[2], "filter", filter)
    );
}

vector _filterstep (vector a; vector x)
{
    return  vector (
        filterstep (a[0], x[0]),
        filterstep (a[1], x[1]),
        filterstep (a[2], x[2])
    );
}
vector _filterstep (vector a; float x)
{
    return  vector (
        filterstep (a[0], x),
        filterstep (a[1], x),
        filterstep (a[2], x)
    );
}
vector _filterstep (float a; vector x)
{
    return  vector (
        filterstep (a, x[0]),
        filterstep (a, x[1]),
        filterstep (a, x[2])
    );
}
vector _filterstep (vector a; float x; float width)
{
    float   dx = max (filterWidth(x), width);
    return  vector (
        filterstep (a[0], x, x+dx),
        filterstep (a[1], x, x+dx),
        filterstep (a[2], x, x+dx)
    );
}

vector _filterstep (float a; vector x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  vector (
        filterstep (a, x[0], x[0]+dx[0]),
        filterstep (a, x[1], x[1]+dx[1]),
        filterstep (a, x[2], x[2]+dx[2])
    );
}

vector _filterstep (vector a; vector x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  vector (
        filterstep (a[0], x[0], x[0]+dx[0]),
        filterstep (a[1], x[1], x[1]+dx[1]),
        filterstep (a[2], x[2], x[2]+dx[2])
    );
}
vector _filterstep (vector a; float x; string filter)
{
    return  vector (
        filterstep (a[0], x, "filter", filter),
        filterstep (a[1], x, "filter", filter),
        filterstep (a[2], x, "filter", filter)
    );
}

vector _filterstep (float a; vector x; string filter)
{
    return  vector (
        filterstep (a, x[0], "filter", filter),
        filterstep (a, x[1], "filter", filter),
        filterstep (a, x[2], "filter", filter)
    );
}

vector _filterstep (vector a; vector x; string filter)
{
    return  vector (
        filterstep (a[0], x[0], "filter", filter),
        filterstep (a[1], x[1], "filter", filter),
        filterstep (a[2], x[2], "filter", filter)
    );
}
vector _filterstep (vector a; float x; float width; string filter)
{
    float   dx = max (filterWidth(x), width);
    return  vector (
        filterstep (a[0], x, x+dx, "filter", filter),
        filterstep (a[1], x, x+dx, "filter", filter),
        filterstep (a[2], x, x+dx, "filter", filter)
    );
}

vector _filterstep (float a; vector x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  vector (
        filterstep (a, x[0], x[0]+dx[0], "filter", filter),
        filterstep (a, x[1], x[1]+dx[1], "filter", filter),
        filterstep (a, x[2], x[2]+dx[2], "filter", filter)
    );
}

vector _filterstep (vector a; vector x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  vector (
        filterstep (a[0], x[0], x[0]+dx[0], "filter", filter),
        filterstep (a[1], x[1], x[1]+dx[1], "filter", filter),
        filterstep (a[2], x[2], x[2]+dx[2], "filter", filter)
    );
}

normal _filterstep (normal a; normal x)
{
    return  normal (
        filterstep (a[0], x[0]),
        filterstep (a[1], x[1]),
        filterstep (a[2], x[2])
    );
}
normal _filterstep (normal a; float x)
{
    return  normal (
        filterstep (a[0], x),
        filterstep (a[1], x),
        filterstep (a[2], x)
    );
}
normal _filterstep (float a; normal x)
{
    return  normal (
        filterstep (a, x[0]),
        filterstep (a, x[1]),
        filterstep (a, x[2])
    );
}
normal _filterstep (normal a; float x; float width)
{
    float   dx = max (filterWidth(x), width);
    return  normal (
        filterstep (a[0], x, x+dx),
        filterstep (a[1], x, x+dx),
        filterstep (a[2], x, x+dx)
    );
}

normal _filterstep (float a; normal x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  normal (
        filterstep (a, x[0], x[0]+dx[0]),
        filterstep (a, x[1], x[1]+dx[1]),
        filterstep (a, x[2], x[2]+dx[2])
    );
}

normal _filterstep (normal a; normal x; float width)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  normal (
        filterstep (a[0], x[0], x[0]+dx[0]),
        filterstep (a[1], x[1], x[1]+dx[1]),
        filterstep (a[2], x[2], x[2]+dx[2])
    );
}
normal _filterstep (normal a; float x; string filter)
{
    return  normal (
        filterstep (a[0], x, "filter", filter),
        filterstep (a[1], x, "filter", filter),
        filterstep (a[2], x, "filter", filter)
    );
}

normal _filterstep (float a; normal x; string filter)
{
    return  normal (
        filterstep (a, x[0], "filter", filter),
        filterstep (a, x[1], "filter", filter),
        filterstep (a, x[2], "filter", filter)
    );
}

normal _filterstep (normal a; normal x; string filter)
{
    return  normal (
        filterstep (a[0], x[0], "filter", filter),
        filterstep (a[1], x[1], "filter", filter),
        filterstep (a[2], x[2], "filter", filter)
    );
}
normal _filterstep (normal a; float x; float width; string filter)
{
    float   dx = max (filterWidth(x), width);
    return  normal (
        filterstep (a[0], x, x+dx, "filter", filter),
        filterstep (a[1], x, x+dx, "filter", filter),
        filterstep (a[2], x, x+dx, "filter", filter)
    );
}

normal _filterstep (float a; normal x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  normal (
        filterstep (a, x[0], x[0]+dx[0], "filter", filter),
        filterstep (a, x[1], x[1]+dx[1], "filter", filter),
        filterstep (a, x[2], x[2]+dx[2], "filter", filter)
    );
}

normal _filterstep (normal a; normal x; float width; string filter)
{
    float   dx[3] = {
        max (filterWidth(x[0]), width),
        max (filterWidth(x[1]), width),
        max (filterWidth(x[2]), width)
    };
    return  normal (
        filterstep (a[0], x[0], x[0]+dx[0], "filter", filter),
        filterstep (a[1], x[1], x[1]+dx[1], "filter", filter),
        filterstep (a[2], x[2], x[2]+dx[2], "filter", filter)
    );
}

#endif
