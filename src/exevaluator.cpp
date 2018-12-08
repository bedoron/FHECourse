#include "exevaluator.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <functional>
#include "seal/evaluator.h"
#include "seal/util/common.h"
#include "seal/util/uintcore.h"
#include "seal/util/uintarith.h"
#include "seal/util/uintarithsmallmod.h"
#include "seal/util/polycore.h"
#include "seal/util/polyarithsmallmod.h"
#include "seal/util/polyfftmultsmallmod.h"

using namespace std;
using namespace seal::util;

ExEvaluator::ExEvaluator(const std::shared_ptr<SEALContext> &context) : 
        context_(context), Evaluator(context) {
    
}

void ExEvaluator::xor_op_inplace(Ciphertext &encrypted1, const Ciphertext &encrypted2) {
            // Verify parameters.
    auto context_data_ptr = context_->context_data(encrypted1.parms_id());
    // Maybe just add them and see if they wont blow up
    Ciphertext tmp;
    add(encrypted1, encrypted2, tmp);
    // If we got here the variables are compatible

    // Extract encryption parameters.
    auto &context_data = *context_data_ptr;
    auto &parms = context_data.parms();
    auto &coeff_modulus = parms.coeff_modulus();
    size_t coeff_count = parms.poly_modulus_degree();
    size_t coeff_mod_count = coeff_modulus.size();
    size_t encrypted1_size = encrypted1.size();
    size_t encrypted2_size = encrypted2.size();
    size_t max_count = max(encrypted1_size, encrypted2_size);
    size_t min_count = min(encrypted1_size, encrypted2_size);

    // Prepare destination
    encrypted1.resize(context_, parms.parms_id(), max_count);

    // Xor ciphertexts
    for (size_t j = 0; j < min_count; j++)
    {
        for (size_t i = 0; i < coeff_mod_count; i++)
        {
            xor_poly_poly_coeffmod(encrypted1.data(j) + (i * coeff_count),
                encrypted2.data(j) + (i * coeff_count), coeff_count, coeff_modulus[i],
                encrypted1.data(j) + (i * coeff_count));
        }
        
    }

    // Copy the remainding polys of the array with larger count into encrypted1
    if (encrypted1_size < encrypted2_size)
    {
        set_poly_poly(encrypted2.data(min_count),
            coeff_count * (encrypted2_size - encrypted1_size),
            coeff_mod_count, encrypted1.data(encrypted1_size));
    }
}