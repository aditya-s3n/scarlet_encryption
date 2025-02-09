#include "aes.hpp"

AES::AES(uint8_t key_size, uint8_t *key) {
}

AES::~AES() {
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
}

void AES::s_box_transform(unsigned char &byte) {
}

void AES::key_expansion() {
}

void AES::sub_word(unsigned char *key[4]) {
}

void AES::rot_word(unsigned char *key[4]) {
}

void AES::add_round_key(unsigned char (&state)[4][4], uint8_t *round_key) {
}

void AES::encrypt() {
}

void AES::sub_bytes(unsigned char (&state)[4][4]) {
}

void AES::shift_rows(unsigned char (&state)[4][4]) {
}

void AES::mix_columns(unsigned char (&state)[4][4]) {
}

void AES::decrypt() {
}

void AES::inv_shift_rows(unsigned char (&state)[4][4]) {
}

void AES::inv_sub_bytes(unsigned char (&state)[4][4]) {
}

void AES::inv_mix_columns(unsigned char (&state)[4][4]) {
}
