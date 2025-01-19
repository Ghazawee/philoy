/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:23:03 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 23:34:13 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_order(t_philo *philo, int *first, int *second)
{
	if (philo->l_fork < philo->r_fork)
	{
		*first = philo->l_fork;
		*second = philo->r_fork;
	}
	else
	{
		*first = philo->r_fork;
		*second = philo->l_fork;
	}
}

void	unlock_fork(t_philo *philo)
{
	int	first;
	int	second;

	pick_order(philo, &first, &second);
	philo->phdata->forks_st[first] = 1;
	philo->phdata->forks_st[second] = 1;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
}

int	can_pickup_forks(t_philo *philo)
{
	int	first;
	int	second;
	int	f1_avail;
	int	f2_avail;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	f1_avail = philo->phdata->forks_st[first];
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	f2_avail = philo->phdata->forks_st[second];
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	if (f1_avail != philo->id && f2_avail != philo->id)
		return (1);
	return (0);
}

void	take_forks(t_philo *philo)
{
	int	first;
	int	second;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	philo->phdata->forks_st[first] = 0;
	pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[second] = 0;
}

void	put_down_forks(t_philo *philo)
{
	int	first;
	int	second;

	pick_order(philo, &first, &second);
	philo->phdata->forks_st[first] = philo->id;
	philo->phdata->forks_st[second] = philo->id;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
}
