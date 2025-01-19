/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pharse.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:36:43 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 21:38:58 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_mutexes(t_phdata *phdata)
{
	if (pthread_mutex_init(&phdata->print, NULL) != 0)
	{
		cleanup_mutexes(phdata);
		gs_error(4);
	}
	if (pthread_mutex_init(&phdata->state, NULL) != 0)
	{
		cleanup_mutexes(phdata);
		gs_error(4);
	}
	if (pthread_mutex_init(&phdata->waiter, NULL) != 0)
	{
		cleanup_mutexes(phdata);
		gs_error(4);
	}
	if (pthread_mutex_init(&phdata->stop_mutex, NULL) != 0)
	{
		cleanup_mutexes(phdata);
		gs_error(4);
	}
}

void	init_fork_mutex(t_phdata *phdata)
{
	int	i;

	i = 0;
	while (i < phdata->num_philo)
	{
		if (pthread_mutex_init(&phdata->forks[i], NULL) != 0)
		{
			free(phdata->forks_st);
			cleanup_mutexes(phdata);
			gs_error(4);
		}
		i++;
	}
}

void	init_philo(t_phdata *phdata)
{
	int	i;

	i = 0;
	while (i < phdata->num_philo)
	{
		phdata->philo[i].id = i + 1;
		phdata->philo[i].permission = i + 1;
		phdata->philo[i].meals_count = 0;
		phdata->philo[i].last_meal = phdata->start_time;
		phdata->philo[i].l_fork = i;
		phdata->philo[i].r_fork = (i + 1) % phdata->num_philo;
		phdata->philo[i].phdata = phdata;
		i++;
	}
}

void	init_forks_philo(t_phdata *phdata)
{
	phdata->forks_st = malloc(sizeof(char) * phdata->num_philo);
	if (!phdata->forks_st)
		gs_error(2);
	init_forks_state(phdata);
	phdata->forks = malloc(sizeof(pthread_mutex_t) * phdata->num_philo);
	if (!phdata->forks)
	{
		free(phdata->forks_st);
		gs_error(2);
	}
	init_fork_mutex(phdata);
	init_mutexes(phdata);
	phdata->philo = malloc(sizeof(t_philo) * phdata->num_philo);
	if (!phdata->philo)
	{
		free(phdata->forks_st);
		cleanup_mutexes(phdata);
		gs_error(2);
	}
	init_philo(phdata);
}

void	gs_init_phdata(char **av, t_phdata *phdata)
{
	if (parse_args(av) == 1)
		gs_error(1);
	phdata->num_philo = ft_atoi(av[1]);
	phdata->time_to_die = ft_atoi(av[2]);
	phdata->time_to_eat = ft_atoi(av[3]);
	phdata->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		phdata->eat_limit = ft_atoi(av[5]);
		if (phdata->eat_limit < 1)
			exit(0);
	}
	else
		phdata->eat_limit = -1;
	if (phdata->num_philo > 200 || phdata->num_philo < 1
		|| phdata->time_to_die < 60 || phdata->time_to_eat < 60
		|| phdata->time_to_sleep < 60)
		gs_error(1);
	phdata->stop_sim = 0;
	phdata->start_time = get_time();
	init_forks_philo(phdata);
}
