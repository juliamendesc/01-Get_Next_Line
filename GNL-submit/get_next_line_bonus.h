/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcarva <julcarva@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:21:16 by julcarva          #+#    #+#             */
/*   Updated: 2021/04/15 20:42:12 by julcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

# define READLINE_OK 1
# define EOF_REACHED 0
# define ERR_HAPPENED -1

# define NOT_ENDLINE 0
# define FOUND_ENDLINE 1

int		get_next_line(int fd, char **line);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif
