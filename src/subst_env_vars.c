/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subst_env_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <vlchinen@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:58:01 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/12 15:58:04 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_varname_len(char *varname);
char	**get_var_names(char **envp);
char	*strreplace(char *str, char *str_to_replace, char *replacement);
int		is_a_valid_var_name(char *s);

char	*parse_vars(char **cmds, size_t *i)
{
	char	*var_name;
	char	*env_value;
	char	*new_arg_str;
	size_t	j;

	j = *i + 1;
	while (is_a_valid_var_name(&((*cmds)[j])))
		j++;
	var_name = ft_substr(&((*cmds)[*i]), 0, j - *i + 1);
	if (!var_name)
		return (perror("minishell:"), NULL);
	env_value = getenv(var_name + 1);
	new_arg_str = strreplace(*cmds, var_name, env_value);
	if (new_arg_str)
	{
		free(*cmds);
		*cmds = new_arg_str;
		if (env_value)
			*i += ft_strlen(env_value) - 1;
		else
			*i = *i - 1;
	}
	else
		perror("minishell:");
	return (free(var_name), new_arg_str);
}

char	*compose_segments(t_words *words, char **cmds)
{
	t_list	*node;
	char	*final_str;
	char	*str;

	final_str = ft_strdup("");
	node = words->segments;
	while (node)
	{
		str = ft_strjoin(final_str, ((t_segment *)node->content)->text);
		free(final_str);
		final_str = str;
		node = node->next;
	}
	*cmds = final_str;
	if (!words[1].segments)
		cmds[1] = NULL;
	return (final_str);
}

char	*subst_vars_segment(char **text, t_minishell *data)
{
	size_t	i;
	char	*exit_code_str;
	char	*tmp_str;

	i = 0;
	while (ft_strnstr(*text, "$?", SIZE_MAX))
	{
		exit_code_str = ft_itoa(data->exit_code);
		tmp_str = *text;
		*text = strreplace(*text, "$?", exit_code_str);
		free(tmp_str);
		free(exit_code_str);
		if (!*text)
			return (NULL);
	}
	while ((*text)[i])
	{
		if ((*text)[i] == '$')
			if (!parse_vars(text, &i))
				return (NULL);
		i++;
	}
	return (*text);
}

size_t	get_words_count(t_words *words)
{
	size_t	i;

	i = 0;
	while (words->segments)
	{
		words++;
		i++;
	}
	return (i);
}

char	**subst_vars(t_words *words, t_minishell *data)
{
	t_list	*node;
	char	**cmds;
	char	**cmds_bk;

	cmds = ft_calloc(get_words_count(words) + 1, sizeof(char *));
	if (!cmds)
		return (NULL);
	cmds_bk = cmds;
	while (words->segments)
	{
		node = words->segments;
		while (node)
		{
			if (((t_segment *)node->content)->quoted != '\'')
				if (!subst_vars_segment(&(((t_segment *)node->content)->text),
						data))
					return (free_arr(cmds_bk, NULL), NULL);
			node = node->next;
		}
		compose_segments(words, cmds++);
		if (!cmds[-1])
			return (free_arr(cmds_bk, NULL), NULL);
		words++;
	}
	return (cmds_bk);
}
