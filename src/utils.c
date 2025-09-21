/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/09/21 17:16:59 by pamalkha         ###   ########.fr       */
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

char	**remove_var(char **vars, int del_index)
{
	char	**new_vars;
	int		vars_size;
	int		i;
	int		j;

	vars_size = 0;
	while (vars[vars_size])
		vars_size++;
	new_vars = (char **)malloc(sizeof(char *) * (vars_size - 1));
	if (!new_vars)
		return (0);
	i = 0;
	j = 0;
	while (vars[i])
	{
		if (i != del_index)
		{
			new_vars[j] = ft_strdup(vars[i]);
			// printf("-- %s %d\n", vars[i], del_index);
			if (!new_vars[j])
			{
				// free_matrix(new_vars);
				return (0);
			}
			j++;
		}
		i++;
	}
	// new_vars[i++] = NULL;
	free_matrix(vars);
	return (vars);
}