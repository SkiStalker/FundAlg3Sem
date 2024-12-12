#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstddef>
#include <sys/stat.h>
using namespace std;


class Encoder {
private:
    vector<byte> key;
public:
    Encoder(const vector<byte>& k) {
        key = k;
    }
    void encode(const string& i_path, const string& o_path, bool encrypt) {

        if (!key.size())
            throw("Empty key error");

        if (i_path == o_path)
            throw("Error same files");

        ifstream i_file(i_path, ios::binary);
        if (!i_file)
            throw("File opening error");

        ofstream o_file(o_path, ios::binary);
        if (!o_file)
            throw("File opening error");

        vector<byte> s_box(256);
        int i, j, k;
        for (i = 0; i < 256; i++) {
            s_box[i] = static_cast<byte>(i);
        }

        j = 0;
        for (i = 0; i < 256; i++) {
            j = (j + static_cast<int>(s_box[i]) + static_cast<int>(key[i % key.size()])) % 256;
            swap(s_box[i], s_box[j]);
        }

        i = 0, k = 0;
        char byte;
        while (i_file.get(byte)) {
            i = (i + 1) % 256;
            j = (j + static_cast<int>(s_box[i])) % 256;
            swap(s_box[i], s_box[j]);
            k = static_cast<int>(s_box[(static_cast<int>(s_box[i]) + static_cast<int>(s_box[j])) % 256]);
            byte ^= static_cast<char>(k);
            o_file.put(byte);
        }

        i_file.close();
        o_file.close();

        if (encrypt)
            cout << "File " << i_path << " was encrypted, result in " << o_path << endl;
        else
            cout << "File " << i_path << " was decrypted, result in " << o_path << endl;
    }

    void change_key(const vector<byte>& new_key) {
        key = new_key;
    }
};

int main() {

    vector<byte> key = { byte(23), byte(234), byte(167), byte(55), byte(5), byte(224), byte(101) };
    Encoder e(key);

    try {
        e.encode("input_52.txt", "encrypted.txt", true);
    }
    catch (const char* except) {
        cout << except << endl;
        return 1;
    }

    try {
        e.encode("encrypted.txt", "decrypted.txt", false);
    }
    catch (const char* except) {
        cout << except << endl;
        return 1;
    }

    return 0;
}