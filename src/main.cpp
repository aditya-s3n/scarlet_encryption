#include <iostream>
#include "vigenere.hpp"
#include "aes.hpp"
#include <iomanip> 

int main() {

    VigenereCipher vignere = VigenereCipher("secret");
    std::string s = vignere.encrypt("Scarlet Scarlet");
    std::cout << s << std::endl;

    std::cout << vignere.decrypt(s) << std::endl;

    
    uint8_t *key = new uint8_t[16]{
        0x2b, 0x7e, 0x15, 0x16, 
        0x28, 0xae, 0xd2, 0xa6, 
        0xab, 0xf7, 0x15, 0x88, 
        0x09, 0xcf, 0x4f, 0x3c
    };
    
    uint8_t state[4][4] = {
        {0x32, 0x88, 0x31, 0xe0}, 
        {0x43, 0x5a, 0x31, 0x37}, 
        {0xf6, 0x30, 0x98, 0x07}, 
        {0xa8, 0x8d, 0xa2, 0x34}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << "0x" 
                      << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(state[i][j]) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    
    AES aes = AES(128, key);
    aes.key_expansion();
    aes.set_state(state);

    uint8_t new_state[4][4];
    aes.encrypt();
    aes.get_state(new_state);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << "0x" 
                      << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(new_state[i][j]) << " ";
        }
        std::cout << std::endl;
    }

    aes.decrypt();
    aes.get_state(new_state);

    std::cout << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << "0x" 
                      << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(new_state[i][j]) << " ";
        }
        std::cout << std::endl;
    }



    return 0;
}