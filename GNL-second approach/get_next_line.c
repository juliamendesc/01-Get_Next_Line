/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcarva <julcarva@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 20:01:42 by apuchill          #+#    #+#             */
/*   Updated: 2021/04/12 16:05:33 by julcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void		ft_free_and_null(char **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

int	check_newline(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int	split_line(char **buffer, char **line, int split_index)
{
	char	*tmp;
	int		len;

	(*buffer)[split_index] = '\0';
	*line = ft_strdup(*buffer); // efetivamente altera o valor da linha por ser pointer de pointer.
	len = ft_strlen(*buffer + split_index + 1);
	if (len == 0)
	{
		ft_free_and_null(&*buffer);
		return (1);
	}
	tmp = ft_strdup((*buffer) + split_index + 1); // PRECISO ENTENDER ISTO MELHOR
	printf("tmp split_line = %s", tmp);
	free(*buffer);
	*buffer = tmp;
	return (1);
}

int	return_all(char **buffer, char **line, int read_size)
{
	int	split_index;

	if (read_size < 0)
		return (-1);
	if (*buffer && (split_index = check_newline(*buffer)) > -1)
		return (split_line(buffer, line, split_index));
	else if (*buffer) // isto é para a última linha. quando já não há '\n', só EOF, portanto o primeiro if vai ser falso
	{
		*line = *buffer;
		*buffer = 0;
		return (0);
	}
	*line = ft_strdup("");
	return (0);
}

int	get_next_line(int fd, char **line)
{
	static char	*buffer[OPEN_MAX]; //Maximum number of files that one process can have open at any one time.
	char		tmp[ARG_MAX]; //Maximum length of argument to the exec functions including environment data. (4096)
	int			read_size;
	int			split_index;

	if ((fd > -1 && !(line == 0) && (BUFFER_SIZE > 0)))
	{
		while ((read_size = read (fd, tmp, BUFFER_SIZE)) > 0)
		{
			tmp[read_size] = '\0';
			buffer[fd] = ft_strjoin(buffer[fd], tmp);
			if ((split_index = check_newline(buffer[fd])) > -1)
				return (split_line (&buffer[fd], line, split_index)); //VOLTAR AQUI
		}
		return (return_all(&buffer[fd], line, read_size));
	}
	ft_free_and_null(&*line);
	return (-1);
}
