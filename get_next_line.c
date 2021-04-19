/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcarva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 18:16:09 by julcarva          #+#    #+#             */
/*   Updated: 2021/04/16 18:16:13 by julcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Project Requirements:
** This project will read a file, one line at a time.
** It should work with multiple files and variable buffer sizes, which is defined upon compilation.
** It should reach the EOF before moving on to the next file.
** External functions allowed: malloc, free, read.
** File reading should be done only once.
*/
/*
** Frees a pointer, and nulls it so it becomes less prone to leaks and bugs.
** @param:	- [char **] a double pointer
** @return:	no returns;
** More info on
** https://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free
*/

static void	free_and_null(char **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

/*
** Copies the remainder of the line into a temporary variable, then
** updates the buffer.
** @param:	- [char **] current buffer
**			- [long long] current buffer position
** @return:	no returns;
** Line-by-line comments:
** @61	-	working with long long ints enables for up to 64 bit systems
**			maximum permissible size for a compiler;
** @62	-	create a string to hold the current line (already read) and
**			add the next line
** @65-66	-	if the current j in the buffer is '\0', free and null it;
** @67-71-	otherwise, while it is different, copy into the temporary
**			variable, which will receive a '\0' in the end.
** @72-73-	free and null the buffer, and redefine it by duplicating
**			the tmp variable.
*/

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
		tmp[i] = '\0';
		free_and_null(&*buffer);
		*buffer = ft_strdup(tmp);
	}
}

/*
** Gets the line from the buffer to the variable line in order to return it
** calls for the copy_excess_buffer function to update the current buffer
** with the remainder.
** @param:	- [char **] dest of the line
**			- [char **] current buffer
** @return:	[int] 1 found the end of the line, 0 not the end of the line;
** Line-by-line comments:
** @100-101	-	working with long long ints enables for up to 64 bit systems
**				maximum permissible size for a compiler;
** @102		-	create a string to hold the current line (already read) and
**				add the next line
** @107-111	-	while we do not reach the end of the string in the buffer, we
**				copy the line into tmp;
** @112		-	free the existing line (eithe non-existent in the first call
**				or already read);
** @114-117	-	while we don't reach the end of string and while we don't find
**				the '\n' character, we join the new line into the tmp variable
**				and add the '\0' char in the end;
** @118		-	redefine the line value by duplicating the tmp;
** @119-123	-	if the buffer position is '\n', copy everything from the next
**				position and return that the end of line has been reached (1);
** @124-125	-	if not, we free and null the buffer memory and return 0
*/

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

/*
** Passed if there is still no buffer. It reads the file, copies its content to
** a temporary variable, adds the final character to end the string and duplicates
** the value into the buffer[fd] variable.
** @param:	- [int] fd
**			- [char **] buffer
**			- [char *] tmp
** @return:	no returns;
** Line-by-line comments:
** @149 - ret[0] holds indicators that will tell us if we are in the last line
** 		or not and ret[1] stores the read function return (bytes read) or the
**		length of the remainder;
** @151 -	We set up ret[0] to guarantee that we will not read the last line;
** @152 -	We set the size of bytes read into ret[1] and read the file into
**			the tmp variable;
** @153 -	If the returned size is equal or higher than 0, we add the '\0' at
**			the end of the file read into tmp;
*/

static void	read_and_copy(int fd, char **buffer, char *tmp)
{
	int	ret[2];

	ret[0] = NOT_ENDLINE;
	ret[1] = read(fd, tmp, BUFFER_SIZE);
	if (ret[1] >= 0)
	{
		tmp[ret[1]] = '\0';
		buffer[fd] = ft_strdup(tmp);
	}
}

/*
** Gets the next line in a open file or stdin. Called in a loop. The line
** passed as an argument gets freed by the caller, NOT get_next_line()
** @param:	- [int] open file or stdin
**			- [char **] dest of the line
** @return:	[int] 1 new line is found, 0 EOF reached, -1 error occurred;
** Line-by-line comments:
** @195 - static variable that will hold all the read size
**		it has a maximum number of files that one process can have
**		open at any one time;
** @196 - A string that will hold the file read according to the determined size.
**		Maximum length of argument to the exec functions including environment
**		data (4096);
** @197 - ret[0] holds indicators that will tell us if we are in the last line
** 		or not and ret[1] stores the read function return (bytes read) or the
**		length of the remainder;
** @199	- Checks for errors. If not met it will return error.
**		read will test if a file descriptor is valid
**		fd - A file descriptor is a non-negative integer;
**		line - must exist to be read;
**		BUFFER_SIZE > 0 - read function only works with positive integers;
**		*line - strdup - we create a pointer to the line variable
**		so that we can change the value from gnl variable (meaning each line can be
**		read and then changed);
** @204 - Create the string we need will read;
** @205 - Set ret[0] as 0 initially to enter the while loop;
** @208-211 - If there is still no buffer, we call the read_and_copy function.
**			If the reading size is below 0, we break and exit the loop.
** @214 - If the buffer is not null (already existing remainder there), ret[1] is
**		redefined to its length;
** @215 - we redefine ret[0] as the return of the ft_buffer_to_line function;
** @216-217 - if ret[1] is 0 (no length to be read), we return the value corresponding
**		to end of file (0);
** @219 - After the while loop, if ret[0] is equal to 1, we return 1 (line read)
*/

int	get_next_line(int fd, char **line)
{
	static char	*buffer[OPEN_MAX];
	char		tmp[ARG_MAX];
	int			ret[2];

	if (read(fd, 0, 0) == -1 || fd < 0 || !line || BUFFER_SIZE < 1)
		return (ERR_HAPPENED);
	*line = ft_strdup("");
	ret[0] = NOT_ENDLINE;
	while (ret[0] == NOT_ENDLINE)
	{
		if (buffer[fd] == NULL)
		{
			read_and_copy(fd, &*buffer, tmp);
			if (ret[1] < 0)
				break ;
		}
		if (buffer[fd] != 0)
			ret[1] = ft_strlen(buffer[fd]);
		ret[0] = buffer_to_line(&*line, &buffer[fd]);
		if (ret[1] == 0)
			return (EOF_REACHED);
	}
	return (READLINE_OK);
}
