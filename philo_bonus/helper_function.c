


#include "philosopher_bonus.h"


void  *monitor(void *arg)
{
  t_philo *philo;
  t_data *data;
  long    time;

  philo = (t_philo *)arg;
  data = philo->data;
  while (!is_simulation_finished(data))
  {
    sem_handler(&philo->sem_meal, SEM_WAIT, NULL, 0);
    time = data->time_to_die - ((get_time(MILLISECOND) - philo->time_last_meal) * 1e3);
    sem_handler(&philo->sem_meal, SEM_POST, NULL, 0);
    if (time < 0)
    {
      sem_handler(&data->stop, SEM_WAIT, NULL, 0);
      data->end_simulation = true;
      //sem_handler(&data->stop, SEM_POST, NULL, 0);
      write_status(DIE, philo);
      sem_handler(&data->dead, SEM_POST, NULL, 0);
      sem_handler(&data->write, SEM_WAIT, NULL, 0);
    }
  }
  return (NULL);
}

bool eaten_enough(t_philo *philo)
{
  bool ret;

  sem_handler(&philo->sem_meal, SEM_WAIT, NULL, 0);
  ret = philo->data->nb_meal > 0 && philo->nb_meal_eat == philo->data->nb_meal;
  sem_handler(&philo->sem_meal, SEM_POST, NULL, 0);
  return (ret);
}


void  clean_data(t_data *data)
{
  int i;
  int j;

  i = -1;
  sem_handler(&data->write, SEM_CLOSE, NULL, 0);
  sem_handler(&data->dead, SEM_CLOSE, NULL, 0);
  sem_handler(&data->stop, SEM_CLOSE, NULL, 0);
  sem_handler(NULL, SEM_UNLINK, WRITE, 0);
  sem_handler(NULL, SEM_UNLINK, DEAD, 0);
  sem_handler(NULL, SEM_UNLINK, FINISH, 0);
  while (++i < data->nb_philo)
  {
    free(data->philo[i].sem_meal_name);
    sem_handler(data->sem_fork + i, SEM_CLOSE, NULL, 0);
    sem_handler(NULL, SEM_UNLINK, data->sem_fork_name[i], 0);
    free(data->sem_fork_name[i]);
    //free(data->sem_fork[i]);
  }
  free(data->philo);
  free(data->sem_fork);
  free(data->sem_fork_name);
}
