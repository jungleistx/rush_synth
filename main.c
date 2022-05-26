/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvuorenl <rvuorenl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 14:21:09 by rvuorenl          #+#    #+#             */
/*   Updated: 2022/05/26 16:39:11 by rvuorenl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/get_next_line.h"
#include <stdio.h>
#include <string.h>	// strncmp
#include <stdlib.h>	// atoi
#include <ctype.h>	// isdigit

typedef struct	s_node
{
	float 			len;
	char			*tone;
	struct	s_node	*next;
}				t_node;

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

// void	read_input(char *str, int *channel, int tracks, t_node *notes)
void	read_input(char *str, int tempo, t_node *data)
{
	int 	i = 0; 	// traverse str
	int 	j = 0;	// compare to tracks
	char 	*tmp;
	float 	len;

	tmp = (char*) malloc(4);
	if (!tmp)
	{
		printf("malloc error in read_input!\n");
		return ;
	}

	int track = atoi(str[i++]);	// add data to correct list
	while (isdigit(str[i]) != 0 || str[i] == ' ' || str[i] == ':' || str[i] == '|')
		i++;	// skip beginning

	while (str[i])
	{
		tmp[0] = 0, tmp[1] = 0, tmp[2] = 0, tmp[3] = 0;
		j = i;
		while (str[j] != '/' && str[j] != ' ')	// buffer for note
			j++;
		// printf(" >> i = %d, j = %d, ", i, j);
		tmp = strncpy(tmp, (const char*)&str[i], (size_t)(j-i));	// copy note to tmp
		// printf("'%s', ", tmp);

			// add tmp data to correct track #
			// tmp to frequenzy

		i = j;
		if (str[i++] == '/')	// get len
		{
			len = atof((const char*)&str[i]);
			while((isdigit(str[i]) != 0 || str[i] == '.'))
				i++;
		}
		else
		{
			len = 1.0;
		}
		len = ((float)tempo / 60 * len);	// update beats to time

		data[track - 1]->len = len;
		data[track - 1]->tone = tmp;


		while (str[i] == ' ' || str[i] == '|')	// skip delimiters
			i++;
		// printf("%f << \n", len);
	}
	free(tmp);
}

// typedef struct	s_node
// {
// 	float 			len;
// 	char			*tone;
// 	struct	s_node	*next;
// }				t_node;

int main(int argc, char **argv)
{
	char	*ptr;
	int		fd;
	int		tracks;
	int 	tempo;
	int		wave[50];	// store 'triange' etc as values
	t_node 	*head;

	if (argc != 2)
		return (0);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (0);

	head = NULL;
	while (get_next_line(fd, &ptr) == 1)
	{
		if (ptr[0] == '#' || strlen(ptr) == 0)
		{
			free(ptr);
			continue ;
		}
		if (strncmp((const char*)ptr, "tempo", 5) == 0)
		{
			tempo = atoi((const char*)&ptr[6]);
		}
		else if (strncmp((const char*)ptr, "tracks", 6) == 0)
		{
			tracks = tracks_line(ptr, wave);
			t_node data[tracks]; 	// each track has an node containing note + len
			head = &data[0];
		}
		else
		{
			read_input(ptr, tempo, &data[0]);
		}
		// printf("%s\n", ptr);
		free(ptr);
	}
	close(fd);
				// test printing
	// int x = 0;
	// while (x < tracks)
	// {
	// 	printf("%d ", wave[x]);
	// 	x++;
	// }
	// printf("\ntempo %d, tracks = %d\n", tempo, tracks);
	return (0);
}