#include "minishell.h"

size_t	get_varname_len(char *varname)
{
	size_t	i;

	i = 0;
	while (*varname)
	{
		if (*varname == '=')
			break ;
		varname++;
		i++;
	}
	return (i);
}

char	**get_var_names(char **envp)
{
	char	**names_arr;
	char	**tmp;

	names_arr = malloc(sizeof(char *) * get_arr_sz(envp));
	if (!names_arr)
		return (NULL);
	tmp = names_arr;
	while (*envp)
	{
		*names_arr = ft_substr(*envp, 0, get_varname_len(*envp));
		if (!*names_arr)
			return (free_arr(tmp, NULL), NULL);
		*names_arr = ft_strjoin("$", *names_arr);
		if (!*names_arr)
			return (free_arr(tmp, NULL), NULL);
		envp++;
		names_arr++;
	}
	names_arr = NULL;
	return (tmp);
}

char	*strreplace(char *str, char *str_to_replace, char *replacement)
{
	char	*needle_p;
	char	*out_str;
	char	*p1;
	char	*p2;
	size_t	i;

	i = 0;
	needle_p = ft_strnstr(str, str_to_replace, SIZE_MAX);
	if (!needle_p)
		return (NULL);
	while (&(str[i]) != needle_p)
		i++;
	p1 = ft_substr(str, 0, i);
	if (!p1)
		return (NULL);
	p2 = ft_substr(str, i + ft_strlen(str_to_replace), SIZE_MAX);
	if (!p2)
		return (free(p1), NULL);
	needle_p = ft_strjoin(p1, replacement);
	if (!needle_p)
		return (free(p1), free(p2), NULL);
	out_str = ft_strjoin(needle_p, p2);
	return (free(p1), free(p2), free(needle_p), out_str);
}

char	**subst_vars(char **cmds)
{
	extern char	**environ;
	char		**var_names;
	char		**var_names_b;
	char		**cmds_b;

	cmds_b = cmds;
	var_names = get_var_names(environ);
	if (!var_names)
		return (NULL);
	var_names_b = var_names;
	while (*cmds)
	{
		var_names = var_names_b;
		while (*var_names)
		{
			while (ft_strnstr(*cmds, *var_names, SIZE_MAX))
				*cmds = strreplace(*cmds, *var_names, getenv((*var_names) + 1));
			var_names++;
		}
		cmds++;
	}
	return (cmds_b);
}
