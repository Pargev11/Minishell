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

void	debug_print_tok_stream(void *void_param)
{
	t_tok_node	*tok_node = (t_tok_node*)void_param;

	switch (tok_node->type)
	{
		case TOK_WORD:
			printf("TOK_WORD[%s](quote=%c)", tok_node->str, tok_node->quote);
			printf(" -> ");
			break;
		case TOK_PIPE:
			printf("TOK_PIPE");
			printf(" -> ");
			break;
		case TOK_INPUT:
			printf("TOK_INPUT");
			printf(" -> ");
			break;
		case TOK_HEREDOC:
			printf("TOK_HEREDOC");
			printf(" -> ");
			break;
		case TOK_OUTPUT_APPEND:
			printf("TOK_OUTPUT_APPEND");
			printf(" -> ");
			break;
		case TOK_OUTPUT:
			printf("TOK_OUTPUT");
			printf(" -> ");
			break;
	}
}


void	analize_command(char *command, t_minishell *data)
{
	t_tok_status	tok_status;
	t_list			*tok_stream;
	//tokenize
	tok_stream = tokenize(command, &tok_status);
	if (tok_status == TOK_STATUS_FATAL_ERROR)
		end_program(data);
	ft_lstiter(tok_stream, debug_print_tok_stream);
	printf("\n");

	//process heredoc_s

	//expand variables

	//parse into AST

	//execute AST
}
