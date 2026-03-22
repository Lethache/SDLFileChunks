#include "WavDraw.h"
#include "AssetController.h"
#include "Renderer.h"
#include "Asset.h"

WAVDraw::WAVDraw()
{
    m_header = { };
    m_data = nullptr;
    m_wSize = { 0, 0 };
    m_stepSize = 0;
    m_wavData = nullptr;
    m_xPos = 0;
    m_prevLeft = { 0, 0 };
    m_prevRight = { 0, 0 };
}

WAVDraw::~WAVDraw() {}

bool WAVDraw::CompareFileExt(string _source, string _ext)
{
    std::transform(_source.begin(), _source.end(), _source.begin(), [](unsigned char c) {
        return (unsigned char)std::tolower(c);
        });
    if (_source.length() >= _ext.length())
        return (0 == _source.compare(_source.length() - _ext.length(), _ext.length(), _ext));
    return false;
}

void WAVDraw::IntegrityChecks(Asset* _rawWav)
{
    M_ASSERT(_rawWav->GetDataSize() > 44, "Not a valid WAV file");
    memcpy(&m_header, _rawWav->GetData(), sizeof(WAVHeader));

    string guid = _rawWav->GetGUID();
    M_ASSERT(CompareFileExt(guid, ".wav") == true, "Not a WAV file");
    M_ASSERT(m_header.AudioFormat == 1, "Only PCM supported");
    M_ASSERT(m_header.NumChannels == 2, "Only Stereo supported");
}

void WAVDraw::DrawWave(Asset* _rawWav, Renderer* _renderer, float _yZoom)
{
    IntegrityChecks(_rawWav);
    m_wSize = _renderer->GetWindowSize();

    _renderer->SetDrawColor(SDL_Color{ 128, 128, 128, 255 });
    _renderer->RenderLine(SDL_FRect{ 0, (float)m_wSize.y / 4, (float)m_wSize.x, 0 });
    _renderer->RenderLine(SDL_FRect{ 0, (float)m_wSize.y * 3 / 4, (float)m_wSize.x, 0 });

    m_stepSize = m_header.DataBytes / m_header.SampleAlignment / m_wSize.x * 2;
    if (m_stepSize < 2) m_stepSize = 2; // Защита от деления на 0

    m_wavData = (short*)((unsigned char*)_rawWav->GetData() + 44);
    m_xPos = 0;

    for (int count = 0; count < (int)m_header.DataBytes / 2; count += m_stepSize)
    {
        int leftYPos = *(m_wavData + count);
        leftYPos = (int)((m_wSize.y / 4) + ((float)leftYPos / 32768.0f) * (m_wSize.y / 4) * _yZoom * -1);

        int rightYPos = *(m_wavData + count + 1);
        rightYPos = (int)((m_wSize.y * 3 / 4) + ((float)rightYPos / 32768.0f) * (m_wSize.y / 4) * _yZoom * -1);

        if (m_xPos > 0)
        {
            _renderer->SetDrawColor(SDL_Color{ 255, 0, 0, 255 }); // Левый - Красный
            _renderer->RenderLine(SDL_FRect{ (float)m_prevLeft.x, (float)m_prevLeft.y, (float)m_xPos - m_prevLeft.x, (float)leftYPos - m_prevLeft.y });

            _renderer->SetDrawColor(SDL_Color{ 0, 0, 255, 255 }); // Правый - Синий
            _renderer->RenderLine(SDL_FRect{ (float)m_prevRight.x, (float)m_prevRight.y, (float)m_xPos - m_prevRight.x, (float)rightYPos - m_prevRight.y });
        }

        m_prevLeft = { m_xPos, leftYPos };
        m_prevRight = { m_xPos, rightYPos };
        m_xPos++;
        if (m_xPos >= m_wSize.x) break;
    }
}