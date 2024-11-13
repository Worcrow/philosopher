//42 header

#include "philosopher.h"
#include <stdbool.h>

void	parse_input(t_data *data, char *argv[])
{
	data->philo_nbr = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]) * 1e3;
	data->time_to_eat = ft_atol(argv[3]) * 1e3;
	data->time_to_sleep = ft_atol(argv[4]) * 1e3;
	data->nbr_meals = -1;
	//check for 60 ms
	if (argv[5])
		data->nbr_meals = ft_atol(argv[5]);
}

static void	assign_fork(t_philo *philo, t_fork *fork, int position)
{
	int	philo_number;

	philo_number = philo->data_table->philo_nbr;
	//to prevent deadlock
	philo->first_fork = &fork[(position + 1) % philo_number];
	philo->second_fork = &fork[position];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &fork[position];
		philo->second_fork = &fork[(position + 1) % philo_number];
	}
}

void	data_init(t_data *data)
{
	int	i;

	i = 0;
	data->end_simulation = false;
	data->all_threads_ready = false;
	mutex_handler(&data->data_mutex, INIT);
	data->philo = allocate_memory(data->philo_nbr * sizeof(t_philo));
	data->fork = allocate_memory(data->philo_nbr * sizeof(t_fork));
	while (i < data->philo_nbr)
	{
		mutex_handler(&data->fork[i].fork, INIT);
		data->fork[i].fork_id = i;
		data->philo[i].id = i + 1;
		data->philo[i].nb_eaten_meal = 0;
		data->philo[i].is_full = false;
		data->philo[i].data_table = data;
		assign_fork(data->philo, data->fork, i);
		++i;
	}
}
