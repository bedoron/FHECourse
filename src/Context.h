#pragma once
#include "FHE.h"
#include <vector>
#include "CtxtExt.h"

using std::vector;

class Context {
public:
    const FHESecKey& secKey; // nullable ?
    const FHEcontext& context;
    const long modulo;    

    Context(FHEcontext *context, FHESecKey *secKey);

    CtxtExt encrypt(const vector<long>& v);
    vector<long> decrypt(const CtxtExt& vec, unsigned int length=0);
};
