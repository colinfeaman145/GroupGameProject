#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP
#include <fmod.hpp>
#include <map>
#include <string>
#include <vector>
#include "Vector2.hpp"

#define SOUND_COOLDOWN 0.1

using namespace FMOD;
using namespace std;
class AudioManager {
public:
    AudioManager() = default;
    bool Initialize(const FMOD_VECTOR& listenerPos, float startDelay = 5);
    void Cleanup();
    void Process(Vector2 pos, float deltaTime);

    bool LoadSound(const string& filepath, const string& soundName, float cooldown = SOUND_COOLDOWN);
    bool PlaySound(const string& soundName, const string& groupName, const FMOD_VECTOR& pos, const FMOD_VECTOR& vel, Vector2 pitchVariance);
    bool IsSoundPlaying();

    //group control
    bool AddGroup(const string& name);
    void SetGroupVolume(const string& name, float volume);
    void SetGroupPaused(const string& name, bool paused);
    bool IsGroupPaused(const string& name);
    void StopGroup(const string& name);

    //music
    bool LoadMusicTrack(const string& path, const string& name);
    void PlayMusic(const string& soundName);
    void StopMusic();
    void ProcessMusic();

    System* GetSystem() { return system; }

private:
    System* system = nullptr;
    map<string, Sound*> sounds;
    map<string, float> soundCooldown;
    map<string, float> soundCooldownDuration;
    map<string, ChannelGroup*> groups;
    FMOD_VECTOR listenerPos;
    bool ambientPlaying;
    bool musicPlaying;
    vector<string> musicTracks;
    string currentTrack;
    Channel* musicChannel = nullptr;
    float musicFadeTimer;
    float musicFadeDuration;
    bool betweenSongs;
    float songBreakTimer;
};

#endif