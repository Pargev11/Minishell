/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/08/16 17:12:39 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_nvirements(char **env)
{
	int			i;
	int			j;
	char		*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while(env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			++j;
		}
		++i;
	}
}

int export(char **cmds)
{
	extern char **environ;
	int			i;

	if (cmds[1] != NULL)
		return (1);
	i = 0;
	sort_nvirements(environ);
	while (environ[i])
		printf("declare -x %s\n", environ[i++]);
	return (1);
}