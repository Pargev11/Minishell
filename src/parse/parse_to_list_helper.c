/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_list_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/12/07 23:34:03 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(char *cmd, int quotes)
{
	t_cmd	*cmd_s;

	cmd_s = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd_s)
		return (NULL);
	cmd_s->cmd = cmd;
	cmd_s->quotes = quotes;
	return (cmd_s);
}

void	free_cmd(void *content)
{
	t_cmd	*cmd_s;

	cmd_s = content;
	if (cmd_s)
	{
		if (cmd_s->cmd)
			free(cmd_s->cmd);
		free(cmd_s);
	}
}

t_cmd	*get_cmd(t_list	*cmds_list)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)(cmds_list->content);
	return (cmd);
}

int	skip_spaces(char *str, int *i)
{
	while (ft_isspace(str[*i]))
		(*i)++;
	return (*i);
}

int	parse_operators(char *command, int i, t_list **cmds, int is_quotation)
{
	char	*operator;

	operator = NULL;
	if (command[i] == '|')
		operator = ft_strdup("|");
	else if (command[i] == '>' && command[i + 1] != '>')
		operator = ft_strdup(">");
	else if (command[i] == '<' && command[i + 1] != '<')
		operator = ft_strdup("<");
	else if (command[i] == '>' && command[i + 1] == '>')
		operator = ft_strdup(">>");
	else if (command[i] == '<' && command[i + 1] == '<')
		operator = ft_strdup("<<");
	if (operator != NULL)
	{
		ft_lstadd_back(cmds, ft_lstnew(new_cmd(operator, is_quotation)));
		i += ft_strlen(operator);
	}
	return (i);
}
