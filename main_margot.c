/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_margot.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-maul <mde-maul@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 16:05:22 by mde-maul          #+#    #+#             */
/*   Updated: 2022/05/25 16:05:24 by mde-maul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL2/SDL.h"
#include <SDL2/SDL_timer.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	printf("hello there\n");
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
		exit (1);
	}
    return (0);
}

/*

3 la diese
4 do diese
5 fa
6 sol diese
7 si

*/