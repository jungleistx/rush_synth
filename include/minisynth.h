/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minisynth.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-maul <mde-maul@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:14:54 by mde-maul          #+#    #+#             */
/*   Updated: 2022/05/26 15:14:55 by mde-maul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISYNTH_H
# define MINISYNTH_H

# include <math.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include <ctype.h>
//# include "SDL2/SDL_mixer.h"
# include "../libft/libft.h"
# define PI2 6.28318530718

typedef struct s_node {
	float 	len;
	float	frequency;
	struct s_node	*next;
}	t_node;

float get_frequency(char *note);
void	play_melody(t_node **list, int track_nb);
void	print_list(t_node *head);
int strings_match(char *str1, char *str2, int char_nb);
void	free_list(t_node **head);

#endif
