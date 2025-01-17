#include "philo.h"

void	lock_set_unlock(pthread_mutex_t *mutex, int *var, int value)
{
	pthread_mutex_lock(mutex);
	*var = value;
	pthread_mutex_unlock(mutex);
}

int	check_dead(t_phdata *phdata)
{
	pthread_mutex_lock(&phdata->stop_mutex);
	if (phdata->stop_sim)
	{
		pthread_mutex_unlock(&phdata->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phdata->stop_mutex);
	return (0);
}

int	check_philo_death(t_phdata *phdata, int i)
{
	pthread_mutex_lock(&phdata->state);
	if (get_time() - phdata->philo[i].last_meal > phdata->time_to_die)
	{

		lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
		pthread_mutex_lock(&phdata->print);
		printf("%ld %d %s\n", get_time() - phdata->start_time, i + 1, "died");
		pthread_mutex_unlock(&phdata->print);
		pthread_mutex_unlock(&phdata->state);
		return (1);
	}
	pthread_mutex_unlock(&phdata->state);
	return (0);
}

int	check_eat_limit(t_phdata *phdata)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (i < phdata->num_philo)
	{
		pthread_mutex_lock(&phdata->state);
		if (phdata->philo[i].meals_count >= phdata->eat_limit)
			count++;
		pthread_mutex_unlock(&phdata->state);
		i++;
	}
	if (count == phdata->num_philo)
		return (1);
	return (0);
}

void    *gs_mont(void *arg)
{
	t_phdata *phdata;
	int i;

	phdata = (t_phdata *)arg;
	while (!check_dead(phdata))
	{
		// i = 0;
		// long check = phdata-> time_to_die;
		// while(i < phdata->num_philo)
		// {
		// 	pthread_mutex_lock(&phdata->state);
		// 	long remains = phdata->philo[i].last_meal + phdata->time_to_die - get_time();
		// 	if (remains < check)
		// 		check = remains;
		// 	pthread_mutex_unlock(&phdata->state);
		// 	i++;
		// }
		// if (check > 0)
		// 	usleep(check * 1000/2);
		i = 0;
		while (i < phdata->num_philo)
		{
			if (check_philo_death(phdata, i))
				return (NULL);
			i++;
		}
		if (phdata->eat_limit != -1 && check_eat_limit(phdata))
		{
			lock_set_unlock(&phdata->stop_mutex, &phdata->stop_sim, 1);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
