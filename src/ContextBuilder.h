#pragma once
#include "FHE.h"
#include "UserParams.h"
#include "Context.h"

class ContextBuilder {
public: 
    static Context build(const UserParams &up);
};