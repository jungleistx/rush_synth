#include <stdio.h>
#include "SDL2/SDL.h"
#include "libft/libft.h"
#define PI2 6.28318530718

int notes_match(char *note1, char *note2, int char_nb)
{
	if (ft_strncmp(note1, note2, char_nb) == 0)
		return (1);
	return (0);
}

double	get_note(char *note)
{
	double frequency = 0.0;

	if (notes_match("c", note, 1))
		frequency = 16.35160;
	if (notes_match("c#", note, 2))
		frequency = 17.32391;
	if (notes_match("d", note, 1))
		frequency = 18.35405;
	if (notes_match("d#", note, 2))
		frequency = 19.44544;
	if (notes_match("e", note, 1))
		frequency = 20.60172;
	if (notes_match("f", note, 1))
		frequency = 21.82676;
	if (notes_match("f#", note, 2))
		frequency = 23.12465;
	if (notes_match("g", note, 1))
		frequency = 24.49971;
	if (notes_match("g#", note, 2))
		frequency = 25.95654;
	if (notes_match("a", note, 1))
		frequency = 27.50000;
	if (notes_match("a#", note, 2))
		frequency = 29.13524;
	if (notes_match("b", note, 1))
		frequency = 30.86771;
	return (frequency);
}

float get_frequency(char *note)
{
	float frequency;
	int octave;
	size_t len;

	len = ft_strlen(note);
	if (ft_strchr(note, '#'))
	{
		if (len == 2)
			octave = 4;
		else
			octave = ft_atoi(&note[2]);
	}
	else
	{
		if (len == 1)
			octave = 4;
		else
			octave = ft_atoi(&note[1]);
	}
	frequency = get_note(note) * pow(2, octave);
	//printf("octave is %d\n", octave);
	return (frequency);
}

int main(void) {
	float note1 = get_frequency("a4");
	float note2 = get_frequency("c4");
	//printf("get frequ b8 %f\n", get_frequency("b8"));
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
    for (int i = 0; i < audio_spec.freq * 2; i++) {
   

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
        int16_t sample = sin(x) * 32000;
        int16_t sample2 = sin(x2) * 32000;

		x += note1 * PI2 / 48000.0;
		if(x >= PI2)
			x -= PI2;
		x2 += note2 * PI2 / 48000.0;
		if(x2 >= PI2)
			x2 -= PI2;

        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(audio_device, &sample, sample_size);
       SDL_QueueAudio(audio_device, &sample2, sample_size);
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
