/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcarva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 18:18:11 by julcarva          #+#    #+#             */
/*   Updated: 2021/04/16 18:18:13 by julcarva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len] != 0)
		len++;
	return (len);
}

char	*ft_strdup(const char *s)
{
	size_t	size;
	size_t	i;
	char	*copy;

	size = ft_strlen(s) + 1;
	copy = malloc((unsigned int)size);
	if (!copy)
		return (NULL);
	i = 0;
	while (i < size)
	{
		((unsigned char *)copy)[i] = ((unsigned char *)s)[i];
		i++;
	}
	return (copy);
}
