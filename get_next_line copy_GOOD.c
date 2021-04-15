#include "get_next_line.h"

/*
free a pointer and turn this pointer into a void pointer.
More info on this on https://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free
*/

static void		ft_free_and_null(char **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

static void	ft_copy_excess_buffer(char **buffer, long long j) // long long is for 64 bit - maximum permissible size for a compiler
{
	long long	i;
	char		tmp[ARG_MAX];

	i = 0;
	if (buffer[0][j] == '\0')
		ft_free_and_null(&*buffer);
	else
	{
		while (buffer[0][j] != '\0')
		{
			tmp[i] = buffer[0][j];
			i++;
			j++;
		}
		tmp[i] = '\0';
		ft_free_and_null(&*buffer);
		*buffer = ft_strdup(tmp);
	}
}

static int	ft_buffer_to_line(char **line, char **buffer)
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
	ft_free_and_null(&*line);
	while (buffer[0][j] != '\0' && buffer[0][j] != '\n')
	{
		tmp[i] = buffer[0][j];
		i++;
		j++;
	}
	tmp[i] = '\0';
	*line = ft_strdup(tmp);
	if (buffer[0][j] == '\n')
	{
		ft_copy_excess_buffer(&*buffer, j + 1); //copia o resto sempre após o '\n'
		return (FOUND_ENDLINE);
	}
	ft_free_and_null(&*buffer);
	return (NOT_ENDLINE);
}

int	get_next_line(int fd, char **line)
{
	static char	*buffer[OPEN_MAX]; //Maximum number of files that one process can have open at any one time.
	char		tmp[ARG_MAX]; //Maximum length of argument to the exec functions including environment data. (4096)
	int			ret[2]; // array com 2 posições. 0 guarda o valor de retorno de cada função, 1 guarda o tamanho de bytes lidos pelo read

	if (fd >= 0 && line && BUFFER_SIZE > 0 && (*line = ft_strdup("\0"))) /*Este if é para garantir que só se inicia algo desde que todas as
					condições básicas sejam atendidas.
					0-input, 1-output e 2-erro. São os casos que queremos abordar. Negativos são erro e portanto caem fora do if (free e return -1)
					line - tem de haver uma linha/string para ser lida
					buffer_size > 0 - o tamanho de leitura nunca pode ser negativo
					*line = ft_strdup("") - se nao houver pointer, cria-se um inicial.
					Se não estiverem, vai limpar a linha e retornar -1 erro */
	{
		ret[0] = NOT_ENDLINE;
		while (ret[0] == NOT_ENDLINE)
		{
			if (buffer[fd] == NULL && (ret[1] = read(fd, tmp, BUFFER_SIZE)) >= 0 && (tmp[ret[1]] = '\0') == 0)
				buffer[fd] = ft_strdup(tmp);
			if (buffer[fd] != NULL)
				ret[1] = ft_strlen(buffer[fd]);
			if (ret[1] < 0)
				break ;
			ret[0] = ft_buffer_to_line(&*line, &buffer[fd]);
			if (ret[1] == 0)
				return (EOF_REACHED);
		}
		if (ret[0] == FOUND_ENDLINE)
			return (READLINE_OK);
	}
	ft_free_and_null(&*line);
	return (ERR_HAPPENED);
}
