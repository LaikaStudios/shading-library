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
#ifndef COLOR_H
#define COLOR_H

#include <sl.h>
#include <math.h>

uniform color colorGridmin( varying color _C )
{
    uniform color   C;

    C = color (
        gridmin( _C[0] ),
        gridmin( _C[1] ),
        gridmin( _C[2] )
    );

    return C;
}

uniform color colorGridmax( varying color _C )
{
    uniform color   C;

    C = color (
        gridmax( _C[0] ),
        gridmax( _C[1] ),
        gridmax( _C[2] )
    );

    return C;
}

/*
 *  While sRGB to light linear conversion is defined for the
 *  0 to 1 region of RGB component values, intensities above
 *  1 are not so much (not to mention negative values).
 *  Extrapolating the sRGB conversion for values > 1 tends to
 *  cause too-bright results, so we define a simple power
 *  function conversion for values above 1; and handle negative
 *  intensities. Note that the smoothstep function is not properly
 *  inverted, so there is some small conversion error when going back
 *  and forth; but it's very small.
 */
#define COLOR_LUMA_TO_LINEAR_EXPONENT     2.0
#define COLOR_LUMA_TO_LINEAR_TRANSITION   1.1
// pow(COLOR_LUMA_TO_LINEAR_TRANSITION,COLOR_LUMA_TO_LINEAR_EXPONENT)
#define COLOR_LINEAR_TO_LUMA_TRANSITION   1.21

// Decode from sRGB luma to linear light
// Added handling of negative values, and power curve extension for values > 1.
float _sRGBDecode(float f)
{
    float   sign_f = sign(f);
    float   abs_f = abs(f);

    float   l_sRGB = mix (
        abs_f / 12.92,
        pow ((abs_f + 0.055) / 1.055, 2.4),
        step (0.04045, abs_f)
    );
    float   l_pow = pow (abs_f, COLOR_LUMA_TO_LINEAR_EXPONENT);

    return sign_f * mix (l_sRGB, l_pow, smoothstep (1.0, COLOR_LUMA_TO_LINEAR_TRANSITION, abs_f));
}

// Encode from linear to sRGB
// Added handling of negative values, and power curve extension for values > 1.
float _sRGBEncode(float f)
{
    float   sign_f = sign(f);
    float   abs_f = abs(f);

    float   l_sRGB = mix (
        abs_f * 12.92,
        1.055 * pow (abs_f, 1/2.4) - 0.055,
        step (0.0031308, abs_f)
    );
    float   l_pow = pow (abs_f, 1/COLOR_LUMA_TO_LINEAR_EXPONENT);

    return sign_f * mix (l_sRGB, l_pow, smoothstep (1.0, COLOR_LINEAR_TO_LUMA_TRANSITION, abs_f));
}

//  Decode sRGB to linear.
float colorSRGBDecode (float x) {
    return _sRGBDecode(x);
}

color colorSRGBDecode (color c)
{
    return color (
        _sRGBDecode(c[0]),
        _sRGBDecode(c[1]),
        _sRGBDecode(c[2])
    );
}

//  Encode linear to sRGB.
float colorSRGBEncode (float x) {
    return _sRGBEncode(x);
}

color colorSRGBEncode (color c)
{
    return color (
        _sRGBEncode(c[0]),
        _sRGBEncode(c[1]),
        _sRGBEncode(c[2])
    );
}

/* sRGB to linear conversion for float values read from textures and intended
 * to be multiplied by color values.
 * returns linearized float value
 */
float colorSRGBToLinear(float sRGBValue)
{
#ifdef RENDER_LINEAR
    return _sRGBDecode(sRGBValue);
#else
    return sRGBValue;
#endif // RENDER_LINEAR
}

/* sRGB to linear conversion for color values read from textures and intended
 * to be used for color related values.
 * returns linearized color value
 */
color colorSRGBToLinear(color sRGBColor)
{
#ifdef RENDER_LINEAR
    color noGammaColor;
    setcomp(noGammaColor, 0, _sRGBDecode(comp(sRGBColor,0)));
    setcomp(noGammaColor, 1, _sRGBDecode(comp(sRGBColor,1)));
    setcomp(noGammaColor, 2, _sRGBDecode(comp(sRGBColor,2)));
    return noGammaColor;
#else
    return sRGBColor;
#endif // RENDER_LINEAR
}

#define sRGBToRGB(srgb) colorSRGBToLinear(srgb)

/* linear to sRGB conversion for float values
 * returns sRGB float value
 */
