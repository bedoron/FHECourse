#include "utils.h"
#include <iostream>

using std::cout;
using std::endl;

vector<long> randomVector(unsigned int length, unsigned int seed, unsigned int bottom, unsigned int top) {
    if (seed != 0) { srand(seed); }

    vector<long> result(length);
    for (int i = 0; i < length; ++i) {
        result[i] = rand() % (top - bottom) + bottom;
    }

    return result;
}

void printVector(vector<long> v) {
    for(auto var : v)
    {
        cout << var << " ";
    }
    cout << endl;
}

function<long(long)> createNormalizer(const long modulo, const long decrement) {
    return [modulo, decrement](long fv) {
        while((fv - decrement) > (modulo/2)) { 
            fv -= modulo; 
        } 
        return fv;
    };
}


