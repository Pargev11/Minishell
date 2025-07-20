/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/07/13 18:06:33 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(void)
{
	char		*command;
	char		*prompt_str;
	t_minishell	data;

	data.cwd = getcwd(NULL, 0);
	while (1)
	{
		prompt_str = ft_strjoin(data.cwd, "> ");
		command = readline(prompt_str);
		free(prompt_str);
		analize_command(command, &data);
	}
	free(data.cwd);
}

int	main(void)
{
	minishell();
	return (0);
}