float colorLinearToSRGB(float linValue)
{
#ifdef RENDER_LINEAR
    return _sRGBEncode(linValue);
#else
    return linValue;
#endif // RENDER_LINEAR
}

/* linear to sRGB conversion for color values
 * returns sRGB color value
 */
color colorLinearToSRGB(color linColor)
{
#ifdef RENDER_LINEAR
    color noGammaColor;
    setcomp(noGammaColor, 0, _sRGBEncode(comp(linColor,0)));
    setcomp(noGammaColor, 1, _sRGBEncode(comp(linColor,1)));
    setcomp(noGammaColor, 2, _sRGBEncode(comp(linColor,2)));
    return noGammaColor;
#else
    return linColor;
#endif // RENDER_LINEAR
}

#define RGBTosRGB(rgb) colorLinearToSRGB(rgb)

/* Gamma function for color values.
 * returns the gamma corrected color
 */
color colorGamma(color inColor; float gamma)
{
    color gammaColor;
    float exponent = 1/gamma;

    setcomp(gammaColor, 0, pow(comp(inColor,0), exponent));
    setcomp(gammaColor, 1, pow(comp(inColor,1), exponent));
    setcomp(gammaColor, 2, pow(comp(inColor,2), exponent));
    return gammaColor;

}

/*
 *  HSY transformations.
 */
color colorRGBToHSY (color in) {
    return ctransform ("RGB", "HSY", in);
}

color colorHSYToRGB (color in) {
    color   Ctmp = in;

    // Keeps hue [0,1)
    Ctmp[0] = mod (Ctmp[0],1);

    #pragma nolint
    return ctransform ("HSY", "RGB", Ctmp);
}

/*
 *  XYZ transformations.
 */

// Transform linear RGB with white=(1,1,1) to XYZ with D65 white point.
color colorRGBToXYZ(color RGB)
{
    color   XYZ;

    XYZ = color (
        vector(RGB).vector(.4142,.3576,.1805),
        vector(RGB).vector(.2126,.7152,.0722),
        vector(RGB).vector(.0193,.1192,.9505)
    );

    return XYZ;
}
#define RGBToXYZ(rgb) colorRGBToXYZ(rgb)

// Transform XYZ with D65 white point to linear RGB with white=(1,1,1).
color colorXYZToRGB(color XYZ)
{
    color   RGB;

    RGB = color (
        vector(XYZ).vector( 3.2406,-1.5372,-0.4986),
        vector(XYZ).vector(-0.9689, 1.8758, 0.0415),
        vector(XYZ).vector( 0.0557,-0.2040, 1.0570)
    );

    return RGB;
}
#define XYZToRGB(xyz) colorXYZToRGB(xyz)

// xyz transformations.
color colorXYZToxyz(color XYZ)
{
    float   sum = vector(XYZ).vector(1);

    return XYZ/sum;
}

color colorxyzToXYZ(color xyz)
{
    return xyz;
}

/*
 *  Lab, Luv, and LCh transformations.
 */

// CIE L*a*b*.
// Assumes XYZ has a D65 white point.
// So not L: 0-100. a: ±128. b: ±128; but L: 0-10, etc. Their xyz and XYZ are also identical.

// BT.709 definition (x=.3127, y=.3290, Y=1) of true D65 (x=.31271, y=.32902, Y=1) White Point.
// XYZ: X = Yx/y; Y = Y; Z = Y(1-x-y)/y.
// sRGB definition uses BT.709 primaries.
#define D65XYZ color(0.950456,1.000000,1.089058)

#define D65u ( 4.0*comp(D65XYZ,0) / vector(D65XYZ).vector(1.0,15.0,3.0) )
#define D65v ( 9.0*comp(D65XYZ,1) / vector(D65XYZ).vector(1.0,15.0,3.0) )

color colorXYZToLab(color XYZ)
{
#if 1
    uniform float   a = pow(116.0/24.0, 2.0) / 3.0; // linear portion conversion coefficients.
    uniform float   b = 16.0/116.0;

    uniform color   t0 = color(pow(24.0/116.0, 3.0));   // transition point.

    color   XYZ1 = XYZ / D65XYZ;
    color   XYZ1abs = _abs(XYZ1);
    color   XYZ1sign = _sign(XYZ1);

    color   Lab;

    XYZ1 = mix (
        a*XYZ1 + b,
        XYZ1sign * _pow(XYZ1abs, color(1.0/3.0)),
        step (t0, XYZ1abs)
    );

    Lab = color (116.0*XYZ1[1] - 16.0, 500.0*(XYZ1[0] - XYZ1[1]), 200.0*(XYZ1[1] - XYZ1[2]));

    return Lab;
#else
    uniform float   e = 216.0/24389.0;
    uniform float   k = 24389.0/27.0;

    color   XYZn = XYZ / D65XYZ;
    color   absXYZn = _abs(XYZn);
    color   signXYZn = _sign(XYZn);

    color   fXYZn;

    fXYZn = mix (
        k*(XYZn + color(16.0)) / 116.0,
        signXYZn * _pow (absXYZn, 1.0/3.0),
        _step (e, absXYZn)
    );

    color   Lab;

    Lab = color (116.0*fXYZn[1] - 16.0, 500.0*(fXYZn[0] - fXYZn[1]), 200.0*(fXYZn[1] - fXYZn[2]));

    return Lab;
#endif
}
#define XYZToLab(xyz) colorXYZToLab(xyz)

