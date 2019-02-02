#pragma once
#include "FHE.h"
#include "UserParams.h"

typedef struct {
    const FHESecKey* secKey;
    const FHEcontext* context;
    const long modulo;
} Context;

class ContextBuilder {
public: 
    static Context build(const UserParams &up);
};