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
#include <stdio.h>

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	printf("hello there\n");
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("Error\n");
		exit (1);
	}
    return (0);
}