// CIE L*a*b*.
// Produces XYZ with D65 white point.
color colorLabToXYZ(color Lab)
{
#if 1
    uniform float   a = pow(116.0/24.0, 2.0) / 3.0; // linear portion conversion coefficients.
    uniform float   b = 16.0/116.0;

    uniform color   t0 = color(24.0/116.0); // transition point.

    float   Y1 = (Lab[0] + 16.0) / 116.0;
    color   XYZ1 = color (Y1 + Lab[1]/500.0, Y1, Y1 - Lab[2]/200.0);
    color   XYZ1abs = _abs(XYZ1);
    color   XYZ1sign = _sign(XYZ1);

    color   XYZ;

    XYZ1 = mix (
        (XYZ1 - b) / a,
        XYZ1sign * _pow(XYZ1abs, color(3.0)),
        step (t0, XYZ1abs)
    );

    XYZ = XYZ1 * D65XYZ;

    return XYZ;
#else
    uniform float   e = 216.0/24389.0;
    uniform float   k = 24389.0/27.0;

    float   fY = (Lab[1] + 16.0) / 116.0;
    color   fXYZ = color (fY + Lab[1]/500.0, fY, fY - Lab[2]/200.0);
    color   absfXYZ = _abs(fXYZ);
    color   signfXYZ = _sign(fXYZ);

    color   XYZn;

    XYZn = _pow(XYZn, 3.0);

    XYZn = mix (
        color (116.0*fXYZ[0] - 16.0, Lab[0], 116.0*fXYZ[2] - 16.0) / k,
        XYZn,
        _step (color(e, k*e, e), color(XYZn[0],Lab[0],XYZn[2]))
    );

    return XYZn * D65XYZ;
#endif
}
#define LabToXYZ(lab) colorLabToXYZ(lab)

// CIE L*u*v*.
// Assumes XYZ has a D65 white point.
// L: 0-100. u: ±100. v: ±100.
color colorXYZToLuv(color XYZ)
{
    uniform float   t0 = 216/2439;  // transition point.

    float   Y1 = XYZ[1] / comp(D65XYZ,1);
    float   Y1abs = abs(Y1);
    float   Y1sign = sign(Y1);

    float   L;
    vector  uv;

    L = mix (
        (24389/27) * Y1abs,
        Y1sign * (116.0*pow(Y1abs, 1.0/3.0) - 16.0),
        step (t0, Y1abs)
    );

    uv = vector(4.0*XYZ[0], 9.0*XYZ[1], 1) / vector(XYZ).vector(1.0,15.0,3.0);
    uv = 13.0*L * (uv - vector(D65u,D65v,0));

    return color(L,uv[0],uv[1]);
}
#define XYZToLuv(xyz) colorXYZToLuv(xyz)

// CIE L*u*v*.
// Produces XYZ with D65 white point.
color colorLuvToXYZ(color Luv)
{
#if 1
    float   L = Luv[0];
    float   Labs = abs(L);
    float   Lsign = sign(L);

    float   Y, u1, v1;

    Y = mix (
        Luv[0] * pow(3.0/29.0, 3.0),
        pow((Luv[0]+16.0)/116.0, 3.0),
        step(8.0, Labs)
    ) * comp(D65XYZ,1);

    u1 = Luv[1]/(13.0*Luv[0]);
    v1 = Luv[2]/(13.0*Luv[0]);

    if (u1==u1) {} else { u1 = 0; }
    if (v1==v1) {} else { v1 = 0; }

    u1 += D65u;
    v1 += D65v;

    color    XYZ = Y * color((9.0*u1)/(4.0*v1), 1.0, (12.0 - 3.0*u1 - 20.0*v1)/(4.0*v1));

    return XYZ;
#else
    uniform float   e = 216.0/24389.0;
    uniform float   k = 24389.0/27.0;

    float   Y = mix (
                Luv[0]/k,
                pow((Luv[0]+16.0)/116.0, 3.0),
                step(k*e, Luv[0])
            );

    float   a = (52.0*Luv[0]/(Luv[1] + 13.0*Luv[0]*D65u) - 1.0) / 3.0;
    float   b = -5.0*Y;
    float   c = -1/3;
    float   d = Y*((39.0*Luv[0]/(Luv[2] + 13.0*Luv[0]*D65v)) - 5.0);

    float   X = (d-b)/(a-c);
    float   Z = X*a + b;

    return color(X,Y,Z);
#endif
}
#define LuvToXYZ(luv) colorLuvToXYZ(luv)

