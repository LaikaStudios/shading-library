/*
 *  Copyright(c) 2012 Laika.
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
//  A bit-wise "and" function.
//
#include "RslPlugin.h"

#include <stdlib.h>

using namespace std;

extern "C" {

    RSLEXPORT int ikaAndFloatFloat (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslFloatIter arg1   (argv[1]);
        RslFloatIter arg2   (argv[2]);
        RslFloatIter result (argv[0]);

        long    larg1, larg2;

        int numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            larg1 = (long)(*arg1);
            larg2 = (long)(*arg2);

            *result = (double)(larg1 & larg2);

            ++arg1;
            ++arg2;
            ++result;
        }

        return 0;
    }

    RSLEXPORT int ikaAndTripleFloat (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslPointIter arg1   (argv[1]);
        RslFloatIter arg2   (argv[2]);
        RslPointIter result (argv[0]);

        long    larg1, larg2;

        int  numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            larg2 = (long)(*arg2);

            for (int j=0; j < 3; ++j) {
                larg1 = long((*arg1)[j]);

                (*result)[j] = (double)(larg1 & larg2);
            }

            ++arg1;
            ++arg2;
            ++result;
        }

        return 0;
    }

    RSLEXPORT int ikaAndFloatTriple (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslFloatIter arg1   (argv[1]);
        RslPointIter arg2   (argv[2]);
        RslPointIter result (argv[0]);

        long    larg1, larg2;

        int  numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            larg1 = (long)(*arg1);

            for (int j=0; j < 3; ++j) {
                larg2 = long((*arg2)[j]);

                (*result)[j] = (double)(larg1 & larg2);
            }

            ++arg1;
            ++arg2;
            ++result;
        }

        return 0;
    }

    RSLEXPORT int ikaAndTripleTriple (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        RslPointIter arg1   (argv[1]);
        RslPointIter arg2   (argv[2]);
        RslPointIter result (argv[0]);

        long    larg1, larg2;

        int  numVals = RslArg::NumValues (argc, argv);
        for (int i=0; i < numVals; ++i) {
            for (int j=0; j < 3; ++j) {
                larg1 = long((*arg1)[j]);
                larg2 = long((*arg2)[j]);

                (*result)[j] = (double)(larg1 & larg2);
            }

            ++arg1;
            ++arg2;
            ++result;
        }

        return 0;
    }

    static RslFunction  andFunctions[] = {
        { "float  and (float,  float)", ikaAndFloatFloat,  NULL, NULL, NULL, NULL },
        { "color  and (color,  float)", ikaAndTripleFloat, NULL, NULL, NULL, NULL },
        { "point  and (point,  float)", ikaAndTripleFloat, NULL, NULL, NULL, NULL },
        { "vector and (vector, float)", ikaAndTripleFloat, NULL, NULL, NULL, NULL },
        { "normal and (normal, float)", ikaAndTripleFloat, NULL, NULL, NULL, NULL },
        { "color  and (float,  color)", ikaAndFloatTriple, NULL, NULL, NULL, NULL },
        { "point  and (float,  point)", ikaAndFloatTriple, NULL, NULL, NULL, NULL },
        { "vector and (float, vector)", ikaAndFloatTriple, NULL, NULL, NULL, NULL },
        { "normal and (float, normal)", ikaAndFloatTriple, NULL, NULL, NULL, NULL },
        { "color  and (color,  color)",  ikaAndTripleTriple, NULL, NULL, NULL, NULL },
        { "point  and (point,  point)",  ikaAndTripleTriple, NULL, NULL, NULL, NULL },
        { "vector and (vector, vector)", ikaAndTripleTriple, NULL, NULL, NULL, NULL },
        { "normal and (normal, normal)", ikaAndTripleTriple, NULL, NULL, NULL, NULL },
        { NULL }
    };
    RSLEXPORT RslFunctionTable  RslPublicFunctions = andFunctions;

}
