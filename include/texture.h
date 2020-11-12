/*
 *  Copyright(c) 2010-2012 Laika, developed under contract by mitchpraterLLC.
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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <color.h>

/* single channel modes */
#define TEXTURE_CHANNEL_R 0
#define TEXTURE_CHANNEL_G 1
#define TEXTURE_CHANNEL_B 2
#define TEXTURE_CHANNEL_A 3

/* multi-channel read / single-channel result */
#define TEXTURE_CHANNEL_LUMA 4
#define TEXTURE_CHANNEL_MAX 5

/* multi-channel read / result */
#define TEXTURE_CHANNEL_RGB 6
#define TEXTURE_CHANNEL_RGBA 7

/* Texture class that encapsulates texture reads. The texture file may be a 
 * ptexture file or normal texture file. In the case of ptexture, the alpha
 * channel is assumed to be channel 3. 
 */
struct texture_s
{

    /* private member variables */
    private uniform string File = "";
    private uniform string Filter = "box";
    private varying float Blur = 0;
    private uniform float Lerp = 0;

    private uniform float _exists = 0;
    private uniform string _type = "unknown";
    private uniform float _numChannels = 0;
    private uniform float _isPtex = 0;

    /* public member variable access methods - these are used internally
       so must appear before their use */

    /* returns 1 if the texture exists */
    public uniform float Exists()
    {
        return _exists;
    }

    /* returns the type of texture as a string */
    public uniform string GetType()
    {
        return _type; 
    }

    /* return 1 if the file is a ptexture */
    public uniform float IsPtex()
    {
        return _isPtex; 
    }

    /* returns the number of channels in the texture */
    public uniform float GetNumChannels()
    {
        return _numChannels;
    }

    /* private texture access methods */

    /* read a float from the texture */
    private float _read(filterregion fr; uniform float channel)
    {
        if (channel >= GetNumChannels()) {
            // fill in alpha channel with 1 if channel not available
            return (channel == TEXTURE_CHANNEL_A) ? 1 : 0;
        }

        if (IsPtex() == 1) {
            varying float faceindex = 0;
            readprimvar("__faceindex", faceindex); 
            return float ptexture(File, channel, faceindex,
                                  "filter", Filter,
                                  "blur", Blur,
                                  "lerp", Lerp);
        } else {
            return float texture(File[channel], fr,
                                 "filter", Filter,
                                 "blur", Blur,
                                 "lerp", Lerp);
        }
    }

    /* read a color from the texture */
    private color _read(filterregion fr)
    {
        if (IsPtex() == 1) {
            varying float faceindex = 0;
            readprimvar("__faceindex", faceindex); 
            return color ptexture(File, 0, faceindex,
                                  "filter", Filter,
                                  "blur", Blur,
                                  "lerp", Lerp);
        } else {
            return color texture(File, fr,
                                 "filter", Filter,
                                 "blur", Blur,
                                 "lerp", Lerp);
        }
    }

    /* read out:
     * TEXTURE_CHANNEL_R
     * TEXTURE_CHANNEL_G
     * TEXTURE_CHANNEL_B
     * TEXTURE_CHANNEL_A
     * as a float
     */
    private void _readChannel(filterregion fr; uniform float channel;
                              output float outFloat)
    {
        outFloat = _read(fr, channel);
    }

    /* read out:
     * TEXTURE_CHANNEL_R
     * TEXTURE_CHANNEL_G
     * TEXTURE_CHANNEL_B
     * TEXTURE_CHANNEL_A
     * as a color
     */
    private void _readChannel(filterregion fr; uniform float channel;
                              output color outColor)
    {
        outColor = _read(fr, channel);
    }

    /* read out TEXTURE_CHANNEL_LUMA as a float */
    private void _readLuminance(filterregion fr; output float outFloat)
    {
        outFloat = colorRGBToLuminance(color _read(fr));
    }

    /* read out TEXTURE_CHANNEL_LUMA as a color */
    private void _readLuminance(filterregion fr; output color outColor)
    {
        outColor = colorRGBToLuminance(color _read(fr));
    }

