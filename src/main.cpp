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
#include "ContextBuilder.h"
#include "Context.h"
#include "utils.h"
#include <algorithm>
#include <functional>

using std::transform;
using std::plus;
using std::function;

int main(int argc, char *argv[]) {
    UserParams up;
    up.parse(argc, argv);
    
    cout << "Bulding context and generating keys..." << endl;
    Context contex = ContextBuilder::build(up);
    cout << "Computations are modulo: " << contex.modulo << endl;

    cout << "Generating random vector..." << endl;
    vector<long> v = randomVector(up.size, 0, 0, (unsigned int) contex.modulo/4);
    printVector(v);

    cout << "Normalizing generated vector..." << endl;
    transform(v.begin(), v.end(), v.begin(), createNormalizer(contex.modulo/8, contex.modulo));
    printVector(v);

    cout << "Encrypting vector..." << endl;
    CtxtExt res = contex.encrypt(v);

    // TODO: Now implement less than etc...


    // CtxtExt c = contex.context->


//     long p = up.p, r = up.r, security = up.k, l;
//     short strategy;
//     unsigned int size;
    
//     // variables_map vm = applyArgumentParser(argc, argv, argumentsParser(&p, &r, &l, &security, &size, &strategy));
//     // printParameters(vm);
//     // ICtxtReducerFactory *reducerFactory = createCtxtReducerFactory(vm);
    
    
//     // Keys keys(security, p, r, l);
//     // CtxtClient client(*keys.secKey, reducerFactory);
//     VectorIO io;
    
//     cout << "All computations are modulu " << keys.getModulu() << endl;
    
//     // Generate random matrix
//     Vector v = io.createRandom(size, 0, 0, (unsigned int)keys.getModulu()/4);
//     transform(v, [p](long num) { return adjustNumber(num - (p/8), p); });
//     printVector(v);
    
// #ifdef DEBUG
//     function<void(const Ctxt&)> debug = [&client, size, p](const Ctxt& c) {
//         Vector tmp = client.decrypt(c, size);
//         transform(tmp, [p](long num) { return adjustNumber(num, p); });
//         printVector(tmp);
//     };
// #endif
    
//     // Encrypt
//     cout << "Encrypt the vector" << endl;
//     Ctxt c = client.encrypt(v);
    
//     // comparing
//     cout << "calculate v < 3" << endl;
//     Ctxt less3c = client.lessThan(c, 3
// #ifdef DEBUG
//                                   , debug
// #endif
//                                   );
    
//     // comparing
//     cout << "calculate v > 3" << endl;
//     Ctxt graeter3c = client.greaterThan(c, 3
// #ifdef DEBUG
//                                         , debug
// #endif
//                                         );
    
//     cout << "calculate between -3 to 3" << endl;
//     Ctxt betweenC = client.between(c, -3, 3);
    
//     // Decrypt
//     cout << "Decrypt the results" << endl;
//     Vector less3 = client.decrypt(less3c, size), graeter3 = client.decrypt(graeter3c, size), between = client.decrypt(betweenC, size);
    
//     cout << "Less than 3:\n" << less3 << endl;
//     cout << "Greater than 3:\n" << graeter3 << endl;
    
//     cout << "Between -3 and 3\n" << between << endl;
    
//     delete reducerFactory;
//     return 0;
}
