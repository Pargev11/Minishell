/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:30:58 by pargev            #+#    #+#             */
/*   Updated: 2025/11/15 14:37:34 by pamalkha         ###   ########.fr       */
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
# include <sys/stat.h>
# include <errno.h>
# include <fcntl.h>

# define SIZE_MAX 10124

typedef struct s_var_info
{
	char	*name;
	char	*value;
}	t_var_info;

typedef struct s_cmd
{
	char	*cmd;
	int		single_q_start;
	int		single_q_end;
}	t_cmd;

typedef struct s_minishell
{
	char	*cwd;
	t_list	**env_list;
	int		exit_code;
}	t_minishell;

typedef struct s_env_info
{
	char	*env_name;
	char	*env_value_name;
}	t_env_info;

//main
void		init(t_minishell *data);
void		end_program(t_minishell *data);

//signals
void		print_nl_handler(int sig);
void		interrupt_signal(int sig);

//parse
void		analize_command(char *command, t_minishell *data);
char		***parse_words(char *cmd, t_minishell *data);
char		*subst_vars(char *cmd, t_minishell *data);
char		***allocate_cmds(t_list **cmds_list);
void		handle_redirects(char ***cmds, int i);

//built-ins
void		cd(char **cmds, t_minishell *data);
void		pwd(char **cmds, t_minishell *data);
void		exit_cmd(char **cmds, t_minishell *data);
void		echo(char **cmds, t_minishell *data);
void		export(char **cmds, t_minishell *data);
void		env(char **cmds, t_minishell *data);
void		unset(char **cmds, t_minishell *data);

//bins execution
void		exec(char **cmds, t_minishell *data);
char		*search_for_path(t_minishell *data, char *program);
char		*dist_path_line(t_minishell *data);

//utils
int			ft_strcmp(const char *s1, const char *s2);
int			check_name(char *name);
int			get_arr_sz(char **arr_2d);
int			is_dir(char const *path);
void		free_arr(char **arr, char ***cmds);

//envirement variables
t_var_info	*var_info(char *variable);
t_list		**env_to_list(void);
char		**list_to_env(t_minishell *data);
void		lst_add_sorted(t_list **lst, t_list *new);
t_var_info	*lst_content(t_list	*lst);
void		free_variable(void	*lst);
char		*get_varible(char *name, t_minishell *data);

#endif