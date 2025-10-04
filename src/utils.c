/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/04 21:16:48 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((unsigned char)*s1 && (unsigned char)*s2)
	{
		if ((unsigned char)*s1 != (unsigned char)*s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	check_name(char *name)
{
	static char *forbidden = "!\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~";
	int			i;

	if (!name || *name == 0)
		return (0);
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (*name)
	{
		i = 0;
		while (forbidden[i])
		{
			if (*name == forbidden[i])
				return (0);
			i++;
		}
		name++;
	}
	return (1);
}