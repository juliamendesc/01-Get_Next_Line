#include "get_next_line.h"

static void		free_and_null(char **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

static void	copy_excess_buffer(char **buffer, long long j)
{
	long long	i;
	char		tmp[ARG_MAX];

	i = 0;
	if (buffer[0][j] == '\0')
		free_and_null(&*buffer);
	else
	{
		while (buffer[0][j] != '\0')
			tmp[i++] = buffer[0][j++];
		tmp[i] = 0;
		free_and_null(&*buffer);
		*buffer = ft_strdup(tmp);
	}
}

static int	buffer_to_line(char **line, char **buffer)
{
	long long	i;
	long long	j;
	char		tmp[ARG_MAX];

	i = 0;
	j = 0;
	while (line[0][i] != '\0')
	{
		tmp[i] = line[0][i];
		i++;
	}
	free_and_null(&*line);
	while (buffer[0][j] != '\0' && buffer[0][j] != '\n')
		tmp[i++] = buffer[0][j++];
	tmp[i] = '\0';
	*line = ft_strdup(tmp);
	if (buffer[0][j] == '\n')
	{
		copy_excess_buffer(&*buffer, j + 1);
		return (FOUND_ENDLINE);
	}
	free_and_null(&*buffer);
	return (NOT_ENDLINE);
}

int	get_next_line(int fd, char **line)
{
	static char	*buffer[OPEN_MAX];
	char		tmp[ARG_MAX];
	int			ret[2];

	*line = ft_strdup("");
	if (fd >= 0 && line && BUFFER_SIZE > 0 && *line)
	{
		ret[0] = NOT_ENDLINE;
		while (ret[0] == NOT_ENDLINE)
		{
			if (buffer[fd] == NULL)
			{
				ret[1] = read(fd, tmp, BUFFER_SIZE);
				if (ret[1] >= 0)
				{
					tmp[ret[1]] = '\0';
					buffer[fd] = ft_strdup(tmp);
				}
				else
					break ;
			}
			if (buffer[fd] != 0)
				ret[1] = ft_strlen(buffer[fd]);
			ret[0] = buffer_to_line(&*line, &buffer[fd]);
			if (ret[1] == 0)
				return (EOF_REACHED);
		}
		if (ret[0] == FOUND_ENDLINE)
			return (READLINE_OK);
	}
	free_and_null(&*line);
	return (ERR_HAPPENED);
}
