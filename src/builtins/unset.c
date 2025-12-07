/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/12/07 16:12:55 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*remove_lst(t_list *current, t_list *next)
{
	t_list	*last;

	last = next;
	ft_lstdelone(current, free_variable);
	return (last);
}

void	unset(char **cmds, t_minishell *data)
{
	t_list	*last;
	t_list	*current;
	int		i;

	i = 0;
	while (cmds[++i])
	{
		last = *(data->env_list);
		current = last->next;
		if (ft_strcmp(lst_content(last)->name, cmds[i]) == 0)
		{
			*data->env_list = remove_lst(last, current);
			continue ;
		}
		while (current != NULL)
		{
			if (ft_strcmp(lst_content(current)->name, cmds[i]) == 0)
			{
				last->next = remove_lst(current, current->next);
				break ;
			}
			last = last->next;
			current = last->next;
		}
	}
}
