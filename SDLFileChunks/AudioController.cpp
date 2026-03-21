#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"
#include "Asset.h"
#include "AssetController.h"





void SDLCALL TrackStoppedCallback(void* _userdata, MIX_Track* _track)
{
    if (_track == AudioController::Instance().GetMusic())
    {
        AudioController::Instance().StopMusic();
    }

    TrackInfo* effect = AudioController::Instance().FindTrack(_track);
    if (effect != nullptr)
    {
        AudioController::Instance().StopEffect(_track);
    }
}


AudioController::AudioController()
{
    M_ASSERT(SDL_InitSubSystem(SDL_INIT_AUDIO) == true, "Failed to initialize SDL Audio");
    M_ASSERT(MIX_Init() == true, "Failed to initialize SDL Mixer");

    m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    M_ASSERT((m_mixer != nullptr), "Failed to initialize Mix Audio");

    SoundEffect::Pool = new ObjectPool<SoundEffect>();
    m_music = nullptr;
}

AudioController::~AudioController()
{
    Shutdown();
}


TrackInfo* AudioController::FindTrack(MIX_Track* _track)
{
    vector<TrackInfo*>& effects = GetEffects();
    auto it = std::find_if(effects.begin(), effects.end(), [_track](TrackInfo* info) {
        return info->m_track == _track;
        });
    return (it != effects.end()) ? *it : nullptr;
}

MIX_Audio* AudioController::GetSDLAudio(SoundEffect* _effect)
{
    Asset* asset = _effect->GetData();
    string guid = asset->GetGUID();

    if (m_audio.count(guid) == 0)
    {
        SDL_IOStream* stream = SDL_IOFromConstMem((void*)asset->GetData(), asset->GetDataSize());
        MIX_Audio* audio = MIX_LoadAudio_IO(m_mixer, stream, true, true);
        m_audio[guid] = audio;
    }
    return m_audio[guid];
}


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


void AudioController::PlayTrack(MIX_Track* _track, SoundEffect* _effect)
{
    M_ASSERT(MIX_SetTrackAudio(_track, GetSDLAudio(_effect)) == true, "Failed to set audio");

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, 0);

    M_ASSERT(MIX_PlayTrack(_track, props) == true, "Failed to play track");
    MIX_SetTrackStoppedCallback(_track, TrackStoppedCallback, nullptr);
}

void AudioController::Play(SoundEffect* _effect)
{
    if (m_effects.size() >= MaxEffectTracks) return;

    TrackInfo* trackInfo = new TrackInfo();
    trackInfo->m_track = MIX_CreateTrack(m_mixer);
    trackInfo->m_name = _effect->GetData()->GetGUID();
    m_effects.push_back(trackInfo);

    PlayTrack(trackInfo->m_track, _effect);
}

void AudioController::Play(Song* _song)
{
    if (m_music != nullptr) StopMusic();

    m_music = MIX_CreateTrack(m_mixer);
    PlayTrack(m_music, _song);

    MIX_Audio* audio = MIX_GetTrackAudio(m_music);
    m_musicLength = to_string(MIX_AudioFramesToMS(audio, MIX_GetAudioDuration(audio)) / 1000);

    SDL_PropertiesID props = MIX_GetAudioProperties(audio);
    m_musicTitle = SDL_GetStringProperty(props, MIX_PROP_METADATA_TITLE_STRING, "Unknown Title");
}


void AudioController::StopEffect(MIX_Track* _track)
{
    if (_track == nullptr) return;
    MIX_StopTrack(_track, 0);
    MIX_DestroyTrack(_track);

    TrackInfo* effect = FindTrack(_track);
    if (effect != nullptr)
    {
        auto it = std::find(m_effects.begin(), m_effects.end(), effect);
        if (it != m_effects.end()) m_effects.erase(it);
        delete effect;
    }
}

void AudioController::StopMusic()
{
    if (m_music == nullptr) return;
    MIX_StopTrack(m_music, 0);
    MIX_DestroyTrack(m_music);
    m_music = nullptr;
}

void AudioController::PauseMusic() { if (m_music) MIX_PauseTrack(m_music); }
void AudioController::ResumeMusic() { if (m_music) MIX_ResumeTrack(m_music); }

string AudioController::MusicPosition()
{
    if (m_music == nullptr) return "0";
    Sint64 pos = MIX_GetTrackPlaybackPosition(m_music);
    return to_string(MIX_TrackFramesToMS(m_music, pos) / 1000);
}

// --- Shutdown ---
void AudioController::Shutdown()
{
    for (auto const& [guid, audio] : m_audio) MIX_DestroyAudio(audio);
    m_audio.clear();

    for (auto* effect : m_effects) {
        MIX_DestroyTrack(effect->m_track);
        delete effect;
    }
    m_effects.clear();

    if (m_music) MIX_DestroyTrack(m_music);
    if (m_mixer) MIX_DestroyMixer(m_mixer);

    delete SoundEffect::Pool;
    SoundEffect::Pool = nullptr;

    MIX_Quit();
    SDL_Quit();
}