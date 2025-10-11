/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/05 18:39:31 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_variables(char **cmds, t_minishell *data)
{
	t_var_info	*variable_info;
	int			i;

	i = 1;
	while (cmds[i])
	{
		variable_info = var_info(cmds[i]);
		if (check_name(variable_info->name))
			lst_add_sorted(data->env_list, ft_lstnew(variable_info));
		else
		{
			printf("bash: export: `%s': not a valid identifier\n", variable_info->name);
			free_variable(variable_info);
		}
		i++;
	}
}

int export(char **cmds, t_minishell *data)
{
	t_list		*current;

	if (cmds[1] == NULL)
	{
		current = *(data->env_list);
		while (current != NULL)
		{
			if (ft_strncmp(lst_content(current)->name, "_", 2) != 0)
			{
				if (lst_content(current)->value == NULL)
					printf("declare -x %s\n", lst_content(current)->name);
				else
					printf("declare -x %s=\"%s\"\n", lst_content(current)->name, lst_content(current)->value);
			}
			current = current->next;
		}
		return (1);
	}
	export_variables(cmds, data);
	return (1);
}