#include "audioManager.hpp"

#include "wave.hpp"

#include <AL/al.h>
#include <assert.h>
// #include <bit>
#include <fstream>
#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>

AudioManager::AudioManager()
{
    outputDevice = alcOpenDevice(nullptr);
    if(!outputDevice) {
        std::cerr << "Failed to find default audio device\n";
        assert(false);
    }

    context = alcCreateContext(outputDevice, nullptr);
    if (!context) {
        std::cerr << "Failed to find default audio device\n";
        assert(false);
    }

    if (!alcMakeContextCurrent(context)) {
        std::cerr << "Failed to set context as active\n";
        assert(false);
    }

    alGenBuffers(1, &buffer);

    assert(buffer > 0);

    ALenum format = AL_FORMAT_STEREO16;

    auto sword1 = loadWAV("assets/sound/sfx/sword1.wav");

    alBufferData(buffer, format, sword1.data.data(), sword1.data.size(), sword1.sampleRate);
    std::cout << "Size: " << sword1.data.size() << "\n";
//     alBufferData(buffer, format, sword1.data.data(), 1024, sword1.sampleRate);

    alGenSources(1, &source);

    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, buffer);

    auto version = alGetString(AL_VERSION);
    std::cout << "Loaded OpenAL version: " << version << "\n";

//     loadMusic();
     // loadSounds();
//
//     std::cout << "Finished Loading sound infiles\n";
}

AudioManager::~AudioManager() {

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

    alcDestroyContext(context);

    ALCboolean closed = alcCloseDevice(outputDevice);
    if (!closed) {
        std::cerr << "Error closing device\n";
    }

//     //Free the sound effects
//     Mix_FreeChunk(soundFX);
//     soundFX = nullptr;
//
//     //Free the music
//     Mix_FreeMusic(primaryMusic);
//     primaryMusic = nullptr;
//
//     //Quit Mixer
//     Mix_Quit();
}

void AudioManager::loadMusic() {
    // Load music
    // primaryMusic = Mix_LoadMUS("assets/sound/music/Exploration_Cave (loop).wav" );
    // assert(primaryMusic != nullptr);
}

void AudioManager::loadSounds() {
    //Load sound effects
    // soundFX = Mix_LoadWAV( "assets/sound/sfx/fireball.wav" );
    // auto sword1 = loadWAV("assets/sound/sfx/sword1.wav");
//     assert(soundFX != nullptr);
}

void AudioManager::playMusic() {
//     // If not playing music
//     if (Mix_PlayingMusic() == 0) {
//         //Play the music
//         Mix_PlayMusic(primaryMusic, -1);
//     }
}

void AudioManager::playSound() {
    alSourcePlay(source);
//     Mix_PlayChannel(-1, soundFX, 0);
}

void AudioManager::pauseMusic() {
//     // If playing music
//     if (Mix_PlayingMusic() != 0) {
//         //If the music is paused
//         if (Mix_PausedMusic() == 1) {
//             //Resume the music
//             Mix_ResumeMusic();
//         } else {
//             //Pause the music
//             Mix_PauseMusic();
//         }
//     }
}

void AudioManager::stopMusic() {
//     if (Mix_PlayingMusic() != 0) {
//         Mix_HaltMusic();
//     }
}

// const uint8_t channels = 2;
// const uint8_t sampleRate = 44100;
// const uint8_t bufferSize = 2048;

int32_t convert_to_int(char* buffer, std::size_t len)
{
    std::int32_t a = 0;
    // if(std::endian::native == std::endian::little) {
        std::memcpy(&a, buffer, len);
    // } else {
       // for(std::size_t i = 0; i < len; ++i) {
       //     reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
       // }
    // }
    return a;
}

WAV AudioManager::loadWAV(std::string filename) {
    std::ifstream infile(filename.c_str(), std::ios::binary);

    if (!infile.is_open()) {
        std::cout << "ERROR\n";
        assert(false);
    }

    char buffer[4];
    if (!infile.is_open()) {
        assert(false);
    }

    // the RIFF
    if (!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
        assert(false);
    }

    if (std::strncmp(buffer, "RIFF", 4) != 0) {
        std::cerr << "ERROR: infile is not a valid WAVE infile (header doesn't begin with RIFF)" << std::endl;
        assert(false);
    }

    // the size of the infile
    if (!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read size of infile" << std::endl;
        assert(false);
    }

    // the WAVE
    if (!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        assert(false);
    }

    if(std::strncmp(buffer, "WAVE", 4) != 0) {
        std::cerr << "ERROR: infile is not a valid WAVE infile (header doesn't contain WAVE)" << std::endl;
        assert(false);
    }

    // "fmt/0"
    if(!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        assert(false);
    }

    if(std::strncmp(buffer, "fmt ", 4) != 0) {
        std::cerr << "ERROR: infile is not a valid WAVE infile (doesn't have 'fmt ' tag)" << std::endl;
        assert(false);
    }

    // this is always 16, the size of the fmt data chunk
    if(!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        assert(false);
    }

    // PCM should be 1?
    if(!infile.read(buffer, 2)) {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        assert(false);
    }

    // the number of channels
    if(!infile.read(buffer, 2)) {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        assert(false);
    }

    auto channels = convert_to_int(buffer, 2);

    // sample rate
    if(!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        assert(false);
    }

    auto sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if(!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        assert(false);
    }

    // ?? dafaq
    if(!infile.read(buffer, 2)) {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        assert(false);
    }

    // bitsPerSample
    if(!infile.read(buffer, 2)) {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        assert(false);
    }

    auto bitsPerSample = convert_to_int(buffer, 2);

    // data chunk header "data"
    if(!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read chunk header" << std::endl;
        assert(false);
    }

    std::cout << buffer << "\n";
    if (std::strncmp(buffer, "LIST", 4) == 0) {
        if (!infile.read(buffer, 4)) {
            std::cerr << "Error: Could not read LIST size\n";
            assert(false);
        }
        auto listSize = convert_to_int(buffer, 4);

        if (!infile.ignore(listSize)) {
            std::cerr << "Error: Could not read LIST body\n";
            assert(false);
        }

        // read the next chunk header...
        if(!infile.read(buffer, 4)) {
            std::cerr << "ERROR: could not read data chunk header" << std::endl;
            assert(false);
        }
    }


    if(std::strncmp(buffer, "data", 4) != 0) {
        std::cerr << "ERROR: infile is not a valid WAVE infile (doesn't have 'data' tag)" << std::endl;
        assert(false);
    }

    // size of data
    if(!infile.read(buffer, 4)) {
        std::cerr << "ERROR: could not read data size" << std::endl;
        assert(false);
    }

    auto size = convert_to_int(buffer, 4);

    /* cannot be at the end of infile */
    if(infile.eof()) {
        std::cerr << "ERROR: reached EOF on the infile" << std::endl;
        assert(false);
    }

    if(infile.fail()) {
        std::cerr << "ERROR: fail state set on the infile" << std::endl;
        assert(false);
    }

    WAV wav;
    wav.data.resize(size);
    wav.channels = channels;
    wav.sampleRate = sampleRate;
    wav.bitsPerSample = bitsPerSample;

    infile.read(wav.data.data(), size);

    std::cout << "Loaded " << filename << "\n";
    std::cout << "Channels: " << static_cast<int>(channels) << "\n";
    std::cout << "Sample Rate: " << static_cast<int>(sampleRate) << "\n";
    std::cout << "Bits per Sample: " << static_cast<int>(bitsPerSample) << "\n";

    return wav;
}
