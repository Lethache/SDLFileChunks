#pragma once
#include <vector>

struct ImageBufferState
{
    std::vector<unsigned char> buffer;
    int currentChunkIndex = 0;        

    bool HasBuffer() const { return !buffer.empty(); }

    void Reset()
    {
        buffer.clear();
        currentChunkIndex = 0;
    }
};
