/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/12/07 22:34:45 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *str)
{
	if (!ft_strncmp(str, ">", 2) || !ft_strncmp(str, "<", 2)
		|| !ft_strncmp(str, ">>", 3) || !ft_strncmp(str, "<<", 3))
		return (1);
	return (0);
}

int	check_redirects(t_list *cmds_list, t_cmd *content, t_cmd *next_content)
{
	if (!content->quotes && is_redirection(content->cmd))
	{
		if (!cmds_list->next)
		{
			ft_printfp("bash: syntax error near unexpected token `newline'\n");
			return (0);
		}
		else if (!next_content->quotes && (!ft_strncmp(next_content->cmd,
					"|", 2) || is_redirection(next_content->cmd)))
		{
			ft_printfp("bash: -syntax error near unexpected token `%s'\n",
				next_content->cmd);
			return (0);
		}
	}
	return (1);
}

int	check_syntax(t_list	*cmds_list, int i)
{
	t_cmd	*content;
	t_cmd	*next_content;

	content = get_cmd(cmds_list);
	if (cmds_list->next)
		next_content = get_cmd(cmds_list->next);
	if (i == 0 && !content->quotes && !ft_strncmp(content->cmd, "|", 2))
	{
		ft_printfp("bash: syntax error near unexpected token `|'\n");
		return (0);
	}
	else if (!content->quotes && !ft_strncmp(content->cmd, "|", 2))
	{
		if (!cmds_list->next || (!next_content->quotes
				&& !ft_strncmp(next_content->cmd, "|", 2)))
		{
			ft_printfp("bash: syntax error near unexpected token `|'\n");
			return (0);
		}
	}
	return (check_redirects(cmds_list, content, next_content));
}

int	process_commands(t_list *cmds_list, t_cmds *cmds, t_minishell *data)
{
	int	i;
	int	j;

	cmds->stdin_fd = (int *)malloc(sizeof(int) * (ft_lstsize(cmds_list) + 1));
	i = 0;
	j = 0;
	while (cmds_list)
	{
		if (!check_syntax(cmds_list, i))
			return (0);
		if (!ft_strncmp(get_cmd(cmds_list)->cmd, "<<", 3))
		{
			cmds->stdin_fd[j] = handle_heredoc(get_cmd(cmds_list->next)->cmd,
					get_cmd(cmds_list)->quotes, data);
			if (cmds->stdin_fd[j] < 0)
				return (0);
			j++;
		}
		cmds_list = cmds_list->next;
		i++;
	}
	return (1);
}

t_cmds	parse_words(char *cmd, t_minishell *data)
{
	t_list	**cmds_list;
	t_cmds	cmds;

	cmds_list = parse_to_list(cmd, data);
	if (process_commands(*cmds_list, &cmds, data))
		allocate_cmds(cmds_list, &cmds);
	else
	{
		data->exit_code = 2;
		cmds.cmds = NULL;
		cmds.quote_mask = NULL;
	}
	ft_lstclear(cmds_list, free_cmd);
	free(cmds_list);
	return (cmds);
}
