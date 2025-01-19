/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:30:13 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/20 00:12:01 by mshaheen         ###   ########.fr       */
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

void	*gs_routi(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->phdata->num_philo == 1)
		return (handle_one_philo(philo));
	while (!check_dead(philo->phdata))
	{
		if(can_pickup_forks(philo))
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
	}
	return (NULL);
}
