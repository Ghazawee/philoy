#include "philo.h"

int is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *s)
{
	int				i;
	int				sign;
	unsigned long	r;

	i = 0;
	r = 0;
	sign = 1;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign *= -1;
		i++;
	}
	while (s[i] && s[i] >= '0' && s[i] <= '9')
	{
		r = r * 10 + (s[i] - '0');
		if (sign == -1 && r >= LLONG_MAX)
			return (0);
		if (r >= LLONG_MAX)
			return (-1);
		i++;
	}
	return (r * sign);
}

// void	*ft_calloc(size_t count, size_t size)
// {
// 	unsigned char	*s;
// 	unsigned int	i;

// 	if (size && count > (SIZE_MAX / size))
// 		return (NULL);
// 	i = count * size;
// 	s = malloc(i);
// 	if (!s)
// 		return (NULL);
// 	ft_memset(s, 0, i);
// 	return (s);
// }