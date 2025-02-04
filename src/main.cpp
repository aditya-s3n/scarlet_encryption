#include <iostream>
#include "vigenere.hpp"

int main() {

    VigenereCipher vignere = VigenereCipher("secret 9213");
    std::string s = vignere.encrypt("Scarlet 12 Scarlet 123 Scarlet Scarlet Scarlet Scarlet Scarlet Scarlet");
    std::cout << s << std::endl;

    std::cout << vignere.decrypt(s) << std::endl;

    return 0;
}