/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:30:45 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 21:39:40 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_digit(char c)
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

int	parse_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		if (av[i][0] == '\0')
			return (1);
		j = 0;
		while (av[i][j])
		{
			if (!is_digit(av[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

unsigned long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: gettimeofday failed\n");
		exit(1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	init_forks_state(t_phdata *phdata)
{
	int	i;

	i = 0;
	while (i < phdata->num_philo)
	{
		phdata->forks_st[i] = 1;
		i++;
	}
}
