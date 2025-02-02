#ifndef VIGENERE
#define VIGENERE

#include <iostream>

class VigenereCipher {
    private:
        std::string key;
        char lower_alphabet[26];
        char upper_alphabet[26];

    public:
        VigenereCipher(std::string key);

        // basic functionality 
        std::string encrypt(std::string plain_text);
        std::string decrypt(std::string encrypted_text);

        // getters and setters
        std::string get_key() { return key; }
        void set_key(std::string new_key) { key = new_key; }

};

#endif