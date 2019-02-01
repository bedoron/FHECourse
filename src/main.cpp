#include <NTL/ZZ.h>
#include <NTL/BasicThreadPool.h>
#include "FHE.h"
#include "timing.h"
#include "EncryptedArray.h"
#include <NTL/lzz_pXFactoring.h>

#include <cassert>
#include <cstdio>

#include <iostream>
#include "CtxtExt.h"
#include "UserParams.h"

void parseArgs(int argc, const  char* argv[]);

int main(int argc, char *argv[]) {
    UserParams up;
    up.parse(argc, argv)

    long p = up.p, r = up.r, security = up.k, l;
    short strategy;
    unsigned int size;
    
    // variables_map vm = applyArgumentParser(argc, argv, argumentsParser(&p, &r, &l, &security, &size, &strategy));
    // printParameters(vm);
    // ICtxtReducerFactory *reducerFactory = createCtxtReducerFactory(vm);
    
    cout << "Generating keys..." << endl;
    Keys keys(security, p, r, l);
    CtxtClient client(*keys.secKey, reducerFactory);
    VectorIO io;
    
    cout << "All computations are modulu " << keys.getModulu() << endl;
    
    cout << "Generate random vector" << endl;
    // Generate random matrix
    Vector v = io.createRandom(size, 0, 0, (unsigned int)keys.getModulu()/4);
    transform(v, [p](long num) { return adjustNumber(num - (p/8), p); });
    printVector(v);
    
#ifdef DEBUG
    function<void(const Ctxt&)> debug = [&client, size, p](const Ctxt& c) {
        Vector tmp = client.decrypt(c, size);
        transform(tmp, [p](long num) { return adjustNumber(num, p); });
        printVector(tmp);
    };
#endif
    
    // Encrypt
    cout << "Encrypt the vector" << endl;
    Ctxt c = client.encrypt(v);
    
    // comparing
    cout << "calculate v < 3" << endl;
    Ctxt less3c = client.lessThan(c, 3
#ifdef DEBUG
                                  , debug
#endif
                                  );
    
    // comparing
    cout << "calculate v > 3" << endl;
    Ctxt graeter3c = client.greaterThan(c, 3
#ifdef DEBUG
                                        , debug
#endif
                                        );
    
    cout << "calculate between -3 to 3" << endl;
    Ctxt betweenC = client.between(c, -3, 3);
    
    // Decrypt
    cout << "Decrypt the results" << endl;
    Vector less3 = client.decrypt(less3c, size), graeter3 = client.decrypt(graeter3c, size), between = client.decrypt(betweenC, size);
    
    cout << "Less than 3:\n" << less3 << endl;
    cout << "Greater than 3:\n" << graeter3 << endl;
    
    cout << "Between -3 and 3\n" << between << endl;
    
    delete reducerFactory;
    return 0;
}

void parseArgs(int argc, const  char* argv[]) {
    ArgMapping amap;

    bool dry=false;
    amap.arg("dry", dry, "dry=1 for a dry-run");

    long R=1;
    amap.arg("R", R, "number of rounds");

    long p=2;
    amap.arg("p", p, "plaintext base");

    long r=1;
    amap.arg("r", r,  "lifting");

    long d=1;
    amap.arg("d", d, "degree of the field extension");
    amap.note("d == 0 => factors[0] defines extension");

    long c=2;
    amap.arg("c", c, "number of columns in the key-switching matrices");


    long k=80;
    amap.arg("k", k, "security parameter");

    long L=0;
    amap.arg("L", L, "# of levels in the modulus chain",  "heuristic");

    long s=0;
    amap.arg("s", s, "minimum number of slots");

    long repeat=1;
    amap.arg("repeat", repeat,  "number of times to repeat the test");

    long chosen_m=0;
    amap.arg("m", chosen_m, "use specified value as modulus", NULL);

    Vec<long> mvec;
    amap.arg("mvec", mvec, "use product of the integers as  modulus", NULL);
    amap.note("e.g., mvec='[5 3 187]' (this overwrite the m argument)");

    Vec<long> gens;
    amap.arg("gens", gens, "use specified vector of generators", NULL);
    amap.note("e.g., gens='[562 1871 751]'");

    Vec<long> ords;
    amap.arg("ords", ords, "use specified vector of orders", NULL);
    amap.note("e.g., ords='[4 2 -4]', negative means 'bad'");

    long seed=0;
    amap.arg("seed", seed, "PRG seed");

    long nt=1;
    amap.arg("nt", nt, "num threads");

}