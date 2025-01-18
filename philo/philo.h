#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdint.h>
# include <sys/time.h>

typedef struct s_phdata	t_phdata;

typedef	struct	s_philo
{
	pthread_t		thrd;
	int				id;
	int		 		meals_count;
	unsigned long         	last_meal;
	int				permission;
	// pthread_mutex_t	*l_fork;
	// pthread_mutex_t	*r_fork;
	int				l_fork;
	int				r_fork;
	t_phdata		*phdata;
}t_philo;

typedef struct	s_phdata
{	
	int     		num_philo;
	unsigned long    		time_to_die;
	unsigned long     		time_to_eat;
	unsigned long     		time_to_sleep;
	int     		eat_limit;
	unsigned long			start_time;
	int				stop_sim;
	char			*forks_st;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t state;
	pthread_mutex_t waiter;
	pthread_mutex_t stop_mutex;
	pthread_t 		monit;
	t_philo 		*philo;
}t_phdata;

//------------------free_error------------------//
void    gs_error(int err);
void	cleanup_mutexes(t_phdata *phdata);
void	cleanup_all(t_phdata *phdata);
//------------------pharse---------------------//
void    gs_init_phdata(char **av, t_phdata *phdata);
unsigned long		get_time();
void	pick_order(t_philo *philo, int *first, int *second);
//---------------philo_routine-----------------//
void    *gs_routi(void *arg);
int		gs_logs(t_phdata *phdata, int id, char *msg);
int		check_dead(t_phdata *phdata);
void	put_down_forks(t_philo *philo);

void    *opt_routi(void *arg);
void	opt_pick_order(t_philo *philo, int *first, int *second);
void	opt_put_down_forks(t_philo *philo);
//---------------monitor-----------------------//
void    *gs_mont(void *arg);
//------------------utils---------------------//
int     is_digit(char c);
int     ft_atoi(const char *s);
//void	*ft_calloc(size_t count, size_t size);
#endif