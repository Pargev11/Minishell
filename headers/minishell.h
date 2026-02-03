/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:30:58 by pargev            #+#    #+#             */
/*   Updated: 2025/07/25 22:28:47 by pargev           ###   ########.fr       */
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
# include <stdint.h>
# include <stdbool.h>

typedef struct s_var_info
{
	char	*name;
	char	*value;
}	t_var_info;

typedef struct s_minishell
{
	char	*cwd;
	t_list	**env_list;
	int		exit_code;
}	t_minishell;

typedef enum tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_INPUT,
	TOK_HEREDOC,
	TOK_OUTPUT_APPEND,
	TOK_OUTPUT
}	t_tok_type;

typedef	struct s_tok_node
{
	t_tok_type	type;
	char		quote;
	char		*str;
}	t_tok_node;


// typedef	struct s_ast
// {
// 	void	*left;
// 	void	*right;
// }

//main
void		init(t_minishell *data);
void		end_program(t_minishell *data);

//signals
void		print_nl_handler(int sig);
void		print_nl_handler_sigquit(int sig);
void		interrupt_signal(int sig);
void		print_nl_handler_and_exit(int sig);

//parse
void	analize_command(char *command, t_minishell *data);

//envirement variables
t_var_info	*var_info(char *variable, t_minishell *data);
t_list		**env_to_list(void);
char		**list_to_env(t_minishell *data);
void		lst_add_sorted(t_list **lst, t_list *new);
t_var_info	*lst_content(t_list	*lst);
void		free_variable(void	*lst);
char		*get_varible(char *name, t_minishell *data);

#endif