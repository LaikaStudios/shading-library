/*
 *  Copyright 2020 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This file may not be copied, modified, or distributed except according to those terms.
 */
/*
 *  Annotates the rendered image with the supplied text.
 */
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "RixPredefinedStrings.hpp"
#include "RixDisplayFilter.h"
#include "RixIntegrator.h"

class Annotate : public RixDisplayFilter
{
  private:

    RixMessages*  rixMsg;

    // InstanceData Definition.
    struct textImage
    {
        // text upper left corner display pixel location.
        int  xpos;
        int  ypos;

        // 8 bit rendered text image.
        int  xsize;
        int  ysize;
        unsigned char*  alpha;
    };

    struct localInstanceData
    {
        static void Delete( RtPointer );
        RixChannelId  InputId;
        RixChannelId  OutputId;
        RixChannelId  AlphaId;
        RtColorRGB Color;
        textImage  Text;
    };

    void setInstanceData(
        RixContext &,
        const RtUString,
        const RixParameterList *,
        InstanceData *
    );

    // GenerateTextImage() declaration.
    void GenerateTextImage(
        const RixParameterList*,
        FT_Face,
        int,
        int,
        int,
        int,
        int,
        int,
        textImage*
    );

    // WriteTextPixel() declaration.
    void WriteTextPixel(
        const textImage*,
        const RtColorRGB*,
        const int,
        const int,
        const bool,
        RtColorRGB*,
        RtFloat*
    );

    enum ui_Id
    {
        ui_File = 0,
        ui_Color,
        ui_Height,

        ui_Text,
        ui_Top,
        ui_Left,

        ui_Input,
        ui_Output,
        ui_Alpha
    };

  public:

    Annotate() {}
    ~Annotate() {}

    const RixSCParamInfo* GetParamTable()
    {
        static RixSCParamInfo s_ptable[] =
        {
            // inputs.
            RixSCParamInfo( RtUString( "Font_File" ), k_RixSCString ),
            RixSCParamInfo( RtUString( "Font_Color" ), k_RixSCColor ),
            RixSCParamInfo( RtUString( "Font_Height" ), k_RixSCFloat ),

            RixSCParamInfo( RtUString( "Text" ), k_RixSCString ),
            RixSCParamInfo( RtUString( "Top" ),  k_RixSCFloat ),
            RixSCParamInfo( RtUString( "Left" ), k_RixSCFloat ),

            RixSCParamInfo( RtUString( "Input" ), k_RixSCString ),
            RixSCParamInfo( RtUString( "Output" ), k_RixSCString ),
            RixSCParamInfo( RtUString( "Alpha" ), k_RixSCInteger ),

            RixSCParamInfo( RtUString( "Notes" ), k_RixSCString ),
            RixSCParamInfo()
        };

        return &s_ptable[0];
    }

    // Create and Synchronize InstanceData definitions.
    void CreateInstanceData(
        RixContext&             Ctx,
        const RtUString         plugNodeName,
        const RixParameterList* pList,
        InstanceData*           instanceData
    ) {
        instanceData->synchronizeHints = RixShadingPlugin::SynchronizeHints::k_All;
    }

    void SynchronizeInstanceData(
        RixContext&             Ctx,
        const RtUString         plugNodeName,
        const RixParameterList* pList,
        uint32_t                editHints,
        InstanceData*           instanceData
    ) {
        setInstanceData( Ctx, plugNodeName, pList, instanceData );
        return;
    }

    // Get pointers to Rix interfaces.
    int Init( RixContext& Ctx, RtUString const plugPathName )
    {
        rixMsg = static_cast< RixMessages* >( Ctx.GetRixInterface( k_RixMessages ));
        return ( !rixMsg ) ? -1 : 0;
    }

    // Filter() declaration.
    void Filter(
        RixDisplayFilterContext &,
        RtPointer
    );

    // Parent class virtual methods, so must define them even though they're no-ops.
    void Finalize( RixContext& ) {}
    void Synchronize( RixContext&, RixSCSyncMsg, const RixParameterList* ) { }
};

/*
 *  GenerateTextImage definition.
 */
