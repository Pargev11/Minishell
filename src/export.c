/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/09/14 19:05:06 by pamalkha         ###   ########.fr       */
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

void	print_env(char *env)
{
	char	*env_name_end;
	char	*env_name;
	char	*env_value_name;
	int		env_name_size;

	// printf("%s%s\n", env);
	env_name_end = ft_strchr(env, '=');
	if (!env_name_end)
		return ;
	env_name_size = env_name_end - env;
	env_name = ft_substr(env, 0, env_name_size);
	env_value_name = ft_substr(env, env_name_size+1, ft_strlen(env) - env_name_size);
	if (ft_strncmp(env_name, "COLUMNS", 8) && ft_strncmp(env_name, "LINES", 6))
		printf("declare -x %s=\"%s\"\n", env_name, env_value_name);
	free(env_name);
	free(env_value_name);
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

void	free_matrix(char **vars)
{
	int	i;

	i = 0;
	while (vars[i])
	{
		free(vars[i]);
		i++;
	}
	free(vars);
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

int export(char **cmds, t_minishell *data)
{
	int			i;

	if (cmds[1] == NULL)
	{
		i = 0;
		sort_nvirements(data->env);
		while (data->env[i])
			print_env(data->env[i++]);
		return (1);
	}
	export_var(cmds + 1, data);
	return (1);
}