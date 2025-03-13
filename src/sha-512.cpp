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
    // Function implementation
    
    
}

uint64_t SHA512::ch(uint64_t x, uint64_t y, uint64_t z) {
    return (x & y) ^ (!x & z);
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
