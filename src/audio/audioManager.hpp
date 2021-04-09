#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

struct WAV;

class AudioManager {
    public:
        AudioManager();

        AudioManager(AudioManager&& other) = default;
        AudioManager& operator=(AudioManager&& other) = default;

        AudioManager(const AudioManager& other) = delete;
        AudioManager& operator=(const AudioManager& other) = delete;

        ~AudioManager();

        void playMusic();
        void pauseMusic();
        void stopMusic();

        void playSound();
    private:
        ALCdevice* outputDevice = nullptr;
        ALCcontext* context = nullptr;
        ALuint buffer;

        ALuint source;

        void loadMusic();
        void loadSounds();

        WAV loadWAV(std::string filename);
};
