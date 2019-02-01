#include "FHE.h"
#include "UserParams.h"

typedef struct {
    FHESecKey* secKey;
    FHEcontext* context;
} Context;

class ContextBuilder {
public: 
    static Context build(const UserParams &up);
};