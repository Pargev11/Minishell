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
	if (!replacement)
		needle_p = p1;
	else
		needle_p = ft_strjoin(p1, replacement);
	if (!needle_p)
		return (free(p1), free(p2), NULL);
	str = ft_strjoin(needle_p, p2);
	return (free(p1), free(p2), str);
}

int		is_a_valid_var_name(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

size_t	parse_vars(char **cmds, size_t i)
{
	char	*var_name;
	char	*env_value;
	char	*new_arg_str;
	size_t	j;

	j = i + 1;
	while (is_a_valid_var_name((*cmds)[j]))
		j++;
	var_name = ft_substr(&((*cmds)[i]), 0, j - i + 1);
	if (!var_name)
		return (perror(NULL), i);
	env_value = getenv(var_name + 1);
	new_arg_str = strreplace(*cmds, var_name, env_value);
	if (new_arg_str)
	{
		free(*cmds);
		*cmds = new_arg_str;
		if (env_value)
			i += ft_strlen(env_value) - 1;
		else
			i--;
	}
	else
		perror(NULL);
	return (free(var_name), i);
}

char	**subst_vars(char **cmds, t_minishell *data)
{
	size_t		i;
	char		**cmds_b;

	cmds_b = cmds;
	while (*cmds)
	{
		i = 0;
		while (ft_strnstr(*cmds, "$?", SIZE_MAX))
			*cmds = strreplace(*cmds, "$?", ft_itoa(data->exit_code));
		while ((*cmds)[i])
		{
			if ((*cmds)[i] == '$')
				i = parse_vars(cmds, i);
			i++;
		}
		cmds++;
	}
	return (cmds_b);
}
