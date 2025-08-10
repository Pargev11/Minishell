/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:30:58 by pargev            #+#    #+#             */
/*   Updated: 2025/08/10 17:28:55 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <termios.h>

typedef struct s_minishell
{
	char	*cwd;
}	t_minishell;

//signals
void	print_nl_handler(int sig);
void	interrupt_signal(int sig);

void	init(t_minishell *data);
void	end_program(t_minishell *data);

void	analize_command(char *command, t_minishell *data);

char	**parse_words(char *cmd);

//built-ins
int		cd(char **cmds, t_minishell *data);
int		pwd(char **cmds);
int		exit_cmd(char **cmds, t_minishell *data);
int		echo(char *command, char **cmds, t_minishell *data);

//bins execution
void	exec(char **cmds);
char	*search_for_path(char **envp, char *program);
char	*dist_path_line(char **envp);

//cleanup
void	free_arr(char **arr, char ***cmds);

#endif