
#include "philosopher.h"

void	wait_all_threads(t_philo *philo)
{
	while(!get_bool(&philo->data_table->data_mutex, &philo->data_table->all_threads_ready))
		;
}
