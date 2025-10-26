/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/26 18:46:15 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_spaces(char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
	return (*i);
}

char	*substr_by_quot(char *command, char *word, int *start, int *i)
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
			word = ft_strjoin3(word, ft_substr(command, *start, first_word_end - *start));
			cmd_in_quotes = ft_substr(command, first_word_end + 1, *i - first_word_end - 1);
			if (quote == '"')
				//  substr
			word = ft_strjoin3(word, cmd_in_quotes);
			(*i)++;
			*start = *i;
			return (word);
		}
	}
	word = ft_strjoin3(word, ft_substr(command, *start, first_word_end - *start));
	*i = first_word_end + 1;
	*start = first_word_end;
	return (word);
}

t_list	**parse_to_list(char *command)
{
	t_list	**cmds;
	int		i;
	int		start;
	char	*word;
	
	cmds = (t_list **)malloc(sizeof(t_list *));
	if (!cmds)
		return (0);
	*cmds = NULL;
	
	i = 0;
	start = skip_spaces(command, &i);
	word = NULL;
	while (1)
	{
		if (command[i] == '\'' || command[i] == '"')
			word = substr_by_quot(command, word, &start, &i);
		if (command[i] == ' ' || command[i] == 0)
		{
			word = ft_strjoin3(word, ft_substr(command, start, i - start));
			// printf("word = %s\n", word);
			ft_lstadd_back(cmds, ft_lstnew(ft_strdup(word)));
			free(word);
			word = NULL;
			start = skip_spaces(command, &i);
		}
		if (command[i] == 0)
			break;
		if (command[i] != '\'' && command[i] != '"')
			i++;
	}
	
	// t_list	*tmp = *cmds;
	// while (tmp)
	// {
	// 	printf("str = %s|\n", (char *)(tmp->content));
	// 	tmp = tmp->next;
	// }
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
	t_list **cmds;
	char	**cmd2;
	
	cmds = parse_to_list(cmd);
	cmd2 = ft_split(cmd, ' ');
	if (!cmd2)
		return (NULL);
	// subst_vars(cmds, data);

	
	t_list	*tmp = *cmds;
	while (tmp)
	{
		printf("str = %s|\n", (char *)(tmp->content));
		tmp = tmp->next;
	}

	
	// cmds = delete_empty_args(cmds);
	return (cmd2);
}
