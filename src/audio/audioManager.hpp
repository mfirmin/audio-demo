#pragma once

#include <SDL2/SDL_mixer.h>

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
        Mix_Music* primaryMusic = nullptr;

        Mix_Chunk* soundFX = nullptr;

        void loadMusic();
        void loadSounds();
};
