/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:21:59 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 21:22:43 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	gs_sleep(int time, t_philo *philo)
{
	unsigned long	start;
	unsigned long	t;

	t = time;
	start = get_time();
	while (get_time() - start < t)
	{
		if (check_dead(philo->phdata))
			return (1);
		usleep(100);
	}
	return (0);
}

int	gs_logs(t_phdata *phdata, int id, char *msg)
{
	pthread_mutex_lock(&phdata->print);
	if (check_dead(phdata))
	{
		pthread_mutex_unlock(&phdata->print);
		return (1);
	}
	printf("%ld %d %s\n", get_time() - phdata->start_time, id, msg);
	pthread_mutex_unlock(&phdata->print);
	return (0);
}

int	philo_eat(t_philo *philo)
{
	if (check_dead(philo->phdata))
		return (1);
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
		return (1);
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
		return (1);
	if (gs_logs(philo->phdata, philo->id, "is eating"))
		return (1);
	if (gs_sleep(philo->phdata->time_to_eat, philo))
		return (1);
	put_down_forks(philo);
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->phdata->state);
	change_permission(philo);
	return (0);
}

int	think_routine(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is thinking"))
		return (1);
	return (0);
}

int	sleep_rout(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is sleeping"))
		return (1);
	if (gs_sleep(philo->phdata->time_to_sleep, philo))
		return (1);
	change_permission(philo);
	return (0);
}
