/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshaheen <mshaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:42:32 by mshaheen          #+#    #+#             */
/*   Updated: 2025/01/19 23:15:26 by mshaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_phdata	t_phdata;

typedef struct s_philo
{
	pthread_t			thrd;
	int					id;
	int					meals_count;
	unsigned long		last_meal;
	int					permission;
	int					l_fork;
	int					r_fork;
	t_phdata			*phdata;
}						t_philo;

typedef struct s_phdata
{
	int					num_philo;
	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	int					eat_limit;
	unsigned long		start_time;
	int					stop_sim;
	// char				*forks_st;
	int					*forks_st;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print;
	pthread_mutex_t		state;
	pthread_mutex_t		waiter;
	pthread_mutex_t		stop_mutex;
	pthread_t			monit;
	t_philo				*philo;
}						t_phdata;
extern int				g_dead;
//------------------free_error------------------//
void					gs_error(int err);
void					cleanup_mutexes(t_phdata *phdata);
void					cleanup_all(t_phdata *phdata);
//------------------pharse---------------------//
void					gs_init_phdata(char **av, t_phdata *phdata);
void					pick_order(t_philo *philo, int *first, int *second);
//---------------philo_routine-----------------//
void					*gs_routi(void *arg);
void					change_permission(t_philo *philo);
int						permission_to_eat(t_philo *philo);
void					*handle_one_philo(t_philo *philo);
int						gs_logs(t_phdata *phdata, int id, char *msg);
//---------------actions-----------------//
int						think_routine(t_philo *philo);
int						sleep_rout(t_philo *philo);
int						philo_eat(t_philo *philo);
int						gs_sleep(int time, t_philo *philo);
//---------------forking-----------------------//
int						can_pickup_forks(t_philo *philo);
void					unlock_fork(t_philo *philo);
void					take_forks(t_philo *philo);
void					put_down_forks(t_philo *philo);
//---------------opt_rout-----------------------//
void					*opt_routi(void *arg);
//---------------monitor-----------------------//
void					*gs_mont(void *arg);
int						check_dead(t_phdata *phdata);
//------------------utils---------------------//
int						is_digit(char c);
int						ft_atoi(const char *s);
void					init_forks_state(t_phdata *phdata);
unsigned long			get_time(void);
int						parse_args(char **av);
#endif