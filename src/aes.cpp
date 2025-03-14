#include "aes.hpp"

AES::AES(int key_size_in, uint8_t *key) {
    // check size of key, and update private vars

    // round nums
    //  128 --> 10 rounds
    //  192 --> 12 rounds
    //  256 --> 14 rounds

    // key schedule
    // 1 word = 4 bytes (in AES)
    //  128 (4 words, 16 bytes)   --> 44 words, 176 bytes
    //  192 (6 words, 24 bytes)   --> 52 words, 208 bytes
    //  256 (8 words, 32 bytes)   --> 60 words, 240 bytes
    if (key_size_in == 128) {
        round_num = 10;
        key_word_size = 4;
        key_original = new uint8_t[16];
        key_schedule = new uint8_t[176];
        
        key_original = key;
        key_size = key_size_in;

    } else if (key_size_in == 192) {
        round_num = 12;
        key_word_size = 6;
        key_original = new uint8_t[24];
        key_schedule = new uint8_t[208];
        
        key_original = key;
        key_size = key_size_in;

    } else if (key_size_in == 256) {
        round_num = 14;
        key_word_size = 8;
        key_original = new uint8_t[32];
        key_schedule = new uint8_t[240];
        
        key_original = key;
        key_size = key_size_in;

    }
}

AES::~AES() {
    delete key_original;
    delete key_schedule;
}

void AES::get_state(uint8_t (&state_in)[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state_in[i][j] = state[i][j];  
        }
    }
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

void AES::inv_s_box_transform(uint8_t &byte) {
    uint8_t row = byte / 0x10;
    uint8_t column = byte % 0x10;

    byte = inv_s_box[row][column];
}

void AES::add_round_key(uint8_t (&state)[4][4], uint8_t round_key[16]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] ^= round_key[(i * 4) + j];
        }
    }
}

uint8_t AES::galois_multi(uint8_t byte, uint8_t multiplier) {
    uint8_t result = 0;

    for (int i = 0; i < 8; i++) {
        if (multiplier & 0x01) {
            result ^= byte;
        }

        bool high_bit_set = byte & 0x80;
        byte <<= 1;

        if (high_bit_set) {
            byte ^= 0x1B;
        }

        multiplier >>= 1;
    }

    return result;
}


// key expansion algorithm
void AES::key_expansion() {
    int i = 0;

    // copy original key into key schedule
    for (i = 0; i < key_word_size * 4; i++) {
        key_schedule[i] = key_original[i];
    }

    // create the other rounds keys
    // e.g 128 --> 44 words, 10 rounds, 176 bytes
    int total_words = (round_num + 1) * key_word_size;
    for (i = key_word_size; i < total_words; i++) {
        // get previous key word
        uint8_t temp[4];
        for (int j = 0; j < 4; j++) {
            temp[j] = key_schedule[(i - 1) * 4 + j];
        }

        // apply key schedule
        // NOTE AES-256 has special subword case, shown in else-if statement
        if (i % key_word_size == 0) {
            rot_word(temp);
            sub_word(temp);

            temp[0] ^= r_con[i / key_word_size - 1][0];
            temp[1] ^= r_con[i / key_word_size - 1][1];
            temp[2] ^= r_con[i / key_word_size - 1][2];
            temp[3] ^= r_con[i / key_word_size - 1][3];

        } else if (key_word_size > 6 && i % key_word_size == 4) {
            sub_word(temp);

        }

        // XOR current key with previous key's first word
        for (int j = 0; j < 4; j++) {
            key_schedule[i * 4 + j] = key_schedule[(i - key_word_size) * 4 + j] ^ temp[j];
        }
    }
}

void AES::sub_word(uint8_t (&key)[4]) {
    for (int i = 0; i < 4; i++) {
        s_box_transform(key[i]);
    }
}

void AES::rot_word(uint8_t (&key)[4]) {
    uint8_t first_byte = key[0];

    for (int i = 0; i < 3; i++) {
        key[i] = key[i + 1];
    }

    key[3] = first_byte;
}


// ciphering algorithm 
void AES::encrypt() {
    // add original key
    uint8_t round_key[16]; // get the round key from the full key schedule
    for (int i = 0; i < 16; i++) {
        round_key[i] = key_schedule[i];
    }
    add_round_key(state, round_key);

    // loop through the rounds, for each key made
    for (int round = 1; round < round_num; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);

        for (int i = 0; i < 16; i++) {
            round_key[i] = key_schedule[round * 16 + i];
        }
        add_round_key(state, round_key);
    }

    // final round, state is now encrypted
    sub_bytes(state);
    shift_rows(state);
    
    for (int i = 0; i < 16; i++) {
        round_key[i] = key_schedule[round_num * 16 + i];
    }
    add_round_key(state, round_key);

}

