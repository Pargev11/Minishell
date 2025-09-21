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

char	**complete_free(char **arr);
int	count_words(char const *s, char const *set);

/*static char	*worddup(char *s, char const *set, char quote_type)
{
	size_t	i;
	size_t	j;
	char	*buff;

	i = 0;
	while (s[i] && ((!quote_type && !ft_strchr(set, s[i]))
			|| (quote_type
				&& (s[i] != quote_type
				|| !ft_strchr(set, s[i + 1])))))
	{
		i++;
	}
	buff = malloc(sizeof(char) * (i + 1));
	if (!buff)
		return (0);
	j = i;
	i = 0;
	while (s[i] && i < j)
	{
		buff[i] = s[i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}*/



char	*set_quote(char *s, size_t i, char *quote_type)
{
	char	*new_s;

	new_s = s;
	while (s[i] && ((!*quote_type && (s[i] == '\'' || s[i] == '"'))
			|| (s[i] == *quote_type)))
	{
		if (!*quote_type && (s[i] == '\'' || s[i] == '"'))
		{
			*quote_type = s[i];
			new_s = str_rm_idx((s), i);
			free(s);
		}
		else if (s[i] == *quote_type)
		{
			*quote_type = '\0';
			new_s = str_rm_idx((s), i);
			free(s);
		}
		s = new_s;
	}
	return (new_s);
}

int	set_sep_arr(char *s, size_t i, char quote_type, char const *set)
{
	if ((ft_strchr(set, s[i]) || !s[i]) && !quote_type)
		return (1);
	return (0);
}

char	**split_core(char *s, char const *set, char **arr, char **backup)
{
	int		sep[2];
	char	quote_type;
	size_t	i;
	size_t	start_idx;

	quote_type = '\0';
	i = 0;
	sep[1] = 1;
	while (!i || s[i - 1])
	{
		sep[0] = sep[1];
		s = set_quote(s, i++, &quote_type);
		if (!s)
			return (complete_free(backup));
		sep[1] = set_sep_arr(s, i - 1, quote_type, set);
		if (!sep[1] && sep[0])
			start_idx = i - 1;
		if (sep[1] && !sep[0])
			*arr++ = ft_substr(s, start_idx, i - 1 - start_idx);
		if (sep[1] && !sep[0])
			if (!arr[-1])
				return (complete_free(backup));
	}
	*arr = 0;
	return (backup);
}

char	**split_with_quotes(char const *s, char const *set)
{
	char	**arr;
	char	**backup;
	int		w_c;
	char	*str;

	w_c = count_words(s, set);
	if (w_c < 0)
		return ((void)printf("Error: unmatched quote\n"), NULL);
	str = ft_strdup(s);
	arr = malloc(sizeof(char *) * (w_c + 1));
	if (!arr)
		return (0);
	backup = arr;
	*arr = NULL;
	return (split_core(str, set, arr, backup));
}
