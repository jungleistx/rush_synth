/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvuorenl <rvuorenl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 14:21:09 by rvuorenl          #+#    #+#             */
/*   Updated: 2022/05/26 21:11:15 by rvuorenl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minisynth.h"

// tracks triangle,sine,sine,sine,sine,sine,sine,saw,saw,saw,saw,saw,saw,sine,sine,sine,sine,sine,triangle,triangle,triangle,square,square,square,kick,snare

int	tracks_line(char *str, int *arr)
{
	int res = 0;
	int i = 6;	// skip beginning "tracks "
	int elem = 0;

	while (str[++i])
	{
		if (strncmp((const char*)&str[i], "sine", 4) == 0)
		{
			arr[elem++] = 1;
			res++;
			i += 4;
		}
		else if (strncmp((const char*)&str[i], "saw", 3) == 0)
		{
			arr[elem++] = 2;
			res++;
			i += 3;
		}
		else if (strncmp((const char*)&str[i], "square", 6) == 0)
		{
			arr[elem++] = 3;
			res++;
			i += 5;
		}
		else if (strncmp((const char*)&str[i], "triangle", 8) == 0)
		{
			arr[elem++] = 4;
			res++;
			i += 8;
		}
		else if (strncmp((const char*)&str[i], "kick", 4) == 0)		//?
		{
			arr[elem++] = 5;
			res++;
			i += 4;
		}
		else if (strncmp((const char*)&str[i], "snare", 5) == 0)	//?
		{
			arr[elem++] = 6;
			res++;
			i += 5;
		}
	}
	return (res);
}

// values for waveform
// 1 sine waves
// 2 saw waves
// 3 square waves
// 4 triangle waves
// 5 kick
// 6 snare

void	fill_node(t_node *list, char *note, float len)
{
	list->frequency = get_frequency(note);
	list->len = len;
	list->next = NULL;
}

void	feed_to_list(t_node **head, char *note, float len)
{
	t_node	*list_ptr;
	t_node	*list_new;

	if (!*head)
	{
		*head = (t_node *)malloc(sizeof(t_node));
		if (!*head)
			return ;
		fill_node(*head, note, len);
	}
	else
	{
		list_ptr = *head;
		while (list_ptr->next)
			list_ptr = list_ptr->next;
		list_new = (t_node *)malloc(sizeof(t_node));
		if (!list_new)
			return ;
		fill_node(list_new, note, len);
		list_ptr->next = list_new;
	}
}

void	update_octave(char	*note, int octave)
{
	size_t	len;

	len = strlen(note);
	if (len == 1)
	{
		note[1] = octave + '0';
	}
	if ((len == 2 && note[1] == '#'))
	{
		note[2] = octave + '0';
	}
}

void	read_input(char *str, int tempo, t_node **head)
{
	// printf(" >>%s<< ", str);
	int 	i = 0; 	// traverse str
	int 	j = 0;	// buffer note
	char 	*tmp;
	float 	len = 0.0;
	int		octave;
	float beat;

	octave = 4;
	beat = 1.0;

	tmp = (char*) malloc(4);
	if (!tmp)
	{
		printf("malloc error in read_input!\n");
		return ;
	}

	int track = atoi((const char*)&str[i]);	// add data to correct list
	while (str[i] && (isdigit(str[i]) != 0 || str[i] == ' ' || str[i] == ':' || str[i] == '|'))
		i++;	// skip beginning

	while (str[i])
	{
		tmp[0] = 0, tmp[1] = 0, tmp[2] = 0, tmp[3] = 0;
		j = i;
		while (str[j] && str[j] != '/' && str[j] != ' ')	// buffer for note
			j++;
		tmp = strncpy(tmp, (const char*)&str[i], (size_t)(j-i));	// copy note to tmp
		if (tmp[1] >= '0' && tmp[1] <= '8')
			octave = tmp[1] - '0';
		update_octave(tmp, octave);

		// add tmp data to correct track #
		// tmp to frequenzy
		i = j;
		if (str[i++] == '/')	// get len
		{
			len = atof((const char*)&str[i]);
			beat = len;
			while((isdigit(str[i]) != 0 || str[i] == '.'))
				i++;
		}
		else
			len = beat;
		len = ((float)60.0 / (float)tempo * len);	// update beats to time
		feed_to_list(&head[track - 1], tmp, len);
		//printf("(%s - %.3f)\t", tmp, len);
		while (str[i] == ' ' || str[i] == '|')	// skip delimiters
			i++;
	}
	//printf("\n");
	free(tmp);
}

void	tests_waves()
{
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

// sine
	float x = 0;
	float sin_x;
		for (int i = 0; i < audio_spec.freq * 3; i++) {
			sin_x = sin(x);
			int16_t sample = sin_x * 32000;
			if (i % 1560 == 0)
				printf("hey sin  is %f\n", sin(x));
			x += 440 * PI2 / 48000.0;
			if(x >= PI2)
				x -= PI2;
			const int sample_size = sizeof(int16_t) * 1;
			SDL_QueueAudio(audio_device, &sample, sample_size);
		}

// square
	x = 0;
		for (int i = 0; i < audio_spec.freq * 3; i++) {
			sin_x = sin(x);
			if (sin_x < 0)
			{
				sin_x = -1;
			}
			else
			{
				sin_x = 1;
			}
			int16_t sample = sin_x * 32000;
			if (i % 1560 == 0)
				printf("hey sin  is %f\n", sin(x));
			x += 440 * PI2 / 48000.0;
			if(x >= PI2)
				x -= PI2;
			const int sample_size = sizeof(int16_t) * 1;
			SDL_QueueAudio(audio_device, &sample, sample_size);
		}

// triangle
	x = 0;
		for (int i = 0; i < audio_spec.freq * 3; i++) {
			sin_x = asin(sin(x)) * (2.0 / M_PI);
			int16_t sample = sin_x * 32000;
			if (i % 1560 == 0)
				printf("hey sin  is %f\n", sin(x));
			x += 440 * PI2 / 48000.0;
			if(x >= PI2)
				x -= PI2;
			const int sample_size = sizeof(int16_t) * 1;
			SDL_QueueAudio(audio_device, &sample, sample_size);


			// waveform[i] = *vol * asin(sin(frequency*t*2*M_PI)) * (2.0 / M_PI);
			//waveform[i] = *vol * sin(frequency*t*2*M_PI);



			    // if (!strcmp(tone, "sine"))
				// {
				// 	for(i=1; i < length; i++) 
				// 	{
				// 		double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
				// 		//sine
				// 		waveform[i] = *vol*sin(frequency*t*2*M_PI);
				// 	}
				// }
				// if (!strcmp(tone, "triangle"))
				// {
				// 	for(i=1; i < length; i++) 
				// 	{
				// 		double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
				// 		//triangle
				// 		waveform[i] = *vol * asin(sin(frequency*t*2*M_PI)) * (2.0 / M_PI);
				// 	}

					
    }

	SDL_PauseAudioDevice(audio_device, 0);

    SDL_Delay(9000);

    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();
}

int    count_tracks(char *str)
{
    int res = 1;
    int i = 6;

    while (str[++i])
    {
        if (str[i] == ',')
            res++;
    }
    return (res);
}

int main(int argc, char **argv)
{
	char	*ptr;
	int		fd;
	int		tracks = 0;
	int 	tempo = 0;
	int		*wave;
	t_node	**head_ptr = NULL;

	if (argc != 2)
		return (0);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (0);
	while (get_next_line(fd, &ptr) == 1)
	{
		if (ptr[0] == '#' || strlen(ptr) == 0)
		{
			free(ptr);
			continue ;
		}
		if (strncmp(ptr, "tempo", 5) == 0)
		{
			tempo = atoi(&ptr[6]);
		}
		else if (strncmp(ptr, "tracks", 6) == 0)
		{
			tracks = count_tracks(ptr);
			wave = (int *)malloc(sizeof(int) * tracks);
			if (!wave)
				return (0);
            tracks = tracks_line(ptr, wave);

			head_ptr = (t_node **)malloc(sizeof(t_node *) * tracks);
			if (!head_ptr)
			{
				free(ptr);
				return (0);
			}
			int k = 0;
			while (k < tracks)
			{
				head_ptr[k] = NULL;
				k++;
			}
		}
		else
		{
			//printf("hey line is %s\n", ptr);
			read_input(ptr, tempo, head_ptr);
		}
		free(ptr);
	}
	close(fd);
	play_melody(head_ptr, tracks, wave);
	//tests_waves();
	free(head_ptr);
	free(wave);
	//system("leaks minisynth");
	return (0);
}