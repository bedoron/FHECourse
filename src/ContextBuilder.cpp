#include "ContextBuilder.h"
#include <iostream>

using std::cout;
using std::endl;

Context ContextBuilder::build(const UserParams &up) {
    long m=0; // Native plaintext space
    // Computations will be 'modulo p'
    long c=3;           // Columns in key switching matrix
    long w=64;          // Hamming weight of secret key
    long d=0;

    cout << "Looking for an m parameter..." << endl;
    m = FindM(up.k, up.L, up.c, up.p, up.d, 0, 0);
    
    FHEcontext* context = new FHEcontext(m, up.p, up.r);
    
    cout << "Building mod chain..." << endl;
    buildModChain(*context, up.L, c);

    FHESecKey* secKey = new FHESecKey(*context);
    
    cout << "Generating secure key..." << endl;
    secKey->GenSecKey(w);

    addSome1DMatrices(*secKey);

    Context ctx(context, secKey);
    return ctx; // Copy Ctor ?!
};
