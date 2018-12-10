#include "seal/seal.h"

using namespace seal;
class ExEvaluator: public Evaluator {
    public:
    ExEvaluator(const std::shared_ptr<SEALContext> &context, const IntegerEncoder &encoder);

    void xor_op_inplace(Ciphertext &encrypted1, const Ciphertext &encrypted2);

    inline void xor_op(const Ciphertext &encrypted1, const Ciphertext &encrypted2,
                Ciphertext &destination) {
        destination = encrypted1;
        xor_op_inplace(destination, encrypted2);
    }


    void xor_trick_inplace(Ciphertext &encrypted1, const Ciphertext &encrypted2);
    
    inline void xor_trick(const Ciphertext &encrypted1, const Ciphertext &encrypted2,
                Ciphertext &destination) {
        destination = encrypted1;
        xor_trick_inplace(destination, encrypted2);
    }

    private:

    std::shared_ptr<SEALContext> context_{ nullptr };
    IntegerEncoder encoder_;


    inline void xor_poly_poly_coeffmod(const std::uint64_t *operand1, 
            const std::uint64_t *operand2, std::size_t coeff_count, 
            const SmallModulus &modulus, std::uint64_t *result)
        {
            const uint64_t modulus_value = modulus.value();
            for (; coeff_count--; result++, operand1++, operand2++)
            {
                std::uint64_t sum = *operand1 ^ *operand2;

                // if the number was negative we're screwed :-(
                *result = sum - (modulus_value & static_cast<std::uint64_t>(
                    -static_cast<std::int64_t>(sum >= modulus_value)));
            }
        }
};