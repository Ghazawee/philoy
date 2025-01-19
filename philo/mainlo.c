/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainlo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:25:23 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 21:52:20 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(t_phdata *phdata)
{
	int	i;

	i = 0;
	while (i < phdata->num_philo)
	{
		if (pthread_join(phdata->philo[i].thrd, NULL) != 0)
		{
			cleanup_all(phdata);
			gs_error(5);
		}
		i++;
	}
	if (pthread_join(phdata->monit, NULL) != 0)
	{
		cleanup_all(phdata);
		gs_error(5);
	}
}

void	create_mthreads(t_philo *philo)
{
	if (pthread_create(&philo->thrd, NULL, opt_routi, philo) != 0)
	{
		cleanup_all(philo->phdata);
		gs_error(3);
	}
}

void	create_threads(t_philo *philo)
{
	if (pthread_create(&philo->thrd, NULL, gs_routi, philo) != 0)
	{
		cleanup_all(philo->phdata);
		gs_error(3);
	}
}

void	create_monit(t_phdata *phdata)
{
	if (pthread_create(&phdata->monit, NULL, gs_mont, phdata) != 0)
	{
		cleanup_all(phdata);
		gs_error(3);
	}
}

int	main(int ac, char **av)
{
	t_phdata	phdata;
	int			i;

	if (ac != 5 && ac != 6)
		gs_error(0);
	gs_init_phdata(av, &phdata);
	i = -1;
	if (phdata.num_philo > 100)
	{
		while (++i < phdata.num_philo)
			create_mthreads(&phdata.philo[i]);
	}
	else
	{
		while (++i < phdata.num_philo)
			create_threads(&phdata.philo[i]);
	}
	create_monit(&phdata);
	join_threads(&phdata);
	cleanup_all(&phdata);
	return (0);
}
