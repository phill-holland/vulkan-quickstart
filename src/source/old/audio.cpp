#include "audio.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "pstring.h"

void audio::reset()
{
    init = false; cleanup();

    init = true;
}

void audio::go()
{
    Mix_Chunk* _sample[1];
    _sample[0] = NULL;

    // memset(_sample, 0, sizeof(Mix_Chunk*) * 2);

    // Set up the audio stream
    int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    if( result < 0 )
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(-1);
    }

    result = Mix_AllocateChannels(4);
    if( result < 0 )
    {
        fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
        exit(-1);
    }

    // Load waveforms
   // for( int i = 0; i < NUM_WAVEFORMS; i++ )
    //{
        _sample[0] = Mix_LoadWAV(string("audio/drum.wav").c_str());//_waveFileNames[i]);
        if( _sample[0] == NULL )
        {
            return;
            //fprintf(stderr, "Unable to load wave file: %s\n", _waveFileNames[i]);
        }
    //}

    Mix_PlayChannel(-1, _sample[0], 0);

    // ****

        //for( int i = 0; i < NUM_WAVEFORMS; i++ )
    //{
        Mix_FreeChunk(_sample[0]);
    //}

    Mix_CloseAudio();
    SDL_Quit();
}

void audio::makeNull()
{

}

void audio::cleanup()
{

}