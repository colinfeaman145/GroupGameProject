#include "AudioManager.hpp"
#include "GameContext.hpp"
#include "Grid.hpp"

bool AudioManager::Initialize(const FMOD_VECTOR& pos, float startDelay) {
    System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, nullptr);
    system->set3DListenerAttributes(0, &pos, nullptr, nullptr, nullptr);
    system->set3DSettings(1.0f, 50, 2.0f);
    betweenSongs = true;
    songBreakTimer = startDelay;
    musicFadeDuration = 10;

    return true;
}

void AudioManager::Cleanup() {
    for (const auto& [name, group] : groups) {
        group->release();
    }
    for (const auto& [name, sound] : sounds) {
        sound->release();
    }
    system->close();
    system->release();
}

void AudioManager::Process(Vector2 pos, float deltaTime) {
    FMOD_VECTOR loc = { pos.x, 100, pos.y };
    system->set3DListenerAttributes(0, &loc, nullptr, nullptr, nullptr);
    system->update();
    for (auto& c : soundCooldown) {
        c.second -= deltaTime;
    }
    songBreakTimer -= deltaTime;

    //fade in music
    if (musicChannel && musicFadeTimer < musicFadeDuration) {
        musicFadeTimer += deltaTime;
        float volume = min(musicFadeTimer / musicFadeDuration, 0.45f);//max volume
        musicChannel->setVolume(volume);
    }

    ProcessMusic();
}

void AudioManager::ProcessMusic() {
    if (IsSoundPlaying()) return; //still playing, do nothing

    if (songBreakTimer > 0) return;
    if (!betweenSongs) {//song ended
        betweenSongs = true;
        uniform_real_distribution<float> songDelayGen(10, 25);
        songBreakTimer = songDelayGen(gen);

        musicFadeTimer = 0;
        return;
    }
    betweenSongs = false;

    if (musicTracks.empty()) return;

    //pick random track, avoid playing same track twice in a row
    uniform_int_distribution<int> trackGen(0, musicTracks.size() - 1);
    string next = musicTracks[trackGen(gen)];

    if (musicTracks.size() > 1)
        while (next == currentTrack)
            next = musicTracks[trackGen(gen)];

    currentTrack = next;
    PlayMusic(currentTrack);
}

bool AudioManager::LoadSound(const string& filepath, const string& soundName, float cooldown) {
    if (sounds.count(filepath)) return 0;

    Sound* sound = nullptr;
    system->createSound(filepath.c_str(), FMOD_2D, nullptr, &sound);   
    sounds[soundName] = sound;
    soundCooldown[soundName] = 0;
    soundCooldownDuration[soundName] = cooldown;
    return 1;
}

/*
* FMOD_VECTOR pos = {x, y, z};
* FMOD_VECTOR vel = {0, 0, 0};
*/
bool AudioManager::PlaySound(const string& soundName, const string& groupName, const FMOD_VECTOR& pos, const FMOD_VECTOR& vel, Vector2 pitchVariance) {
    if (!sounds.contains(soundName)) return 0; //sound not loaded
    if (soundCooldown[soundName] > 0) return 0; //wait for cooldown period

    //set pitch based on variance
    uniform_real_distribution<float> pitchGen(pitchVariance.x, pitchVariance.y);
    groups[groupName]->setPitch(pitchGen(gen));//change group pitch

    Channel* channel = nullptr;
    system->playSound(sounds[soundName], groups[groupName], true, &channel);
    channel->set3DAttributes(&pos, &vel);
    channel->setPaused(false); //unpause once attributes set

    //update cooldown
    soundCooldown[soundName] = soundCooldownDuration[soundName];
    return 1;
}

bool AudioManager::IsSoundPlaying() {
    if (!musicChannel) return false;
    bool playing = false;
    musicChannel->isPlaying(&playing);
    return playing;
}


//GROUP CONTROL
bool AudioManager::AddGroup(const string& name) {
    if (groups.contains(name)) return false;

    ChannelGroup* group = nullptr;
    system->createChannelGroup(name.c_str(), &group);
    groups[name] = group;
    return true;
}

void AudioManager::SetGroupVolume(const string& name, float volume) {
    if (!groups.contains(name)) return;
    groups[name]->setVolume(volume);
}

void AudioManager::SetGroupPaused(const string& name, bool paused) {
    if (!groups.contains(name)) return;
    groups[name]->setPaused(paused);
}

bool AudioManager::IsGroupPaused(const string& name) {
    if (!groups.contains(name)) return 0;
    bool paused = false;
    groups[name]->getPaused(&paused);
    return paused;
}

void AudioManager::StopGroup(const string& name) {
    if (!groups.contains(name)) return;
    groups[name]->stop();
}


//MUSIC
void AudioManager::PlayMusic(const string& soundName) {
    if (!sounds.contains(soundName)) return;

    system->playSound(sounds[soundName], groups["Music"], true, &musicChannel);
    musicChannel->setVolume(0.0f); //start silent
    musicFadeTimer = 0.0f;         //reset fade
    musicChannel->setPaused(false);
}

bool AudioManager::LoadMusicTrack(const string& path, const string& name) {
    if (sounds.count(name)) return false;

    Sound* sound = nullptr;
    system->createSound(path.c_str(), FMOD_2D | FMOD_LOOP_OFF, nullptr, &sound); // 2D, no loop
    sounds[name] = sound;
    musicTracks.push_back(name);
    return true;
}

void AudioManager::StopMusic() {
    if (!musicChannel) return;
    musicChannel->stop();
    musicChannel = nullptr;
    musicFadeTimer = 0.0f;
}