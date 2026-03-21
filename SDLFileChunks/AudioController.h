#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include "StandartIncludes.h"

class SoundEffect;
class Song;

static void SDLCALL TrackStoppedCallback(void* _userdata, MIX_Track* _track);

class AudioController : public Singleton<AudioController>
{
public:
    // Constructors/Destructors
    AudioController();
    virtual ~AudioController();

    // Accessors
    string GetMusicTitle() { return m_musicTitle; }
    string GetMusicLength() { return m_musicLength; }
    string GetEffectTitle() { return m_effectTitle; }
    MIX_Track* GetEffect() { return m_effect; }
    MIX_Track* GetMusic() { return m_music; }

    // Methods
    SoundEffect* LoadEffect(string _guid);
    Song* LoadSong(string _guid);
    void PlayTrack(MIX_Track* _track, SoundEffect* _effect);
    void Play(SoundEffect* _effect);
    void Play(Song* _song);
    void StopEffect();
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
    MIX_Track* m_effect;
    MIX_Track* m_music;
    string m_musicLength;
    string m_musicTitle;
    string m_effectTitle;
};

#endif // AUDIOCONTROLLER_H