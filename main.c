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
		while (str[i] == ' ' || str[i] == '|')	// skip delimiters
			i++;
	}
	free(tmp);
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
			read_input(ptr, tempo, head_ptr);
		free(ptr);
	}
	close(fd);
	play_melody(head_ptr, tracks, wave);
	free(head_ptr);
	free(wave);
	return (0);
}
