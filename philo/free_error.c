/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:24:23 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 21:24:36 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	gs_error(int err)
{
	if (err == 0)
		printf("Error: Wrong number of arguments\n");
	else if (err == 1)
		printf("Error: Invalid argument\n");
	else if (err == 2)
		printf("Error: Malloc failed\n");
	else if (err == 3)
		printf("Error: Thread creation failed\n");
	else if (err == 4)
		printf("Error: Mutex initialization failed\n");
	else if (err == 5)
		printf("Error: Thread join failed\n");
	exit(1);
}

void	cleanup_mutexes(t_phdata *phdata)
{
	int	i;

	i = 0;
	while (i < phdata->num_philo)
	{
		pthread_mutex_destroy(&phdata->forks[i]);
		i++;
	}
	free(phdata->forks);
	pthread_mutex_destroy(&phdata->print);
	pthread_mutex_destroy(&phdata->state);
	pthread_mutex_destroy(&phdata->waiter);
	pthread_mutex_destroy(&phdata->stop_mutex);
}

void	cleanup_all(t_phdata *phdata)
{
	if (phdata->forks)
		cleanup_mutexes(phdata);
	if (phdata->philo)
		free(phdata->philo);
	free(phdata->forks_st);
}
