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
#ifndef surfaceSampleV7_h
#define surfaceSampleV7_h

#define surfaceSample_s surfaceSampleV7_s 
struct  surfaceSampleV7_s
{
    /*
     *  Surface Sample variables.
     */
    private varying float   Importance = 1; // Percentage (0-1) value representing how important this sample is.
                                            // Also for use by lights and blockers to determine if they need to run.

    private varying point   P = point 0;    // Surface response lobe apex, un-biased, in current space.
    private varying vector  S = vector 0;   // Surface response lobe (normalized) direction in current space.

    private varying float   Cone = 0;       // Surface response lobe coneangle ( = half the apex angle) in radians.

    // Optional cone specification allowing anisotropic cone shape.
    private varying float   minCone = 0;        // min and max Cone are the coneangles in radians.
    private varying float   maxCone = 0;
    private varying vector  minS = vector 0;    // min and max S are the (normalized) current space directions.
    private varying vector  maxS = vector 0;    // Orthogonal to S, but not necessarily each other.


    /*
     *  Data and methods for computing a filterregion of the sample.
     *  XXX ***Not yet implemented*** Always produces a mirror reflection.
     */
    private filterregion    Fr;
    private float           Fr_Computed = 0;

    public void ResetFilterregion() { Fr_Computed = 0; }

    private void Compute_Fr() {
        if (minCone*maxCone == 0) {
            Fr->calculate3d(S);
        }
        else {
            Fr->calculate3d(S);
        }

        Fr_Computed = 1;
    }

    private void Init_Fr() {
        if (Fr_Computed == 0)
            Compute_Fr();
    }

    public filterregion GetFilterregion() {      
        Init_Fr();
        return Fr;
    }

    /*
     * Public access methods
     */
    public void SetP (varying point sp) {
        P = sp;
    }

    public void SetS (varying vector ss) {
        S = ss;
        Fr_Computed = 0;
    }

    public void SetCone (varying float scone) {
        Cone = scone;
        Fr_Computed = 0;
    }

    public void SetCone (varying float mincone, maxcone; varying vector mins, maxs) {
        minCone = mincone;
        maxCone = maxcone;
        minS = mins;
        maxS = maxs;
        Fr_Computed = 0;
    }

    public void SetImportance (varying float importance) {
        Importance = importance;
    }

    public void Set (
        varying point   sp;
        varying vector  ss;
        varying float   scone;
    ) {
        P = sp;
        S = ss;
        Cone = scone;
        Fr_Computed = 0;
    }

    public void Set (
        varying point   sp;
        varying vector  ss;
        varying float   scone;
        varying float   importance;
    ) {
        Importance = importance;
        P = sp;
        S = ss;
        Cone = scone;
        Fr_Computed = 0;
    }

    public void Set (
        varying point   sp;
        varying vector  ss;
        varying float   mincone, maxcone;
        varying vector  mins, maxs;
    ) {
        P = sp;
        S = ss;
        minCone = mincone;
        maxCone = maxcone;
        minS = mins;
        maxS = maxs;
        Fr_Computed = 0;
    }

    public void Set (
        varying point   sp;
        varying vector  ss;
        varying float   mincone, maxcone;
        varying vector  mins, maxs;
        varying float   importance;
    ) {
        Importance = importance;
        P = sp;
        S = ss;
        minCone = mincone;
        maxCone = maxcone;
        minS = mins;
        maxS = maxs;
        Fr_Computed = 0;
    }

    public point  GetP() { return P; }
    public vector GetS() { return S; }
    public float  GetCone() { return Cone; }
    public float  GetMinCone() { return minCone; }
    public float  GetMaxCone() { return maxCone; }
    public vector GetMinS() { return minS; }
    public vector GetMinS() { return maxS; }
}

#endif
