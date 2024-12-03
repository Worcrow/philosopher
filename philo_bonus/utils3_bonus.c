// 42 header
//

#include "philosopher_bonus.h"

size_t  ft_strlen(char *str)
{
  size_t  i;

  if (!str)
    return (0);
  i = 0;
  while (str[i++])
    ;
  return (i);
}

char *ft_itoa(int n)
{
  size_t  len;
  int     temp;
  char    *res;

  temp = n;
  len = 0;
  while (temp > 0)
  {
    len++;
    temp /= 10;
  }
  res = allocate_memory(sizeof(char) * (len + 1));
  res[len] = '\0';
  while (n > 0)
  {
    res[--len] = n % 10 + '0';
    n /= 10;
  }
  return (res);
}

char *ft_strjoin(char *str1, char *str2)
{
  int i;
  int j;
  char *res;

  i = -1;
  j = 0;
  res = allocate_memory(sizeof(char) * (ft_strlen(str1) + ft_strlen(str2) + 1));
  while (str1[++i])
    res[j++] = str1[i];
  i = -1;
  while (str2[++i])
    res[j++] = str2[i];
  res[j] = '\0';
  return (res);
}

void  write_status(e_status status, t_philo *philo)
{
  sem_handler(&philo->data->write, SEM_WAIT, NULL, 0);
  if (status == GRAB_FORK && !is_simulation_finished(philo->data))
    printf("%ld %d has taken a fork\n", get_time(MILLISECOND) - philo->data->start_simulation, philo->id);
  else if (status == THINK && !is_simulation_finished(philo->data))
    printf("%ld %d is thinking\n", get_time(MILLISECOND) - philo->data->start_simulation, philo->id);
  else if (status == EAT && !is_simulation_finished(philo->data))
    printf("%ld %d is eating\n", get_time(MILLISECOND) - philo->data->start_simulation, philo->id);
  else if (status == SLEEP && !is_simulation_finished(philo->data))
    printf("%ld %d is sleeping\n", get_time(MILLISECOND) - philo->data->start_simulation, philo->id);
  else if (status == DIE)
    printf("%ld %d died\n", get_time(MILLISECOND) - philo->data->start_simulation, philo->id);
  else
    ;//printf("wrong status to write status\n");
  sem_handler(&philo->data->write, SEM_POST, NULL, 0);
}

void  wait_and_kill(t_data *data)
{
  int i;

  sem_handler(&data->dead, SEM_WAIT, NULL, 0);
  i = -1;
  while (++i < data->nb_philo)
    kill(data->philo[i].pid, SIGKILL);
}
