/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/11/16 18:57:30 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_variables(char **cmds, t_minishell *data)
{
	t_var_info	*variable_info;
	int			i;

	data->exit_code = 0;
	i = 1;
	while (cmds[i])
	{
		if (*cmds[i] == '=')
		{
			ft_printfp("bash: export: `%s': not a valid identifier\n", cmds[i]);
			data->exit_code = 1;
		}
		else
		{
			variable_info = var_info(cmds[i], data);
			if (check_name(variable_info->name))
			{
				lst_add_sorted(data->env_list, ft_lstnew(variable_info));
			}
			else
			{
				data->exit_code = 1;
				ft_printfp("bash: export: `%s': not a valid identifier\n",
					variable_info->name);
				free_variable(variable_info);
			}
		}
		i++;
	}
}

void	export(char **cmds, t_minishell *data)
{
	t_list		*current;

	data->exit_code = 0;
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
					printf("declare -x %s=\"%s\"\n", lst_content(current)->name,
						lst_content(current)->value);
			}
			current = current->next;
		}
	}
	else
		export_variables(cmds, data);
}
