/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subst_env_vars2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <vlchinen@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:58:09 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/12 15:58:12 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	*ptr_arr[3];
	size_t	i;

	i = 0;
	ptr_arr[2] = ft_strnstr(str, str_to_replace, SIZE_MAX);
	if (!ptr_arr[2])
		return (NULL);
	while (&(str[i]) != ptr_arr[2])
		i++;
	ptr_arr[0] = ft_substr(str, 0, i);
	if (!ptr_arr[0])
		return (NULL);
	ptr_arr[1] = ft_substr(str, i + ft_strlen(str_to_replace), SIZE_MAX);
	if (!ptr_arr[1])
		return (free(ptr_arr[0]), NULL);
	if (!replacement)
		ptr_arr[2] = ptr_arr[0];
	else
		ptr_arr[2] = ft_strjoin(ptr_arr[0], replacement);
	if (!ptr_arr[2])
		return (free(ptr_arr[0]), free(ptr_arr[1]), NULL);
	str = ft_strjoin(ptr_arr[2], ptr_arr[1]);
	if (replacement)
		free(ptr_arr[2]);
	return (free(ptr_arr[0]), free(ptr_arr[1]), str);
}

int	is_a_valid_var_name(char *s)
{
	if ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')
		|| (*s >= '0' && *s <= '9' && s[-1] != '$') || *s == '_')
		return (1);
	return (0);
}
