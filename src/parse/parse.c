/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/12/01 00:25:11 by pargev           ###   ########.fr       */
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

char	*substr_by_quot(char *command, char *word, int *start, int *i, t_minishell *data)
{
	int		first_word_end;
	char	quote[2];
	char	*cmd_in_quotes;

	first_word_end = *i;
	quote[0] = command[*i];
	quote[1] = '\0';
	while (command[*i] != 0)
	{
		(*i)++;
		if (command[*i] == quote[0])
		{
			word = ft_strjoin3(word, subst_vars(ft_substr(command, *start, first_word_end - *start), data));
			cmd_in_quotes = ft_substr(command, first_word_end + 1, *i - first_word_end - 1);
			if (quote[0] == '"')
				cmd_in_quotes = subst_vars(cmd_in_quotes, data);
			word = ft_strjoin3(word, cmd_in_quotes);
			*start = ++(*i);
			return (word);
		}
	}
	word = ft_strjoin2(word, quote);
	*i = first_word_end + 1;
	*start = *i;
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
			ft_lstadd_back(cmds, ft_lstnew(new_cmd(ft_strdup(args[i]), 0)));
			i++;
		}
		free_arr(args, NULL);
	}
		
}

t_list	**parse_to_list(char *command, t_minishell *data)
{
	t_list	**cmds;
	int		i;
	int		start;
	char	*word;
	int		is_quotation;

	cmds = (t_list **)malloc(sizeof(t_list *));
	if (!cmds)
		return (0);
	*cmds = NULL;
	i = 0;
	start = skip_spaces(command, &i);
	word = NULL;
	is_quotation = 0;
	while (1)
	{
		if (command[i] == '\'' || command[i] == '"')
		{
			word = substr_by_quot(command, word, &start, &i, data);
			is_quotation = 1;
		}
		if (ft_isspace(command[i]) || command[i] == 0 || command[i] == '|' || command[i] == '>' || command[i] == '<')
		{
			if (i > 0 && !ft_isspace(command[i - 1]))
			{
				word = ft_strjoin3(word, subst_vars(ft_substr(command, start, i - start), data));
				parese_by_spaces(cmds, word, is_quotation);
				// if (!(*word == '\0' && !is_quotation))
				// 	ft_lstadd_back(cmds, ft_lstnew(new_cmd(ft_strdup(word), is_quotation)));
				free(word);
				word = NULL;
			}
			i = parse_operators(command, i, cmds, is_quotation);
			is_quotation = 0;
			start = skip_spaces(command, &i);
		}
		if (command[i] == 0)
			break ;
		if (command[i] != '\'' && command[i] != '"' && command[i] != '|' && command[i] != '>' && command[i] != '<')
			i++;
	}
	
	return (cmds);
}

int	check_syntax(t_list	*cmds_list)
{
	int		i;
	t_cmd	*content;

	i = 0;
	while (cmds_list)
	{
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
		cmds_list = cmds_list->next;
		i++;
	}
	return (1);
}

char	***parse_words(char *cmd, int ***quote_mask, t_minishell *data)
{
	t_list	**cmds_list;
	char	***cmds;

	cmds_list = parse_to_list(cmd, data);
	if (check_syntax(*cmds_list))
		cmds = allocate_cmds(cmds_list, quote_mask);
	else
	{
		data->exit_code = 2;
		cmds = NULL;
	}
	ft_lstclear(cmds_list, free_cmd);
	free(cmds_list);
	return (cmds);
}
