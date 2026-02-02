
#include "StackAllocator.h"
#include <cstring>

StackAllocator::StackAllocator()
    : m_marker(nullptr), m_stackStart(nullptr), m_stackPosition(nullptr), m_stackEnd(nullptr)
{
}

StackAllocator::~StackAllocator()
{
    delete[] m_stackStart;
    m_marker = m_stackStart = m_stackPosition = m_stackEnd = nullptr;
}

void StackAllocator::AllocateStack(unsigned int stackSizeBytes)
{
    delete[] m_stackStart;

    m_stackStart = new unsigned char[stackSizeBytes];
    m_stackPosition = m_stackStart;
    m_marker = m_stackStart;
    m_stackEnd = m_stackStart + stackSizeBytes;
}

unsigned char* StackAllocator::GetMemory(unsigned int sizeBytes)
{
    if (m_stackPosition + sizeBytes > m_stackEnd)
        return nullptr;

    unsigned char* mem = m_stackPosition;
    m_stackPosition += sizeBytes;
    return mem;
}

void StackAllocator::Mark()
{
    m_marker = m_stackPosition;
}

void StackAllocator::FreeToMarker()
{
    m_stackPosition = m_marker;
}

void StackAllocator::ClearMemory()
{
    m_stackPosition = m_stackStart;
    m_marker = m_stackStart;
}
void StackAllocator::Reset()
{
    m_stackPosition = m_stackStart;
    m_marker = m_stackStart;
}