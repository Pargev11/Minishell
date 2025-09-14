/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/09/14 18:00:00 by pamalkha         ###   ########.fr       */
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

int	copy_env(t_minishell *data)
{
	extern char **environ;
	int			env_count;
	int			i;

	env_count = 0;
	while (environ[env_count])
		env_count++;
	data->env = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!data->env)
		return (0);
	i = 0;
	while (environ[i])
	{
		data->env[i] = ft_strdup(environ[i]);
		if (!data->env[i])
			return (0);
		i++;
	}
	data->env[i] = NULL;
	return (1);
}

void	init(t_minishell *data)
{
	if (!copy_env(data))
		exit(0);
	data->cwd = getcwd(NULL, 0);
	signal(SIGINT, interrupt_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	end_program(t_minishell *data)
{
	int	i;

	free(data->cwd);
	i = 0;
	while (data->env[i])
	{
		free(data->env[i]);
		i++;
	}
	
	free(data->env);
}
