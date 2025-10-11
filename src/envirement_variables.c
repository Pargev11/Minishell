/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement_variables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/05 19:34:00 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var_info *var_info(char *variable)
{
	char		*var_name_end;
	t_var_info	*variable_info;
	int			var_name_size;

	variable_info = (t_var_info *)malloc(sizeof(t_var_info));
	if (!variable_info)
		return (0);
	var_name_end = ft_strchr(variable, '=');
	if (*variable == '=')
	{
		var_name_size = ft_strlen(variable);
		variable_info->name = ft_strdup("\0");
	}
	if (var_name_end == 0)
	{
		var_name_size = ft_strlen(variable);
		variable_info->value = NULL;
	}
	else
	{
		var_name_size = var_name_end - variable;
		variable_info->value = ft_substr(variable, var_name_size + 1, ft_strlen(variable) - var_name_size);
	}
	variable_info->name = ft_substr(variable, 0, var_name_size);
	return (variable_info);
}

void	increase_shlvl(t_var_info *var_info)
{
	int	i;

	i = 0;
	while (var_info->value[i])
	{
		if (!ft_isalnum(var_info->value[i]))
		{
			free(var_info->value);
			var_info->value = ft_strdup("1");
			break ;
		}
		i++;
	}
	var_info->value = ft_itoa(ft_atoi(var_info->value)+1);
}

t_list	**env_to_list()
{
	extern char **environ;
	t_var_info	*variable_info;
	t_list		**env_list;
	
	env_list = (t_list**)malloc(sizeof(t_list *));
	if (!env_list)
		return (0);
	*env_list = NULL;
	int i = 0;
	while (*environ)
	{
		variable_info = var_info(*environ);
		if (ft_strncmp(variable_info->name, "SHLVL", 6) == 0)
			increase_shlvl(variable_info);
		lst_add_sorted(env_list, ft_lstnew(variable_info));
		environ++;
		i++;
	}
	return (env_list);
}

char	**list_to_env(t_minishell *data)
{
	char	**env;
	t_list	*current;
	int		i;
	
	current = *(data->env_list);
	env = (char **)malloc(sizeof(char *) * (ft_lstsize(current) + 1));
	if (!env)
		return (0);
	i = 0;
	while (current != NULL)
	{
		if (lst_content(current)->value != NULL)
		{
			env[i] = ft_strjoin(lst_content(current)->name, "=");
			env[i] = ft_strjoin(env[i], lst_content(current)->value);
			i++;
		}
		current = current->next;
	}
	env[i] = NULL;

	return (env);
}