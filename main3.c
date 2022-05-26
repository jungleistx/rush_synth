#include <stdio.h>
#include "SDL2/SDL.h"
#define PI2 6.28318530718

int main(void) {

	float freq = 440; // a
	//freq = 261.6256; // c
	float freq2 = 261.6256; // c
	float freq3 = 880; // c
    SDL_Init(SDL_INIT_AUDIO);

    // the representation of our audio device in SDL:
    SDL_AudioDeviceID audio_device;

    // opening an audio device:
    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = 48000;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 2;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;

    audio_device = SDL_OpenAudioDevice(
        NULL, 0, &audio_spec, NULL, 0);

    // pushing 3 seconds of samples to the audio buffer:
    float x = 0;
    float x2 = 0;
    float x3 = 0;
    for (int i = 0; i < audio_spec.freq * 2; i++) {
   

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
        int16_t sample = sin(x) * 32000;
        int16_t sample2 = sin(x2) * 32000;
        int16_t sample3 = sin(x3) * 32000;

		x += freq * PI2 / 48000.0;
		if(x >= PI2)
			x -= PI2;
		
		x2 += freq2 * PI2 / 48000.0;
		if(x2 >= PI2)
			x2 -= PI2;
		x3 += freq3 * PI2 / 48000.0;
		if(x3 >= PI2)
			x3 -= PI2;

		(void)sample3;
        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(audio_device, &sample, sample_size);
       SDL_QueueAudio(audio_device, &sample2, sample_size);
       //SDL_QueueAudio(audio_device, &sample3, sample_size);
    }


/*
		  x = 0;
    for (int i = 0; i < audio_spec.freq * 1; i++) {
        x += .010f;

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
        int16_t sample = sin(x * 6) * 5000;

        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(audio_device, &sample, sample_size);
    
    }
*/


    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(audio_device, 0);

    SDL_Delay(4000);

    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();

    return 0;
}
