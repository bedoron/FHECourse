#include "Context.h"
#include "EncryptedArray.h"
#include <iostream>

using std::cout;
using std::endl;

Context::Context(FHEcontext *context, FHESecKey *secKey): 
    context(*context), secKey(*secKey), modulo(context->alMod.getPPowR()) {

}

CtxtExt Context::encrypt(const vector<long>& v) {
    const FHEPubKey& pubKey = secKey;
    cout << "Creating an encrypted array..." << endl;
    EncryptedArray ea(pubKey.getContext());
    
    cout << "Duplicating original vector..." << endl;
    vector<long> tmp(v);
    tmp.resize(ea.size(), 0);

    cout << "Creating a ciphertext..." << endl;
    CtxtExt res(pubKey);

    cout << "Encrypting temporary vector" << endl;
    ea.encrypt(res, pubKey, tmp);
    return res;
}
