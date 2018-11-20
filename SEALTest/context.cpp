#include "CppUnitTest.h"
#include "seal/context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST_CLASS(ContextTest)
    {
    public:
        TEST_METHOD(ContextConstructor)
        {
            // Nothing set
            auto scheme = scheme_type::BFV;
            EncryptionParameters parms(scheme);
            {
                auto context = SEALContext::Create(parms);
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsFalse(qualifiers.parameters_set);
                Assert::IsFalse(qualifiers.using_fft);
                Assert::IsFalse(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Not relatively prime coeff moduli
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 2, 30 });
            parms.set_plain_modulus(2);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsFalse(qualifiers.parameters_set);
                Assert::IsFalse(qualifiers.using_fft);
                Assert::IsFalse(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Plain modulus not relatively prime to coeff moduli
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 17, 41 });
            parms.set_plain_modulus(34);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsFalse(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Plain modulus not smaller than product of coeff moduli
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 2 });
            parms.set_plain_modulus(3);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(2ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsFalse(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsFalse(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // FFT poly but not NTT modulus
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 3 });
            parms.set_plain_modulus(2);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(3ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsFalse(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsFalse(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Parameters OK; no fast plain lift
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 17, 41 });
            parms.set_plain_modulus(18);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(697ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsTrue(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Parameters OK; fast plain lift
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 17, 41 });
            parms.set_plain_modulus(16);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(697ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsTrue(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsTrue(qualifiers.using_fast_plain_lift);
            }

            // Parameters OK; no batching due to non-prime plain modulus
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 17, 41 });
            parms.set_plain_modulus(49);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(697ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsTrue(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Parameters OK; batching enabled
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 17, 41 });
            parms.set_plain_modulus(73);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(697ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsTrue(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsTrue(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Parameters OK; batching and fast plain lift enabled
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 137, 193 });
            parms.set_plain_modulus(73);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(26441ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsTrue(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsTrue(qualifiers.using_batching);
                Assert::IsTrue(qualifiers.using_fast_plain_lift);
            }

            // Negative noise standard deviation
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 137, 193 });
            parms.set_plain_modulus(73);
            parms.set_noise_standard_deviation(-0.1);
            parms.set_random_generator(UniformRandomGeneratorFactory::default_factory());
            {
                auto context = SEALContext::Create(parms);
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsFalse(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsFalse(qualifiers.using_ntt);
                Assert::IsFalse(qualifiers.using_batching);
                Assert::IsFalse(qualifiers.using_fast_plain_lift);
            }

            // Parameters OK; batching and fast plain lift enabled; nullptr RNG
            parms.set_poly_modulus_degree(4);
            parms.set_coeff_modulus({ 137, 193 });
            parms.set_plain_modulus(73);
            parms.set_noise_standard_deviation(3.19);
            parms.set_random_generator(nullptr);
            {
                auto context = SEALContext::Create(parms);
                Assert::AreEqual(26441ULL, *context->context_data()->total_coeff_modulus().get());
                auto qualifiers = context->context_data()->qualifiers();
                Assert::IsTrue(qualifiers.parameters_set);
                Assert::IsTrue(qualifiers.using_fft);
                Assert::IsTrue(qualifiers.using_ntt);
                Assert::IsTrue(qualifiers.using_batching);
                Assert::IsTrue(qualifiers.using_fast_plain_lift);
            }
        }
    };
}