// CIE LCh.
// Cylindrical form of CIE L*a*b* or L*u*v* spaces.
// L is lightness, C is chroma, h is hue.
// L and C range depends on input.
// h: ±PI. Magenta≈0, Red≈¼·PI, Yellow≈½⋅PI, Green≈¾·PI, Cyan≈±PI, Aqua≈-¾⋅PI, Blue≈-½⋅PI, Purple≈-¼·PI.
color colorLxxToLCh(color Lxx)
{
    float   C, h;

    C = length( vector(Lxx[1],Lxx[2],0) );

    h = atan( Lxx[2],Lxx[1] );
    if (h==h) {} else { h = 0; }

    color   LCh = color( Lxx[0], C, h );

    return LCh;
}
#define LxxToLCh(lxx) colorLxxToLCh(lxx)

color colorLChToLxx(color LCh)
{
    color   Lxx = color( LCh[0], LCh[1]*cos(LCh[2]), LCh[1]*sin(LCh[2]) );

    return Lxx;
}
#define LChToLxx(lch) colorLChToLxx(lch)


/*
 * Combined transformations.
 */
color colorsRGBToXYZ(color sRGB) { return RGBToXYZ(sRGBToRGB(sRGB)); }
color colorXYZTosRGB(color XYZ)  { return RGBTosRGB(XYZToRGB(XYZ)); }
#define sRGBToXYZ(srgb) colorsRGBToXYZ(srgb)
#define XYZTosRGB(xyz)  colorXYZTosRGB(xyz)

color colorsRGBToLab(color sRGB) { return XYZToLab(sRGBToXYZ(sRGB)); }
color colorLabTosRGB(color Lab)  { return XYZTosRGB(LabToXYZ(Lab)); }
#define sRGBToLab(sRGB) colorsRGBToLab(sRGB)
#define LabTosRGB(Lab)  colorLabTosRGB(Lab)

color colorsRGBToLuv(color sRGB) { return XYZToLuv(sRGBToXYZ(sRGB)); }
color colorLuvTosRGB(color Luv)  { return XYZTosRGB(LuvToXYZ(Luv)); }
#define sRGBToLuv(sRGB) colorsRGBToLuv(sRGB)
#define LuvTosRGB(Luv)  colorLuvTosRGB(Luv)

color colorsRGBToLabCh(color sRGB)  { return LxxToLCh(sRGBToLab(sRGB)); }
color colorLabChTosRGB(color LabCh) { return LabTosRGB(LChToLxx(LabCh)); }
#define sRGBToLabCh(sRGB)  colorsRGBToLabCh(sRGB)
#define LabChTosRGB(LabCh) colorLabChTosRGB(LabCh)

color colorsRGBToLuvCh(color sRGB)  { return LxxToLCh(sRGBToLuv(sRGB)); }
color colorLuvChTosRGB(color LuvCh) { return LuvTosRGB(LChToLxx(LuvCh)); }
#define sRGBToLuvCh(sRGB)  colorsRGBToLuvCh(sRGB)
#define LuvChTosRGB(LuvCh) colorLuvChTosRGB(LuvCh)

color colorsRGBToLCh(color sRGB) { return sRGBToLabCh(sRGB); }
color colorLChTosRGB(color LCh)  { return LabChTosRGB(LCh); }
#define sRGBToLCh(sRGB) colorsRGBToLCh(sRGB)
#define LChTosRGB(LCh)  colorLChTosRGB(LCh)


/*
 * Delta E computations.
 */

// CIE 76
float colorLabToDE76(color Lab1, Lab2)
{
    color   dLab = Lab2 - Lab1;

    return length( vector(dLab) );
}
#define LabToDE76(Lab1,Lab2) colorLabToDE76(Lab1,Lab2)


