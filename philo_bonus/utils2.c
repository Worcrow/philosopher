

//42 header


#include "philosopher_bonus.h"

static void  sem_error_handler(int status, t_code opcode)
{
  if (opcode == SEM_OPEN && errno == EACCES)
    send_error("The required permissions (for reading and/or writing) are denied for the given flags;\
or O_CREAT is specified, the object does not exist, and permission to create the semaphore is denied.\n");
  else if (opcode == SEM_OPEN && errno == EEXIST)
      send_error(" O_CREAT and O_EXCL were specified and the semaphore exists.\n");
  else if (opcode == SEM_OPEN && errno == ENFILE)
    send_error("Too many semaphores or file descriptors are open on the system.\n");
  else if (opcode == SEM_OPEN && errno == ENOENT)
    send_error("O_CREAT is not set and the named semaphore does not exist.\n");
  else if (opcode == SEM_OPEN && errno == EMFILE)
    send_error("The process has already reached its limit for semaphores\
              or file descriptors in use.\n");
  else if (opcode == SEM_OPEN && errno == ENOSPC)
    send_error("O_CREAT is specified, the file does not exist, and there is \
                insufficient space available to create the semaphore.\n");
  else if (status == EDEADLK && opcode == SEM_WAIT)
    send_error("A deadlock was detected\n");
  else if (status == EINTR && opcode == SEM_WAIT)
    send_error("The call was interrupted by a signal\n");
  else if (status == EINVAL && (opcode == SEM_WAIT || \
      opcode == SEM_POST || opcode == SEM_CLOSE))
    send_error("sem is not a valid semaphore descriptor\n");
  else if (status == EACCES && opcode == SEM_UNLINK)
    send_error("Permission is denied to be remove the semaphore.\n");
  else if (status == ENOENT && opcode == SEM_UNLINK)
    send_error("The named semaphore does not exist.\n");
}



void  sem_handler(sem_t **sem, t_code opcode, const char *sem_name, int nb)
{
  if (opcode == SEM_OPEN)
  {
    *sem = sem_open(sem_name, O_CREAT, 0666, nb);
    sem_error_handler(0, SEM_OPEN);
  }
  else if (opcode == SEM_CLOSE)
    sem_error_handler(sem_close(*sem), SEM_CLOSE);
  else if (opcode == SEM_POST)
    sem_error_handler(sem_post(*sem), SEM_POST);
  else if (opcode == SEM_WAIT)
    sem_error_handler(sem_wait(*sem), SEM_WAIT);
  else if (opcode == SEM_UNLINK)
    sem_error_handler(sem_unlink(sem_name), SEM_UNLINK);
  else
    send_error("Invalid argument to sem_handler\n");
}

size_t	get_time(e_time time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		send_error("gettimeofday Failed!\n");
	if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		send_error("Wrong Input to gettimeofday\n");
	return (0);
}

// void  ft_usleep(long time, t_data *data)
// {
//   size_t  start;
//   size_t  passed_time;
//   size_t  remained_time;

//   start = get_time(MICROSECOND);
//   while (get_time(MICROSECOND) - start < time)
//   {
//     if (is_simulation_finished(data))
//       break ;
//     passed_time = get_time(MICROSECOND) - start;
//     remained_time = time - passed_time;
//     if (remained_time > 100)
//       usleep(remained_time / 2);
//     else
//       while(get_time(MICROSECOND) - start < time)
//         ;
//   }
// }

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

bool  is_simulation_finished(t_data *data)
{
  bool  ret;

  sem_handler(&data->stop, SEM_WAIT, NULL, 0);
  ret = data->end_simulation;
  sem_handler(&data->stop, SEM_POST, NULL, 0);
  return (ret);
}
