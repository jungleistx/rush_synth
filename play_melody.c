/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_melody.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-maul <mde-maul@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 22:26:33 by mde-maul          #+#    #+#             */
/*   Updated: 2022/05/26 22:26:35 by mde-maul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minisynth.h"

void	print_track(t_node *head)
{
	while (head)
	{
		printf("%f, ", head->frequency);
		head = head->next;
	}
	printf("\n");
}

void print_whole_file(t_node **head, int tracks)
{
	int i;

	i = 0;
	while (i < tracks)
	{
		print_track(head[i]);
		i++;
	}
}

void	initialize_x_ar(float *x_ar, int track_nb)
{
	int i;

	i = 0;
	while (i < track_nb)
	{
		x_ar[i] = 0;
		i++;
	}
}

int	end_of_music(t_node **tracks_ar, int track_nb)
{
	int	i;

	i = 0;
	while (i < track_nb)
	{
		if (tracks_ar[i])
			return (0);
		i++;
	}
	return (1);
}

int	get_shortest(t_node **tracks_ar, int track_nb)
{
	int	i;
	int	shortest;

	shortest = 0;
	i = 0;
	while (i < track_nb)
	{
		if (tracks_ar[i])
		{
			shortest = i;
			break;
		}
		i++;
	}
	i = 0;
	//printf("hey3 track nb %d\n", track_nb);
	while (i < track_nb)
	{
		if (tracks_ar[i])
		{
			//printf("hey4 %d, len %f\n", i, tracks_ar[i]->len);
			if (tracks_ar[i]->len < tracks_ar[shortest]->len)
				shortest = i;
		}
		i++;
	}
	return (shortest);
}

void	update_notes_length(t_node **tracks_ar, int track_nb, int shortest)
{
	float shortest_len;
	int	i;

	shortest_len = tracks_ar[shortest]->len;
	i = 0;
	while (i < track_nb)
	{
		if (i != shortest && tracks_ar[i])
			tracks_ar[i]->len -= shortest_len;
		i++;
	}
}

int16_t get_sample(float *x_ar, int track_nb)
{
	int	i;
	int16_t	sample;
	double	sin_sum;
	
	i = 0;
	sample = 0;
	sin_sum = 0.0;
	while (i < track_nb)
	{
		sin_sum += sin(x_ar[i]);
		i++;
	}
	sample = sin_sum / track_nb * 32000;
	return (sample);
}

void	update_x_ar(float *x_ar, t_node **tracks_ar, int track_nb)
{
	int	i;

	i = 0;
	while (i < track_nb)
	{
		if (tracks_ar[i])
		{
			x_ar[i] += tracks_ar[i]->frequency * PI2 / 48000.0;
			if(x_ar[i] >= PI2)
				x_ar[i] -= PI2;
		}
		else
			x_ar[i] = 0;
		i++;
	}
}

void	play_melody(t_node **head, int track_nb)
{
	float x = 0;
	(void)x;
	float	total_time;
	t_node *list;
	t_node *tmp;
	list = head[0];
	float	x_ar[track_nb];
	(void)x_ar;

	int	shortest;
	(void)shortest;

	int i;


	total_time = 0;
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

    audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

	//print_whole_file(head, track_nb);


	int h = 0;
	while (h < track_nb)
	{
		if (head[h])
			printf("track nb %d, first note is %f len is %f\n", h, head[h]->frequency, head[h]->len);
		h++;
	}

	initialize_x_ar(x_ar, track_nb);

	i = 0;

int l = 0;
	while (!end_of_music(head, track_nb))
	{
		// calc shortest note
		shortest = get_shortest(head, track_nb);
		//printf("shorstest is %d\n", shortest);
		total_time += head[shortest]->len;

		
	

		//queue everything
		for (int p = 0; p < audio_spec.freq * head[shortest]->len; p++) {

			int16_t sample = get_sample(x_ar, track_nb);

			update_x_ar(x_ar, head, track_nb);

			const int sample_size = sizeof(int16_t) * 1;
			SDL_QueueAudio(audio_device, &sample, sample_size);
		}

		// update all the other notes length (all but shortest)
		update_notes_length(head, track_nb, shortest);

		// move to next note (shortest)
		tmp = head[shortest]->next;
		free(head[shortest]);
		head[shortest] = NULL;
		head[shortest] = tmp;
		

		l++;
	}



	
	/*while (list)
	{
		x = 0;
		for (int i = 0; i < audio_spec.freq * list->len; i++) {
			int16_t sample = sin(x) * 32000;
			x += list->frequency * PI2 / 48000.0;
			if(x >= PI2)
				x -= PI2;
			const int sample_size = sizeof(int16_t) * 1;
			SDL_QueueAudio(audio_device, &sample, sample_size);
		}
		total_time += list->len;
		list = list->next;
	}*/

	SDL_PauseAudioDevice(audio_device, 0);

	printf("total time is %f\n", total_time);
    SDL_Delay(total_time * 1000);

    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();

}


// gcc -Wall -Werror -Wextra main.c get_frequency.c play_melody.c  -I ./include -L libft -lft -L lib -lSDL2