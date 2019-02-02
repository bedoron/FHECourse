#pragma once
#include "Ctxt.h"

class CtxtExt: public Ctxt {
public:
/**
 * Return 1 for non zeros, otherwise, 0 (number was )
 * */
    using Ctxt::Ctxt;

    CtxtExt& isNonZero();
};