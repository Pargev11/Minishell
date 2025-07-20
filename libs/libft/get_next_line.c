/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:20:44 by pamalkha          #+#    #+#             */
/*   Updated: 2025/05/14 20:06:18 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*read_buffer(int fd, char *line)
{
	char	*buffer;
	ssize_t	read_bytes;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
	{
		free(line);
		return (0);
	}
	buffer[0] = 0;
	read_bytes = 1;
	while (!ft_strchr(buffer, '\n') && read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(line);
			free(buffer);
			return (0);
		}
		buffer[read_bytes] = 0;
		line = ft_strjoin2(line, buffer);
	}
	free(buffer);
	return (line);
}

char	*find_first_line(char *line)
{
	int		size;
	int		i;
	char	*new_line;

	size = 0;
	if (!line[0])
		return (0);
	while (line[size] != 0 && line[size] != '\n')
		size++;
	new_line = (char *)malloc(sizeof(char) * (size + 2));
	if (!new_line)
		return (0);
	i = 0;
	while (line[i] != 0 && line[i] != '\n')
	{
		new_line[i] = line[i];
		i++;
	}
	if (line[size] == '\n')
	{
		new_line[size++] = '\n';
	}
	new_line[size] = 0;
	return (new_line);
}

char	*next_line(char *line)
{
	int		i;
	char	*res;
	int		j;

	i = 0;
	while (line[i] != 0 && line[i] != '\n')
		i++;
	if (!line[i])
	{
		free(line);
		return (0);
	}
	res = (char *)malloc(sizeof(char) * (ft_strlen(line) - i + 1));
	if (!res)
		return (0);
	j = 0;
	while (line[++i] != 0)
		res[j++] = line[i];
	res[j] = 0;
	free(line);
	return (res);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*new_line;

	if (fd < 0 || BUFFER_SIZE < 0)
	{
		if (line)
			free(line);
		line = NULL;
		return (0);
	}
	if (!(line && ft_strchr(line, '\n')))
		line = read_buffer(fd, line);
	if (!line)
		return (0);
	new_line = find_first_line(line);
	line = next_line(line);
	return (new_line);
}
