#include "CtxtExt.h"
#include <iostream>

using std::cout;
using std::endl;

CtxtExt& CtxtExt::isNonZero() {
    if (this->effectiveR() != 1) {
        throw "Unsupported R parameter, please use 1.";
    }
    
    long p = this->getPtxtSpace();
    
    CtxtExt *result = NULL;
    for (long i = p/2; i > 0; i--) {
        cout << i << endl;
        CtxtExt tmp(*this);
        cout << i << " - Adding" << endl;
        tmp.addConstant(ZZ(i));

        if (result == NULL) {
            cout << i << " - Initializing" << endl;
            result = new CtxtExt(tmp);
            continue;
        }

        cout << i << " - Multiplying" << endl;
        result->multiplyBy(tmp);
    }

    cout << "Finished, power of " << p - 1 << endl;
    result->power(p - 1);
    cout << "Finished..." << endl;
    return *result;
}