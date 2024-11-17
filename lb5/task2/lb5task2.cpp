#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef> // Для std::byte
#include <string>
#include <stdexcept> // Для обработки исключений

using namespace std;

class Encoder {
private:
    vector<byte> key;


    void rc4_init(vector<byte>& S) const {
        size_t key_length = key.size();
        S.resize(256);
        for (size_t i = 0; i < 256; ++i) {
            S[i] = static_cast<byte>(i);
        }
        size_t j = 0;
        for (size_t i = 0; i < 256; ++i) {
            j = (j + static_cast<size_t>(S[i]) + static_cast<size_t>(key[i % key_length])) % 256;
            swap(S[i], S[j]);
        }
    }


    void rc4_generate(const vector<byte>& S, vector<byte>& keystream, size_t length) const {
        keystream.resize(length);
        size_t i = 0, j = 0;
        vector<byte> S_copy = S;
        for (size_t n = 0; n < length; ++n) {
            i = (i + 1) % 256;
            j = (j + static_cast<size_t>(S_copy[i])) % 256;
            swap(S_copy[i], S_copy[j]);
            keystream[n] = S_copy[(static_cast<size_t>(S_copy[i]) + static_cast<size_t>(S_copy[j])) % 256];
        }
    }

public:

    Encoder(const vector<byte>& encryptionKey) : key(encryptionKey) {
        if (key.empty()) {
            throw invalid_argument("Encryption key cannot be empty.");
        }
    }

 
    void setKey(const vector<byte>& newKey) {
        if (newKey.empty()) {
            throw invalid_argument("Encryption key cannot be empty.");
        }
        key = newKey;
    }

    
    bool encode(const string& inputPath, const string& outputPath, bool encrypt) const {
        try {
            ifstream inputFile(inputPath, ios::binary);
            if (!inputFile.is_open()) {
                throw runtime_error("Failed to open input file: " + inputPath);
            }

            ofstream outputFile(outputPath, ios::binary);
            if (!outputFile.is_open()) {
                throw runtime_error("Failed to open output file: " + outputPath);
            }
            vector<byte> inputData;
            istreambuf_iterator<char> it(inputFile);
            istreambuf_iterator<char> end;
            while (it != end) {
                inputData.push_back(static_cast<byte>(*it));  
                ++it;
            }
            inputFile.close();

            if (inputData.empty()) {
                throw runtime_error("Input file is empty: " + inputPath);
            }
            vector<byte> S;
            rc4_init(S);
            vector<byte> keystream;
            rc4_generate(S, keystream, inputData.size());
            vector<byte> outputData(inputData.size());
            for (size_t i = 0; i < inputData.size(); ++i) {
                outputData[i] = inputData[i] ^ keystream[i];
            }
            outputFile.write(reinterpret_cast<const char*>(outputData.data()), outputData.size());
            outputFile.close();

            return true;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            return false;
        }
    }
};


int main() {
    try {
        vector<byte> key = { byte{0x12}, byte{0x34}, byte{0x56}, byte{0x78} };
        Encoder encoder(key);
        if (encoder.encode("input.txt", "encrypted.txt", true)) {
            cout << "File encrypted successfully.\n";
        }
        else {
            cerr << "Encryption failed.\n";
        }
        if (encoder.encode("encrypted.txt", "decrypted.txt", false)) {
            cout << "File decrypted successfully.\n";
        }
        else {
            cerr << "Decryption failed.\n";
        }
    }
    catch (const exception& e) {
        cerr << "Critical error: " << e.what() << endl;
    }

    return 0;
}

