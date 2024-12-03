//42 header
//

#include "philosopher_bonus.h"

void  parse_input(int argc, char *argv[], t_data *data)
{
  data->nb_philo = ft_atol(argv[1]);
  data->time_to_die = ft_atol(argv[2]) * 1e3;
  data->time_to_eat = ft_atol(argv[3]) * 1e3;
  data->time_to_sleep = ft_atol(argv[4]) * 1e3;
  data->nb_meal = -1;
  if (argc == 6)
  {
    data->nb_meal = ft_atol(argv[5]);
    if (data->nb_meal < 0)
      send_error("Invalid number of meals\n");
  }
}

void  create_sem_name(char **location, char *sem_name, int nb)
{
    char *tmp;

    tmp = ft_itoa(nb);
    *location = ft_strjoin(sem_name, tmp);
    free(tmp);
}

void  assign_fork(t_philo *philo)
{
  t_data *data;

  data = philo->data;
  philo->left_fork = data->sem_fork + (philo->id % data->nb_philo);
  philo->right_fork = data->sem_fork + (philo->id - 1);
  if (philo->id % 2 == 0)
  {
    philo->left_fork = data->sem_fork + (philo->id - 1);
    philo->right_fork = data->sem_fork + (philo->id % data->nb_philo);
  }
}

void  init_data(t_data *data)
{
  int i;

  i = -1;
  data->sem_fork_name = allocate_memory(sizeof(char *) * data->nb_philo);
  data->sem_fork = allocate_memory(sizeof(sem_t *) * data->nb_philo);
  data->philo = allocate_memory(sizeof(t_philo) * data->nb_philo);
  data->end_simulation = false;
  //sem_handler(NULL, SEM_UNLINK, WRITE, 0);
  //sem_handler(NULL, SEM_UNLINK, DEAD, 0);
 // sem_handler(NULL, SEM_UNLINK, FINISH, 0);
  
  sem_handler(&data->write, SEM_OPEN, WRITE, 1);
  sem_handler(&data->dead, SEM_OPEN, DEAD, 0);
  sem_handler(&data->stop, SEM_OPEN, FINISH, 1);
  while (++i < data->nb_philo)
  {
    data->philo[i].id = i + 1;
    data->philo[i].nb_meal_eat = 0;
    data->philo[i].is_full = false;
    data->philo[i].data = data;
    create_sem_name(&data->philo[i].sem_meal_name, MEAL, i + 1);
    create_sem_name(&data->sem_fork_name[i], "/sem_fork", i + 1);
    //sem_handler(NULL, SEM_UNLINK, data->sem_fork_name[i], 0);
    sem_handler(data->sem_fork + i, SEM_OPEN, data->sem_fork_name[i], 1);
    assign_fork(data->philo + i);
  }
}

void  init_philo(t_philo *philo)
{
  int i;
  t_data *data;

  i = -1;
  data = philo->data;
  //open global semaphores for interprocess communication
  sem_handler(&data->write, SEM_OPEN, WRITE, 1);
  sem_handler(&data->dead, SEM_OPEN, DEAD, 0); // 0, cause no philo is died
  sem_handler(&data->stop, SEM_OPEN, FINISH, 1);
  // sem_meal, prevent data racing between child process and it's monitor 
  sem_handler(&philo->sem_meal, SEM_OPEN, philo->sem_meal_name, 1);
  sem_handler(NULL, SEM_UNLINK, philo->sem_meal_name, 0);
  philo->time_last_meal = get_time(MILLISECOND);
  // grab from right to left fork,  if philo id is even grab from left to right
  // this algo prevent deadlock that can happen between each philo process
  if (pthread_create(&philo->monitor, NULL, monitor, philo) != 0)
    send_error("Failed to create monitor thread\n");
}