    /* read out TEXTURE_CHANNEL_MAX as a float */
    private void _readMaxChannel(filterregion fr; output float outFloat)
    {
        color tmp = _read(fr);
        outFloat = comp (ctransform ("rgb", "hsv", tmp), 2);
    }

    /* read out TEXTURE_CHANNEL_MAX as a color */
    private void _readMaxChannel(filterregion fr; output color outColor)
    {
        color tmp = _read(fr);
        outColor = comp (ctransform ("rgb", "hsv", tmp), 2);
    }

    /* read out TEXTURE_CHANNEL_RGB as a color */
    private void _readRGB(filterregion fr; output color outColor)
    {
        outColor = _read(fr);
    }

    /* read out TEXTURE_CHANNEL_RGBA as a color and float */
    private void _readRGBA(filterregion fr;
                           output color outColor;
                           output float outAlpha)
    {
        outColor = _read(fr);
        outAlpha = _read(fr, TEXTURE_CHANNEL_A);
    }

    /* Update texture info members based on the File member */
    private void _updateInfo()
    {
        // get some information from the texture that we will need later
        uniform float dummy;
        if (File != "")
        {
            _exists = textureinfo(File, "exists", dummy);
            textureinfo(File, "channels", _numChannels);
            textureinfo(File, "type", _type);
        }
        else
        {
            _exists = 0;
            _numChannels = 0;
            _type = "unknown";
        }
        _isPtex = (_type == "ptexture") ? 1 : 0;
    }

    /* public methods */

    /* Initialize the texture object given the parameters:
     * file - Path to the texture/ptexture file
     * filter - texture filter
     * blur - blur amount
     * lerp - interpolate mip levels
     */
    public void Init(uniform string file, filter;
                     varying float blur; uniform float lerp)
    {
        File = file;
        Filter = filter;
        Blur = blur;
        Lerp = lerp;

        _updateInfo();
    }

    /* Read out a float value from the texture.
     * fr - filterregion where to access the texture
     * channel - single channel to access for multi-channel textures.
     * outFloat - return value
     */
    public void Read(filterregion fr; uniform float channel;
                     output float outFloat)
    {
        if (File != "") {
            if (channel < TEXTURE_CHANNEL_LUMA) {
                _readChannel(fr, channel, outFloat);
            } else if (channel == TEXTURE_CHANNEL_LUMA) {
                _readLuminance(fr, outFloat);
            } else { // (channel == TEXTURE_CHANNEL_MAX)
                _readMaxChannel(fr, outFloat);
            }
        } else if (channel == TEXTURE_CHANNEL_A) {
            outFloat = 0;
        }
    } 

    /* Read out a color value from the texture.
     * fr - filterregion where to access the texture
     * channel - single or multiple channels to access.
     * outColor - return value
     */
    public void Read(filterregion fr; uniform float channel;
                     output color outColor)
    {
        if (File != "") {
            if (channel < TEXTURE_CHANNEL_LUMA) {
                _readChannel(fr, channel, outColor);
            } else if (channel == TEXTURE_CHANNEL_LUMA) {
                _readLuminance(fr, outColor);
            } else if (channel == TEXTURE_CHANNEL_MAX) {
                _readMaxChannel(fr, outColor);
            } else { // (channel == TEXTURE_CHANNEL_RGB)
                _readRGB(fr, outColor);
            }
        } else if (channel == TEXTURE_CHANNEL_A) {
            outColor = color 0;
        }
    }

    /* Read out a color and alpha value from the texture.
     * fr - filterregion where to access the texture
     * channel - single or multiple channels to access for the color component
     * outColor - return value
     * outAlpha - return alpha value
     */
    public void Read(filterregion fr; uniform float channel;
                     output color outColor; output float outAlpha)
    {
        outColor = 0;
        outAlpha = 0;

        if (File != "") {
            if (channel < TEXTURE_CHANNEL_LUMA) {
                _readChannel(fr, channel, outColor);
            } else if (channel == TEXTURE_CHANNEL_LUMA) {
                _readLuminance(fr, outColor);
            } else if (channel == TEXTURE_CHANNEL_MAX) {
                _readMaxChannel(fr, outColor);
            } else if (channel == TEXTURE_CHANNEL_RGB) {
                _readRGB(fr, outColor);
            } else { // (channel == TEXTURE_CHANNEL_RGBA)
                _readRGBA(fr, outColor, outAlpha);
            }
        } else if (channel == TEXTURE_CHANNEL_A) {
            outColor = color 1;
        }
    }

