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
		//usleep(100);
		usleep(time/10);
		//action -= 1;
	}
}

int log_think(t_phdata *phdata, t_philo *philo, int id, char *msg)
{
	philo->is_thinking = 1;
	pthread_mutex_lock(&phdata->print);
	if(check_dead(phdata))
	{
		pthread_mutex_unlock(&phdata->print);
		return (1);
	}
	printf("%ld Philosopher %d %s\n", get_time() - phdata->start_time, id, msg);
	pthread_mutex_unlock(&phdata->print);
	// philo->is_thinking = 1;
	return (0);
}

int	gs_logs(t_phdata *phdata, int id, char *msg)
{
	if(check_dead(phdata))
		return (1);
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

	// philo->phdata->forks_st[first] = 1;
	// philo->phdata->forks_st[second] = 1;
	// next = (philo->id % philo->phdata->num_philo) + 1;
	// if (first == philo->l_fork)
	// {
	// 	philo->phdata->f_owner[first] = ((philo->id)% philo->phdata->num_philo);
	// 	philo->phdata->f_owner[second] = ((philo->id -1) % philo->phdata->num_philo);
	// }
	// else
	// {
	// 	philo->phdata->f_owner[first] = ((philo->id - 1) % philo->phdata->num_philo);
	// 	philo->phdata->f_owner[second] = ((philo->id) % philo->phdata->num_philo);
	// }
	// pthread_mutex_lock(&philo->phdata->forks[first]);
	// pthread_mutex_lock(&philo->phdata->forks[second]);
	// philo->phdata->f_owner[first] = next -1;
	// philo->phdata->f_owner[second] = next - 1;
	philo->phdata->forks_st[first] = 1;
	philo->phdata->forks_st[second] = 1;
	pthread_mutex_unlock(&philo->phdata->forks[second]);
	pthread_mutex_unlock(&philo->phdata->forks[first]);
	// if (philo->phdata->num_philo % 2 && philo->id % 2)
	// {
	// 	// if (philo->id %  == 0)
	// 	usleep(philo->phdata->time_to_eat / 5);
	// 		// usleep(500);
	// }
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
	gs_sleep(philo->phdata->time_to_eat, philo); // maybe make gs_sleep return 0/1 if philo is dead or not, and in this case i need to unlock forks before returning
	pthread_mutex_lock(&philo->phdata->state);
	philo->last_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->phdata->state);
	unlock_fork(philo);
	// if (philo->phdata->num_philo % 2 && philo->id % 2)
	// {
		// if (philo->id %  == 0)
		// usleep(philo->phdata->time_to_eat / 5);
			// usleep(500);
	// }
}

void	*handle_one_philo(t_philo *philo)
{
	if (gs_logs(philo->phdata, philo->id, "is thinking"))
		return (NULL);
	usleep(philo->phdata->time_to_die * 1000);
	return (NULL);
}


int	check_last_meal(t_philo *philo)
{
	long current_time;
	long left_last_meal;
	long right_last_meal;

	current_time = get_time();

	pthread_mutex_lock(&philo->phdata->state);
	left_last_meal = philo->phdata->philo[(philo->id - 2 + philo->phdata->num_philo) % philo->phdata->num_philo].last_meal;
	right_last_meal = philo->phdata->philo[philo->id % philo->phdata->num_philo].last_meal;

	if ((current_time - left_last_meal) > (current_time - philo->last_meal) ||
		(current_time - right_last_meal) > (current_time - philo->last_meal))
	{
		pthread_mutex_unlock(&philo->phdata->state);
		return (0);
	}
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


int	is_forks_pickable(t_philo *philo)
{
	int first;
	int second;
	// long current_time;
	// long hunger_level;

	// current_time = get_time();
	// hunger_level = current_time - philo->last_meal;
	
	// if(philo->phdata->num_philo % 2 == 0 && philo->meals_count > 0 && hunger_level < philo->phdata->time_to_eat * (1 + (philo->id % 3) * 0.2))
	// 	return (0);
	// if (philo->phdata->num_philo % 2 && philo->meals_count > 0 && hunger_level < philo->phdata->time_to_eat * 2)
	//  	return (0);
	// if (philo->phdata->num_philo % 2 && philo->meals_count > 0 && hunger_level < philo->phdata->time_to_eat * (1 + (philo->id % 3) * 0.2))	
	
	pick_order(philo, &first, &second);
	// pthread_mutex_lock(&philo->phdata->forks[first]);
	// if (philo->phdata->forks_st[first] == 0 
	// 	&& (philo->phdata->f_owner[first] != philo->id - 1))
	// {
	// 	pthread_mutex_unlock(&philo->phdata->forks[first]);
	// 	return (0);
	// }
	// pthread_mutex_lock(&philo->phdata->forks[second]);
	// if (philo->phdata->forks_st[second] == 0 
	// 	&& (philo->phdata->f_owner[second] != philo->id - 1))
	// {
	// 	pthread_mutex_unlock(&philo->phdata->forks[second]);
	// 	pthread_mutex_unlock(&philo->phdata->forks[first]);
	// 	return (0);
	// }
	// philo->phdata->forks_st[second] = 0;
	// philo->phdata->f_owner[second] = ((philo->id) % philo->phdata->num_philo);
	// printf("forks_st[second] = %d f_owner = %d\n", philo->phdata->forks_st[second], philo->phdata->f_owner[second]);
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
	// philo->phdata->f_owner[first] = philo->id -1;
	// philo->phdata->f_owner[second] = philo->id - 1;
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
		// if (philo->id % 2 == 0 || philo->id == philo->phdata->num_philo) // same here
		// {
		// 	// if (gs_logs(philo->phdata, philo->id, "is thinking"))
		// 	// 		return (NULL);
		// 	usleep(200);
		// }
	// }
	if (philo->id % 2 == 0)
		usleep(500);
		// usleep(philo->phdata->time_to_eat * 8.33);
		// usleep(philo->phdata->time_to_eat * 1000 / 2);
	// else
	// 	usleep(philo->phdata->time_to_eat * 500);
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
			gs_sleep(philo->phdata->time_to_sleep, philo); // maybe turn 
			//continue ;
			if(check_dead(philo->phdata))
				return (NULL);
			if (gs_logs(philo->phdata, philo->id, "is thinking"))
				return (NULL);
			// usleep(philo->phdata->time_to_eat * 4.17);
			usleep(250);
		}
		else
		{
			usleep(philo->phdata->time_to_eat / 5);
			//usleep(500);
		}
	}
	return (NULL);
}
