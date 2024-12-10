#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <cstddef>
#include <stdexcept>
#include <string>

using namespace std;

class Encoder {
private:
    vector<byte> key;

    void init(vector<byte>& s) {
        size_t keyLenght = key.size();
        for (size_t i = 0; i < 256; ++i) {
            s[i] = static_cast<byte>(i);
        }

        size_t j = 0;
        for (size_t i = 0; i < 256; ++i) {
            j = (j + static_cast<size_t>(s[i]) + static_cast<size_t>(key[i % keyLenght])) % 256;
            swap(s[i], s[j]);
        }
    }

    void generate(vector<byte>& s, vector<byte>& data) {
        size_t i = 0, j = 0;
        for (size_t k = 0; k < data.size(); ++k) {
            i = (i + 1) % 256;
            j = (j + static_cast<size_t>(s[i])) % 256;
            swap(s[i], s[j]);
            data[k] ^= s[(static_cast<size_t>(s[i]) + static_cast<size_t>(s[j])) % 256];
        }
    }

public:
    explicit Encoder(const vector<byte>& encryptionKey) : key(encryptionKey) {
        if (key.empty()) {
            throw invalid_argument("Key cannot be empty");
        }
    }

    void setKey(const vector<byte>& newKey) {
        if (newKey.empty()) {
            throw invalid_argument("Key cannot be empty");
        }
        key = newKey;
    }

    void encode(const string& inputFile, const string& outputFile) {
        ifstream input(inputFile, ios::binary);
        if (!input.is_open()) {
            throw runtime_error("Unable to open input file");
        }

        vector<byte> data;
        istreambuf_iterator<char> start(input), end;
        transform(start, end, back_inserter(data), [](char c) { return static_cast<byte>(c); });
        input.close();

        vector<byte> s(256);
        init(s);
        generate(s, data);

        ofstream output(outputFile, ios::binary);
        if (!output.is_open()) {
            throw runtime_error("Unable to open output file");
        }

        output.write(reinterpret_cast<const char*>(data.data()), data.size());
        output.close();
    }
};

int main() {
    try {
        vector<byte> key = {byte{0x12}, byte{0x34}, byte{0x56}, byte{0x78}};

        Encoder encoder(key);

        encoder.encode("input.txt", "encrypted.txt"); // флаг выбора алгоритма можно убрать, так как они идентичны

        encoder.encode("encrypted.txt", "decrypted.txt");
        
        cout << "Шифрование и дешифрование выполнены успешно." << endl;
    } catch (exception& ex) {
        cerr << "Ошибка: " << ex.what() << endl;
    }

    return 0;
}