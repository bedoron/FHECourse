#include "Context.h"
#include "EncryptedArray.h"

Context::Context(FHEcontext *context, FHESecKey *secKey): 
    context(*context), secKey(*secKey), modulo(context->alMod.getPPowR()) {

}

CtxtExt Context::encrypt(const vector<long>& v) {
    const FHEPubKey& pubKey = secKey;
    EncryptedArray ea(pubKey.getContext());
    
    vector<long> tmp(v);
    tmp.resize(ea.size(), 0);
    CtxtExt res(pubKey);
    ea.encrypt(res, pubKey, tmp);
    return res;
}
