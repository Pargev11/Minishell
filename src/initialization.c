/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/07/20 23:37:49 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_minishell *data)
{
	data->cwd = getcwd(NULL, 0);
	signal(SIGINT, interrupt_signal);
	signal(SIGQUIT, quit_signal);
}

void	end_program(t_minishell *data)
{
	free(data->cwd);
}
