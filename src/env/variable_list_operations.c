/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_list_operations.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/12/07 00:03:21 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var_info	*lst_content(t_list	*lst)
{
	return ((t_var_info *)lst->content);
}

char	*get_varible(char *name, t_minishell *data)
{
	t_list	*current;

	current = *(data->env_list);
	while (current != NULL)
	{
		if (ft_strcmp(lst_content(current)->name, name) == 0)
			return (lst_content(current)->value);
		current = current->next;
	}
	return (0);
}

void	free_variable(void	*content)
{
	t_var_info	*variable_info;

	variable_info = content;
	if (variable_info->name)
		free(variable_info->name);
	if (variable_info->value)
		free(variable_info->value);
	free(variable_info);
}