void Annotate::GenerateTextImage(
    const RixParameterList* pList,
    const FT_Face  face,
    const int  size,
    const int  xres,
    const int  yres,
    const int  ui_Text,
    const int  ui_Top,
    const int  ui_Left,
    textImage* TextImage
) {
    RtUString  Text( Rix::k_empty );
    pList->EvalParam( ui_Text, 0, &Text );

    // Short-circuit for an empty Text string.
    if( Text == Rix::k_empty )
    {
        TextImage->alpha = NULL;
        return;
    }

    // Get the x,y display position for the Text.
    RtFloat  Top( 0.0f );
    pList->EvalParam( ui_Top, 0, &Top );

    RtFloat  Left( 0.0f );
    pList->EvalParam( ui_Left, 0, &Left );

    TextImage->ypos = std::round( Top * yres );
    TextImage->xpos = std::round( Left * xres );

    // Convert Text to C string.
    const char* text = Text.CStr();
    const int   textLen = Text.Length();

    //
    //  Determine the text's alpha image dimensions based on the Text's characters.
    //
    // Set the glyph image transformation to zero.
    FT_Vector  pen;
    pen.x = 0;
    pen.y = 0;
    FT_Set_Transform( face, NULL, &pen );

    // Render the first character's glyph bitmap into the face->glyph "slot".
    FT_Load_Char( face, text[0], FT_LOAD_RENDER );
    const FT_GlyphSlot  slot = face->glyph;

    // Save its alpha image starting x offset.
    const int  xstart = -( slot->bitmap_left );

    // Save its character width (in 1/64 pixel units).
    FT_Pos  textWidth = slot->advance.x;

    // Highest pixel above the baseline (baseline=0, + is up).
    int  textTop = int( slot->bitmap_top );

    // Lowest pixel below the baseline, minus 1 (baseline=0. Would need to add 1 to get the actual pixel location).
    int  textBottom = int( slot->bitmap_top - slot->bitmap.rows );

    // Process the remaining characters.
    for( int c=1; c < textLen; ++c )
    {
        // Render the character's glyph bitmap into the face->glyph "slot".
        FT_Load_Char( face, text[c], FT_LOAD_RENDER );
        const FT_GlyphSlot  slot = face->glyph;

        // Accumulate the character width (in 1/64 pixel units).
        textWidth += slot->advance.x;

        // Highest pixel above the baseline (baseline=0, + is up).
        textTop = std::max( textTop, int( slot->bitmap_top ));

        // Lowest pixel below the baseline, minus 1 (baseline=0. Would need to add 1 to get the actual pixel location).
        textBottom = std::min( textBottom, int( slot->bitmap_top - slot->bitmap.rows ));
    }

    //
    //  Generate the Text string's bitmap alpha image.
    //
    const int  xsize = int( textWidth >> 6 ) + 1; // Convert to pixels.
    const int  ysize = textTop - textBottom; // Since we didn't add 1 when computing textBottom,
                                             // we don't need to add 1 to ysize.
    // Save the text image dimensions.
    TextImage->xsize = xsize;
    TextImage->ysize = ysize;

    // Allocate the text image memory.
    const int  alphaSize = xsize * ysize;
    TextImage->alpha = new unsigned char[ alphaSize ];

    // Convenience pointer.
    unsigned char*  alphaImage = TextImage->alpha;
    for( int i=0; i < alphaSize; ++i ) alphaImage[i] = 0; // Zero data.

    // Reset the pen position.
    // Start at the xstart offset of the first character. pen is in 1/64 pixel units.
    // We could also set this to 0, and add xstart to the alphaIndex (x), but this is more optimized.
    pen.x = xstart << 6;
    pen.y = 0;

    // For each character.
    for( int c=0; c < textLen; ++c )
    {
        // Set the glyph image transformation to the current pen location.
        FT_Set_Transform( face, NULL, &pen );

        // Render the character's glyph bitmap into the face->glyph "slot".
        FT_Load_Char( face, text[c], FT_LOAD_RENDER );
        const FT_GlyphSlot slot = face->glyph;

        // Left edge of this character.
        int  x = slot->bitmap_left;
        // Offset from the image top (y=0) to put this character's baseline at the image's baseline row.
        int  y = textTop - slot->bitmap_top;

        // Loop over this glpyh's bitmap and merge it into the text image.
        const FT_Bitmap  bitmap = slot->bitmap;
        for( int j=0; j < bitmap.rows; ++j ) for( int i=0; i < bitmap.width; ++i )
        {
            const int  alphaIndex = x+i + (y+j)*xsize;
            if( 0 <= alphaIndex && alphaIndex < alphaSize )
            {
                alphaImage[ alphaIndex ] = std::max( alphaImage[ alphaIndex ], bitmap.buffer[ i + j*bitmap.width ] );
            }
        }

        // Advance the pen position.
        pen.x += slot->advance.x;
    }
}

