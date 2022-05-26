/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvuorenl <rvuorenl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 14:21:09 by rvuorenl          #+#    #+#             */
/*   Updated: 2022/05/26 15:06:41 by rvuorenl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/get_next_line.h"
#include <stdio.h>
#include <string.h>	// strncmp
#include <stdlib.h>	// atoi
#include <ctype.h>	// isdigit

typedef struct	s_node
{
	float 	len;
	str 	tone;
	t_node	*next;
}				t_node;

// tracks triangle,sine,sine,sine,sine,sine,sine,saw,saw,saw,saw,saw,saw,sine,sine,sine,sine,sine,triangle,triangle,triangle,square,square,square,kick,snare

int	tracks_line(char *str, int *arr)
{
	int res = 0;
	int i = 6;	// skip "tracks"
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

// sine waves
// saw waves
// square waves
// triangle waves
// kick
// snare

// read_input(ptr, channel, tracks, &notes);
void	read_input(char *str, int *channel, int tracks, t_node *notes)
{
	int 	i = 0; 	// traverse str
	int 	x = 0;	// # of instructions
	int 	j = 0;	// compare to tracks
	int 	len = 0;	// len of note, end with '/' or ' '
	char 	tmp[5];

	while (isdigit(str[i]) != 0)
		i++;
	i += 2;
	while (str[i])
	{
		j = i;
		while (str[j] != '/' && str[j] != ' ')
			j++;
		tmp = strncpy(tmp, (const char*)&str[j-i], (size_t)(j-i));	// get note
	// tmp to frequenzy
		tmp[0] = 0, tmp[1] = 0, tmp[2] = 0, tmp[3] = 0, tmp[4] = 0;
		// i += j;
		if (str[j++] == '/')	// get len
		{
			notes->len[x++] = atof((const char*)&str[j])
			i = j;
			while((isdigit(str[i]) != 0 || str[i] == '.'))
				i++;
		}
		else
		{
			notes->len[x++] = 1.0;
			while (str[i] == ' ')
				i++;
		}
	}
}

int main(int argc, char **argv)
{
	char	*ptr;
	int		fd;
	int		wave[50];
	int 	channel[50];
	int		tracks;
	int 	tempo;
	t_data 	*head;

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
		if (strncmp((const char*)ptr, "tempo", 5) == 0)
		{
			tempo = atoi((const char*)&ptr[6]);
		}
		else if (strncmp((const char*)ptr, "tracks", 6) == 0)
		{
			tracks = tracks_line(ptr, wave);
			t_node notes[tracks]; 	// each track has an array of tones + len
		}
		// else
		// {
		// 	read_input(ptr, channel, tracks, &notes);
		// }
		printf("%s\n", ptr);
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
	// printf("\n\n%d %d %d %d\n", wave[0], wave[1], wave[2], wave[3]);
	// printf("\ntempo %d, tracks = %d\n", tempo, tracks);
	return (0);
}