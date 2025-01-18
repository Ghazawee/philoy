#include "philo.h"


// int	gs_sleep(int time, t_philo *philo) // maybe return 0/1 if philo is dead or not
// {
// 	long	start;

// 	unsigned long t;
// 	t = time;
// // 	// action = time;
// 	start = get_time();
// 	while (get_time() - start < t)
// 	{
// 		if (check_dead(philo->phdata))
// 			return (1);
// 		usleep(100);
// 		// usleep(time/10);
// 		//action -= 1;
// 	}
// 	return (0);
// }

int	opt_sleep(int time, t_philo *philo)
{
	struct timeval	begin;
	struct timeval	end;
	long			left;
	long			passed;
	long 			usec;

	usec = time * 1000;
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

int	opt_logs(t_phdata *phdata, int id, char *msg)
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
void opt_change_permission(t_philo *philo)
{
	// pthread_mutex_lock(&philo->phdata->waiter);
	// if (philo->permission == philo->phdata->num_philo)
	// 	philo->permission = philo->permission + 1 % philo->phdata->num_philo;
	// else
	// 	philo->permission = philo->permission + 1;
	// pthread_mutex_unlock(&philo->phdata->waiter);
	pthread_mutex_lock(&philo->phdata->waiter);
	if (philo->permission == philo->phdata->num_philo)
		philo->permission = 1;
		// philo->permission = philo->permission + 1 % philo->phdata->num_philo;
	else
		philo->permission += 1;
	pthread_mutex_unlock(&philo->phdata->waiter);
}

void	opt_unlock_fork(t_philo *philo) // remove waiter from the function, this will only handle forks
{
	int first;
	int second;

	opt_pick_order(philo, &first, &second);
	// pthread_mutex_lock(&philo->phdata->forks[first]);
	// pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[first] = 1;
	philo->phdata->forks_st[second] = 1;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
}

void	opt_philo_eat(t_philo *philo)
{
	if(check_dead(philo->phdata))
	{
		opt_unlock_fork(philo);
		return ;
	}
	if (opt_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		opt_unlock_fork(philo);
		return ;
	}
	if (opt_logs(philo->phdata, philo->id, "has taken a fork"))
	{
		opt_unlock_fork(philo);
		return ;
	}
	if (opt_logs(philo->phdata, philo->id, "is eating"))
	{
		opt_unlock_fork(philo);
		return ;
	}
	if	(opt_sleep(philo->phdata->time_to_eat, philo))
	{
		opt_unlock_fork(philo);
		return ;
	}
	// unlock_fork(philo);
	opt_put_down_forks(philo);
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->phdata->state);
	opt_change_permission(philo);
	// usleep(philo->phdata->time_to_eat / 5);
	// usleep(500);
}

void	*opt_handle_one_philo(t_philo *philo)
{
	if (opt_logs(philo->phdata, philo->id, "is thinking"))
		return (NULL);
	usleep(philo->phdata->time_to_die * 1000);
	return (NULL);
}


void	opt_pick_order(t_philo *philo, int *first, int *second)
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


int	opt_is_forks_pickable(t_philo *philo)
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
int opt_permission_to_eat(t_philo *philo)
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
int opt_can_pickup_forks(t_philo *philo)
{
	int first;
	int second;
	int f1_avail;
	int f2_avail;
	opt_pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	f1_avail = philo->phdata->forks_st[first];
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	f2_avail = philo->phdata->forks_st[second];
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	if (f1_avail && f2_avail)
		return (1);
	// if (philo->phdata->forks_st[first] == 1 && philo->phdata->forks_st[second] == 1)
	// {
	// 	pthread_mutex_unlock(&philo->phdata->forks[second]);
	// 	pthread_mutex_unlock(&philo->phdata->forks[first]);
	// 	return (1);
	// }
	// pthread_mutex_unlock(&philo->phdata->forks[second]);
	// pthread_mutex_unlock(&philo->phdata->forks[first]);
	return (0);
}

int opt_think_routine(t_philo *philo)
{
	if (opt_logs(philo->phdata, philo->id, "is thinking"))
		return (1);
	// if (gs_sleep(philo->phdata->time_to_eat / 5, philo))
	// 	return (1);
	
	return (0);
}

int opt_wait_permission(t_philo *philo)
{
	
	if (opt_sleep(philo->phdata->time_to_eat / 2, philo))
		return (1);
	while(!opt_can_pickup_forks(philo))
	{
		// if (gs_sleep(philo->phdata->time_to_eat / 2, philo))
		// 	return (1);
		usleep(100);
	}
	opt_change_permission(philo);
	return (0);
}


void	opt_take_forks(t_philo *philo)
{
	int first;
	int second;

	pick_order(philo, &first, &second);
	pthread_mutex_lock(&philo->phdata->forks[first]);
	philo->phdata->forks_st[first] = 0;
	// pthread_mutex_unlock(&philo->phdata->forks[first]);
	pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[second] = 0;
	// pthread_mutex_unlock(&philo->phdata->forks[second]);
}

void	opt_put_down_forks(t_philo *philo)
{
	int first;
	int second;

	pick_order(philo, &first, &second);
	// pthread_mutex_lock(&philo->phdata->forks[first]);
	philo->phdata->forks_st[first] = 1;
	// pthread_mutex_lock(&philo->phdata->forks[second]);
	philo->phdata->forks_st[second] = 1;
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	pthread_mutex_unlock(&philo->phdata->forks[second]);
}
int opt_sleep_rout(t_philo *philo)
{
	if (opt_logs(philo->phdata, philo->id, "is sleeping"))
		return (1);
	if (opt_sleep(philo->phdata->time_to_sleep, philo))
		return (1);
	return (0);
}

void    *opt_routi(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->phdata->num_philo == 1)
		return (opt_handle_one_philo(philo));
	while(!check_dead(philo->phdata))
	{
		if(opt_permission_to_eat(philo))
		{

			opt_take_forks(philo);
			opt_philo_eat(philo);
			// put_down_forks(philo);
			if (check_dead(philo->phdata))
				return (NULL);
			if (opt_sleep_rout(philo))
				return (NULL);
			opt_change_permission(philo);
			if (opt_think_routine(philo))
				return (NULL);
		}
		else
		{
			if (opt_wait_permission(philo))
				return (NULL);
		}
	}
	return (NULL);
}
