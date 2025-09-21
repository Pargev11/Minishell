/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/09/21 17:15:32 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_nvirements(char **env)
{
	int			i;
	int			j;
	char		*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while(env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			++j;
		}
		++i;
	}
}

t_env_info get_env_info(char *env)
{
	char		*env_name_end;
	t_env_info	env_info;
	int			env_name_size;

	// printf("%s%s\n", env);
	env_name_end = ft_strchr(env, '=');
	if (env_name_end == 0)
	{
		env_name_size = ft_strlen(env);
		env_info.env_value_name = ft_strdup("\0");
	}
	else
	{
		env_name_size = env_name_end - env;
		env_info.env_value_name = ft_substr(env, env_name_size+1, ft_strlen(env) - env_name_size);
	}
	env_info.env_name = ft_substr(env, 0, env_name_size);
	return (env_info);
}

int	copy_vars(char **vars, char **tmp_env, t_minishell *data)
{
	int	i;
	int	j;

	i = 0;
	while (tmp_env[i])
	{
		data->env[i] = ft_strdup(tmp_env[i]);
		if (!data->env[i])
			return (0);
		i++;
	}
	j = 0;
	while (vars[j])
	{
		data->env[i] = ft_strdup(vars[j]);
		if (!data->env[i])
			return (0);
		i++;
		j++;
	}
	data->env[i] = NULL;
	return (1);
}

int	export_var(char **vars, t_minishell *data)
{
	char		**tmp_env;
	int			env_count;
	int			args_count;

	tmp_env = data->env;
	env_count = 0;
	while (tmp_env[env_count])
		env_count++;
	args_count = 0;
	while (vars[args_count])
		args_count++;
	data->env = (char **)malloc(sizeof(char *) * (env_count + args_count + 1));
	if (!data->env)
	{
		free(tmp_env);
		return (0);
	}
	if (!copy_vars(vars, tmp_env, data))
	{
		free_matrix(vars);
		free(tmp_env);
		return (0);
	}
	free(tmp_env);
	return (1);
}

char	**sort_vars(char **vars, t_minishell *data)
{
	t_env_info	env_info;
	char		**new_vars;
	int			i;

	new_vars = vars;
	i = 0;
	while (vars[i])
	{
		env_info = get_env_info(vars[i]);
		printf("%s\n", env_info.env_name);
		if (!check_name(env_info.env_name))
		{
			printf("bash: export: `%s': not a valid identifier\n", env_info.env_name);
			new_vars = remove_var(new_vars, i);
			if (!new_vars)
				return (0);
		}
		free(env_info.env_name);
		free(env_info.env_value_name);
		i++;
	}
	return (new_vars);
}

int export(char **cmds, t_minishell *data)
{
	t_env_info	env_info;
	char		**sorted_vars;
	int			i;

	if (cmds[1] == NULL)
	{
		i = 0;
		sort_nvirements(data->env);
		while (data->env[i])
		{
			env_info = get_env_info(data->env[i++]);
			if (ft_strncmp(env_info.env_name, "COLUMNS", 8) && ft_strncmp(env_info.env_name, "LINES", 6))
				printf("declare -x %s=\"%s\"\n", env_info.env_name, env_info.env_value_name);
			free(env_info.env_name);
			free(env_info.env_value_name);
		}
		return (1);
	}
	sorted_vars = sort_vars(cmds + 1, data);
	export_var(sorted_vars, data);
	return (1);
}