/*
 *  WriteTextPixel definition.
 */
void Annotate::WriteTextPixel(
    const textImage*  TextImage,
    const RtColorRGB* Color,
    const RtInt  x,
    const RtInt  y,
    const bool   useAlpha,
    RtColorRGB*  Input,
    RtFloat*     Alpha
) {
    if( TextImage->alpha )
    {
        const int  xsize = TextImage->xsize;
        const int  ysize = TextImage->ysize;
        const int  xpos  = TextImage->xpos;
        const int  ypos  = TextImage->ypos;

        // Is the x,y display pixel within the text image?
        if( xpos <= x && x < xpos+xsize && ypos <= y && y < ypos+ysize )
        {
            // Get the text image pixel's alpha value.
            const unsigned char*  alpha = TextImage->alpha;
            const float  a = float( alpha[ x-xpos + (y-ypos)*xsize ] )/255.0f;

            // Modify the display pixel.
            *Input = RixMix( *Input, *Color, a );
            if( useAlpha ) *Alpha = a + *Alpha*( 1.0f-a );
        }
    }
}

/*
 *  Delete and Set Instance Data.
 */
void Annotate::localInstanceData::Delete( RtPointer data )
{
    localInstanceData*  iData = static_cast< localInstanceData* >( data );

    if( iData->Text.alpha ){ delete[] iData->Text.alpha; }

    delete iData;
}