void AES::sub_bytes(uint8_t (&state)[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            s_box_transform(state[i][j]);
        }
    }
}

void AES::shift_rows(uint8_t (&state)[4][4]) {
    // Copy state to original_state
    uint8_t original_state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            original_state[i][j] = state[i][j];
        }
    }

    // Row 1: Shift left by 1
    for (int i = 0; i < 4; i++) {
        state[1][i] = original_state[1][(i + 1) % 4];
    }

    // Row 2: Shift left by 2
    for (int i = 0; i < 4; i++) {
        state[2][i] = original_state[2][(i + 2) % 4];
    }

    // Row 3: Shift left by 3
    for (int i = 0; i < 4; i++) {
        state[3][i] = original_state[3][(i + 3) % 4];
    }
}

void AES::mix_columns(uint8_t (&state)[4][4]) {
    // make a copy of the existing state
    uint8_t temp_col[4];

    // use galois matrix multiplcation to find new indicies
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp_col[j] = state[j][i];
        }
        
        // change the state as per the algorithm formula
        state[0][i] = galois_multi(temp_col[0], 0x02) ^ galois_multi(temp_col[1], 0x03) ^ temp_col[2] ^ temp_col[3];
        state[1][i] = temp_col[0] ^ galois_multi(temp_col[1], 0x02) ^ galois_multi(temp_col[2], 0x03) ^ temp_col[3];
        state[2][i] = temp_col[0] ^ temp_col[1] ^ galois_multi(temp_col[2], 0x02) ^ galois_multi(temp_col[3], 0x03);
        state[3][i] = galois_multi(temp_col[0], 0x03) ^ temp_col[1] ^ temp_col[2] ^ galois_multi(temp_col[3], 0x02);
    }
}


// deciphering algorithm
void AES::decrypt() {
    // add original key
    uint8_t round_key[16]; // get the round key from the full key schedule
    for (int i = 0; i < 16; i++) {
        round_key[i] = key_schedule[round_num * 16 + i];
    }
    add_round_key(state, round_key);

    // loop through the rounds, for each key made
    for (int round = round_num - 1; round >= 1; round--) {
        inv_shift_rows(state);

        
        inv_sub_bytes(state);
        
        for (int i = 0; i < 16; i++) {
            round_key[i] = key_schedule[round * 16 + i];
        }
        add_round_key(state, round_key);

        inv_mix_columns(state);
    }

    // final round, state is now decrypted
    inv_shift_rows(state);
    inv_sub_bytes(state);
    
    for (int i = 0; i < 16; i++) {
        round_key[i] = key_schedule[i];
    }
    add_round_key(state, round_key);
}

void AES::inv_shift_rows(uint8_t (&state)[4][4]) {
    // Copy state to original_state
    uint8_t original_state[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            original_state[i][j] = state[i][j];
        }
    }

    // Row 1: Shift right by 1
    for (int i = 0; i < 4; i++) {
        state[1][i] = original_state[1][(i - 1 + 4) % 4];
    }

    // Row 2: Shift right by 2
    for (int i = 0; i < 4; i++) {
        state[2][i] = original_state[2][(i - 2 + 4) % 4];
    }

    // Row 3: Shift right by 3
    for (int i = 0; i < 4; i++) {
        state[3][i] = original_state[3][(i - 3 + 4) % 4];
    }
}

void AES::inv_sub_bytes(uint8_t (&state)[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            inv_s_box_transform(state[i][j]);
        }
    }
}

void AES::inv_mix_columns(uint8_t (&state)[4][4]) {
    // make a copy of the existing state
    uint8_t temp_col[4];

    // use galois matrix multiplcation to find new indicies
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp_col[j] = state[j][i];
        }
        
        // change the state as per the algorithm formula
        state[0][i] = galois_multi(temp_col[0], 0x0e) ^ galois_multi(temp_col[1], 0x0b) ^ galois_multi(temp_col[2], 0x0d) ^ galois_multi(temp_col[3], 0x09);
        state[1][i] = galois_multi(temp_col[0], 0x09) ^ galois_multi(temp_col[1], 0x0e) ^ galois_multi(temp_col[2], 0x0b) ^ galois_multi(temp_col[3], 0x0d);
        state[2][i] = galois_multi(temp_col[0], 0x0d) ^ galois_multi(temp_col[1], 0x09) ^ galois_multi(temp_col[2], 0x0e) ^ galois_multi(temp_col[3], 0x0b);
        state[3][i] = galois_multi(temp_col[0], 0x0b) ^ galois_multi(temp_col[1], 0x0d) ^ galois_multi(temp_col[2], 0x09) ^ galois_multi(temp_col[3], 0x0e);
    }
}
