/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/12/07 23:54:22 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	substr_by_quot2(char quote, t_parsing_data *d, int first_word_end)
{
	char	quote2[2];

	quote2[0] = quote;
	quote2[1] = '\0';
	d->word = ft_strjoin2(d->word, quote2);
	d->i = first_word_end + 1;
	d->start = d->i;
}

void	substr_by_quot(char *command, t_parsing_data *d, t_minishell *data)
{
	int		first_word_end;
	char	quote;
	char	*cmd_in_quotes;

	first_word_end = d->i;
	quote = command[d->i];
	while (command[d->i] != 0)
	{
		(d->i)++;
		if (command[d->i] == quote)
		{
			d->word = ft_strjoin3(d->word,
					subst_vars(ft_substr(command, d->start,
							first_word_end - d->start), data, d->cmds));
			cmd_in_quotes = ft_substr(command, first_word_end + 1,
					d->i - first_word_end - 1);
			if (quote == '"')
				cmd_in_quotes = subst_vars(cmd_in_quotes, data, d->cmds);
			d->word = ft_strjoin3(d->word, cmd_in_quotes);
			d->start = ++(d->i);
			return ;
		}
	}
	substr_by_quot2(quote, d, first_word_end);
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

int	parse_to_list2(char *command, t_parsing_data *d, t_minishell *data)
{
	if (command[d->i] == '\'' || command[d->i] == '"')
	{
		substr_by_quot(command, d, data);
		d->is_quotation = 1;
	}
	if (ft_isspace(command[d->i]) || !command[d->i]
		|| ft_strchr("|><", command[d->i]))
	{
		if (d->i > 0 && !ft_isspace(command[d->i - 1]))
		{
			d->word = ft_strjoin3(d->word, subst_vars(ft_substr(command,
							d->start, d->i - d->start), data, d->cmds));
			parese_by_spaces(d->cmds, d->word, d->is_quotation);
			free(d->word);
			d->word = NULL;
		}
		d->i = parse_operators(command, d->i, d->cmds, d->is_quotation);
		d->is_quotation = 0;
		d->start = skip_spaces(command, &d->i);
	}
	if (command[d->i] == 0)
		return (0);
	if (!ft_strchr("'\"|><", command[d->i]))
		d->i++;
	return (1);
}

t_list	**parse_to_list(char *command, t_minishell *data)
{
	t_parsing_data	d;

	d.cmds = (t_list **)malloc(sizeof(t_list *));
	if (!d.cmds)
		return (0);
	*d.cmds = NULL;
	d.i = 0;
	d.start = skip_spaces(command, &d.i);
	d.word = NULL;
	d.is_quotation = 0;
	while (1)
	{
		if (!parse_to_list2(command, &d, data))
			break ;
	}
	return (d.cmds);
}
