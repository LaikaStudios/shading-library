/*
 *  Copyright(c) 2012 Laika, developed under contract by mitchpraterLLC.
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
//  A pow() function that produces continuous results for any real value base and exponent.
//  Allows float and triple arguments in any combination.
//
#include "RslPlugin.h"

#include <complex>

using namespace std;

extern "C" {

    RSLEXPORT int ikaPowFloatFloat (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslFloatIter base   (argv[1]);
        RslFloatIter expon  (argv[2]);
        RslFloatIter result (argv[0]);

        complex<double> cbase, cexpon, cresult;

        int numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            cbase = *base;
            cexpon = *expon;

            cresult = pow (cbase,cexpon);
            *result = real (cresult);

            if (isinf(*result) || isnan(*result))
                *result = 1.0;

            else if (!(*expon >= 0.0 || *base >= 0.0))
                    *result = abs(*result);

            ++base;
            ++expon;
            ++result;
        }

        return 0;
    }

    RSLEXPORT int ikaPowTripleFloat (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslPointIter base   (argv[1]);
        RslFloatIter expon  (argv[2]);
        RslPointIter result (argv[0]);

        complex<double>  cbase, cexpon, cresult;

        int  numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            cexpon = *expon;

            for (int j=0; j < 3; ++j) {
                cbase = (*base)[j];

                cresult = pow (cbase,cexpon);
                (*result)[j] = real (cresult);

                if (isinf((*result)[j]) || isnan((*result)[j]))
                    (*result)[j] = 1.0;

                else if (!(*expon >= 0.0 || (*base)[j] >= 0.0))
                        (*result)[j] = abs((*result)[j]);
            }

            ++base;
            ++expon;
            ++result;
        }

        return 0;
    }

    RSLEXPORT int ikaPowFloatTriple (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslFloatIter base   (argv[1]);
        RslPointIter expon  (argv[2]);
        RslPointIter result (argv[0]);

        complex<double>  cbase, cexpon, cresult;

        int  numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            cbase = *base;

            for (int j=0; j < 3; ++j) {
                cexpon = (*expon)[j];

                cresult = pow (cbase,cexpon);
                (*result)[j] = real (cresult);

                if (isinf((*result)[j]) || isnan((*result)[j]))
                    (*result)[j] = 1.0;

                else if (!((*expon)[j] >= 0.0 || *base >= 0.0))
                        (*result)[j] = abs((*result)[j]);
            }

            ++base;
            ++expon;
            ++result;
        }

        return 0;
    }

    RSLEXPORT int ikaPowTripleTriple (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslPointIter base   (argv[1]);
        RslPointIter expon  (argv[2]);
        RslPointIter result (argv[0]);

        complex<double>  cbase, cexpon, cresult;

        int  numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            for (int j=0; j < 3; ++j) {
                cbase = (*base)[j];
                cexpon = (*expon)[j];

                cresult = pow (cbase,cexpon);
                (*result)[j] = real (cresult);

                if (isinf((*result)[j]) || isnan((*result)[j]))
                    (*result)[j] = 1.0;

                else if (!((*expon)[j] >= 0.0 || (*base)[j] >= 0.0))
                        (*result)[j] = abs((*result)[j]);
            }

            ++base;
            ++expon;
            ++result;
        }

        return 0;
    }

    static RslFunction  _powFunctions[] = {
        { "float  _pow (float,  float)", ikaPowFloatFloat,  NULL, NULL, NULL, NULL },
        { "color  _pow (color,  float)", ikaPowTripleFloat, NULL, NULL, NULL, NULL },
        { "point  _pow (point,  float)", ikaPowTripleFloat, NULL, NULL, NULL, NULL },
        { "vector _pow (vector, float)", ikaPowTripleFloat, NULL, NULL, NULL, NULL },
        { "normal _pow (normal, float)", ikaPowTripleFloat, NULL, NULL, NULL, NULL },
        { "color  _pow (float,  color)", ikaPowFloatTriple, NULL, NULL, NULL, NULL },
        { "point  _pow (float,  point)", ikaPowFloatTriple, NULL, NULL, NULL, NULL },
        { "vector _pow (float, vector)", ikaPowFloatTriple, NULL, NULL, NULL, NULL },
        { "normal _pow (float, normal)", ikaPowFloatTriple, NULL, NULL, NULL, NULL },
        { "color  _pow (color,  color)",  ikaPowTripleTriple, NULL, NULL, NULL, NULL },
        { "point  _pow (point,  point)",  ikaPowTripleTriple, NULL, NULL, NULL, NULL },
        { "vector _pow (vector, vector)", ikaPowTripleTriple, NULL, NULL, NULL, NULL },
        { "normal _pow (normal, normal)", ikaPowTripleTriple, NULL, NULL, NULL, NULL },
        { NULL }
    };
    RSLEXPORT RslFunctionTable  RslPublicFunctions = _powFunctions;

}
