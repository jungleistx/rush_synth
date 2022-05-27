/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_frequency.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-maul <mde-maul@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 15:08:05 by mde-maul          #+#    #+#             */
/*   Updated: 2022/05/26 15:08:06 by mde-maul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minisynth.h"

double	get_note(char *note)
{
	double frequency = 0.0;

	if (strings_match("c", note, 1))
		frequency = 16.35160;
	if (strings_match("c#", note, 2))
		frequency = 17.32391;
	if (strings_match("d", note, 1))
		frequency = 18.35405;
	if (strings_match("d#", note, 2))
		frequency = 19.44544;
	if (strings_match("e", note, 1))
		frequency = 20.60172;
	if (strings_match("f", note, 1))
		frequency = 21.82676;
	if (strings_match("f#", note, 2))
		frequency = 23.12465;
	if (strings_match("g", note, 1))
		frequency = 24.49971;
	if (strings_match("g#", note, 2))
		frequency = 25.95654;
	if (strings_match("a", note, 1))
		frequency = 27.50000;
	if (strings_match("a#", note, 2))
		frequency = 29.13524;
	if (strings_match("b", note, 1))
		frequency = 30.86771;
	return (frequency);
}

float get_frequency(char *note)
{
	float frequency;
	int octave;
	size_t len;

	len = strlen(note);
	if (strchr(note, '#'))
	{
		if (len == 2)
			octave = 4;
		else
			octave = atoi(&note[2]);
	}
	else
	{
		if (len == 1)
			octave = 4;
		else
			octave = atoi(&note[1]);
	}
	frequency = get_note(note) * pow(2, octave);
	return (frequency);
}

