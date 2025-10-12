/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:51:00 by vlchinen          #+#    #+#             */
/*   Updated: 2025/02/20 16:01:58 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_words(char const *s, char const *set);
void	free_segment(void *ptr);

void	complete_free_words(t_words *words)
{
	t_words	*backup_w;

	backup_w = words;
	while (words->segments)
	{
		ft_lstclear(&(words->segments), free_segment);
		words++;
	}
	free(backup_w);
}

void	set_quote(char const *s, t_split_data *data)
{
	while (s && s[data->i] && ((!data->quote_type
				&& (s[data->i] == '\'' || s[data->i] == '"'))
			|| (s[data->i] == data->quote_type)))
	{
		if (!data->quote_type && (s[data->i] == '\'' || s[data->i] == '"'))
			data->quote_type = s[data->i];
		else if (s[data->i] == data->quote_type)
			data->quote_type = '\0';
		data->i++;
	}
}

int	copy_word_segment(char const *str, char const *set,
	t_split_data *data, t_words *words)
{
	size_t		i;
	t_list		*node;
	t_segment	*segment;

	i = data->i;
	while (str && str[data->i] && ((!ft_strchr(set, str[data->i])
				&& !ft_strchr("\"'", str[data->i]) && !data->quote_type)
			|| (str[data->i] != data->quote_type && data->quote_type)))
		data->i++;
	segment = ft_calloc(1, sizeof(*segment));
	if (!segment)
		return (0);
	segment->text = ft_substr(str, i, data->i - i);
	if (!segment->text)
		return (free(segment), 0);
	segment->quoted = data->quote_type;
	node = ft_lstnew(segment);
	if (!node)
		return (free(segment->text), free(segment), 0);
	ft_lstadd_back(&(words->segments), node);
	if (str[data->i] && str[data->i] == data->quote_type && ++data->i)
		data->quote_type = '\0';
	data->i--;
	return (1);
}

t_words	*split_core(char const *s, char const *set, t_words *words)
{
	t_split_data	data;
	t_words			*backup;

	backup = words;
	ft_bzero(&data, sizeof(data));
	while (s[data.i])
	{
		set_quote(s, &data);
		if (s[data.i] && (!ft_strchr(set, s[data.i]) || data.quote_type))
		{
			if (!copy_word_segment(s, set, &data, words))
				return (complete_free_words(backup),
					perror("minishell:"), NULL);
		}
		else if ((data.in_word && ft_strchr(set, s[data.i])
				&& !data.quote_type) || !s[data.i])
			words++;
		if (!ft_strchr(set, s[data.i]) || data.quote_type)
			data.in_word = 1;
		else
			data.in_word = 0;
		if (s[data.i])
			(data.i)++;
	}
	return (backup);
}

t_words	*split_with_quotes(char const *s, char const *set)
{
	t_words	*words;
	int		w_c;

	w_c = count_words(s, set);
	if (w_c < 0)
		return ((void)printf("Error: unmatched quote\n"), NULL);
	words = ft_calloc((w_c + 1), sizeof(t_words));
	if (!words)
		return (NULL);
	return (split_core(s, set, words));
}
