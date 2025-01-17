#include "philo.h"


int log_think(t_phdata *phdata, t_philo *philo, int id, char *msg)
{
	pthread_mutex_lock(&phdata->state);
	philo->is_thinking = 1;
	pthread_mutex_unlock(&phdata->state);
	pthread_mutex_lock(&phdata->stop_mutex);
	if (phdata->stop_sim)
	{
		pthread_mutex_unlock(&phdata->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phdata->stop_mutex);
	pthread_mutex_lock(&phdata->print);
	if(check_dead(phdata))
	{
		pthread_mutex_unlock(&phdata->print);
		return (1);
	}
	printf("%ld %d %s\n", get_time() - phdata->start_time, id, msg);
	pthread_mutex_unlock(&phdata->print);
	// philo->is_thinking = 1;
	return (0);
}
// int	gs_sleep(int time, t_philo *philo) // maybe return 0/1 if philo is dead or not
// {
// 	long	start;

// 	// action = time;
// 	start = get_time();
// 	while (get_time() - start < time)
// 	{
// 		if (check_dead(philo->phdata))
// 			return (1);
// 		//usleep(100);
// 		usleep(time/10);
// 		//action -= 1;
// 	}
// 	return (0);
// }

int	gs_sleep(int time, t_philo *philo) // maybe return 0/1 if philo is dead or not
{
	struct timeval	begin;
	struct timeval	end;
	long			left;
	long			passed;
	long 			usec;

	usec = time * 1000L;
	passed = 0;
	gettimeofday(&begin, NULL);
	while (passed < usec)
	{
		if (check_dead(philo->phdata))
			return (1);
		gettimeofday(&end, NULL);
		passed = (end.tv_sec - begin.tv_sec) * 1000000L + (end.tv_usec - begin.tv_usec);
		left = usec - passed;
		if (left > 1000)
			usleep(250);
	}
	return (0);
}

int	gs_logs(t_phdata *phdata, int id, char *msg)
{
	pthread_mutex_lock(&phdata->print);
	if(check_dead(phdata))
	{
		pthread_mutex_unlock(&phdata->print);
		return (1);
	}
	printf("%ld %d %s\n", get_time() - phdata->start_time, id, msg);
	pthread_mutex_unlock(&phdata->print);
	return (0);
}
void change_permission(t_philo *philo)
{
	pthread_mutex_lock(&philo->phdata->waiter);
	if (philo->permission == philo->phdata->num_philo)
		philo->permission = philo->permission + 1 % philo->phdata->num_philo;
	else
		philo->permission = philo->permission + 1;
	pthread_mutex_unlock(&philo->phdata->waiter);
}

void	unlock_fork(t_philo *philo) // remove waiter from the function, this will only handle forks
{
	int first;
	int second;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[first] = 1;
	philo->phdata->forks_st[second] = 1;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
}

void	philo_eat(t_philo *philo)
{
	if(check_dead(philo->phdata))
		return ;
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		unlock_fork(philo);
		return ;
	}
	if (gs_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		unlock_fork(philo);
		return ;
	}
	if (gs_logs(philo->phdata, philo->id, "is eating"))
	{
		unlock_fork(philo);
		return ;
	}
	if	(gs_sleep(philo->phdata->time_to_eat, philo))
	{
		unlock_fork(philo);
		return ;
	}
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	philo->ate = 1;
	pthread_mutex_unlock(&philo->phdata->state);
	unlock_fork(philo);
	change_permission(philo);
	// usleep(philo->phdata->time_to_eat / 5);
	// usleep(500);
}

void	*handle_one_philo(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is thinking"))
		return (NULL);
	usleep(philo->phdata->time_to_die * 1000);
	return (NULL);
}


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


int	is_forks_pickable(t_philo *philo)
{
	int first;
	int second;
	
	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	if (philo->phdata->forks_st[first] == 0)
	{
		pthread_mutex_unlock(&philo->phdata->forks[first]);
		return (0);
	}
	pthread_mutex_lock(&philo->phdata->forks[second]);
	if (philo->phdata->forks_st[second] == 0)
	{
		pthread_mutex_unlock(&philo->phdata->forks[second]);
		pthread_mutex_unlock(&philo->phdata->forks[first]);
		return (0);
	}
	philo->phdata->forks_st[first] = 0;
	philo->phdata->forks_st[second] = 0;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	return (1);
}
int permission_to_eat(t_philo *philo)
{
	int permission;

	permission = 0;
	if (philo->permission % 2 == 0)
	{
		permission = 1;
		return (permission);
	}
	return (permission);
}
int can_pickup_forks(t_philo *philo)
{
	int first;
	int second;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	if (philo->phdata->forks_st[first] == 1 && philo->phdata->forks_st[second] == 1)
	{
		pthread_mutex_unlock(&philo->phdata->forks[second]);
		pthread_mutex_unlock(&philo->phdata->forks[first]);
		return (1);
	}
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	return (0);
}

int think_wait_permission(t_philo *philo)
{
	// if (philo->is_thinking == 1)
	// {
		if (gs_logs(philo->phdata, philo->id, "is thinking"))
			return (1);
		if (gs_sleep(philo->phdata->time_to_eat / 2, philo))
			return (1);
	// 	philo->is_thinking = 0;
	// }
	while(!can_pickup_forks(philo))
	{
		// if (check_dead(philo->phdata))
		// 	return (1);
		usleep(100);
	}
	change_permission(philo);
	return (0);
}


void	take_forks(t_philo *philo)
{
	int first;
	int second;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[first] = 0;
	philo->phdata->forks_st[second] = 0;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
}

void	put_down_forks(t_philo *philo)
{
	int first;
	int second;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[first] = 1;
	philo->phdata->forks_st[second] = 1;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
}

void    *gs_routi(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while(!check_dead(philo->phdata))
	{
		if (philo->phdata->num_philo == 1)
			return (handle_one_philo(philo));
		//pthread_mutex_lock(&philo->phdata->state);
		// if(philo->ate == 1 && philo->is_thinking != 1)
		// {
		// 	if (log_think(philo->phdata, philo, philo->id, "is thinking"))
		// 	{
		// 		//pthread_mutex_unlock(&philo->phdata->state);
		// 		return (NULL);
		// 	}
		// }
		//pthread_mutex_unlock(&philo->phdata->state);
		if(permission_to_eat(philo))
		{
			// if (is_forks_pickable(philo))
			// {
				take_forks(philo);
				philo_eat(philo);
				put_down_forks(philo);
				if (check_dead(philo->phdata))
					return (NULL);
				if (gs_logs(philo->phdata, philo->id, "is sleeping"))
					return (NULL);
				if (gs_sleep(philo->phdata->time_to_sleep, philo))
					return (NULL);
				change_permission(philo);
				// pthread_mutex_lock(&philo->phdata->state);
				// philo->is_thinking = 1;
				// pthread_mutex_unlock(&philo->phdata->state);
				if(check_dead(philo->phdata))
					return (NULL);
			// if (gs_logs(philo->phdata, philo->id, "is thinking"))
			// 	return (NULL);
			// usleep(philo->phdata->time_to_eat * 4.17);
			// if (philo->phdata->num_philo % 2)
				//usleep(250);
			// gs_sleep(250 * 1000, philo);
			// }
		}
		else
		{
			if (think_wait_permission(philo))
				return (NULL);
			// gs_sleep(250 * 1000, philo);
			//usleep(100);
			//usleep(philo->phdata->time_to_eat / 5);
			//usleep(500);
		}
	}
	return (NULL);
}
