#pragma once
#include <fstream>
#include <string>
#include <vector>

inline void SaveBufferToFile(const std::string& filename, const std::vector<unsigned char>& data)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) return;

    if (!data.empty())
        out.write((const char*)data.data(), (std::streamsize)data.size());

    out.close();
}
