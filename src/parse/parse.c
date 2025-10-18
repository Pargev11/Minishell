/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/12 21:11:17 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**delete_empty_args(char **cmds)
{
	size_t	i;
	size_t	j;
	char	**cmds_b;

	cmds_b = cmds;
	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (*(cmds[i]))
			cmds[j++] = cmds[i];
		else
			free(cmds[i]);
		i++;
	}
	cmds[j] = NULL;
	return (cmds_b);
}

char	**parse_words(char *cmd, t_minishell *data)
{
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	if (!cmds)
		return (NULL);
	cmds = subst_vars(cmds, data);
	cmds = delete_empty_args(cmds);
	return (cmds);
}