    /* Read out a float, average, and alpha value from the texture.
     * fr - filterregion where to access the texture
     * channel - single channel to access
     * outFloat - return value
     * outAvg - average (filtered) outFloat
     * outAlpha - return alpha value
     */
    public void Read (
        filterregion    fr;
        uniform float   channel;

        output float    outFloat;
        output float    outAvg;
        output float    outAlpha;
    ) {
        outFloat = outAvg = 0;
        outAlpha = 0;

        if (File != "") {
            uniform float   x = .5, y = .5;

            if (channel <= TEXTURE_CHANNEL_A) {
                _readChannel (fr, channel, outFloat);
                outAvg = texture (File[channel], x, y, "blur", 1);
            }

            else if (channel == TEXTURE_CHANNEL_LUMA) {
                _readLuminance (fr, outFloat);
                varying color   tmp = texture (File, x, y, "blur", 1);
                outAvg = colorRGBToLuma(tmp);
            }

            else if (channel == TEXTURE_CHANNEL_MAX) {
                _readMaxChannel (fr, outFloat);
                varying color   tmp = texture (File, x, y, "blur", 1); // Not correct, but the best we can do.
                outAvg = max (comp(tmp,0), comp(tmp,1), comp(tmp,2));
            }

            else {
                print ("texture.h: ERROR: Read() method channel = ", channel, "not valid for float return value.");
                return;
            }
        }
    }

    /* Read out a color, average, and alpha value from the texture.
     * fr - filterregion where to access the texture
     * channel - single or multiple channels to access for the color component
     * outColor - return value
     * outAvg - average (filtered) outColor
     * outAlpha - return alpha value
     */
    public void Read (
        filterregion    fr;
        uniform float   channel;

        output color    outColor;
        output color    outAvg;
        output float    outAlpha;
    ) {
        outColor = outAvg = color 0;
        outAlpha = 0;

        if (File != "") {
            uniform float   x = .5, y = .5;

            if (channel == TEXTURE_CHANNEL_RGBA) {
                _readRGBA (fr, outColor, outAlpha);
                outAvg = texture (File, x, y, "blur", 1);
            }

            else if (channel == TEXTURE_CHANNEL_RGB) {
                _readRGB (fr, outColor);
                outAvg = texture (File, x, y, "blur", 1);
            }

            else if (channel == TEXTURE_CHANNEL_LUMA) {
                _readLuminance (fr, outColor);
                outAvg = texture (File, x, y, "blur", 1);
                outAvg = colorRGBToLuma(outAvg);
            }

            else if (channel == TEXTURE_CHANNEL_MAX) {
                _readMaxChannel (fr, outColor);
                outAvg = texture (File, x, y, "blur", 1); // Not correct, but the best we can do.
                outAvg = max (comp(outAvg,0), comp(outAvg,1), comp(outAvg,2));
            }

            else {
                _readChannel (fr, channel, outColor);
                outAvg = texture (File[channel], x, y, "blur", 1);
            }
        }
    }

    /* Read out a vector value from the texture.
     * fr - filterregion where to access the texture
     * space - space to transform the vector into
     * outVector - return value
     */
    public void Read(
        filterregion    fr;
        uniform string  space;
        output vector   outVector;
    ) {
        outVector = vector .5;

        if (File != "") {
            color outColor = _read(fr);
            outVector = vector(outColor);

            if (space != "") {
                outVector = vtransform(space, "current", outVector);
            }
        }
    }

    /* Read out a vector value from the texture.
     * fr - filterregion where to access the texture
     * outVector - return value
     */
    public void Read(
        filterregion    fr;
        output vector   outVector;
    ) {
        outVector = vector .5;

        if (File != "") {
            color outColor = _read(fr);
            outVector = vector(outColor);
        }
    }
}

#endif // TEXTURE_H

