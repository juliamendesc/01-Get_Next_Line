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
** Gets the next line in a open file or stdin. Called in a loop. The line
** passed as an argument gets freed by the caller, NOT get_next_line()
** @param:	- [char **] a double pointer
** @return:	no returns;
** free a pointer, and nulls it so it becomes less prone to leaks and bugs.
** More info on
** https://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free
*/

static void	ft_free_and_null(char **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

/*
** Gets the next line in a open file or stdin. Called in a loop. The line
** passed as an argument gets freed by the caller, NOT get_next_line()
** @param:	- [char **] current buffer
**			- [long long] current buffer position
** @return:	no returns;
** Line-by-line comments:
** @1	-	working with long long ints enables for up to 64 bit systems
**			maximum permissible size for a compiler;
** @2	-	create a string to hold the current line (already read) and
**			add the next line
** @3	-	if the current j in the buffer is '\0', free and null it;
** @5	-	otherwise, while it is different, copy into the temporary
**			variable, which will receive a '\0' in the end.
** @6	-	free and null the buffer, and redefine it by duplicating
**			the tmp variable.
*/

static void	ft_copy_excess_buffer(char **buffer, long long j)
{
	long long	i;
	char		tmp[ARG_MAX];

	i = 0;
	if (buffer[0][j] == '\0')
		ft_free_and_null(&*buffer);
	else
	{
		while (buffer[0][j] != '\0')
			tmp[i++] = buffer[0][j++];
		tmp[i] = '\0';
		ft_free_and_null(&*buffer);
		*buffer = ft_strdup(tmp);
	}
}

/*
** Gets the next line in a open file or stdin. Called in a loop. The line
** passed as an argument gets freed by the caller, NOT get_next_line()
** @param:	- [char **] dest of the line
**			- [char **] current buffer
** @return:	[int] 1 found the end of the line, 0 not the end of the line;
** Line-by-line comments:
** @1-2 -	working with long long ints enables for up to 64 bit systems
**			maximum permissible size for a compiler;
** @3	-	create a string to hold the current line (already read) and
**			add the next line
** @5	-	copy the line into tmp;
** @6	-	free the existing line (eithe non-existent in the first call
**			or already read);
** @7-8	-	join the new line into the tmp variable and add the '\0' char;
** @9	-	redefine the line value by duplicating the tmp;
** @10	-	if the buffer position is '\n', copy everything from the next
**			position and return that the end of line has been reached (1);
** @11	-	if not, we free and null the buffer memory and return 0
*/

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
		tmp[i++] = buffer[0][j++];
	tmp[i] = '\0';
	*line = ft_strdup(tmp);
	if (buffer[0][j] == '\n')
	{
		ft_copy_excess_buffer(&*buffer, j + 1);
		return (FOUND_ENDLINE);
	}
	ft_free_and_null(&*buffer);
	return (NOT_ENDLINE);
}

/*
** Gets the next line in a open file or stdin. Called in a loop. The line
** passed as an argument gets freed by the caller, NOT get_next_line()
** @param:	- [int] open file or stdin
**			- [char **] dest of the line
** @return:	[int] 1 new line is found, 0 EOF reached, -1 error occurred;
** Line-by-line comments:
** @1 - static variable that will hold all the read size
**		it has a maximum number of files that one process can have
**		open at any one time;
** @2 - A string that will hold the file read according to the determined size.
**		Maximum length of argument to the exec functions including environment
**		data (4096);
** @3 - ret[0] holds indicators that will tell us if we are in the last line
** 		or not and ret[1] stores the read function return (bytes read) or the
**		length of the remainder;
** @4 && @15-16- Requirements to enter the function. If not met it will free the
**		line and return -1 (error happened);
**		fd - A file descriptor is a non-negative integer;
**		line - must exist to be read;
**		BUFFER_SIZE > 0 - read function only works with positive integers;
**		*line - strdup - we create a pointer to the line variable
**		so that we can change the value from gnl variable (meaning each line can be
**		read and then changed);
** @5 -	We set up ret[0] to guarantee that we will not read the last line;
** @6 -	We sabe the size of bytes read into ret[1] and read the file into the tmp variable;
** @7 - We add the '\0' at the end of the file read into tmp;
** @8 - While we know we are not in the last line, we enter the while loop
** @9 - If the buffer is null, the size read is equal or above 0 and the
**		last character of the tmp is '\0', we can duplicate the tmp into the buffer;
** @10 - If the buffer is not null (already existing remainder there), ret[1] is
**		redefined to its length;
** @11 - it ret[1] is below 0, we break and exit the while loop;
** @12 - we redefine ret[0] as the return of the ft_buffer_to_line function;
** @13 - if ret[1] is 0 (no length to be read), we return the value corresponding
**		to end of file (0);
** @14 - After the while loop, if ret[0] is equal to 1, we return 1 (line read)
*/

int	get_next_line(int fd, char **line)
{
	static char	*buffer[OPEN_MAX];
	char		tmp[ARG_MAX];
	int			ret[2];

	if (fd >= 0 && line && BUFFER_SIZE > 0)
	{
		*line = ft_strdup("\0");
		ret[0] = NOT_ENDLINE;
		while (ret[0] == NOT_ENDLINE)
		{
			ret[1] = read(fd, tmp, BUFFER_SIZE);
			tmp[ret[1]] = '\0';
			if (buffer[fd] == NULL && ret[1] >= 0 && tmp[ret[1]] == '\0')
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
