#include <iostream>
#include "vigenere.hpp"
#include "aes.hpp"
#include <iomanip> 

int main() {

    VigenereCipher vignere = VigenereCipher("secret");
    std::string s = vignere.encrypt("Scarlet Scarlet");
    std::cout << s << std::endl;

    std::cout << vignere.decrypt(s) << std::endl;

    
    uint8_t *key = new uint8_t[24]{
        0x8e, 0x80, 0x73, 0x90, 0xb0, 0x79, 0xf7, 0xe5, 
        0xda, 0x62, 0x0e, 0xf8, 0x64, 0xea, 0x52, 0xd2, 
        0xc8, 0x52, 0x10, 0x2c, 0xf3, 0x6b, 0x2b, 0x7b
    };
    
    AES aes = AES(192, key);
    aes.key_expansion();

    return 0;
}