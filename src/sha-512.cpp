#include "sha-512.hpp"

SHA512::SHA512(uint64_t message_block[16]) {
    // Constructor implementation
    message_block = message_block;
}

SHA512::~SHA512() { }

void SHA512::preprocessing() {
    // Function implementation


}

void SHA512::hash_computation() {
    /** 
     * NOTE, no need to count for overflow since uint64 wraps around via hardware level behaviour
     */

    uint64_t word_blocks[80];
    // iterate 80 times
    for (int i = 0; i < 80; i++) {
        // prepare the message
        uint64_t word;
        if (i < 16) {
            word_blocks[i] = message_block[i];
        } else {
            word_blocks[i] = message_sigma_1(word_blocks[i - 2]) 
                            + word_blocks[i - 7] 
                            + message_sigma_1(word_blocks[i - 15]) 
                            + word_blocks[i - 16];
        }

        // assign temp variables
        uint64_t a = hash_val[0];
        uint64_t b = hash_val[1];
        uint64_t c = hash_val[2];
        uint64_t d = hash_val[3];
        uint64_t e = hash_val[4];
        uint64_t f = hash_val[5];
        uint64_t g = hash_val[6];
        uint64_t h = hash_val[7];

        // do math computation / new temp values
        uint64_t T1 = h + compression_sigma_1(e) + ch(e, f, g) + K_CONSTANTS[i] + word_blocks[i];
        uint64_t T2 = compression_sigma_0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;

        // assign to new values
        hash_val[0] += a;
        hash_val[1] += b;
        hash_val[2] += c;
        hash_val[3] += d;
        hash_val[4] += e;
        hash_val[5] += f;
        hash_val[6] += g;
        hash_val[7] += h;
    }
}

uint64_t SHA512::ch(uint64_t x, uint64_t y, uint64_t z) {
    return (x & y) ^ (~x & z);
}

uint64_t SHA512::maj(uint64_t x, uint64_t y, uint64_t z) {
    return (x & y) ^ (x & z) ^ (y ^ z);
}

uint64_t SHA512::rotr(uint64_t x, uint64_t n) {
    // w = 64 bit words
    return (x >> n) | (x << (64 - n));
}

uint64_t SHA512::shr(uint64_t x, uint64_t n) {
    return (x >> n);
}

uint64_t SHA512::message_sigma_0(uint64_t x) {
    return rotr(x, 1) ^ rotr(x, 8) ^ shr(x, 7);
}

uint64_t SHA512::message_sigma_1(uint64_t x) {
    return rotr(x, 19) ^ rotr(x, 61) ^ shr(x, 6);
}

uint64_t SHA512::compression_sigma_0(uint64_t x) {
    return rotr(x, 28) ^ rotr(x, 34) ^ rotr(x, 39);
}

uint64_t SHA512::compression_sigma_1(uint64_t x) {
    return rotr(x, 14) ^ rotr(x, 18) ^ rotr(x, 41);
}

uint64_t* SHA512::get_hash() {
    return hash_val;
}

uint64_t* SHA512::get_message() {
    return message_block;
}
