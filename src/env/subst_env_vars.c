/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subst_env_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 22:39:25 by pargev            #+#    #+#             */
/*   Updated: 2025/11/15 17:12:10 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strreplace(char *str, char *str_to_replace, char *replacement)
{
	char	*ptr_arr[3];
	size_t	i;

	i = 0;
	ptr_arr[2] = ft_strnstr(str, str_to_replace, SIZE_MAX);
	if (!ptr_arr[2])
		return (NULL);
	while (&(str[i]) != ptr_arr[2])
		i++;
	ptr_arr[0] = ft_substr(str, 0, i);
	if (!ptr_arr[0])
		return (NULL);
	ptr_arr[1] = ft_substr(str, i + ft_strlen(str_to_replace), SIZE_MAX);
	if (!ptr_arr[1])
		return (free(ptr_arr[0]), NULL);
	if (!replacement)
		ptr_arr[2] = ptr_arr[0];
	else
		ptr_arr[2] = ft_strjoin(ptr_arr[0], replacement);
	if (!ptr_arr[2])
		return (free(ptr_arr[0]), free(ptr_arr[1]), NULL);
	str = ft_strjoin(ptr_arr[2], ptr_arr[1]);
	if (replacement)
		free(ptr_arr[2]);
	return (free(ptr_arr[0]), free(ptr_arr[1]), str);
}

int	is_a_valid_var_name(char *s)
{
	if ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')
		|| (*s >= '0' && *s <= '9' && s[-1] != '$') || *s == '_')
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
	while (is_a_valid_var_name(&(cmd[j])))
		j++;
	var_name = ft_substr(&(cmd[*i]), 0, j - *i);
	if (!var_name)
		return (perror(NULL), cmd);
	env_value = get_varible(var_name + 1, data);
	new_arg_str = strreplace(cmd, var_name, env_value);
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

char	*subst_vars(char *cmd, t_minishell *data, t_list **cmds)
{
	size_t		i;
	char		*exit_code_char;
	char		*tmp_str;

	i = 0;
	if (cmds && *cmds)
		if (!ft_strcmp(((t_cmd *)(ft_lstlast(*cmds)->content))->cmd, "<<"))
			return (cmd);
	while (cmd[i])
	{
		if (cmd[i] == '$' && cmd[i + 1] == '?')
		{
			exit_code_char = ft_itoa(data->exit_code);
			i += ft_strlen(exit_code_char) - 1;
			tmp_str = cmd;
			cmd = strreplace(cmd, "$?", exit_code_char);
			free(tmp_str);
			free(exit_code_char);
		}
		if (cmd[i] == '$' && cmd[i + 1] != '\0' && !ft_isspace(cmd[i + 1]))
			cmd = parse_vars(cmd, &i, data);
		i++;
	}
	return (cmd);
}