void Annotate::setInstanceData(
    RixContext&             Ctx,
    const RtUString         plugNodeName,
    const RixParameterList* pList,
    InstanceData*           instanceData
) {
    // If we've already created instanceData, free it first so we can synchronize with the latest settings.
    if( instanceData->data && instanceData->freefunc ) instanceData->freefunc( instanceData->data );

    // Init instanceData.
    instanceData->datalen = 0;
    instanceData->data = NULL;
    instanceData->freefunc = NULL;
    instanceData->paramtable = NULL;

    // Create new local instance data.
    localInstanceData*  iData = new localInstanceData;
    if( !iData ) return;

    // Get render state and integrator environment.
    RixRenderState* rixRS = static_cast< RixRenderState* >( Ctx.GetRixInterface( k_RixRenderState ));

    const RixIntegratorEnvironment* rixIE;
    {
        RixRenderState::FrameInfo  rixFI;
        rixRS->GetFrameInfo( &rixFI );

        rixIE = static_cast< const RixIntegratorEnvironment* >( rixFI.integratorEnv );
    }

    // Get image dimensions in pixels.
    RixRenderState::FrameInfo frameInfo;
    rixRS->GetFrameInfo( &frameInfo );

    const int  xres = frameInfo.displayState.resolution[0];
    const int  yres = frameInfo.displayState.resolution[1];

    //
    //  Get the display channel Id's.
    //
    RtUString  Input( Rix::k_Ci );
    pList->EvalParam( ui_Input, 0, &Input );

    RtUString  Output( Rix::k_Ci );
    pList->EvalParam( ui_Output, 0, &Output );

    RtInt  Alpha = 1;
    pList->EvalParam( ui_Alpha, 0, &Alpha );

    // Init Id's.
    RixChannelId  InputChannelId = k_InvalidChannelId;
    RixChannelId  OutputChannelId = k_InvalidChannelId;
    RixChannelId  AlphaChannelId = k_InvalidChannelId;

    // Find matching channels.
    for( int i=0; i < rixIE->numDisplays; ++i )
    {
        if( rixIE->displays[i].channel == Input ) InputChannelId = rixIE->displays[i].id;
        if( rixIE->displays[i].channel == Output ) OutputChannelId = rixIE->displays[i].id;
        if( Alpha == 1 && rixIE->displays[i].channel == Rix::k_a ) AlphaChannelId = rixIE->displays[i].id;
    }

    // Error check.
    if( InputChannelId == k_InvalidChannelId )
    {
        rixMsg->Warning( "The Input channel \"%s\" has not been declared.", Input);
        delete iData;
        return;
    }

    if( OutputChannelId == k_InvalidChannelId )
    {
        rixMsg->Warning( "The Output channel \"%s\" has not been declared.", Output);
        delete iData;
        return;
    }

    if( Alpha == 1 && AlphaChannelId == k_InvalidChannelId )
    {
        rixMsg->Warning( "The Alpha channel was not found." );
        delete iData;
        return;
    }

    // Set instanceData.
    iData->InputId = InputChannelId;
    iData->OutputId = OutputChannelId;
    iData->AlphaId = AlphaChannelId; // k_InvalidChannelId if not using alpha.

    //
    //  Get Font data and initialize the type face.
    //
    iData->Color = RtColorRGB( 1.0f, 0.7f, 0.0f );
    pList->EvalParam( ui_Color, 0, &iData->Color );

    // Determine text size in pixels.
    RtFloat  Height = 0.03f;
    pList->EvalParam( ui_Height, 0, &Height );

    const int  size = std::max( 8.0f, std::round( Height * yres ));

    // Get the desired font file.
    RtUString  File( Rix::k_empty );
    pList->EvalParam( ui_File, 0, &File );

    // Init FreeType library.
    FT_Library  library;
    FT_Init_FreeType( &library );

    // Get the type face (i.e. font) from the specified file.
    FT_Face  face;
    FT_Error err = FT_New_Face( library, File.CStr(), 0, &face );

rixMsg->Info( "err = %d", err );
    if( !err )
    {
        // Set the font's rendered glyph's em-square pixel height (and width).
        FT_Set_Pixel_Sizes( face, 0, size );

        // Generate the Text Image iData.
        Annotate::GenerateTextImage( pList, face, size, xres, yres, ui_Text, ui_Top, ui_Left, &( iData->Text ));

        // Done with FreeType face.
        FT_Done_Face( face );

        // Set instanceData to iData.
        instanceData->datalen = sizeof( *iData );
        instanceData->data = static_cast< void* >( iData );
        instanceData->freefunc = localInstanceData::Delete;
    }
    else
    {
rixMsg->Info( "Delete iData" );
        // Leave instanceData->data = NULL, etc.
        delete iData;
    }

    // Done with FreeType library.
rixMsg->Info( "Done with FreeType" );
    FT_Done_FreeType( library );

rixMsg->Info( "instanceData->data = %p", instanceData->data );
}


/*
 *  Filter definition.
 */
void Annotate::Filter(
    RixDisplayFilterContext&  fCtx,
    RtPointer                 instanceData
) {
    // Retrieve the non-connectable parameter values.
    const localInstanceData*  iData = static_cast< const localInstanceData* >( instanceData );

rixMsg->Info( "iData = %p", iData );

    // Nothing to do.
    if( !iData ) return;

    const bool  useAlpha = iData->AlphaId != k_InvalidChannelId;

    // Loop over the current bucket of display pixels.
    for( int y = fCtx.ymin; y < fCtx.ymax; ++y )
    for( int x = fCtx.xmin; x < fCtx.xmax; ++x )
    {
        RtColorRGB  Input;
        fCtx.Read( iData->InputId, x, y, Input );

        RtFloat  Alpha;
        if( useAlpha ) fCtx.Read( iData->AlphaId, x, y, Alpha );

        // Annotate::WriteTextPixel( &( iData->Text ), &( iData->Color ), x, y, useAlpha, &Input, &Alpha );

        fCtx.Write( iData->OutputId, x, y, Input );
        if( useAlpha ) fCtx.Write( iData->AlphaId, x, y, Alpha );
    }
}


/*
 *  These are the entrypoints to the plugin from the renderer.
 */
RIX_DISPLAYFILTERCREATE
{
    return new Annotate();
}

RIX_DISPLAYFILTERDESTROY
{
    delete static_cast< Annotate* >( filter );
}
