#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include "RISC-V.h"

class Memory
{
private:
    static const int MAX_N = 1 << 20;
    std::vector<unsigned> storage;
    char bytes_buf[20];

    char getChar()
    {
        char ch = getchar();
        while (ch != -1 && ch != '@' &&
               !isdigit(ch) && !isupper(ch))
            ch = getchar();
        return ch;
    }

    unsigned Bytes2Int(int n_bytes = 2) // base 16
    {
        static const unsigned HEX_BASE = 16;
        unsigned ret = 0;
        for (int i = 0; i < n_bytes; ++i)
        {
            char ch = bytes_buf[i];
            ret = ret * HEX_BASE + (unsigned)(ch >= 'A' ? ch - 'A' + 10 : ch - '0');
        }
        return ret;
    }

public:
    Memory(const char *fname = "input.data") : storage(MAX_N)
    {
        freopen(fname, "r", stdin);
        std::memset(bytes_buf, 0, sizeof(bytes_buf));
    }

    ~Memory() { fclose(stdin); }

    void initMemory()
    {
        unsigned current_addr = 0;
        char current_char = getchar();
        while (current_char != -1)
        {
            if (current_char == '@')
            {
                // address
                static const int ADDR_LEN = 8;
                for (auto i = 0; i < ADDR_LEN; ++i)
                    bytes_buf[i] = getChar();
                current_addr = Bytes2Int(ADDR_LEN);
            }
            else
            {
                // one byte
                bytes_buf[0] = current_char;
                bytes_buf[1] = getChar();
                storage[current_addr++] = Bytes2Int();
            }
            current_char = getChar();
        }
    }

    void printMemory()
    {
        std::cout << "Memory:" << std::endl;
        for (auto i = 0; i < MAX_N; ++i)
            if (storage[i] != 0)
                std::cout << "Addr " << std::hex << i
                          << std::setw(2) << std::setfill('0')
                          << " : " << storage[i] << '\n';
        std::cout << std::dec << std::endl;
    }

    void store(unsigned addr, unsigned bytes_value, int n_bytes)
    {
        static const unsigned BYTE_MASK = 0xff;
        static const unsigned BYTE_OFFSET = 8;

        for (int i = 0; i < n_bytes; ++i)
        {
            storage[addr + i] = bytes_value & BYTE_MASK;
            bytes_value >>= BYTE_OFFSET;
        }
    }

    unsigned load(unsigned addr, int n_bytes)
    {
        static const unsigned BYTE_OFFSET = 8;

        unsigned ret = 0;
        for (int i = n_bytes - 1; i >= 0; --i)
            ret = (ret << BYTE_OFFSET) + storage[addr + i];
        return ret;
    }
};

#endif