/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/07/20 13:48:34 by vlchinen         ###   ########.fr       */
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
	t_words	*words;

	words = split_with_quotes(cmd, " \t\n");
	if (!words)
		return (NULL);
	cmds = subst_vars(words, data);
	complete_free_words(&words);
	cmds = delete_empty_args(cmds);
	return (cmds);
}
