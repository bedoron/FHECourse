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

#include "seal/seal.h"

using namespace std;
using namespace seal;


int main()
{
    EncryptionParameters parms(scheme_type::BFV);
    parms.set_poly_modulus_degree(2048);
    parms.set_coeff_modulus(coeff_modulus_128(2048));
    parms.set_plain_modulus(1 << 8);
    auto context = SEALContext::Create(parms);

    IntegerEncoder encoder(parms.plain_modulus());
    KeyGenerator keygen(context);
    PublicKey public_key = keygen.public_key();
    SecretKey secret_key = keygen.secret_key();

    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);

    Decryptor decryptor(context, secret_key);

    int value1 = 5;
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
    Ciphertext encrypted1, encrypted2;
    cout << "Encrypting plain1: ";
    encryptor.encrypt(plain1, encrypted1);
    cout << "Done (encrypted1)" << endl;

    cout << "Encrypting plain2: ";
    encryptor.encrypt(plain2, encrypted2);
    cout << "Done (encrypted2)" << endl;



    // Decrypt part
    Plaintext plain_result;
    cout << "Decrypting result: ";
    decryptor.decrypt(encrypted1, plain_result);
    cout << "Done" << endl;

    cout << "Plaintext polynomial: " << plain_result.to_string() << endl;
    cout << "Decoded integer: " << encoder.decode_int32(plain_result) << endl;
}

