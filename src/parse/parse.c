/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/01 23:25:30 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_spaces(char *str, int *i)
{
	while (str[*i] == ' ')
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
		if (command[i] == ' ' || command[i] == 0 || command[i] == '|')
		{
			if (i > 0 && command[i - 1] != ' ')
			{
				word = ft_strjoin3(word, subst_vars(ft_substr(command, start, i - start), data));
				if (!(*word == '\0' && !is_quotation))
					ft_lstadd_back(cmds, ft_lstnew(ft_strdup(word)));
				free(word);
				word = NULL;
			}
			if (command[i] == '|')
			{
				ft_lstadd_back(cmds, ft_lstnew(ft_strdup("|")));
				i++;
			}
			is_quotation = 0;
			start = skip_spaces(command, &i);
		}
		if (command[i] == 0)
			break ;
		if (command[i] != '\'' && command[i] != '"' && command[i] != '|')
			i++;
	}
	return (cmds);
}

char	***parse_words(char *cmd, t_minishell *data)
{
	t_list	**cmds_list;
	char	***cmds;

	cmds_list = parse_to_list(cmd, data);
	cmds = allocate_cmds(cmds_list);
	ft_lstclear(cmds_list, free);
	free(cmds_list);
	return (cmds);
}
