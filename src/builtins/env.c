/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/11/08 16:33:04 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(char **cmds, t_minishell *data)
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
	data->exit_code = 0;
}
