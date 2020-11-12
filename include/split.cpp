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
// A function to split a given string into an array of strings given a separator
// character. FIXME: This could probably be more robust - i.e. not return empty
// strings, etc.
//
#include "RslPlugin.h"

#include <cstring>
using namespace std;

extern "C" {

    void cleanup(RixContext* ctx, void* data)
    {
        delete[] reinterpret_cast<char*>(data);
    }

    RSLEXPORT int split (
        RslContext*     rslContext,
        int             argc,
        const RslArg*   argv[]
    ) {
        assert(argc == 3);

        assert(argv[0]->IsResizable());
        RslResizer* resultResizer = argv[0]->GetResizer();

        //RixMessages* msgs = (RixMessages*) rslContext->GetRixInterface(k_RixMessages);

        RslStringIter theStringIter(argv[1]);
        RslStringIter separatorIter(argv[2]);

        // sanity checks for arguments (uniform strings)
        assert(!theStringIter.IsVarying());
        assert(!separatorIter.IsVarying());

        char* theString = *theStringIter;
        int theStringLength = strlen(theString);

        if (strlen(*separatorIter) == 0 || theStringLength == 0) {
            return 0;
        }

        char separator = **separatorIter;

        // allocate / retrieve memory for the result. We'll just copy theString
        // and replace the separator character with \0 since the strings have
        // to be null terminated.
        RixStorage* storage = rslContext->GetThreadStorage();
        char* resultData = reinterpret_cast<char*>(storage->Get(theString));
        if (!resultData) {
            resultData = new char[theStringLength + 1];
            strcpy(resultData, theString);
            storage->Set(theString, resultData, cleanup);
        }

        // count the number instances of the separator to ensure we resize
        // the result appropriately. We always have at least one word
        // (we have already exited at this point if the string is empty).
        int count = 1;
        for (int i = 0; i < theStringLength; ++i) {
            if (separator == theString[i]) {
                ++count;
            }
        }

        resultResizer->Resize(count);

        // NOTE: array iter has to be created AFTER the array is resized
        RslStringArrayIter result(argv[0]);

        // now replace the separator characters and update the result
        char* lastWord = resultData;
        int wordIndex = 0;
        for (int i = 0; i < theStringLength; ++i) {
            if (separator == theString[i]) {
                resultData[i] = '\0';
                result[wordIndex] = lastWord;
                lastWord = &resultData[i+1];
                ++wordIndex;
            }
        }
        result[wordIndex] = lastWord;

        return 0;
    }

    static RslFunction  _splitFunctions[] = {
        { "uniform string[] split(uniform string theString, uniform string separator)",
          split, NULL, NULL, NULL, NULL },
        { NULL }
    };
    RSLEXPORT RslFunctionTable  RslPublicFunctions = _splitFunctions;

}
