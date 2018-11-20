#pragma once

#include <stdexcept>
#include "seal/memorymanager.h"
#include "seal/util/modulus.h"

namespace seal
{
    namespace util
    {
        class NTTTables
        {
        public:
            NTTTables() = default;

            NTTTables(int coeff_count_power, const Modulus &modulus);

            inline bool is_generated() const
            {
                return generated_;
            }

            bool generate(int coeff_count_power, const Modulus &modulus);

            void reset();

            inline const std::uint64_t *get_root() const
            {
#ifdef SEAL_DEBUG
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return root_.get();
            }

            inline const std::uint64_t *get_from_root_powers(std::size_t index) const 
            {
#ifdef SEAL_DEBUG
                if (index >= coeff_count_)
                {
                    throw std::out_of_range("index");
                }
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return root_powers_.get() + index * coeff_uint64_count_;
            }

            inline const std::uint64_t *get_from_scaled_root_powers(std::size_t index) const
            {
#ifdef SEAL_DEBUG
                if (index >= coeff_count_)
                {
                    throw std::out_of_range("index");
                }
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return scaled_root_powers_.get() + index * coeff_uint64_count_;
            }

            inline const std::uint64_t *get_from_inv_root_powers(std::size_t index) const
            {
#ifdef SEAL_DEBUG
                if (index >= coeff_count_)
                {
                    throw std::out_of_range("index");
                }
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return inv_root_powers_.get() + index * coeff_uint64_count_;
            }

            inline const std::uint64_t *get_from_scaled_inv_root_powers(std::size_t index) const
            {
#ifdef SEAL_DEBUG
                if (index >= coeff_count_)
                {
                    throw std::out_of_range("index");
                }
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return scaled_inv_root_powers_.get() + index * coeff_uint64_count_;
            }

            inline const std::uint64_t *get_from_inv_root_powers_div_two(std::size_t index) const
            {
#ifdef SEAL_DEBUG
                if (index >= coeff_count_)
                {
                    throw std::out_of_range("index");
                }
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return inv_root_powers_div_two_.get() + index * coeff_uint64_count_;
            }

            inline const std::uint64_t *get_from_scaled_inv_root_powers_div_two(std::size_t index) const 
            {
#ifdef SEAL_DEBUG
                if (index >= coeff_count_)
                {
                    throw std::out_of_range("index");
                }
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return scaled_inv_root_powers_div_two_.get() + index * coeff_uint64_count_;
            }

            inline const std::uint64_t *get_inv_degree_modulo() const 
            {
#ifdef SEAL_DEBUG
                if (!generated_)
                {
                    throw std::logic_error("tables are not generated");
                }
#endif
                return inv_degree_modulo_.get();
            }

            inline const Modulus &modulus() const
            {
                return modulus_;
            }

            inline int coeff_count_power() const
            {
                return coeff_count_power_;
            }

            inline std::size_t coeff_count() const
            {
                return coeff_count_;
            }

            inline std::size_t coeff_uint64_count() const
            {
                return coeff_uint64_count_;
            }

        private:
            NTTTables(const NTTTables &copy) = delete;

            NTTTables(NTTTables &&source) = delete;

            NTTTables &operator =(const NTTTables &assign) = delete;

            NTTTables &operator =(NTTTables &&assign) = delete;

            // Computed bit-scrambled vector of first 1 << coeff_count_power powers of a primitive root.
            void ntt_powers_of_primitive_root(std::uint64_t *root, std::uint64_t *destination);

            // Scales the elements of a vector returned by powers_of_primitive_root(...) by word_size/modulus and rounds down.
            void ntt_scale_powers_of_primitive_root(std::uint64_t *input, std::uint64_t *destination);

            MemoryPoolHandle pool_ = MemoryManager::GetPool();

            // Size coeff_uint64_count_ * coeff_count_
            Pointer<std::uint64_t> root_powers_;

            // Size coeff_uint64_count_ * coeff_count_
            Pointer<std::uint64_t> scaled_root_powers_;

            // Size coeff_uint64_count_ * coeff_count_
            Pointer<std::uint64_t> inv_root_powers_div_two_;

            // Size coeff_uint64_count_ * coeff_count_
            Pointer<std::uint64_t> scaled_inv_root_powers_div_two_;

            bool generated_ = false;

            int coeff_count_power_ = 0;

            std::size_t coeff_count_ = 0;

            std::size_t coeff_uint64_count_ = 0;

            // Allocation for modulus, size coeff_uint64_count_
            Pointer<std::uint64_t> modulus_alloc_;

            Modulus modulus_;

            // Size uint64_count_
            Pointer<std::uint64_t> root_;

            // Size coeff_uint64_count_ * coeff_count_
            Pointer<std::uint64_t> inv_root_powers_;

            // Size coeff_uint64_count_ * coeff_count_
            Pointer<std::uint64_t> scaled_inv_root_powers_;

            // Size coeff_uint64_count_
            Pointer<std::uint64_t> inv_degree_modulo_;
        };

        void ntt_negacyclic_harvey(std::uint64_t *operand, const NTTTables &tables, MemoryPool &pool);
        
        void inverse_ntt_negacyclic_harvey(std::uint64_t *operand, const NTTTables &tables, MemoryPool &pool);
    }
}
