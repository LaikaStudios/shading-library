/*
 *  Copyright(c) 2010-2011 Laika, developed under contract by mitchpraterLLC.
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
#ifndef manifold_h
#define manifold_h

struct manifold_s {

    /* (mostly) private variables */
    private varying point   Q = point 0;
    private varying vector  dQu = vector 0;
    private varying vector  dQv = vector 0;
    private varying float   dQ = 0;             // uPolygon area.
    filterregion    Qfr;
    private varying float   dQfr = 0;           // uPolygon area.
    uniform string  Space = "current";

    private uniform float   dQ_Computed = 0;
    private uniform float   Qfr_Computed = 0;
    private uniform float   dQfr_Computed = 0;


    /* (mostly) private methods */
    private float GetFilterwidth (point q) { return sqrt(area(q)); }
    private float GetFilterwidth (vector dqu, dqv) { return sqrt(length(dqu^dqv)); }
    private float GetFilterwidth (filterregion fr) { return sqrt(fr->minsize() * fr->maxsize()); }

    private void Compute_Qfr()
    {
        Qfr->calculate3d(Q);
        Qfr_Computed = 1;
    }

    private void Init_Qfr()
    {
        if (Qfr_Computed == 0) {
            Compute_Qfr();
        }
    }

    public filterregion GetQfr() { /* public */
        Init_Qfr();
        return Qfr;
    }

    private void Compute_dQfr()
    {
        Init_Qfr();
        dQfr = GetFilterwidth(Qfr);
        dQfr_Computed = 1;
    }

    private void Init_dQfr()
    {
        if (dQfr_Computed == 0) {
            Compute_dQfr();
        }
    }

    public float GetdQfr() { /* public */
        Init_dQfr();
        return dQfr;
    }

    private void Compute_dQ()
    {
        extern float    VolumeField;

#if SLIM_SHADERTYPEID != SLIM_TYPEID_volume
        if (VolumeField == 0) {
            dQu = Du(Q)*du;
            dQv = Dv(Q)*dv;
            dQ = GetFilterwidth(Q);
        }
        else {
            dQu = vector 0;
            dQv = vector 0;
            dQ = GetdQfr();
        }
#else
        dQu = vector 0;
        dQv = vector 0;
        dQ = GetdQfr();
#endif
        dQ_Computed = 1;
    }

    private void Init_dQ()
    {
        if (dQ_Computed == 0) {
            Compute_dQ();
        }
    }


    /* public methods */
    public void Set (point q; vector dqu; vector dqv) {
        Q = q;
        dQu = dqu;
        dQv = dqv;
        // Be sure to use the supplied dq values since there could be a discontinuity in q.
        dQ = GetFilterwidth(dqu,dqv); 
        dQ_Computed = 1;
        Qfr_Computed = 0;
        dQfr_Computed = 0;
    }

    public void Set (point q; vector dqu; vector dqv; string space) {
        Q = q;
        dQu = dqu;
        dQv = dqv;
        Space = space;
        // Be sure to use the supplied dq values since there could be a discontinuity in q.
        dQ = GetFilterwidth(dqu,dqv); 
        dQ_Computed = 1;
        Qfr_Computed = 0;
        dQfr_Computed = 0;
    }

    public void Set (point q) {
        Q = q;
        dQ_Computed = 0;
        Qfr_Computed = 0;
        dQfr_Computed = 0;
    }

    public void Set (point q; string space) {
        Q = q;
        Space = space;
        dQ_Computed = 0;
        Qfr_Computed = 0;
        dQfr_Computed = 0;
    }

    public point GetQ() {
        return Q;
    }

    public vector GetdQu() {
        Init_dQ();
        return dQu;
    }

    public vector GetdQv() {
        Init_dQ();
        return dQv;
    }

    public float GetdQ() {
        Init_dQ();
        return dQ;
    }

    public float GetdQfrMin() {
        Init_Qfr();
        return Qfr->minsize();
    }

    public float GetdQfrMax() {
        Init_Qfr();
        return Qfr->maxsize();
    }

    public void SetSpace (string space) {
        Space = space;
    }

    public string GetSpace() {
        return Space;
    }

}

#endif
