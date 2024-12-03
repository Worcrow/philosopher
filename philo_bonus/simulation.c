// 42 header
//

#include "philosopher_bonus.h"
#include <sys/types.h>
#include <sys/ptrace.h>

static void grab_forks(t_philo *philo)
{
  sem_handler(philo->left_fork, SEM_WAIT, NULL, 0);
  write_status(GRAB_FORK, philo);
  sem_handler(philo->right_fork, SEM_WAIT, NULL, 0);
  write_status(GRAB_FORK, philo);
}

static void puts_forks(t_philo *philo)
{
  sem_handler(philo->left_fork, SEM_POST, NULL, 0);
  sem_handler(philo->right_fork, SEM_POST, NULL, 0);
}

static void eat(t_philo *philo)
{
  grab_forks(philo);
  write_status(EAT, philo);
  sem_handler(&philo->sem_meal, SEM_WAIT, NULL, 0);
  philo->time_last_meal = get_time(MILLISECOND);
  sem_handler(&philo->sem_meal, SEM_POST, NULL, 0);

  ft_usleep(philo->data->time_to_eat, philo->data);

  sem_handler(&philo->sem_meal, SEM_WAIT, NULL, 0);
  philo->nb_meal_eat++;
  sem_handler(&philo->sem_meal, SEM_POST, NULL, 0);

  puts_forks(philo);
}

static void think(t_philo *philo, bool silent)
{
  time_t	time_to_think;

	sem_wait(philo->sem_meal);
	time_to_think = (philo->data->time_to_die
			- (get_time(MILLISECOND) - philo->time_last_meal)
			- philo->data->time_to_eat - 1) / 2;
	sem_post(philo->sem_meal);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(THINK, philo);
	ft_usleep(time_to_think, philo->data);
}


void  start_dinner(t_philo *philo)
{
  init_philo(philo);
  //if (philo->id % 2 == 0)
  while (!is_simulation_finished(philo->data))
  {
    if (eaten_enough(philo))
      break;
    //think(philo, true);
    eat(philo);
    write_status(SLEEP, philo);
    ft_usleep(philo->data->time_to_sleep, philo->data);
    think(philo, false);
  }
  pthread_join(philo->monitor, NULL);
  sem_handler(&philo->sem_meal, SEM_CLOSE, NULL, 0);
}

void  start_simulation(t_data *data)
{
  pid_t pid;
  int   i;

  i = -1;
  data->start_simulation = get_time(MILLISECOND);
  while (++i < data->nb_philo)
  {
    pid = fork();
    if (pid < 0)
      send_error("fork failed\n");
    else if (pid == 0)
    {
      start_dinner(data->philo + i);
      exit(0);
    }
    else
      data->philo[i].pid = pid;
  }
  wait_and_kill(data);
}
