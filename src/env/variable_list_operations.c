/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_list_operations.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/18 17:07:21 by pamalkha         ###   ########.fr       */
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
	int		line_difference;

	last = *lst;
	if (!last)
	{
		*lst = new;
		return ;
	}
	line_difference = ft_strcmp(lst_content(last)->name,
			lst_content(new)->name);
	if (line_difference > 0)
	{
		new->next = last;
		*lst = new;
	}
	else if (line_difference == 0)
	{
		if (lst_content(new)->value != NULL)
		{
			new->next = (*lst)->next;
			ft_lstdelone(*lst, free_variable);
			*lst = new;
		}
		else
			ft_lstdelone(new, free_variable);
	}
	else
	{
		current = last->next;
		while (current != NULL)
		{
			line_difference = ft_strcmp(lst_content(current)->name,
					lst_content(new)->name);
			if (line_difference > 0)
			{
				new->next = current;
				last->next = new;
				return ;
			}
			else if (line_difference == 0)
			{
				if (lst_content(new)->value != NULL)
				{
					new->next = current->next;
					ft_lstdelone(current, free_variable);
					last->next = new;
				}
				else
					ft_lstdelone(new, free_variable);
				return ;
			}
			last = current;
			current = last->next;
		}
		last->next = new;
	}
	return ;
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
