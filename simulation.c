#include "philosopher.h"


void	*dinner_simulation(void	*data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo);
	
	while (!is_simulation_finished(philo->data_table))
	{
		if (philo->is_full)
			break;
		
		eat(philo);
		
		write_status(SLEEP, philo);
		ft_usleep(philo->data_table->time_to_sleep, philo->data_table);

		think(philo);
	}

	return (NULL);
}

void	start_simulation(t_data *data)
{
	int	i;

	i = -1;
	if (data->nbr_meals == 0)
		return ;
	else if (data->philo_nbr == 1)
		;//todo
	else
	{
		while (++i < data->philo_nbr)
			threads_handler(&data->philo[i].thread_id, dinner_simulation, &data->philo[i], CREATE);
	}
	//start of the simulation
	data->start_simulation = get_time(MILLISECOND);
	set_bool(&data->data_mutex, &data->all_threads_ready, true);
	i = -1;
	while(++i < data->philo_nbr)
		threads_handler(&data->philo[i].thread_id, NULL, NULL, JOIN);
}
