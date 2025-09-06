/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/09/06 18:06:47 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_valid_var(t_minishell *data)
{
	
}

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

int export(char **cmds, t_minishell *data)
{
	extern char **environ;
	int			i;

	if (cmds[1] == NULL)
	{
		i = 0;
		sort_nvirements(environ);
		while (environ[i])
			print_env(environ[i++]);
		return (1);
	}
	
	return (1);
}