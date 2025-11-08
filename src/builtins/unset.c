/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/11/08 16:33:51 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			*data->env_list = current;
			ft_lstdelone(last, free_variable);
			continue ;
		}
		while (current != NULL)
		{
			if (ft_strcmp(lst_content(current)->name, cmds[i]) == 0)
			{
				last->next = current->next;
				ft_lstdelone(current, free_variable);
				break ;
			}
			last = last->next;
			current = last->next;
		}
	}
	data->exit_code = 0;
}
