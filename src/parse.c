#include "minishell.h"

char **parse_words(char *cmd)
{
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	if (!cmds)
		return (NULL);
	return (cmds);
}