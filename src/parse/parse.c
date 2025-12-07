/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/12/07 19:58:20 by pargev           ###   ########.fr       */
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

char	*substr_by_quot(char *command, char *word, t_parsing_data *d, t_minishell *data)
{
	int		first_word_end;
	char	quote[2];
	char	*cmd_in_quotes;

	first_word_end = d->i;
	quote[0] = command[d->i];
	quote[1] = '\0';
	while (command[d->i] != 0)
	{
		(d->i)++;
		if (command[d->i] == quote[0])
		{
			word = ft_strjoin3(word, subst_vars(ft_substr(command, d->start, first_word_end - d->start), data, d->cmds));
			cmd_in_quotes = ft_substr(command, first_word_end + 1, d->i - first_word_end - 1);
			if (quote[0] == '"')
				cmd_in_quotes = subst_vars(cmd_in_quotes, data, d->cmds);
			word = ft_strjoin3(word, cmd_in_quotes);
			d->start = ++(d->i);
			return (word);
		}
	}
	word = ft_strjoin2(word, quote);
	d->i = first_word_end + 1;
	d->start = d->i;
	return (word);
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
		// printf("--\n");
		ft_lstadd_back(cmds, ft_lstnew(new_cmd(operator, is_quotation)));
		i += ft_strlen(operator);
	}
	return (i);
}

void	parese_by_spaces(t_list **cmds, char *word, int is_quotation)
{
	char	**args;
	int		i;

	if (is_quotation)
		ft_lstadd_back(cmds, ft_lstnew(new_cmd(ft_strdup(word), is_quotation)));
	else if (!is_quotation && *word != '\0')
	{
		args = ft_split2(word, " \t\n");
		i = 0;
		while (args[i])
		{
			if (args[i][0] == '>' || args[i][0] == '<' || args[i][0] == '|')
				ft_lstadd_back(cmds, ft_lstnew(new_cmd(ft_strdup(args[i]), 1)));
			else
				ft_lstadd_back(cmds, ft_lstnew(new_cmd(ft_strdup(args[i]), 0)));
			i++;
		}
		free_arr(args, NULL);
	}
		
}

t_list	**parse_to_list(char *command, t_minishell *data)
{
//	t_list			**cmds;
//	int				i;
//	int				start;
	char			*word;
	int				is_quotation;
	t_parsing_data	d;

	d.cmds = (t_list **)malloc(sizeof(t_list *));
	if (!d.cmds)
		return (0);
	*d.cmds = NULL;
	d.i = 0;
	d.start = skip_spaces(command, &d.i);
	word = NULL;
	is_quotation = 0;
	while (1)
	{
		if (command[d.i] == '\'' || command[d.i] == '"')
		{
			word = substr_by_quot(command, word, &d, data);
			is_quotation = 1;
		}
		if (ft_isspace(command[d.i]) || !command[d.i] || ft_strchr("|><", command[d.i]))
		{
			if (d.i > 0 && !ft_isspace(command[d.i - 1]))
			{
				// printf("++\n");
				word = ft_strjoin3(word, subst_vars(ft_substr(command, d.start, d.i - d.start), data, d.cmds));
				parese_by_spaces(d.cmds, word, is_quotation);
				// if (!(*word == '\0' && !is_quotation))
				// 	ft_lstadd_back(cmds, ft_lstnew(new_cmd(ft_strdup(word), is_quotation)));
				free(word);
				word = NULL;
			}
			d.i = parse_operators(command, d.i, d.cmds, is_quotation);
			is_quotation = 0;
			d.start = skip_spaces(command, &d.i);
		}
		if (command[d.i] == 0)
			break ;
		if (!ft_strchr("'\"|><", command[d.i]))
			d.i++;
	}
	return (d.cmds);
}

int	check_syntax(t_list	*cmds_list, int i)
{
	t_cmd	*content;

	content = get_cmd(cmds_list);
	if (i == 0 && !content->quotes && !ft_strncmp(content->cmd, "|", 2))
	{
		ft_printfp("bash: syntax error near unexpected token `|'\n");
		return (0);
	}
	if (!content->quotes && !ft_strncmp(content->cmd, "|", 2))
	{
		cmds_list = cmds_list->next;
		if (!cmds_list || (!get_cmd(cmds_list)->quotes && !ft_strncmp(get_cmd(cmds_list)->cmd, "|", 2)))
		{
			ft_printfp("bash: syntax error near unexpected token `|'\n");
			return (0);
		}
		i++;
	}
	if (!content->quotes && (!ft_strncmp(content->cmd, ">", 2) || !ft_strncmp(content->cmd, "<", 2) || !ft_strncmp(content->cmd, ">>", 3) || !ft_strncmp(content->cmd, "<<", 3)))
	{
		cmds_list = cmds_list->next;
		if (cmds_list)
			content = get_cmd(cmds_list);
		if (!cmds_list)
		{
			ft_printfp("bash: syntax error near unexpected token `newline'\n");
			return (0);
		}
		else if (!content->quotes && (!ft_strncmp(content->cmd, "|", 2) || !ft_strncmp(content->cmd, ">", 2) || !ft_strncmp(content->cmd, "<", 2) || !ft_strncmp(content->cmd, ">>", 3) || !ft_strncmp(content->cmd, "<<", 3)))
		{
			ft_printfp("bash: syntax error near unexpected token `%s'\n", content->cmd);
			return (0);
		}
		i++;
	}
	return (1);
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
			cmds->stdin_fd[j] = handle_heredoc(get_cmd(cmds_list->next)->cmd, get_cmd(cmds_list)->quotes, data);
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
