#include "philo.h"

int	gs_sleep(int time, t_philo *philo) // maybe return 0/1 if philo is dead or not
{
	long	start;

	// action = time;
	start = get_time();
	while (get_time() - start < time)
	{
		if (check_dead(philo->phdata))
			return (1);
		//usleep(100);
		usleep(time/10);
		//action -= 1;
	}
	return (0);
}

int	gs_logs(t_phdata *phdata, int id, char *msg)
{
	// if(check_dead(phdata))
	// 	return (1);
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
	if (gs_sleep(philo->phdata->time_to_eat, philo))
	{
		unlock_fork(philo);
		return ;
	}
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->phdata->state);
	unlock_fork(philo);
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
	// long current_time;
	// long hunger_level;

	// current_time = get_time();
	// hunger_level = current_time - philo->last_meal;
	

	// if (philo->phdata->num_philo % 2 && philo->meals_count > 0 && hunger_level < philo->phdata->time_to_eat * 2)
	//  	return (0);
	
	
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


void    *gs_routi(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	
	// if (philo->phdata->num_philo % 2 && philo->id % 2 == 0)
	// 	usleep(500);
		// usleep(philo->phdata->time_to_eat * 8.33);
	while(!check_dead(philo->phdata))
	{
		if (philo->phdata->num_philo == 1)
			return (handle_one_philo(philo)); 
		if (is_forks_pickable(philo))
		{
			philo_eat(philo); // need to check if forks avail before hand and only then i eat and sleep, else they think
			if (check_dead(philo->phdata))
				return (NULL);
			if (gs_logs(philo->phdata, philo->id, "is sleeping"))
				return (NULL);
			if (gs_sleep(philo->phdata->time_to_sleep, philo))
				return (NULL);
			//continue ;
			if(check_dead(philo->phdata))
				return (NULL);
			if (gs_logs(philo->phdata, philo->id, "is thinking"))
				return (NULL);
			usleep(200);
			// if (gs_sleep(philo->phdata->time_to_eat / 2, philo))
			// 	return (NULL);
			// usleep(philo->phdata->time_to_eat * 4.17);
			// usleep(250);
		}
		else
		{
			// usleep(philo->phdata->time_to_eat / 5);
			usleep(100);
		}
	}
	return (NULL);
}
