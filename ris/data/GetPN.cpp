/*
 *  Copyright 2016-2018 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
/*
 *  Returns the original (undisplaced) and displaced P and N values.
 */
#include "RixShading.h"
#include "RixPattern.h"

class GetPN : public RixPattern
{
  private:
    enum out_Ids
    {
        out_Po = 0,
        out_No,
        out_P,
        out_N,
        numOutputs
    };

  public:
    GetPN() {}
    ~GetPN() {}

    // Parent class virtual methods, so must define them even though they're no-ops.
    int  Init( RixContext &, RtUString const ) { return 0; }
    void Finalize( RixContext & ) { return; }
    void Synchronize( RixContext&, RixSCSyncMsg, const RixParameterList* ) { return; }
    int  CreateInstanceData( RixContext&, const RtUString, const RixParameterList*, InstanceData* ) { return 0; }
    bool Bake2dOutput( const RixBakeContext*, Bake2dSpec&, RtPointer ) { return false; }
    bool Bake3dOutput( const RixBakeContext*, Bake3dSpec&, RtPointer ) { return false; }

    const RixSCParamInfo* GetParamTable()
    {
        static RixSCParamInfo s_ptable[] =
        {

            RixSCParamInfo( RtUString( "Po" ), k_RixSCPoint, k_RixSCOutput ),
            RixSCParamInfo( RtUString( "No" ), k_RixSCNormal, k_RixSCOutput ),
            RixSCParamInfo( RtUString( "P" ), k_RixSCPoint, k_RixSCOutput ),
            RixSCParamInfo( RtUString( "N" ), k_RixSCNormal, k_RixSCOutput ),

            RixSCParamInfo( RtUString( "Notes" ), k_RixSCString ),

            RixSCParamInfo()
        };

        return &s_ptable[0];
    }

    int ComputeOutputParams(
        const RixShadingContext* sCtx,
        RtInt*                   noutputs,
        OutputSpec**             outputs,
        RtPointer                instanceData, // unused.
        const RixSCParamInfo*    paramInfo // unused.
    ) {
        int numPts = sCtx->numPts;

        // Allocate an OutputSpec for each output parameter.
        RixShadingContext::Allocator pool( sCtx );
        OutputSpec  *out = pool.AllocForPattern< OutputSpec >( numOutputs );

        *noutputs = numOutputs;
        *outputs = out;

        // Copy builtin values to their OutputSpec as needed.
        RixSCType             type;
        RixSCConnectionInfo   cinfo;

        // Po
        out[out_Po].paramId = out_Po;
        out[out_Po].detail = k_RixSCInvalidDetail;
        out[out_Po].value = NULL;

        sCtx->GetParamInfo( out_Po, &type, &cinfo );

        if( cinfo == k_RixSCNetworkValue )
        {
            const RtPoint3* ptrPo;
            sCtx->GetBuiltinVar( RixShadingContext::k_Po, &ptrPo );

            out[out_Po].detail = k_RixSCVarying;
            out[out_Po].value = pool.AllocForPattern< RtPoint3 >( numPts );

            for( int p=0; p < numPts; ++p ) ( (RtPoint3*) out[out_Po].value )[p] = ptrPo[p];
        }

        // No
        out[out_No].paramId = out_No;
        out[out_No].detail = k_RixSCInvalidDetail;
        out[out_No].value = NULL;

        sCtx->GetParamInfo( out_No, &type, &cinfo );

        if( cinfo == k_RixSCNetworkValue )
        {
            const RtNormal3* ptrNo;
            sCtx->GetBuiltinVar( RixShadingContext::k_Non, &ptrNo );

            out[out_No].detail = k_RixSCVarying;
            out[out_No].value = pool.AllocForPattern< RtNormal3 >( numPts );

            for( int p=0; p < numPts; ++p ) ( (RtNormal3*) out[out_No].value )[p] = ptrNo[p];
        }

        // P
        out[out_P].paramId = out_P;
        out[out_P].detail = k_RixSCInvalidDetail;
        out[out_P].value = NULL;

        sCtx->GetParamInfo( out_P, &type, &cinfo );

        if( cinfo == k_RixSCNetworkValue )
        {
            const RtPoint3* ptrP;
            sCtx->GetBuiltinVar( RixShadingContext::k_P, &ptrP );

            out[out_P].detail = k_RixSCVarying;
            out[out_P].value = pool.AllocForPattern< RtPoint3 >( numPts );

            for( int p=0; p < numPts; ++p ) ( (RtPoint3*) out[out_P].value )[p] = ptrP[p];
        }

        // N
        out[out_N].paramId = out_N;
        out[out_N].detail = k_RixSCInvalidDetail;
        out[out_N].value = NULL;

        sCtx->GetParamInfo( out_N, &type, &cinfo );

        if( cinfo == k_RixSCNetworkValue )
        {
            const RtNormal3* ptrN;
            sCtx->GetBuiltinVar( RixShadingContext::k_Nn, &ptrN );

            out[out_N].detail = k_RixSCVarying;
            out[out_N].value = pool.AllocForPattern< RtNormal3 >( numPts );

            for( int p=0; p < numPts; ++p ) ( (RtNormal3*) out[out_N].value )[p] = ptrN[p];
        }

        return 0;
    }
};


RIX_PATTERNCREATE
{
    return new GetPN();
}

RIX_PATTERNDESTROY
{
    delete static_cast< GetPN* >( pattern );
}
