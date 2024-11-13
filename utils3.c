//42 header

#include "philosopher.h"

void	write_status(Wstatus status, t_philo *philo)
{
	long	passed_time;

	passed_time = get_time(MILLISECOND) - philo->data_table->start_simulation;
	if (philo->is_full)
		return ;
	mutex_handler(&philo->data_table->write_mutex, LOCK);
	if ((status == TAKEN_FIRST_FORK || status == TAKEN_SECOND_FORK ) \
	 && !is_simulation_finished(philo->data_table))
		printf("[%ld] %d has taken a fork\n", passed_time, philo->id);
	else if (status == THINK && !is_simulation_finished(philo->data_table))
		printf("[%ld] %d is thinking\n", passed_time, philo->id);
	else if (status == SLEEP && !is_simulation_finished(philo->data_table))
		printf("[%ld] %d is sleeping\n", passed_time, philo->id);
	else if (status == EAT && !is_simulation_finished(philo->data_table))
		printf("[%ld] %d is eating\n", passed_time, philo->id);
	else
		StreamOut("Wrong status for write status\n", 2);
	mutex_handler(&philo->data_table->write_mutex, UNLOCK);
}
