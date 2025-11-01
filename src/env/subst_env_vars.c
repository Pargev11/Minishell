/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subst_env_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 22:39:25 by pargev            #+#    #+#             */
/*   Updated: 2025/11/01 23:27:19 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strreplace(char *str, char *str_to_replace, char *replacement, int clear_sources)
{
	char	*needle_p;
	char	*p1;
	char	*p2;
	size_t	i;

	i = 0;
	needle_p = ft_strnstr(str, str_to_replace, SIZE_MAX);
	if (!needle_p)
		return (NULL);
	while (&(str[i]) != needle_p)
		i++;
	p1 = ft_substr(str, 0, i);
	if (!p1)
		return (NULL);
	p2 = ft_substr(str, i + ft_strlen(str_to_replace), SIZE_MAX);
	if (!p2)
		return (free(p1), NULL);
	if (!replacement)
		needle_p = p1;
	else
		needle_p = ft_strjoin(p1, replacement);
	if (!needle_p)
		return (free(p1), free(p2), NULL);
	if (clear_sources)
		free(str);
	str = ft_strjoin(needle_p, p2);
	if (clear_sources)
		free(replacement);
	if (replacement)
		free(needle_p);
	return (free(p1), free(p2), str);
}

int	is_a_valid_var_name(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

char	*parse_vars(char *cmd, size_t *i, t_minishell *data)
{
	char	*var_name;
	char	*env_value;
	char	*new_arg_str;
	size_t	j;

	j = *i + 1;
	while (is_a_valid_var_name((cmd[j])))
		j++;
	var_name = ft_substr(&(cmd[*i]), 0, j - *i);
	if (!var_name)
		return (perror(NULL), cmd);
	env_value = get_varible(var_name + 1, data);
	new_arg_str = strreplace(cmd, var_name, env_value, 0);
	if (new_arg_str)
	{
		free(cmd);
		cmd = new_arg_str;
		if (env_value)
			*i += ft_strlen(env_value) - 1;
		else
			(*i)--;
	}
	else
		perror(NULL);
	return (free(var_name), cmd);
}

char	*subst_vars(char *cmd, t_minishell *data)
{
	size_t		i;
	char		*exit_code_char;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '$' && cmd[i + 1] == '?')
		{
			exit_code_char = ft_itoa(data->exit_code);
			i += ft_strlen(exit_code_char) - 1;
			cmd = strreplace(cmd, "$?", exit_code_char, 1);
		}
		if (cmd[i] == '$' && cmd[i + 1] != '\0' && cmd[i + 1] != ' ')
			cmd = parse_vars(cmd, &i, data);
		i++;
	}
	return (cmd);
}
