#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	int len;

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
	if (!(copy = malloc((unsigned int)size)))
		return (NULL);
	i = 0;
	while (i < size)
	{
		((unsigned char *)copy)[i] = ((unsigned char *)s)[i];
		i++;
	}
	//ft_memcpy(copy, s, size);
	return (copy);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;
	size_t	i;

	d = (char *)dest;
	s = (char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
