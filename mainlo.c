#include "philo.h"


void	join_threads(t_phdata *phdata)
{
	int	i;

	i = 0;
	while (i < phdata->num_philo)
	{
		if (pthread_join(phdata->philo[i].thrd, NULL) != 0)
		{
			cleanup_all(phdata);
			gs_error(3); // add new error msg for join failing
		}
		i++;
	}
	if (pthread_join(phdata->monit, NULL) != 0)
	{
		cleanup_all(phdata);
		gs_error(3);
	}
}

void	create_threads(t_philo *philo)
{
	if (pthread_create(&philo->thrd, NULL, gs_routi, philo) != 0)
	{
		cleanup_all(philo->phdata);
		gs_error(3);
	}
}

void	create_monit(t_phdata *phdata)
{
	if (pthread_create(&phdata->monit, NULL, gs_mont, phdata) != 0)
	{
		cleanup_all(phdata);
		gs_error(3);
	}
}

int	main(int ac, char **av)
{
	t_phdata	phdata;
	int			i;

	if(ac != 5 && ac != 6)
		gs_error(0);
	gs_init_phdata(av, &phdata);
	i = 0;
	while (i < phdata.num_philo)
	{
		create_threads(&phdata.philo[i]);
		i++;
	}
	create_monit(&phdata);
	join_threads(&phdata);
	cleanup_all(&phdata);
	return (0);
}
