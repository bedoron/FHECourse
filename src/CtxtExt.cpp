#include "CtxtExt.h"

CtxtExt CtxtExt::isNonZero() {
    if (this->effectiveR() != 1) {
        throw "Unsupported R parameter, please use 1.";
    }
    
    long p = this->getPtxtSpace();
    
    CtxtExt *result = NULL;
    for (long i = p/2; i > 0; i--) {
        CtxtExt tmp(*this);
        tmp.addConstant(ZZ(i));

        if (result == NULL) {
            result = &tmp;
            continue;
        }

        result->multiplyBy(tmp);
    }

    result->power(p - 1);
    return *result;
}