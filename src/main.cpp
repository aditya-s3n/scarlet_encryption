#include <iostream>
#include "vigenere.hpp"

int main() {

    VigenereCipher vignere = VigenereCipher("secret");

    std::string s = vignere.encrypt("Scarlet 12 12 ok");
    std::cout << s << std::endl;

    std::cout << vignere.decrypt(s) << std::endl;

    return 0;
}