// CIE 94
float colorLabToDE94(color Lab1, Lab2)
{
    uniform float   k1 = 0.045;
    uniform float   k2 = 0.015;

    float   dL = Lab1[0] - Lab2[0];
    float   dE = length( vector(Lab2 - Lab1) );
    float   C1 = length( vector(Lab1)*vector(0,1,1) );
    float   C2 = length( vector(Lab2)*vector(0,1,1) );
    float   dC = C1 - C2;
    float   dH = sqrt( dE*dE - dL*dL - dC*dC );

    if (dH==dH) {} else { dH = 0.0; }

    color   dLCh = color( dL, dC/(1.0 + k1*C1), dH/(1.0 + k2*C1) );

    return length( vector(dLCh) );
}
#define LabToDE94(Lab1,Lab2) colorLabToDE94(Lab1,Lab2)


// CIE 2000
// The k constants are the inverse of the CIE spec: kL=1/kL, etc.
// so this single function can be used for Ch and h based DE computations.
float colorLabToDE2000(color Lab1, Lab2; float kL, kC, kH)
{
    uniform float   pow25_7 = pow(25.0,7.0);

    color   LCh1 = LxxToLCh(Lab1);
    color   LCh2 = LxxToLCh(Lab2);

    float   dL = LCh1[0] - LCh2[0];
    float   avgL = (LCh1[0] + LCh2[0]) / 2.0;
    float   avgC = (LCh1[1] + LCh2[1]) / 2.0;

    float   avgC_7 = pow( avgC,7 );
    float   ka = sqrt( avgC_7 / (avgC_7 + pow25_7) );

    float   a1 = Lab1[1] + (Lab1[1]/2.0)*(1.0 - ka);
    float   a2 = Lab2[1] + (Lab2[1]/2.0)*(1.0 - ka);

    float   Ca1 = length( vector(a1,Lab1[2],0) );
    float   Ca2 = length( vector(a2,Lab2[2],0) );
    float   dCa = Ca2 - Ca1;
    float   avgCa = (Ca1 + Ca2) / 2.0;
    float   meanCa = sqrt( Ca1*Ca2 );

    float   ha1 = atan( Lab1[2],a1 );  // ±PI
    float   ha2 = atan( Lab2[2],a2 );  // ±PI
    if (ha1==ha1) {} else { ha1 = 0.0; }
    if (ha2==ha2) {} else { ha2 = 0.0; }

    float   dha = abs(ha2 - ha1); if (dha > PI) dha = TWOPI - dha;
    float   dHa = 2.0*meanCa * sin(dha/2.0);

    ha1 += PI;
    ha2 += PI;

    float   avgHa = (ha1 + ha2) / 2.0;
    if (abs(ha1-ha2) > PI) avgHa = (ha1 + ha2 + TWOPI) / 2.0;

    float   T = 1.0
              - 0.17*cos( avgHa - radians(30.0) )
              + 0.24*cos( 2.0*avgHa )
              + 0.32*cos( 3.0*avgHa + radians(6.0) )
              - 0.20*cos( 4.0*avgHa - radians(63.0) );

    float   avgL_2 = pow( avgL-50.0, 2 );
    float   SL = 1.0 + (0.015*avgL_2)/sqrt(20.0 + avgL_2);
    float   SC = 1.0 + 0.045*avgCa;
    float   SH = 1.0 + 0.015*avgCa*T;
    float   RT = -2.0*sqrt(ka) * sin(radians(60.0)*exp(-pow((avgHa-radians(275.0))/radians(25.0),2)));

    if (kC > 0.0) RT *= kC*(dCa/SC);
    if (kH > 0.0) RT *= kH*(dHa/SH);

    float   DE = sqrt( pow(kL*dL/SL,2) + pow(kC*dCa/SC,2) + pow(kH*dHa/SH,2) + RT );

    return DE;
}
#define LabToDE2000(Lab1,Lab2,kL,kC,kH) colorLabToDE2000(Lab1,Lab2,kL,kC,kH)


/*
 * LCh interpolation functions.
 * Hue is blended pseudo-spectrally, using rgb primaries.
 */
color colorMixLabCh( color LCh0, LCh1; float a )
{
    color   RGB0, RGB1, RGB, LCh, result;

    RGB0 = XYZToRGB(LabToXYZ(LChToLxx(LCh0)));
    RGB1 = XYZToRGB(LabToXYZ(LChToLxx(LCh1)));

    RGB = mix( RGB0, RGB1, a );
    LCh = LxxToLCh(XYZToLab(RGBToXYZ(RGB)));

    result = mix( color(LCh0[0],LCh0[1],LCh[2]), color(LCh1[0],LCh1[1],LCh[2]), a );

    return result;
}

