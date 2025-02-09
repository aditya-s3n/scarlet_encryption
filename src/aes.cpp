#include "aes.hpp"

AES::AES(uint8_t key_size, uint8_t *key) {
    // check size of key, and update private vars

    // round nums
    //  128 --> 10 rounds
    //  192 --> 12 rounds
    //  256 --> 14 rounds

    // key schedule
    //  128 (4 words, 8 bytes)   --> 44 words, 88 bytes
    //  192 (6 words, 12 bytes)  --> 52 words, 104 bytes
    //  256 (8 words, 16 bytes)  --> 60 words, 120 bytes
    if (key_size == 128) {
        round_num = 10;
        key_word_size = 4;
        key_original = new uint8_t[8];
        key_schedule = new uint8_t[88];
        
        key_original = key;
        key_size = key_size;

    } else if (key_size == 192) {
        round_num = 12;
        key_word_size = 6;
        key_original = new uint8_t[12];
        key_schedule = new uint8_t[104];
        
        key_original = key;
        key_size = key_size;

    } else if (key_size == 256) {
        round_num = 14;
        key_word_size = 8;
        key_original = new uint8_t[16];
        key_schedule = new uint8_t[120];
        
        key_original = key;
        key_size = key_size;

    }
}

AES::~AES() {
    delete key_original;
    delete key_schedule;
}


std::array<std::array<uint8_t, 4>, 4> AES::get_state() {
    std::array<std::array<uint8_t, 4>, 4> state_copy;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state_copy[i][j] = state[i][j];  
        }
    }

    return state_copy;
}

void AES::set_state(uint8_t new_state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = new_state[i][j];
        }
    }
}


void AES::s_box_transform(uint8_t &byte) {
    uint8_t row = byte / 0x10;
    uint8_t column = byte % 0x10;

    byte = s_box[row][column];
}

void AES::add_round_key(uint8_t (&state)[4][4], uint8_t *round_key) {
    
}


// key expansion algorithm
void AES::key_expansion() {
}

void AES::sub_word(uint8_t key[4]) {
    for (int i = 0; i < 4; i++) {
        s_box_transform(key[i]);
    }
}

void AES::rot_word(uint8_t key[4]) {
    uint8_t first_byte = key[0];

    for (int i = 0; i < 3; i++) {
        key[i] = key[i + 1];
    }

    key[3] = first_byte;
}


// ciphering algorithm 
void AES::encrypt() {
}

void AES::sub_bytes(uint8_t (&state)[4][4]) {
}

void AES::shift_rows(uint8_t (&state)[4][4]) {
}

void AES::mix_columns(uint8_t (&state)[4][4]) {
}


// diciphering algorithm
void AES::decrypt() {
}

void AES::inv_shift_rows(uint8_t (&state)[4][4]) {
}

void AES::inv_sub_bytes(uint8_t (&state)[4][4]) {
}

void AES::inv_mix_columns(uint8_t (&state)[4][4]) {
}
