/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/01 15:47:46 by pargev           ###   ########.fr       */
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
	char 	quote;
	char	*cmd_in_quotes;
	
	first_word_end = *i;
	quote = command[*i];
	while (command[*i] != 0)
	{
		(*i)++;
		if (command[*i] == quote)
		{
			word = ft_strjoin3(word, subst_vars(ft_substr(command, *start, first_word_end - *start), data));
			cmd_in_quotes = ft_substr(command, first_word_end + 1, *i - first_word_end - 1);
			if (quote == '"')
				cmd_in_quotes = subst_vars(cmd_in_quotes, data);
			word = ft_strjoin3(word, cmd_in_quotes);
			(*i)++;
			*start = *i;
			return (word);
		}
	}
	word = ft_strjoin3(word, subst_vars(ft_substr(command, *start, first_word_end - *start), data));
	*i = first_word_end + 1;
	*start = first_word_end;
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
		if (command[i] == ' ' || command[i] == 0)
		{
			word = ft_strjoin3(word, subst_vars(ft_substr(command, start, i - start), data));
			if (!(*word == '\0' && !is_quotation))
				ft_lstadd_back(cmds, ft_lstnew(ft_strdup(word)));
			free(word);
			word = NULL;
			is_quotation = 0;
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

char	**parse_words(char *cmd, t_minishell *data)
{
	t_list **cmds_list;
	char	**cmds;
	int		i;
	
	cmds_list = parse_to_list(cmd, data);
	cmds = (char **)malloc(sizeof(char *) * (ft_lstsize(*cmds_list) + 1));
	if (!cmds)
		return (NULL);
	t_list	*cmd_list = *cmds_list;
	i = 0;
	while (cmd_list)
	{
		cmds[i++] = ft_strdup((char *)(cmd_list->content));
		cmd_list = cmd_list->next;
	}
	cmds[i] = NULL;
	ft_lstclear(cmds_list, free);
	free(cmds_list);
	return (cmds);
}
