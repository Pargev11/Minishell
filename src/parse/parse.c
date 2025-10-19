/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/19 17:05:58 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_spaces(char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
	return (*i);
}

int	substr_by_quot(char *command,  int i)
{
	char 	quote;

	quote = command[i];
	while (command[i] != 0)
	{
		i++;
		if (command[i] == quote)
			return (i+1);
	}
	return (i);
}

t_list	**parse_to_list(char *command)
{
	t_list	**cmds;
	int		i;
	int		start;
	
	cmds = (t_list **)malloc(sizeof(t_list *));
	if (!cmds)
		return (0);
	*cmds = NULL;
	i = 0;
	start = skip_spaces(command, &i);
	while (1)
	{
		if (command[i] == '\'' || command[i] == '"')
			i = substr_by_quot(command, i);
		if (command[i] == ' ' || command[i] == 0)
		{
			ft_lstadd_back(cmds, ft_lstnew(ft_substr(command, start, i - start)));
			start = skip_spaces(command, &i);
		}
		if (command[i] == 0)
			break;
		if (command[i] != '\'' && command[i] != '"')
			i++;
	}
	
	t_list	*tmp = *cmds;
	while (tmp)
	{
		printf("str = %s|\n", (char *)(tmp->content));
		tmp = tmp->next;
	}
	return (cmds);
}

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

	parse_to_list(cmd);
	cmds = ft_split(cmd, ' ');
	if (!cmds)
		return (NULL);
	cmds = subst_vars(cmds, data);
	cmds = delete_empty_args(cmds);
	return (cmds);
}
