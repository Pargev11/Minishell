/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_list_add.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/12/07 01:00:01 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*lst_add_befor(t_list *current, t_list *new)
{
	new->next = current;
	return (new);
}

t_list	*lst_add_instand(t_list *last, t_list *current, t_list *new)
{
	if (lst_content(new)->value != NULL)
	{
		new->next = current->next;
		ft_lstdelone(current, free_variable);
		return (new);
	}
	else
	{
		ft_lstdelone(new, free_variable);
		return (last);
	}
}

void	lst_add_sorted2(t_list *last, t_list *new)
{
	t_list	*current;
	int		line_difference;

	current = last->next;
	while (current != NULL)
	{
		line_difference = ft_strcmp(lst_content(current)->name,
				lst_content(new)->name);
		if (line_difference > 0)
			last->next = lst_add_befor(current, new);
		else if (line_difference == 0)
			last->next = lst_add_instand(last->next, current, new);
		if (line_difference > 0 || line_difference == 0)
			return ;
		last = current;
		current = last->next;
	}
	last->next = new;
}

void	lst_add_sorted(t_list **lst, t_list *new)
{
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
		*lst = lst_add_befor(last, new);
	else if (line_difference == 0)
		*lst = lst_add_instand(*lst, *lst, new);
	else
		lst_add_sorted2(last, new);
	return ;
}