color colorMixLabCh( color LCh0, LCh1, a )
{
    color   RGB0, RGB1, RGB, LCh, result;

    RGB0 = XYZToRGB(LabToXYZ(LChToLxx(LCh0)));
    RGB1 = XYZToRGB(LabToXYZ(LChToLxx(LCh1)));

    RGB = mix( RGB0, RGB1, a[2] );
    LCh = LxxToLCh(XYZToLab(RGBToXYZ(RGB)));

    result = mix( color(LCh0[0],LCh0[1],LCh[2]), color(LCh1[0],LCh1[1],LCh[2]), a );

    return result;
}

#define MixLabCh(LCh0,LCh1,a) colorMixLabCh(LCh0,LCh1,a)


/*
 *  RGB to Luma conversions.
 */
#define COLOR_BT601Coefficients   vector(.298939,.586625,.114436)
#define COLOR_BT709Coefficients   vector(.212671,.715160,.072169)

float colorRGBdotCoefficients (color in; uniform vector coeff) {
    return (vector(in).coeff);
}

#define colorRGBToBT601Luma(C)  colorRGBdotCoefficients(C,COLOR_BT601Coefficients)
#define colorRGBToBT709Luma(C)  colorRGBdotCoefficients(C,COLOR_BT709Coefficients)

// Use 601 Luma by default
#define colorRGBToLuma(C)       colorRGBdotCoefficients(C,COLOR_BT601Coefficients)
#define colorRGBToLuminance(C)  colorRGBdotCoefficients(C,COLOR_BT601Coefficients)
#define colorSRGBToLuma(C)       colorRGBdotCoefficients(C,COLOR_BT601Coefficients)
#define colorSRGBToLuminance(C)  colorRGBdotCoefficients(C,COLOR_BT601Coefficients)

color colorRGBSetLuma (color in; float luma) {
    float   inLuma = colorRGBToLuma(in);
    return  in * (luma / inLuma);
}

#define colorRGBClampLuminance(c,a,b) colorRGBClampLuma(c,a,b)
color colorRGBClampLuma (color in; float a, b) {
    float   luma = colorRGBToLuma(in);
    luma = clamp (luma, 0, 1);
    return colorRGBSetLuma(in,luma);
}

#define colorSRGBToValue(c) colorRGBToValue(c)
float colorRGBToValue (color in)
{
    color   Ctmp = ctransform ("RGB", "hsv", in);
    return comp(Ctmp,2);
}

#define colorSRGBToAvg(c) colorRGBToAvg(c)
float colorRGBToAvg (color in)
{
    return (vector(in).vector(1/3));
}

/*
 *  Edge functions.
 */
color colorEdgeSRGBLuma (color c; float t) {
    float   luma = colorRGBToLuma(c);
    float   _sign_ = sign(luma);

    luma = mathEdge (_sign_*luma, .5, t);
    luma *= _sign_;

    return colorRGBSetLuma (c, luma);
}

color colorEdgeLinearLuma (color c; float t) {
    float   luma = colorRGBToLuma(c);
    float   _sign_ = sign(luma);

    luma = mathEdge (_sign_*luma, .218, t);
    luma *= _sign_;

    return colorRGBSetLuma (c, luma);
}

color colorEdgeLuma (color c; float t) {
#ifdef RENDER_LINEAR
    return colorEdgeLinearLuma (c, t);
#else
    return colorEdgeSRGBLuma (c, t);
#endif
}

// de-clipping hack from Doug Creel
color colorDeclip(color Cin)
{
    color Cout = Cin;
    float cineonheal = pow(685/1024, 1/2.5);
    float ix;
    for(ix=0; ix<3; ix+=1) {
        if(comp(Cin, ix) > cineonheal) {
            float clipamount = comp(Cin, ix) - cineonheal;
            float correction = (1 - exp(-clipamount)) * (1 - cineonheal);
            setcomp(Cout, ix, cineonheal + correction);
        }
    }
    return Cout;
}

// de-clipping hack from Doug Creel
float colorDeclip(float Cin)
{
    float Cout = Cin;
    float cineonheal = pow(685/1024, 1/2.5);
    if(Cin > cineonheal) {
        float clipamount = Cin - cineonheal;
        float correction = (1 - exp(-clipamount)) * (1 - cineonheal);
        Cout = cineonheal + correction;
    }
    return Cout;
}



color colorAdjust
(
    output varying color    response;

    varying float   intensity;
    varying float   saturation;
    varying color   tint;
) {
    response *= intensity;
    response = mix( color(colorRGBToLuma(response)), response, saturation );
    response *= tint;
}


