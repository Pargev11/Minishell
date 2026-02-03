#include "minishell.h"

// void	analize_command(char *command, t_minishell *data)
// {
// 	t_cmds	cmds;

// 	if (!command)
// 		analize_command_pt2(data);
// 	if (*command)
// 	{
// 		add_history(command);
// 		cmds = parse_words(command, data);
// 		if (cmds.cmds && *(cmds.cmds))
// 		{
// 			signal(SIGINT, print_nl_handler);
// 			signal(SIGQUIT, print_nl_handler_sigquit);
// 			if (char_cmds_count(cmds.cmds) == 1
// 				&& changes_shell_state(cmds.cmds[0]))
// 				execute_command(&cmds, data);
// 			else
// 				execute_pipeline(&cmds, char_cmds_count(cmds.cmds), data);
// 			signal(SIGINT, interrupt_signal);
// 			signal(SIGQUIT, SIG_IGN);
// 		}
// 		free_cmds(&cmds);
// 	}
// 	free(command);
// }

void	analize_command(char *command, t_minishell *data)
{
	//tokenize

	//expand variables

	//parse into AST

	//execute AST
}
