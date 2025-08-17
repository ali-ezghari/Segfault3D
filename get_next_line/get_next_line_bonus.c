/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohalaou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:45:40 by mohalaou          #+#    #+#             */
/*   Updated: 2024/11/29 13:42:37 by mohalaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*set_line(char *line)
{
	char	*remaining;
	int		i;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (!line[i])
		return (NULL);
	remaining = ft_substr(line, i + 1, ft_strlen(line) - i - 1);
	if (!remaining)
		return (NULL);
	line[i + 1] = '\0';
	return (remaining);
}

char	*fill_line(int fd, char *static_buffer, char *buf)
{
	ssize_t	ret;
	char	*tmp;

	while (1)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == -1)
		{
			free(static_buffer);
			static_buffer = NULL;
			return (NULL);
		}
		else if (ret == 0)
			break ;
		buf[ret] = '\0';
		if (!static_buffer)
			static_buffer = ft_strdup("");
		tmp = static_buffer;
		static_buffer = ft_strjoin(tmp, buf);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (static_buffer);
}

char	*get_next_line(int fd)
{
	static char	*static_buffer[MAX_FD];
	char		*line;
	char		*buf;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
	{
		free(buf);
		buf = NULL;
		return (NULL);
	}
	line = fill_line(fd, static_buffer[fd], buf);
	free(buf);
	buf = NULL;
	if (!line)
		return (NULL);
	static_buffer[fd] = set_line(line);
	return (line);
}
