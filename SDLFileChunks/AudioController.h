#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include "StandartIncludes.h"

class SoundEffect;
class Song;

static void SDLCALL TrackStoppedCallback(void* _userdata, MIX_Track* _track);

constexpr int MaxEffectTracks = 16;

struct TrackInfo
{
    MIX_Track* m_track;
    string m_name;

    TrackInfo()
    {
        m_track = nullptr;
        m_name = "";
    }
};

class AudioController : public Singleton<AudioController>
{
public:
    // Constructors/Destructors
    AudioController();
    virtual ~AudioController();

    // Accessors
    string GetMusicTitle() { return m_musicTitle; }
    string GetMusicLength() { return m_musicLength; }
    vector<TrackInfo*>& GetEffects() { return m_effects; }
    MIX_Track* GetMusic() { return m_music; }

    // Methods
    SoundEffect* LoadEffect(string _guid);
    Song* LoadSong(string _guid);
    TrackInfo* FindTrack(MIX_Track* _track);
    void PlayTrack(MIX_Track* _track, SoundEffect* _effect);
    void Play(SoundEffect* _effect);
    void Play(Song* _song);
    void StopEffect(MIX_Track* _track); 
    string MusicPosition();
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    void Shutdown();

private:
    // Methods
    MIX_Audio* GetSDLAudio(SoundEffect* _effect);

    // Members
    MIX_Mixer* m_mixer;
    map<string, MIX_Audio*> m_audio;
    vector<TrackInfo*> m_effects; 
    MIX_Track* m_music;
    string m_musicLength;
    string m_musicTitle;
};

#endif // AUDIOCONTROLLER_H