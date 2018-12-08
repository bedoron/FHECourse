#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <memory>
#include <limits>
#include <complex>
#include <iterator>
#include <vector>
#include "seal/seal.h"

using namespace std;
using namespace seal;

const uint coeff_type_size = sizeof(Ciphertext::ct_coeff_type)*8;

void handleSmallPoly(const std::uint64_t *poly, std::size_t coeff_count, const SmallModulus &modulus, vector<bool> &enc) {
                const uint64_t modulus_value = modulus.value();
    for (; coeff_count--; poly++)
    {
        std::int64_t non_zero = (*poly != 0);
        auto v = (modulus_value - *poly) & static_cast<std::uint64_t>(-non_zero);
        
        Ciphertext::ct_coeff_type mask = 1;
        for (int j = 0; j < coeff_type_size; ++j) {
            bool val = mask & v;
            enc.push_back(val);
            mask <<= 1;
        }

    }
}

vector<bool> toVec2(const std::shared_ptr<SEALContext> &context, const Ciphertext &encrypted) {
    const auto &context_data = context->context_data(encrypted.parms_id());;
    const auto &parms = context_data->parms();
    auto &coeff_modulus = parms.coeff_modulus();
    size_t coeff_count = parms.poly_modulus_degree();
    size_t coeff_mod_count = coeff_modulus.size();
    size_t encrypted_size = encrypted.size();

    vector<bool> enc;
    for (size_t j = 0; j < encrypted_size; j++)
    {
        for (size_t i = 0; i < coeff_mod_count; i++)
        {
            handleSmallPoly(encrypted.data(j) + (i * coeff_count),coeff_count,coeff_modulus[i], enc);
        }
    }

    return enc;
}


vector<bool> toVec(Ciphertext encrypted) {
    uint bits = encrypted.poly_modulus_degree()*coeff_type_size;
    vector<bool> enc(bits, 0);
    for (int i = 0; i < encrypted.poly_modulus_degree(); ++i) {
        Ciphertext::ct_coeff_type v = encrypted[i];
        Ciphertext::ct_coeff_type mask = 1;
        for (int j = 0; j < coeff_type_size; ++j) {
            uint offset = i*coeff_type_size+j;
            enc[offset] = mask & v;
            mask <<= 1;
        }
    }

    return enc;
}

#define TO_NUM(x) (x ? 1 : 0)

bool leftGreater(bool x, bool y) {
    int X = TO_NUM(x);
    int Y = TO_NUM(y);
    return (X * Y + X) % 2 == 1;
}

bool equals(bool x, bool y) {
    int X = TO_NUM(x);
    int Y = TO_NUM(y);
    return (X + Y + 1) % 2 == 1;
}

bool recurseEq(vector<bool> enc1, vector<bool> enc2, int i, int j);
// t_i,j
bool recurseGt(vector<bool> enc1, vector<bool> enc2, int i, int j) {
    if (j == 1) {
        return equals(enc1[i], enc2[i]);
    }

    int l = ceil(j / 2.0);
    bool p1 = recurseGt(enc1, enc2, i + l , j - l);
    bool p2 = recurseEq(enc1, enc2, i + l , j - l);
    bool p3 = recurseGt(enc1, enc2, i, l);

    return (TO_NUM(p1) + TO_NUM(p2 && p3)) % 2 == 1;
}

// z_i,j
bool recurseEq(vector<bool> enc1, vector<bool> enc2, int i, int j) {
    if (j == 1) {
        return equals(enc1[i], enc2[i]);
    }

    int l = ceil(j / 2.0);
    bool p1 = recurseEq(enc1, enc2, i + l , j - l);
    bool p3 = recurseEq(enc1, enc2, i, l);

    return p1 && p3;
}

int compare2(Evaluator& evaluator, Ciphertext& encrypted1, Ciphertext& encrypted2) {
    Ciphertext subtraction;
    evaluator.sub(encrypted1, encrypted2, subtraction);

    

    return 0;
}

int compare(vector<bool> enc1, vector<bool> enc2) {
    // TODO: Recurse
    bool equals = recurseEq(enc1, enc2, 0, enc1.size());
    cout << "Equals is: " << equals << "\n";
    return 0 ;
}

int main()
{
    EncryptionParameters parms(scheme_type::BFV);
    parms.set_poly_modulus_degree(1 << 8);
    parms.set_coeff_modulus(coeff_modulus_128(2048));
    parms.set_plain_modulus(1 << 8);
    auto context = SEALContext::Create(parms);

    IntegerEncoder encoder( parms.plain_modulus());
    KeyGenerator keygen(context);
    PublicKey public_key = keygen.public_key();
    SecretKey secret_key = keygen.secret_key();

    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);

    Decryptor decryptor(context, secret_key);

    int value_one = 1;
    Plaintext plain_one = encoder.encode(value_one);
    cout << "Encoded " << value_one << " as polynomial " << plain_one.to_string() 
        << " (plain_one)" << endl;

    int value1 = 0;
    Plaintext plain1 = encoder.encode(value1);
    cout << "Encoded " << value1 << " as polynomial " << plain1.to_string() 
        << " (plain1)" << endl;

    int value2 = 5;
    Plaintext plain2 = encoder.encode(value2);
    cout << "Encoded " << value2 << " as polynomial " << plain2.to_string() 
        << " (plain2)" << endl;

    /*
    Encrypting the encoded values is easy.
    */
    Ciphertext encrypted1, encrypted2, encrypted_one;
    cout << "Encrypting plain1: ";
    encryptor.encrypt(plain1, encrypted1);
    cout << "Done (encrypted1)" << endl;

    cout << "Encrypting plain2: ";
    encryptor.encrypt(plain2, encrypted2);
    cout << "Done (encrypted2)" << endl;

    cout << "Encrypting plain_one: ";
    encryptor.encrypt(plain_one, encrypted_one);
    cout << "Done (encrypted_one)" << endl;

    // evaluator.multiply_inplace(encrypted1, encrypted_one);

    // evaluator.add_inplace(encrypted1, encrypted_one);
    // evaluator.sub_inplace(encrypted1, encrypted_one);

    cout << "Converting to bitarray\n";
    vector<bool> enc1 = toVec2(context, encrypted1);
    vector<bool> enc2 = toVec2(context, encrypted2);
    cout << "Bits: " << enc1.size() << "\n";
    cout << "Bytes: " << enc1.size() / 8 << "\n";
    cout << "uint64: " << enc1.size() / 64 << "\n";
    cout << "Comparing...\n";
    cout << compare(enc1, enc2) << "\n";
    // compare2(evaluator, encrypted1, encrypted2);

    // for (int i = 0; i <1; ++i) {
    //     evaluator.multiply_inplace(encrypted1, encrypted2);
    // }

    // for (size_t i = 0; i < encrypted1.poly_modulus_degree(); i++) {
    //     cout << " " << encrypted1[i] << " ";
    // }

    // cout << "\n";
    // for (size_t i = 0; i < encrypted2.poly_modulus_degree(); i++) {
    //     cout << " " << encrypted2[i] << " ";
    // }

    cout << "\n";

    /* Decrypt part */
    Plaintext plain_result;
    cout << "Decrypting result: ";
    decryptor.decrypt(encrypted1, plain_result);
    cout << "Done" << endl;

    cout << "Plaintext polynomial: " << plain_result.to_string() << endl;
    cout << "Decoded integer: " << encoder.decode_int32(plain_result) << endl;
}

