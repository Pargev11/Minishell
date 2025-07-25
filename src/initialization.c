/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/07/25 22:29:35 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	config_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	init(t_minishell *data)
{
	data->cwd = getcwd(NULL, 0);
	config_terminal();
	signal(SIGINT, interrupt_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	end_program(t_minishell *data)
{
	free(data->cwd);
}
