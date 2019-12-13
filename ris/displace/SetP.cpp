/*
 *  Copyright 2016-2019 Laika, LLC. Authored by Mitch Prater.
 *
 *  Licensed under the Apache License Version 2.0 http://apache.org/licenses/LICENSE-2.0,
 *  or the MIT license http://opensource.org/licenses/MIT, at your option.
 *
 *  This program may not be copied, modified, or distributed except according to those terms.
 */
/*
 *  P = mix( P, Q, Enable ). That's it.
 */
#include "RixDisplacement.h"
#include "RixShadingUtils.h"

/*
 *  RixDisplacementFactory class declaration.
 */
class SetPFactory : public RixDisplacementFactory
{
  private:

    enum ui_Id
    {
        ui_Enable,
        ui_Q,
        ui_numParams // The number of shader parameters.
    };

    // Default shader parameter values.
    // Can't initialize these here without c++11...
    RtFloat   default_Enable;
    RtPoint3  default_Q;

  public:
    // ...so the class constructor sets the default values.
    // Note: these values will be used when the shader UI does not explicitly set
    // the parameter's value - the default value in the .args file, while displayed
    // by the UI, will not.
    SetPFactory()
    {
        default_Enable = RtFloat( 1.0f );
        default_Q = RtPoint3( 0.0f );
    }
    ~SetPFactory() {}


    // GetParamTable() method.
    const RixSCParamInfo* GetParamTable()
    {
        static RixSCParamInfo s_ptable[] =
        {
            RixSCParamInfo( RtUString( "Enable" ), k_RixSCFloat ),
            RixSCParamInfo( RtUString( "Q" ), k_RixSCPoint ),

            // End of the table.
            RixSCParamInfo()
        };

        return &s_ptable[0];
    }

    // BeginDisplacement() declaration.
    RixDisplacement* BeginDisplacement(
        const RixShadingContext *,
        RixSCShadingMode,
        RtPointer
    );
    void EndDisplacement( RixDisplacement* ) {}

    // Parent class virtual methods, so must define them even though they're no-ops.
    int  Init( RixContext &, RtUString const ) { return 0; }
    void Finalize( RixContext & ) {}
    void Synchronize( RixContext&, RixSCSyncMsg, const RixParameterList* ) {}
#if _PRMANAPI_VERSION_ > 22
    void CreateInstanceData( RixContext&, const RtUString, const RixParameterList*, InstanceData* ) {}
#else
    int  CreateInstanceData( RixContext&, const RtUString, const RixParameterList*, InstanceData* ) { return 0; }
#endif
};


/*
 *  RixDisplacement class declaration.
 */
class SetPDisp : public RixDisplacement
{
    private:
        // Shader-specific parameter(s).
        const RixSCDetail         m_detail_Enable;
        const RtFloat*            m_Enable;
        const RixSCConnectionInfo m_cinfo_Q;
        const RtPoint3*           m_Q;

    public:
        // Class constructor.
        // An instance of this class is created in BeginDisplacement().
        // The constructor's purpose is to set its class method variables
        // for use in its GetDisplacement() method.
        SetPDisp(
            const RixShadingContext*  sCtx,
            RixDisplacementFactory*   dFac,

            const RixSCDetail         detail_Enable,
            const RtFloat*            Enable,
            const RixSCConnectionInfo cinfo_Q,
            const RtPoint3*           Q
        ) :
            // Sets the protected RixDisplacement class variables
            // 'shadingCtx' to sCtx, and 'displacementFactory' to dFac.
            RixDisplacement( sCtx, dFac ),

            // Sets user parameter variables.
            m_detail_Enable( detail_Enable ),
            m_Enable( Enable ),
            m_cinfo_Q( cinfo_Q ),
            m_Q( Q )
        { }

        // The class destructor is a no-op.
        ~SetPDisp() {}


        // RixDisplacement::GetDisplacement() method.
        // Called by the renderer to affect P.
        // XXX What is the value of P: P or Po?
        bool GetDisplacement( RtPoint3* P )
        {
            // Leave P alone if Q is not connected.
            if( m_cinfo_Q != k_RixSCNetworkValue ) return false;

            if( m_detail_Enable == k_RixSCUniform )
            {
                if( m_Enable[0] == 0.0f ) return false;

                for( int i=0; i < shadingCtx->numPts; ++i ) P[i] = RixMix( P[i], m_Q[i], m_Enable[0] );
            }
            else
            {
                for( int i=0; i < shadingCtx->numPts; ++i ) P[i] = RixMix( P[i], m_Q[i], m_Enable[i] );
            }

            return true;
        }
};


/*
 *  RixDisplacementFactory::BeginDisplacement()
 *
 *  The BeginDisplacement method returns an instance of this shader's RixDisplacement class.
 *  It also computes any class variables needed by the RixDisplacement class and passes them
 *  to its constructor. Such variables typically come from user parameters that are used to
 *  control the shader's functionality.
 */
RixDisplacement* SetPFactory::BeginDisplacement(
    const RixShadingContext* sCtx,        // See RixShading.h line 457.
    RixSCShadingMode         shadingMode, // See RixShading.h line 333.
    RtPointer                instanceData // Can be used to hold data in certain circumstances.
                                          // XXX See RixShading.h line 170?
) {
    const bool  promoteToVarying = true;
    const bool  leaveDetailAlone = false;
    const int   nonArray = -1; // The array parameter index. -1 signifies a non-array parameter.

    // SetPDisp() parameters.
    RixSCDetail         detail_Enable;
    const RtFloat*      Enable;
    RixSCConnectionInfo cinfo_Q;
    const RtPoint3*     Q = NULL;

    // Get the parameter data.
    detail_Enable = sCtx->EvalParam( ui_Enable, nonArray, &Enable, &default_Enable, leaveDetailAlone );
    assert( detail_Enable == k_RixSCUniform || detail_Enable == k_RixSCVarying );

    RixSCType   type = k_RixSCPoint;
    sCtx->GetParamInfo( ui_Q, &type, &cinfo_Q );

    if( detail_Enable != k_RixSCUniform || Enable[0] != 0.0f )
    {
        sCtx->EvalParam( ui_Q, nonArray, &Q, &default_Q, promoteToVarying );
    }
    // XXX else what? Leaves Q = NULL.

    // Allocate memory for this shader's RixDisplacement object in the
    // renderer's Displacement (shares Bxdf) shading context memory pool.
    RixShadingContext::Allocator    pool( sCtx );
    void*   mem = pool.AllocForBxdf< SetPDisp >(1);

    // Create an instance of this shader's RixDisplacement object
    // and pass its parameter data to it.
    SetPDisp* disp = new (mem) SetPDisp( sCtx, this,
                                    detail_Enable,
                                    Enable,
                                    cinfo_Q,
                                    Q
                                    );

    return disp;
}


/*
 *  These are the entrypoints to the plugin from the renderer.
 */
extern "C" PRMANEXPORT RixDisplacementFactory* CreateRixDisplacementFactory( RtConstString hint )
{
    return new SetPFactory();
}

extern "C" PRMANEXPORT void DestroyRixDisplacementFactory( RixDisplacementFactory* dFac )
{
    delete static_cast< SetPFactory* >( dFac );
}
