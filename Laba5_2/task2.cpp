#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <limits.h>

using namespace std;

enum ret_type_t {
    SUCCESS,            //Happy end
    ERROR_OPEN_FILE,    //Failed to open file
    ERROR_SAME_FILES,   //You're trying to overwrite file
    ERROR_NO_KEY,       //Null key, can't do encoding
};

void logErrors(int code) {
    switch(code) {
        case ERROR_OPEN_FILE: {
            cout << "Failed to open file" << endl;
            break;
        }
        case ERROR_SAME_FILES: {
            cout << "You're trying to overwrite file" << endl;
            break;
        }
        case ERROR_NO_KEY: {
            cout << "Null key, can't do encoding" << endl;
            break;
        }
    }
}

class Encoder {
public:
    Encoder(const vector<byte>& _key) : key(_key) {}

    int encode(const string& inputFilePath, const string& outputFilePath) {

        if (areFilesSame(inputFilePath, outputFilePath)) {
            return ERROR_SAME_FILES;
        }

        ifstream inputFile(inputFilePath, ios::binary);
        if (!inputFile) {
            return ERROR_OPEN_FILE;
        }

        ofstream outputFile(outputFilePath, ios::binary);
        if (!outputFile) {
            return ERROR_OPEN_FILE;
        }

        vector<byte> S(256);
        for (int i = 0; i < 256; ++i) {
            S[i] = static_cast<byte>(i);
        }

        if (key.size() == 0) {
            return ERROR_NO_KEY;
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + to_integer<int>(S[i]) + to_integer<int>(key[i % key.size()])) % 256;
            swap(S[i], S[j]);
        }

        int i = 0, k = 0;
        char byte;
        while (inputFile.get(byte)) {
            i = (i + 1) % 256;
            j = (j + to_integer<int>(S[i])) % 256;
            swap(S[i], S[j]);
            k = to_integer<int>(S[(to_integer<int>(S[i]) + to_integer<int>(S[j])) % 256]);

            //if (encrypt) {
            //    byte ^= static_cast<char>(k);
            //} else {
            //    byte ^= static_cast<char>(k);
            //}

            byte ^= static_cast<char>(k);

            outputFile.put(byte);
        }

        inputFile.close();
        outputFile.close();

        return SUCCESS;
    }

    void setKey(const vector<byte>& newKey) {
        key = newKey;
    }

private:
    vector<byte> key;

    bool areFilesSame(const string& filePath1, const string& filePath2) {
        struct stat stat1, stat2;

        if (stat(filePath1.c_str(), &stat1) != 0) {
            return false;
        }

        if (stat(filePath2.c_str(), &stat2) != 0) {
            return false;
        }

        return stat1.st_ino == stat2.st_ino;
    }
};


int main() {
    vector<byte> key = {byte(0x01), byte(0x23), byte(0x45), byte(0x67), byte(0x89), byte(0xAB), byte(0xCD), byte(0xEF)};
    Encoder encoder(key);

    int code = encoder.encode("input.txt", "encrypted.txt");
    if (code) {
        logErrors(code);
        return code;
    }

    code = encoder.encode("encrypted.txt", "decrypted.txt");
    if (code) {
        logErrors(code);
        return code;
    }

    return SUCCESS;
}