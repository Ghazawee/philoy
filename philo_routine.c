#include "philo.h"

void	gs_sleep(int time, t_philo *philo) // maybe return 0/1 if philo is dead or not
{
	long	start;

	// action = time;
	start = get_time();
	while (get_time() - start < time)
	{
		if (check_dead(philo->phdata))
			return ;
		usleep(100);
		//action -= 1;
	}
}

int	gs_logs(t_phdata *phdata, int id, char *msg)
{
	
	pthread_mutex_lock(&phdata->print);
	if(check_dead(phdata))
	{
		pthread_mutex_unlock(&phdata->print);
		return (1);
	}
	printf("%ld Philosopher %d %s\n", get_time() - phdata->start_time, id, msg);
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
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	// if (philo->l_fork < philo->r_fork)
	// {
	// 	pthread_mutex_unlock(philo->l_fork);
	// 	pthread_mutex_unlock(philo->r_fork);
	// }
	// else
	// {
	// 	pthread_mutex_unlock(philo->r_fork);
	// 	pthread_mutex_unlock(philo->l_fork);
	// }

}

void	philo_eat(t_philo *philo)
{
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
	gs_sleep(philo->phdata->time_to_eat, philo); // maybe make gs_sleep return 0/1 if philo is dead or not, and in this case i need to unlock forks before returning
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
// int	is_forks_pickable(t_philo *philo)
// {
// 	while (!check_dead(philo->phdata))
// 	{
// 		if(pthread_mutex_lock(philo->l_fork) == 0)
// 		{
// 			if(pthread_mutex_lock(philo->r_fork) == 0)
// 			{
// 				return (1);
// 			}
// 		}
// 	}
// 	return (0);
// }

int	check_last_meal(t_philo *philo)
{
   // int	i;
    long current_time;
	long left;
	long right;
	//i = 0;
    current_time = get_time();
    pthread_mutex_lock(&philo->phdata->waiter);
	// left = current_time - philo->phdata->ps_last_meal[philo->id - 2 % philo->phdata->num_philo];
	// right = current_time - philo->phdata->ps_last_meal[philo->id % philo->phdata->num_philo];
	left = current_time - philo->phdata->philo[philo->id - 2 % philo->phdata->num_philo].last_meal;
	right = current_time - philo->phdata->philo[philo->id % philo->phdata->num_philo].last_meal;
	//abort();
	if ((current_time - left) > (current_time - philo->last_meal) || (current_time - right) > (current_time - philo->last_meal))
	{
		philo->phdata->wait = 1;
		pthread_mutex_unlock(&philo->phdata->waiter);
		return (0);
	}
	// while(i < philo->phdata->num_philo)
    // {
    //     if (i != philo->id - 1 && 
    //         (current_time - philo->phdata->philo[i].last_meal) > 
    //         (current_time - philo->last_meal))
    //     {
    //         pthread_mutex_unlock(&philo->phdata->state);
    //         usleep(500);
    //         return (0);
    //     }
	// 	i++;
    // }
	philo->phdata->wait = 0;
    pthread_mutex_unlock(&philo->phdata->state);
	return (1);
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
int check_starve(t_philo *philo)
{
	pthread_mutex_lock(&philo->phdata->waiter);
	if(philo->phdata->wait)
	{
		philo->phdata->wait = 0;
		pthread_mutex_unlock(&philo->phdata->waiter);
		return (0);
	}
	pthread_mutex_unlock(&philo->phdata->waiter);
	return (1);
}

int	is_forks_pickable(t_philo *philo)
{
	int first;
	int second;


	if(!check_starve(philo))
		return (0);
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
	// if(philo->phdata->num_philo % 2) // might cause data race
	// {
	// 	if (philo->id % 2 == 0 || philo->id == philo->phdata->num_philo) // same here
	// 	{
	// 		if (gs_logs(philo->phdata, philo->id, "is thinking"))
	// 				return (NULL);
	// 		usleep(1000);
	// 	}
	// }
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
			gs_sleep(philo->phdata->time_to_sleep, philo);
			//continue ;
			if (gs_logs(philo->phdata, philo->id, "is thinking"))
				return (NULL);
		}
		usleep(1000);
		// else
		// 	usleep(500);
			//gs_sleep(1, philo);
	}
	return (NULL);
}
