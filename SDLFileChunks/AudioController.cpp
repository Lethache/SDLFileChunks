#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"
#include "Asset.h"
#include "ObjectPool.h"
#include "Resource.h"
// --- Callback ---
void SDLCALL TrackStoppedCallback(void* _userdata, MIX_Track* _track)
{
    if (_track == AudioController::Instance().GetMusic())
    {
        AudioController::Instance().StopMusic();
    }
    if (_track == AudioController::Instance().GetEffect())
    {
        AudioController::Instance().StopEffect();
    }
}

// --- Lifecycle ---
AudioController::AudioController()
{
    M_ASSERT(SDL_InitSubSystem(SDL_INIT_AUDIO) == true,
        "Failed to initialize SDL Audio Subsystem");
    M_ASSERT(MIX_Init() == true, "Failed to initialize SDL Mixer");

    m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    M_ASSERT((m_mixer != nullptr), "Failed to initialize Mix Audio");

    SoundEffect::Pool = new ObjectPool<SoundEffect>();
    m_music = nullptr;
    m_effect = nullptr;
}

AudioController::~AudioController()
{
    Shutdown();
}

// --- Loading ---
SoundEffect* AudioController::LoadEffect(string _guid)
{
    SoundEffect* effect = SoundEffect::Pool->GetResource();
    effect->Load(_guid);
    return effect;
}

Song* AudioController::LoadSong(string _guid)
{
    SoundEffect* song = SoundEffect::Pool->GetResource();
    song->Load(_guid);
    return static_cast<Song*>(song);
}

MIX_Audio* AudioController::GetSDLAudio(SoundEffect* _effect)
{
    Asset* asset = _effect->GetData();
    string guid = asset->GetGUID();

    if (m_audio.count(guid) == 0)
    {
        // If not found create the SDL io buffer from the asset data
        SDL_IOStream* stream = SDL_IOFromConstMem((void*)asset->GetData(), asset->GetDataSize());
        MIX_Audio* audio = MIX_LoadAudio_IO(m_mixer, stream, true, true);
        m_audio[guid] = audio;
    }
    return m_audio[guid];
}

// --- Playback Control ---
void AudioController::PlayTrack(MIX_Track* _track, SoundEffect* _effect)
{
    M_ASSERT(MIX_SetTrackAudio(_track, GetSDLAudio(_effect)) == true,
        "Failed to set SFX audio");

    SDL_PropertiesID props = SDL_CreateProperties();
    M_ASSERT(SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, 0) == true,
        "Failed to set SFX loop property");

    M_ASSERT(MIX_PlayTrack(_track, props) == true, "Failed to play SFX");
    M_ASSERT(MIX_SetTrackStoppedCallback(_track, TrackStoppedCallback, nullptr) == true,
        "Failed to set SFX stopped callback");
}

void AudioController::Play(SoundEffect* _effect)
{
    if (m_effect != nullptr)
    {
        StopEffect();
    }
    m_effect = MIX_CreateTrack(m_mixer);
    M_ASSERT(m_effect != nullptr, "Failed to create SFX track");
    PlayTrack(m_effect, _effect);
    m_effectTitle = _effect->GetData()->GetGUID();
}

void AudioController::Play(Song* _song)
{
    if (m_music != nullptr)
    {
        StopMusic();
    }
    m_music = MIX_CreateTrack(m_mixer);
    M_ASSERT(m_music != nullptr, "Failed to create SFX track");
    PlayTrack(m_music, _song);

    MIX_Audio* audio = MIX_GetTrackAudio(GetMusic());
    Sint64 duration = MIX_GetAudioDuration(audio);
    float duration_sec = MIX_AudioFramesToMS(audio, duration);
    m_musicLength = to_string(duration_sec / 1000);

    SDL_PropertiesID props = MIX_GetAudioProperties(audio);
    m_musicTitle = SDL_GetStringProperty(props, MIX_PROP_METADATA_TITLE_STRING, "Unknown Title");
}

// --- Transport ---
void AudioController::StopEffect()
{
    if (m_effect == nullptr) return;
    MIX_StopTrack(m_effect, 0);
    MIX_DestroyTrack(m_effect);
    m_effect = nullptr;
    m_effectTitle = "";
}

string AudioController::MusicPosition()
{
    if (m_music != nullptr)
    {
        Sint64 pos = MIX_GetTrackPlaybackPosition(m_music);
        float pos_ms = MIX_TrackFramesToMS(m_music, pos);
        return to_string(pos_ms / 1000);
    }
    return "";
}

void AudioController::StopMusic()
{
    if (m_music == nullptr) return;
    MIX_StopTrack(m_music, 0);
    MIX_DestroyTrack(m_music);
    m_music = nullptr;
    m_musicLength = "";
    m_musicTitle = "";
}

void AudioController::PauseMusic()
{
    if (m_music == nullptr) return;
    MIX_PauseTrack(m_music);
}

void AudioController::ResumeMusic()
{
    if (m_music == nullptr) return;
    MIX_ResumeTrack(m_music);
}

// --- Shutdown ---
void AudioController::Shutdown()
{
    for (auto ie = m_audio.begin(); ie != m_audio.end(); ie++)
    {
        MIX_DestroyAudio(ie->second);
    }
    m_audio.clear();

    if (m_music != nullptr)
    {
        MIX_DestroyTrack(m_music);
        m_music = nullptr;
    }

    if (m_effect != nullptr)
    {
        MIX_DestroyTrack(m_effect);
        m_effect = nullptr;
    }

    if (m_mixer != nullptr)
    {
        MIX_DestroyMixer(m_mixer); // This also closes the audio device
        m_mixer = nullptr;
    }

    delete SoundEffect::Pool;
    SoundEffect::Pool = nullptr;

    // Quit SDL_mixer and SDL
    MIX_Quit();
    SDL_Quit();
}