#define COLOR_CLAMP_OFF 0
#define COLOR_CLAMP_ON 1
#define COLOR_CLAMP_DECLIP 2

color colorAdjust(varying color inColor;
                  uniform float remapMin, remapMax;
                  uniform float saturate;
                  uniform float clampop;
                  uniform color tint;)
{
    color outColor = inColor;

    if (remapMin != 0 || remapMax != 1) {
        // not trusting the compiler to do this...
        uniform color cmin = color(remapMin, remapMin, remapMin);
        uniform color cmax = color(remapMax, remapMax, remapMax);
        outColor = mix(cmin, cmax, outColor);
    }

    if (saturate != 0) {
        color hsv = ctransform("hsv", outColor);
        setcomp(hsv, 1, clamp(comp(hsv, 1)+saturate, 0, 1));
        outColor = ctransform("hsv", "rgb", hsv);
    }

    if (clampop == COLOR_CLAMP_ON) {
        // not trusting the compiler to do this...
        uniform color zero = color(0,0,0);
        uniform color one = color(1,1,1);
        outColor = clamp(outColor, zero, one);
    } else if (clampop == COLOR_CLAMP_DECLIP) {
        outColor = colorDeclip(outColor);
    }

    outColor *= tint;

    return outColor;
}

float colorAdjust(varying float inFloat;
                  uniform float remapMin, remapMax;
                  uniform float clampop)
{
    float outFloat = inFloat;

    if (remapMin != 0 || remapMax != 1) {
        outFloat = mix (remapMin, remapMax, outFloat);
    }

    if (clampop == COLOR_CLAMP_ON) {
        outFloat = clamp(outFloat, 0, 1);
    } else if (clampop == COLOR_CLAMP_DECLIP) {
        outFloat = colorDeclip(outFloat);
    }

    return outFloat;
}
                  
/* Adjust color map using brightness, contrast, HSV, and Tint.
 * Assume both inColor and tint are already linearized.
 * returns the adjusted color
 */
color colorAdjustMap(varying color inColor; 
                     float brightness, contrast;
                     float hue, saturation, value; 
                     color tint)
{
    color outColor = inColor;

    // Adjust HSV 
    outColor = ctransform("rgb", "hsv", outColor);
    setcomp(outColor, 0, outColor[0]+hue);
    outColor[1] += saturation;
    outColor[2] += value;
    outColor = ctransform("hsv", "rgb", outColor);

    // Adjust brightness and contrast
    if (contrast != 0.0) {
        outColor = mix(color(0.5,0.5,0.5), outColor, contrast);
    }
    outColor *= brightness;

    // Apply tint
    outColor *= tint;

    return outColor;
}

/* Adjust color map using brightness, contrast, HSV, tint, and
 * gamma correction. Assume both inColor and tint are already linearized.
 * returns the adjusted color
 */
color colorAdjustMap(varying color inColor; 
                     float brightness, contrast;
                     float hue, saturation, value; 
                     color tint; 
                     float gamma)
{
    color outColor = colorAdjustMap(inColor, brightness, contrast, 
                                    hue, saturation, value, tint);
    return colorGamma(outColor, gamma);

}


/* Shift color - shift inColor1 to include the color details of inColor2.
 * Example of usage is in the feather shader.
 * returns shifted color
 */
color colorShift(color inColor1; color inColor2; color averageColor;
                 float shiftMask;
                 float hueShiftScale; float satShiftScale; float lumShiftScale)
{
    // Note that "HsY" is intentional because specifying "hsy" does not work 
    // Note tht HsY returns hue outside of the 0 to 1 range.  We use mod() to
    // put hue in 0 to 1 range

    color inColor1HsY = ctransform("rgb", "HsY", inColor1);
    setcomp(inColor1HsY, 0, mod(comp(inColor1HsY, 0), 1.0));

    color inColor2HsY = ctransform("rgb", "HsY", inColor2);
    setcomp(inColor2HsY, 0, mod(comp(inColor2HsY, 0), 1.0));

    color avgColorHsY = ctransform("rgb", "HsY", averageColor);
    setcomp(avgColorHsY, 0, mod(comp(avgColorHsY, 0), 1.0));

    color shiftColorHsY = ctransform("rgb", "HsY", color(0,0,0));
    setcomp(shiftColorHsY, 0, hueShiftScale * (comp(inColor2HsY, 0) - comp(avgColorHsY, 0)));
    setcomp(shiftColorHsY, 1, satShiftScale * (comp(inColor2HsY, 1) - comp(avgColorHsY, 1)));
    setcomp(shiftColorHsY, 2, lumShiftScale * (comp(inColor2HsY, 2) - comp(avgColorHsY, 2)));
    shiftColorHsY *= shiftMask;

    color outColor = inColor1HsY + shiftColorHsY;
    setcomp(outColor, 0, mod(comp(outColor, 0), 1.0));

    outColor = ctransform("HsY", "rgb", outColor);

    return outColor;

}

