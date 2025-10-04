/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/04 13:50:39 by pargev           ###   ########.fr       */
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

void	free_matrix(char **vars)
{
	int	i;

	i = 0;
	while (vars[i])
	{
		free(vars[i]);
		i++;
	}
	free(vars);
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

char	**remove_variable(char **args, int del_index)
{
	char	**new;
	int		size;
	int		i;
	int		j;

	size = 0;
	while (args[size])
		size++;
	new = (char **)malloc(sizeof(char *) * size);
	if (!new)
		return (0);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (i != del_index)
		{
			new[j] = ft_strdup(args[i]);
			if (!new[j])
			{
				free_matrix(new);
				return (0);
			}
			j++;
		}
		i++;
	}
	new[i++] = NULL;
	return (new);
}

char	**copy_args(char **args)
{
	char	**copy;
	int		size;
	int		i;

	size = 0;
	while (args[size])
		size++;
	copy = (char **)malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (0);
	i = 0;
	while (args[i])
	{
		copy[i] = ft_strdup(args[i]);
		if (!copy[i])
		{
			free_matrix(copy);
			return (0);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}