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
	while (i < track_nb)
	{
		if (tracks_ar[i])
		{
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

float	get_sin_value(float x, int i, int *waves)
{
	float	sin_value;

	sin_value = 0.0;
	if (waves[i] == 2 || waves[i] == 4) // saw & triangle
	{
		sin_value = asin(sin(x)) * (2.0 / M_PI);
	}
	else if (waves[i] == 3) // square
	{
		sin_value = sin(x) < 0 ? -1 : 1;
	}
	else
	{
		sin_value = sin(x);
	}
	return (sin_value);
}

int16_t get_sample(float *x_ar, int track_nb, int *waves)
{
	int	i;
	int16_t	sample;
	double	sin_sum;
	float	sin_value;

	i = 0;
	sample = 0;
	sin_sum = 0.0;
	while (i < track_nb)
	{
		sin_value = get_sin_value(x_ar[i], i, waves);
		sin_sum += sin_value;
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

void	play_melody(t_node **head, int track_nb, int *waves)
{
	float	total_time;
	t_node *list;
	t_node *tmp;
	list = head[0];
	float	x_ar[track_nb];
	int	shortest;
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
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

	initialize_x_ar(x_ar, track_nb);

	i = 0;

	while (!end_of_music(head, track_nb))
	{
		// calc shortest note
		shortest = get_shortest(head, track_nb);

		total_time += head[shortest]->len;

		//queue everything
		for (int p = 0; p < audio_spec.freq * head[shortest]->len; p++) {

			int16_t sample = get_sample(x_ar, track_nb, waves);

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
	}

	SDL_PauseAudioDevice(audio_device, 0);

    SDL_Delay(total_time * 1000);

    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();

}
