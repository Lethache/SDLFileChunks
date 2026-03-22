#ifndef WAV_DRAW_H
#define WAV_DRAW_H

#include "StandartIncludes.h"

class Renderer;
class Asset;

#pragma pack(push, 1)
typedef struct
{
    char RIFFHeader[4];
    unsigned int WavSize;
    char WavHeader[4];
    char FMTHeader[4];
    unsigned int FMTChunkSize;
    unsigned short AudioFormat;
    unsigned short NumChannels;
    unsigned int SampleRate;
    unsigned int ByteRate;
    unsigned short SampleAlignment;
    unsigned short BitDepth;
    char DataHeader[4];
    unsigned int DataBytes;
} WAVHeader;
#pragma pack(pop)

class WAVDraw
{
public:
    WAVDraw();
    virtual ~WAVDraw();

    void DrawWave(Asset* _rawWav, Renderer* _renderer, float _yZoom);

private:
    bool CompareFileExt(string _source, string _ext);
    void IntegrityChecks(Asset* _rawWav);

    WAVHeader m_header;
    Asset* m_data;
    Point m_wSize;
    int m_stepSize;
    short* m_wavData;
    int m_xPos;
    Point m_prevLeft;
    Point m_prevRight;
};

#endif // WAV_DRAW_H