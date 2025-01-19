/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_rout.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:26:31 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/20 00:10:34 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	opt_sleep(int time, t_philo *philo)
{
	struct timeval	begin;
	struct timeval	end;
	long			left;
	long			passed;
	long			usec;

	usec = time * 1000L;
	passed = 0;
	gettimeofday(&begin, NULL);
	while (passed < usec)
	{
		if (check_dead(philo->phdata))
			return (1);
		gettimeofday(&end, NULL);
		passed = (end.tv_sec - begin.tv_sec) * 1000000L + (end.tv_usec
				- begin.tv_usec);
		left = usec - passed;
		if (left > 1000)
			usleep(250);
	}
	return (0);
}

int	opt_philo_eat(t_philo *philo)
{
	if (check_dead(philo->phdata))
		return (1);
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
		return (1);
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
		return (1);
	if (gs_logs(philo->phdata, philo->id, "is eating"))
		return (1);
	if (opt_sleep(philo->phdata->time_to_eat, philo))
		return (1);
	put_down_forks(philo);
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->phdata->state);
	change_permission(philo);
	return (0);
}

int	opt_wait_permission(t_philo *philo)
{
	if (opt_sleep(philo->phdata->time_to_eat / 2, philo))
		return (1);
	while (!can_pickup_forks(philo))
	{
		usleep(100);
	}
	change_permission(philo);
	return (0);
}

int	opt_sleep_rout(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is sleeping"))
		return (1);
	if (opt_sleep(philo->phdata->time_to_sleep, philo))
		return (1);
	change_permission(philo);
	return (0);
}

void	*opt_routi(void *arg)
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
			if (opt_philo_eat(philo))
			{
				unlock_fork(philo);
				return (NULL);
			}
			if (opt_sleep_rout(philo) || think_routine(philo))
				return (NULL);
		}
	}
	return (NULL);
}
