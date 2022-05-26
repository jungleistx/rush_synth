#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "minisynth.h"

#define PI2 6.28318530718

float x1 = 0;
float x2 = 0;
float freq1 = 440;
float freq2 = 0;

void callback(void* userdata, Uint8* stream, int len) {
	(void)userdata;
	short * snd = (short *)(stream);
	len /= sizeof(*snd);
	for(int i = 0; i < len; i++) //Fill array with frequencies, mathy-math stuff
	{
		snd[i] = 32000 * ((sin(x1) + sin(x2)) / 2);
		
		x1 += freq1 * PI2 / 48000.0;
		if(x1 >= PI2)
			x1 -= PI2;
		x2 += freq2 * PI2 / 48000.0;
		if(x2 >= PI2)
			x2 -= PI2;
	}
}

int main(int argc, char ** argv)
{
	(void)argc;
	(void)argv;
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec spec, aspec; // the specs of our piece of "music"
	SDL_zero(spec);
	spec.freq = 48000; //declare specs
	spec.format = AUDIO_S16SYS;
	spec.channels = 1;
	spec.samples = 4096;
	spec.callback = callback;
	spec.userdata = NULL;

  //Open audio, if error, print
	int id;
	if ((id = SDL_OpenAudioDevice(NULL, 0, &spec, &aspec, SDL_AUDIO_ALLOW_ANY_CHANGE)) <= 0 )
	{
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  exit(-1);
	}

	/* Start playing, "unpause" */
	SDL_PauseAudioDevice(id, 0);

	int total_time = 0;
	while(total_time < 8) //Stall for time while audio plays
	{

    	  //Play A
          freq1 = get_frequency("a4");;
          freq2 = get_frequency("c4");
          SDL_Delay(3000);
		total_time += 3;
	  //Play middle C
          //freq1 = 261.6256;
          freq1 = get_frequency("a5");
		  freq2 = get_frequency("c5");
          SDL_Delay(3000);
		  total_time += 3;
		//sleep(1000);
		//printf("hello\n");
          //if needed, you can do cool stuff here, like change frequency for different notes: 
          //https://en.wikipedia.org/wiki/Piano_key_frequencies

          //Another cool thing:
          /*
	  while(true)
	  {
	    for(freq = 440; freq < 880; freq += 10)
	        SDL_Delay(10);
	    for(freq = 870; freq > 450; freq -= 10)
	        SDL_Delay(10);
	  } */
	}
}