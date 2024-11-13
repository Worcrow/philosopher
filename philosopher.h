// 42 header
//
#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

typedef struct philo t_philo;

typedef enum opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	DETACH,
	JOIN,
}	t_opcode;

typedef enum time
{
	SECOND,
	MICROSECOND,
	MILLISECOND,
}	e_time;

typedef	enum write_e
{
	SLEEP,
	THINK,
	EAT,
	TAKEN_FIRST_FORK,
	TAKEN_SECOND_FORK,
} Wstatus;

typedef struct fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}	t_fork;

typedef struct data
{
	long			philo_nbr;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_die;
	long			nbr_meals;
	long			start_simulation;
	bool			end_simulation;
	bool			all_threads_ready;
	pthread_mutex_t	data_mutex;
	pthread_mutex_t	write_mutex;
	t_philo			*philo;
	t_fork			*fork;
}	t_data;

struct philo
{
	int			id;
	long		nb_eaten_meal;
	bool		is_full;
	long		time_last_meal;
	pthread_t	thread_id;
	t_fork		*first_fork;
	t_fork		*second_fork;
	t_data		*data_table;
};


void	StreamOut(char *message, int stream);
int		ft_atol(char *str);
void	parse_input(t_data *data, char *argv[]);
void	data_init(t_data *data);
void	*allocate_memory(size_t bytes);
void	mutex_handler(pthread_mutex_t *mutex, t_opcode opcode);
void	threads_handler(pthread_t *thread, void *(*routine)(void*), void *data, t_opcode opcode);
void	error_handler(int status, t_opcode opcode);
void	start_simulation(t_data *data);
void	free_data(t_data *data);
void	set_bool(pthread_mutex_t *mutex, bool *dest, bool val);
bool	get_bool(pthread_mutex_t *mutex, bool *value);
void	set_long(pthread_mutex_t *mutex, long *dest, long value);
long	get_long(pthread_mutex_t *mutex, long *value);
bool	is_simulation_finished(t_data *data);
void	wait_all_threads(t_philo *philo);
long	get_time(e_time time_code);
void	ft_usleep(long usec, t_data *philo);
void	write_status(Wstatus status, t_philo *philo);



#endif