/*
 *  Invert the hue of a color.
 *  Preserves the color's luma using the given luma definition.
 *  Needed for negative intensity illumination.
 *  Matrices derived from invertHueMatrix.h
 */
color colorInvert601Hue (color c) {
    uniform matrix  invertBT601Hue = (-0.4021219611, 0.5978780389, 0.5978781581, 0, 1.173249960, 0.1732500196, 1.173249960, 0, 0.2288719416, 0.2288720310, -0.7711279392, 0, 0, 0, 0, 1);

    return color transform (invertBT601Hue, point c);
}

color colorInvert709Hue (color c) {
    uniform matrix  invertBT709Hue = (-0.5746579766, 0.4253420234, 0.4253421426, 0, 1.430319905, 0.4303199649, 1.430319786, 0, 0.1443379223, 0.1443380117, -0.8556619883, 0, 0, 0, 0, 1);

    return color transform (invertBT709Hue, point c);
}

// Use BT601 luma definition if not specified.
#define colorSRGBInvertHue(C) colorInvert601Hue(C)

#define colorLinearInvertHue(c) colorInvert709Hue(c)


/* Convert RGB to Lambda which is in wavelength (nm). We are outputting
 * saturation and value in case we need to convert from lambda back to RGB.
 * param[out] saturation
 * param[out] value
 * returns wavelength
 */
float colorRGBToLambda(color inColor; 
                       output float saturation, value)
{
   // Transform RGB to HSV color space to get hue angle
   color outColor = ctransform("rgb", "hsv", inColor); 

   // Fit hue to 0 to 360 angle 
   float hue = comp(outColor, 0);
   hue = mix (0, 360, hue);

   // Compute wavelength
   float wavelength = 650 - (hue/240 * (650 - 475));
   wavelength = clamp(wavelength, 457, 650);

   // Output saturation and value
   saturation = comp(outColor, 1);
   value = comp(outColor, 2);

   return wavelength;
}

/* Convert Lambda (in wavelength) to RGB using input lamda, saturation,
 * and value.
 * returns color at the given wavelength, and given saturation and value
 */
color colorLambdaToRGB(float lambda, saturation, value)
{
   float hue = (650 - lambda) * 240/(650 - 475);
   hue = hue / 360;

   color outColor = color(0,0,0);
   outColor = ctransform("rgb", "hsv", outColor); 

   setcomp(outColor, 0, hue);
   setcomp(outColor, 1, saturation);
   setcomp(outColor, 2, value);

   outColor = ctransform("hsv", "rgb", outColor); 
   return outColor;
}

color colorRationalSpline (color c0, c1, c2; float x, t)
{
    float   c = mathRationalSpline (x, t);

    if (c < .5)
        return mix (c0, c1, linear (0, .5, c));
    else
        return mix (c1, c2, linear (.5, 1, c));
}


color colorScreen (color a, b) {
    return (color 1 - (color 1 - a)*(color 1 - b));
}

color colorOverlay (color Base, Blend)
{
    return mix (2*Base*Blend,
                color 1 - 2*(color 1 - Base)*(color 1 - Blend),
                step (color .5, Base));
}

color colorSoftLight (color Base, Blend)
{
    return mix (2*Base*Blend + Base*Base*(color 1 - 2*Blend),
                _sqrt(Base)*(2*Blend - color 1) + 2*Base*(color 1 - Blend),
                step (color .5, Blend));
}

color colorBurn (color Base, Blend)
{
    return (Blend == color 0) ? Blend : color 1 - (color 1 - Base) / Blend;
}

color colorDodge (color Base, Blend)
{
    return (Blend == color 1) ? Blend : Base / (color 1 - Blend);
}

color colorDarken (color Base, Blend)
{
    return min (Base, Blend);
}

color colorLighten (color Base, Blend)
{
    return max (Base, Blend);
}

color colorHardLight (color Base, Blend)
{
    return mix (2*Base*Blend,
                color 1 - 2*(color 1 - Base)*(color 1 - Blend),
                step (color .5, Blend));
}

#endif // COLOR_H
