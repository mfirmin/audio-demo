#include "audioManager.hpp"

#include <assert.h>
#include <iostream>
#include <SDL2/SDL.h>

AudioManager::AudioManager()
{
    //Initialize SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "Failed to initialize Audio\n";
        assert(false);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0) {
        std::cout << "Failed to initialize SDL Mixer\n";
        assert(false);
    }

    std::cout << "AudioManager Initialized!\n";

    loadMusic();
    loadSounds();

    std::cout << "Finished Loading sound files\n";
}

AudioManager::~AudioManager() {
    //Free the sound effects
    Mix_FreeChunk(soundFX);
    soundFX = nullptr;

    //Free the music
    Mix_FreeMusic(primaryMusic);
    primaryMusic = nullptr;

    //Quit Mixer
    Mix_Quit();
}

void AudioManager::loadMusic() {
    // Load music
    primaryMusic = Mix_LoadMUS("assets/sound/music/Exploration_Cave (loop).wav" );
    assert(primaryMusic != nullptr);
}

void AudioManager::loadSounds() {
    //Load sound effects
    soundFX = Mix_LoadWAV( "assets/sound/sfx/fireball.wav" );
    assert(soundFX != nullptr);
}

void AudioManager::playMusic() {
    // If not playing music
    if (Mix_PlayingMusic() == 0) {
        //Play the music
        Mix_PlayMusic(primaryMusic, -1);
    }
}

void AudioManager::playSound() {
    Mix_PlayChannel(-1, soundFX, 0);
}

void AudioManager::pauseMusic() {
    // If playing music
    if (Mix_PlayingMusic() != 0) {
        //If the music is paused
        if (Mix_PausedMusic() == 1) {
            //Resume the music
            Mix_ResumeMusic();
        } else {
            //Pause the music
            Mix_PauseMusic();
        }
    }
}

void AudioManager::stopMusic() {
    if (Mix_PlayingMusic() != 0) {
        Mix_HaltMusic();
    }
}
