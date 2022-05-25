/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvuorenl <rvuorenl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 14:21:09 by rvuorenl          #+#    #+#             */
/*   Updated: 2022/05/25 14:55:43 by rvuorenl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/get_next_line.h"
#include <stdio.h>

typedef struct	s_data
{
	int	tempo;
}t_data;

// tracks triangle,sine,sine,sine,sine,sine,sine,saw,saw,saw,saw,saw,saw,sine,sine,sine,sine,sine,triangle,triangle,triangle,square,square,square,kick,snare

int	tracks(char *str, int *arr)
{
	int res = 0;
	int i = 7;	// skip "tracks"
	int elem = 0;

	while (str[i])
	{
		if (ft_strncmp((const char*)&str[i], "sine", 4 == 0))
		{
			arr[elem++] = 1;
			res++;
			i += 5;
		}
		else if (ft_strncmp((const char*)&str[i], "saw", 3 == 0))
		{
			arr[elem++] = 2;
			res++;
			i += 4;
		}
		else if (ft_strncmp((const char*)&str[i], "square", 5 == 0))
		{
			arr[elem++] = 3;
			res++;
			i += 6;
		}
		else if (ft_strncmp((const char*)&str[i], "triangle", 8 == 0))
		{
			arr[elem++] = 4;
			res++;
			i += 9;
		}
	}
	return (res);
}

// sine waves
// saw waves
// square waves
// triangle waves

int main(int argc, char **argv)
{
	char	*ptr;
	int		fd;
	int		wave[50];
	// t_data data[50];
	int		tracks;
	int tempo;

	if (argc != 2)
		return (0);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (0);

	while (get_next_line(fd, &ptr) == 1)
	{
		if (ft_strncmp((const char*)ptr, "tempo", 5) == 0)
		{
			tempo = ft_atoi((const char*)&ptr[6]);
		}
		else if (ft_strncmp((const char*)ptr, "tracks", 6) == 0)
		{
			tracks = tracks(ptr, wave);
		}

		printf("%s\n", ptr);

		free(ptr);
	}
	close(fd);
	return (0);
}