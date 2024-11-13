//42 header

#include "philosopher.h"
#include <pthread.h>

void	error_handler(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	else if (opcode == LOCK || opcode == UNLOCK || opcode == INIT || opcode == DESTROY)
		StreamOut("Error With Mutex\n", 2);
	else if (opcode == CREATE || opcode == JOIN || opcode == DETACH)
		StreamOut("Error with threads\n", 2);
	else
		StreamOut("Invalid opcode\n", 2);
}

void	threads_handler(pthread_t *thread, void *(*routine)(void*), void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		error_handler(pthread_create(thread, NULL, routine, data), CREATE);
	else if (opcode == JOIN)
		error_handler(pthread_join(*thread, NULL), JOIN);
	else if (opcode == DETACH)
		error_handler(pthread_detach(*thread), DETACH);
	else
		StreamOut("Invalid threads opcode\n", 2);
}


void	mutex_handler(pthread_mutex_t *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		error_handler(pthread_mutex_lock(mutex), LOCK);
	else if (opcode == UNLOCK)
		error_handler(pthread_mutex_unlock(mutex), UNLOCK);
	else if (opcode == INIT)
		error_handler(pthread_mutex_init(mutex, NULL), INIT);
	else if (opcode == DESTROY)
		error_handler(pthread_mutex_destroy(mutex), DESTROY);
	else
		StreamOut("Invalid mutex opcode\n", 2);
}

long	get_time(e_time time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		StreamOut("gettimeofday Failed!\n", 2);
	if (time_code == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		StreamOut("Wrong Input to gettimeofday\n", 2);
	return (0);
}

/**
* A pricise usleep
*/

void	ft_usleep(long usec, t_data *philo)
{
	long	start;
	long	passed_time;
	long	remained_time;

	start = get_time(MICROSECOND);
	while(get_time(MICROSECOND) - start < usec)
	{
		if (is_simulation_finished(philo))
			break;
		passed_time = get_time(MICROSECOND) - start;
		remained_time = passed_time - usec;
		//set a threeshold, 1e3 is an arbitary value
		if (remained_time > 1e3)
			usleep(remained_time / 2);
		else
			while (get_time(MICROSECOND) - start < usec)
				;
	}
}
