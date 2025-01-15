//#include "philo.h"

//old is fork pickable
// int	is_forks_pickable(t_philo *philo)
// {
// 	pthread_mutex_t *f_fork;
// 	pthread_mutex_t *s_fork;

// 	if(!check_last_meal(philo))
// 		return (0);
// 	if (philo->l_fork < philo->r_fork)
// 	{
// 		f_fork = philo->l_fork;
// 		s_fork = philo->r_fork;
// 	}
// 	else
// 	{
// 		f_fork = philo->r_fork;
// 		s_fork = philo->l_fork;
// 	}
// 	while (!check_dead(philo->phdata))
// 	{
// 		if(pthread_mutex_lock(f_fork) == 0)
// 		{
// 			if(pthread_mutex_lock(s_fork) == 0)
// 			{
// 				return (1);
// 			}
// 			pthread_mutex_unlock(f_fork);
// 		}
// 	}
// 	return (0);
// }

// old unlock fork
// void	unlock_fork(t_philo *philo) // remove waiter from the function, this will only handle forks
// {
// 	if (philo->l_fork < philo->r_fork)
// 	{
// 		pthread_mutex_unlock(philo->l_fork);
// 		pthread_mutex_unlock(philo->r_fork);
// 	}
// 	else
// 	{
// 		pthread_mutex_unlock(philo->r_fork);
// 		pthread_mutex_unlock(philo->l_fork);
// 	}

// }