/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-maul <mde-maul@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:16:14 by mde-maul          #+#    #+#             */
/*   Updated: 2022/05/26 15:16:16 by mde-maul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minisynth.h"

void	tests(void)
{
	float note1 = get_frequency("a4");
	float note2 = get_frequency("c4");
	float note3 = get_frequency("c6");
	//printf("get frequ b8 %f\n", get_frequency("b8"));
    SDL_Init(SDL_INIT_AUDIO);

    // the representation of our audio device in SDL:
    SDL_AudioDeviceID audio_device;

    // opening an audio device:
    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = 48000;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;

    audio_device = SDL_OpenAudioDevice(
        NULL, 0, &audio_spec, NULL, 0);

    // pushing 3 seconds of samples to the audio buffer:
    float x = 0;
    float x2 = 0;
    float x3 = 0;
    for (int i = 0; i < audio_spec.freq * 2; i++) {

        int16_t sample = ((sin(x) + sin(x2) + sin(x3)) / 3) * 32000;

        int16_t sample2 = sin(x3) * 32000;

		x += note1 * PI2 / 48000.0;
		if(x >= PI2)
			x -= PI2;
		x2 += note2 * PI2 / 48000.0;
		if(x2 >= PI2)
			x2 -= PI2;
		x3 += note3 * PI2 / 48000.0;
		if(x3 >= PI2)
			x3 -= PI2;

        const int sample_size = sizeof(int16_t) * 1;
        SDL_QueueAudio(audio_device, &sample, sample_size);
		(void)sample2;
		(void)sample;
       //SDL_QueueAudio(audio_device, &sample2, sample_size);
    }

    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(audio_device, 0);

    SDL_Delay(2000);

    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();

}

void	print_list(t_node *list)
{
	while (list)
	{
		printf("list freq is %f\n", list->frequency);
		list = list->next;
	}
}


void	fill_node(t_node *list, char *note, int tempo, float duration)
{
	list->frequency = get_frequency(note);
	list->len = tempo / 60 * duration;
	//printf("len is %f \n", tempo / 60 * duration);
	list->next = NULL;
}

int main(void) {
	//tests();
	t_node	*list;
	//t_node	*list2;
	t_node	*list_ptr;
	t_node	*list_new;
	int	tempo = 60;
	char	*melody[8] = {"c4", "d4", "e4", "f4", "g4", "a4", "b4", "c5"};
	//char *wtfsegfault[3] = {"c4", "b5"};
	//char	*melgrgfefewody2[9] = {"c4", "c4", "d4", "e4", "f4", "g4", "a4", "b4", "c5"};
	int i;
	double duration;

(void)melody;
//(void)wtfsegfault;
	i = 0;
	while (i < 8)
	{
		duration = 1.0;
		/*
		if (i % 2)
			duration = 0.5;
		*/
		if (!list)
		{
			list = (t_node *)malloc(sizeof(t_node *));
			if (!list)
				return (1);
			fill_node(list, melody[i], tempo, duration);
		}
		else
		{
			list_ptr = list;
			while (list_ptr->next)
				list_ptr = list_ptr->next;
			list_new = (t_node *)malloc(sizeof(t_node *));
			if (!list_new)
				return (1); 
			fill_node(list_new, melody[i], tempo, duration);
			list_ptr->next = list_new;
		}
		i++;
	}


	//print_list(list);
	play_melody(list);
	//melodious_melody->frequency = get_frequency("c4");
    return 0;
}
