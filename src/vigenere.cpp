#include "vigenere.hpp"

VigenereCipher::VigenereCipher(std::string key) {
    this->key = key;

    // fill up lower case alphabet
    for (char ch = 'a'; ch <= 'z'; ch++) {
        lower_alphabet[ch - 'a'] = ch;
    }

    // full up upper case alphabet
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        upper_alphabet[ch - 'A'] = ch;
    }
}

std::string VigenereCipher::encrypt(std::string plain_text) {
    if (plain_text == "" || key == "") {
        return "";
    }

    std::string encrypted_text;
    int key_index = 0;

    for (int i = 0; i < plain_text.length(); i++) {
        char plain_char = plain_text[i];
        char key_char = key[key_index % key.length()];

        key_index++;

        // if non valid character, just add to encrypted
        if (!isalpha(plain_char) || !isalpha(key_char)) {
            encrypted_text += plain_char;
            continue;
        }

        // get the base character, upper or lower case
        char base_char = 'a';
        key_char = tolower(key_char);
        if (isupper(plain_char)) { 
            base_char = 'A'; 
            key_char = toupper(key_char);
        }

        // get indexes for each character
        int plain_index = plain_char - base_char;
        int key_index = key_char - base_char;
        
        // get encrypted character index
        // C = (P + K)  mod 26
        int encrypted_index = (plain_index + key_index) % 26;

        // add to encrypted text
        if (base_char == 'A') {
            encrypted_text += upper_alphabet[encrypted_index];
        } else {
            encrypted_text += lower_alphabet[encrypted_index];
        }
    }

    return encrypted_text;
}

std::string VigenereCipher::decrypt(std::string encrypted_text) {
    if (encrypted_text == "" || key == "") {
        return "";
    }

    std::string decrypted_text;
    int key_index = 0;

    for (int i = 0; i < encrypted_text.length(); i++) {
        char encrypted_char = encrypted_text[i];
        char key_char = key[key_index % key.length()];

        key_index++;

        // if non valid character, just add to encrypted
        if (!isalpha(encrypted_char) || !isalpha(key_char)) {
            decrypted_text += encrypted_char;
            continue;
        }

        // get the base character, upper or lower case
        char base_char = 'a';
        if (isupper(encrypted_char)) { 
            base_char = 'A'; 
            key_char = toupper(key_char);
        }

        // get indexes for each character
        int encrypted_index = encrypted_char - base_char;
        int key_index = key_char - base_char;
        
        // get decrypted character index
        // P = (C - K)  mod 26
        int decrypted_index = (encrypted_index - key_index + 26) % 26;

        // add to encrypted text
        if (base_char == 'A') {
            decrypted_text += upper_alphabet[decrypted_index];
        } else {
            decrypted_text += lower_alphabet[decrypted_index];
        }
    }

    return decrypted_text;
}

std::string VigenereCipher::get_key() {
    return std::string();
}

void VigenereCipher::set_key(std::string new_key) {
    key = new_key;
}