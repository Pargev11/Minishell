/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/09/14 17:01:59 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(t_minishell *data)
{
	char		*command;
	char		*prompt_str;

	while (1)
	{
		prompt_str = ft_strjoin(data->cwd, "$ ");
		command = readline(prompt_str);
		free(prompt_str);
		analize_command(command, data);
	}
	free(data->cwd);
}

int	main(void)
{
	t_minishell	data;

	init(&data);
	minishell(&data);
	return (0);
}
