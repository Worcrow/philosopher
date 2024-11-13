#include "philosopher.h"

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool val)
{
	mutex_handler(mutex, LOCK);
	*dest = val;
	mutex_handler(mutex, UNLOCK);
}

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	bool	val;

	mutex_handler(mutex, LOCK);
	val = *value;
	mutex_handler(mutex, UNLOCK);
	return (val);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	mutex_handler(mutex, LOCK);
	*dest = value;
	mutex_handler(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	val;

	mutex_handler(mutex, LOCK);
	val = *value;
	mutex_handler(mutex, UNLOCK);
	return (val);
}

bool	is_simulation_finished(t_data *data)
{
	return (get_bool(&data->data_mutex, &data->end_simulation));
}
