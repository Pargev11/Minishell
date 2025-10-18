/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/18 14:55:19 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **cmds, t_minishell *data)
{
	t_list		*current;

	if (cmds[1] == NULL)
	{
		current = *(data->env_list);
		while (current != NULL)
		{
			if (lst_content(current)->value != NULL)
				printf("%s=%s\n", lst_content(current)->name,
					lst_content(current)->value);
			current = current->next;
		}
	}
	return (0);
}
