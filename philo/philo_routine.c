/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:30:13 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 21:30:27 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	change_permission(t_philo *philo)
{
	pthread_mutex_lock(&philo->phdata->waiter);
	if (philo->permission == philo->phdata->num_philo)
		philo->permission = 1;
	else
		philo->permission += 1;
	pthread_mutex_unlock(&philo->phdata->waiter);
}

void	*handle_one_philo(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is thinking"))
		return (NULL);
	usleep(philo->phdata->time_to_die * 1000);
	return (NULL);
}

int	permission_to_eat(t_philo *philo)
{
	int	permission;

	permission = 0;
	if (philo->permission % 2 == 0)
	{
		permission = 1;
		return (permission);
	}
	return (permission);
}

int	wait_permission(t_philo *philo)
{
	if (gs_sleep(philo->phdata->time_to_eat / 2, philo))
		return (1);
	while (!can_pickup_forks(philo))
	{
		usleep(100);
	}
	change_permission(philo);
	return (0);
}

void	*gs_routi(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->phdata->num_philo == 1)
		return (handle_one_philo(philo));
	while (!check_dead(philo->phdata))
	{
		if (permission_to_eat(philo))
		{
			take_forks(philo);
			if (philo_eat(philo))
			{
				unlock_fork(philo);
				return (NULL);
			}
			if (sleep_rout(philo) || think_routine(philo))
				return (NULL);
		}
		else
		{
			if (wait_permission(philo))
				return (NULL);
		}
	}
	return (NULL);
}
