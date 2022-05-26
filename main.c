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

// typedef struct	s_node
// {
// 	float 			len;
// 	// char			*tone;
// 	float			frequency;
// 	struct	s_node	*next;
// }				t_node;

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
	// list->len = tempo / 60 * len;
	list->len = len;
	//printf("len is %f \n", tempo / 60 * len);
	list->next = NULL;
}

void	feed_to_list(t_node **head, char *note, float len)
{
	t_node	*list;
	t_node	*list_ptr;
	t_node	*list_new;

	// list = *head;
	// while (list->next)
	// 	list = list->next;
	if (!*head)
	{
		list = (t_node *)malloc(sizeof(t_node));
		if (!list)
			return ;
		*head = list;
		fill_node(list, note, len);
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
	(void)octave;

	len = strlen(note);
	if (len == 1)
	{
		//*note[1] = octave + '0';
	}
	if ((len == 2 && note[1] == '#'))
	{
		note[2] = octave + '0';
	}
}

void	read_input(char *str, int tempo, t_node **head)
// void	read_input(char *str, int tempo)
{
	// printf(" >>%s<< ", str);
	int 	i = 0; 	// traverse str
	int 	j = 0;	// buffer note
	char 	*tmp;
	float 	len = 0.0;
	int	octave;
	float beat;

	octave = 4;
	beat = 1.0;


	tmp = (char*) malloc(4);
	if (!tmp)
	{
		printf("malloc error in read_input!\n");
		return ;
	}

// a a4 a# a#4 a/1

	int track = atoi((const char*)&str[i]);	// add data to correct list
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
		if (tmp[1] >= '0' && tmp[1] <= '8')
			octave = tmp[1] - '0';
		update_octave(tmp, octave);
		// printf("'%s', ", tmp);

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
		{
			len = beat;
		}
		len = (60 / (float)tempo * len);	// update beats to time
	
			// printf("%p\n", head_ptr);
			// printf("%p\n", &head_ptr[0]);
			// printf("%p\n", *head_ptr);
			// printf("%p\n", head_ptr[1]);

		feed_to_list(&head[track - 1], tmp, len);
		printf("(%s - %.3f)\t", tmp, len);

		while (str[i] == ' ' || str[i] == '|')	// skip delimiters
			i++;
		// printf("%f << \n", len);
	}
	printf("\n");
	free(tmp);
}

void	print_list(t_node *head)
{
	t_node *list;

	list = head;
	while (list)
	{
		printf("'%f' - '%f'\t", list->frequency, list->len);
		list = list->next;
	}
	printf("\n");
}



void	free_list(t_node **head)
{
	t_node *tmp;

	if (!*head)
	{
		return ;
	}
	else
	{
		while ((*head)->next)
		{
			tmp = *head;
			*head = (*head)->next;
			free(tmp);
		}
		free(*head);
	}
}

int main(int argc, char **argv)
{
	char	*ptr;
	int		fd;
	int		tracks = 0;
	int 	tempo = 0;
	int		wave[50];	// store 'triange' etc as values
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
		if (strncmp((const char*)ptr, "tempo", 5) == 0)
		{
			tempo = atoi((const char*)&ptr[6]);
		}
		else if (strncmp((const char*)ptr, "tracks", 6) == 0)
		{
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
			// printf("%p\n", head_ptr);
			// printf("%p\n", &head_ptr[0]);
			// printf("%p\n", *head_ptr);
			// printf("%p\n", head_ptr[1]);
			// printf("%p\n", &head_ptr);
			// t_node ptr[tracks];
		}
		else
		{
			// int channel = atoi((const char*)&ptr[0]);
			// printf("	aaaaa	");
			read_input(ptr, tempo, head_ptr);
			// read_input(ptr, tempo);
			// read_input(ptr, tempo);
			// printf("	ccccc	");
		}
		// printf("%s\n", ptr);
		free(ptr);
	}
	close(fd);
	// int xx = 0;
	// while (xx < tracks)
	// {
	// 	print_list(head_ptr[xx]);
	// 	free_list(&head_ptr[xx]);
	// 	xx++;
	// }


				// test printing
	// int x = 0;
	// while (x < tracks)
	// {
	// 	printf("%d ", wave[x]);
	// 	x++;
	// }
	// printf("\ntempo %d, tracks = %d\n", tempo, tracks);

	play_melody(head_ptr, tracks);
	free(head_ptr);
	return (0);
}