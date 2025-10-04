/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_list_operations.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/04 22:49:12 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var_info	*lst_content(t_list	*lst)
{
	return ((t_var_info *)lst->content);
}

void	lst_add_sorted(t_list **lst, t_list *new)
{
	t_list	*current;
	t_list	*last;

	last = *lst;
	if (!last)
		*lst = new;
	else if (ft_strcmp(lst_content(last)->name, lst_content(new)->name) > 0)
	{
		new->next = last;
		*lst = new;
	}
	else
	{
		current = last->next;
		while (current != NULL)
		{
			if (ft_strcmp(lst_content(current)->name, lst_content(new)->name) > 0)
			{
				new->next = current;
				last->next = new;
				return ;
			}
			last = current;
			current = last->next;
		}
		last->next = new;
	}
}

void	free_variable(void	*content)
{
	t_var_info	*variable_info;

	variable_info = content;
	free(variable_info->name);
	free(variable_info->value);
	free(variable_info);
}