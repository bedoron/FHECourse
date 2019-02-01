#include "ContextBuilder.h"

Context ContextBuilder::build(const UserParams &up) {
    Context ctx;
    long m=0; // Native plaintext space
    // Computations will be 'modulo p'
    long c=3;           // Columns in key switching matrix
    long w=64;          // Hamming weight of secret key
    long d=0;
    m = FindM(up.k, up.L, up.c, up.p, up.d, 0, 0);
    
    ctx.context = new FHEcontext(m, up.p, up.r);
    buildModChain(*(ctx.context), up.L, c);

    ctx.secKey = new FHESecKey(*(ctx.context));
    ctx.secKey->GenSecKey(w);

    addSome1DMatrices(*(ctx.secKey));
    return ctx;
};
