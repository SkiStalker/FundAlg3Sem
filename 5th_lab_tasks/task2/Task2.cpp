#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <algorithm>

std::ostream &operator<<(std::ostream &os, unsigned char b)
{
    return os << static_cast<char>(b);
}

class encoder final
{
private:
    std::vector<unsigned char> S, _key;
    int x, y;

    int to_int(unsigned char byte) { return static_cast<int>(byte); }

    void init()
    {
        int key_length = _key.size();
        for (int i = 0; i < 256; i++)
        {
            S[i] = static_cast<unsigned char>(i);
        }

        int j = 0;
        for (int i = 0; i < 256; i++)
        {
            j = (j + to_int(S[i]) + to_int(_key[i % key_length])) % 256;
            std::swap(S[i], S[j]);
        }
    }

    unsigned char get_s_item()
    {
        x = (x + 1) % 256;
        y = (y + to_int(S[x])) % 256;

        std::swap(S[x], S[y]);
        return S[(to_int(S[x]) + to_int(S[y])) % 256];
    }

public:
    encoder(std::vector<unsigned char> &key) : S(256), _key(key), x(0), y(0)
    {
        if (key.empty())
            throw std::length_error("Zero key length");
    }

    void set_key(std::vector<unsigned char> const &key)
    {
        if (key.empty())
            throw std::length_error("Zero key length");
        _key = key;
    }

    void encode(std::string const &input_name, std::string const &output_name)
    {
        if (input_name == output_name)
        {
            throw std::runtime_error("Input and output files must be different");
        }

        std::ifstream in(input_name, std::ios::binary);
        if (!in.is_open())
        {
            throw std::runtime_error("Couldn't open the input file");
        }

        std::ofstream out(output_name, std::ios::binary);
        if (!out.is_open())
        {
            in.close();
            throw std::runtime_error("Couldn't open the output file");
        }

        init();
        x = 0;
        y = 0;

        unsigned char byte;
        while (in.read(reinterpret_cast<char *>(&byte), 1))
        {
            unsigned char encoded_byte = byte ^ get_s_item();
            out.write(reinterpret_cast<char *>(&encoded_byte), 1);
        }

        in.close();
        out.close();
    }

    static std::vector<unsigned char> generate_key(size_t size)
    {
        srand(static_cast<unsigned>(time(nullptr)));
        std::vector<unsigned char> key(size);
        for (size_t i = 0; i < size; ++i)
        {
            key[i] = static_cast<unsigned char>(rand() % 256);
        }
        return key;
    }
};

int main()
{
    std::vector<unsigned char> key = encoder::generate_key(100);

    encoder encdr(key);

    // Определение имен файлов
    std::string a = "in.txt";     // Входной файл
    std::string b = "code.txt";   // Закодированный файл
    std::string c = "decode.txt"; // Декодированный файл
    std::string d = "decode_2.txt"; // Еще одна декодированная версия

    try
    {
        // Кодирование и декодирование
        encdr.encode(a, b); // Кодирование файла "a" в "b"
        encdr.encode(b, c); // Декодирование "b" обратно в "c"

        // Смена ключа и повторное кодирование
        encdr.set_key(encoder::generate_key(256)); // Установка нового ключа
        encdr.encode(a, b); // Повторное кодирование "a" с новым ключом
        encdr.encode(b, d); // Декодирование "b" обратно в "d"
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
