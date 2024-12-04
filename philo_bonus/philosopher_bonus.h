
#ifndef PHILOSOPHER_BONUS_H
#define PHILOSOPHER_BONUS_H
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#define MEAL "/sem_meals10"                    // create sem for child, to prevent data racing between child process and the monitor
#define WRITE "/sem_writes10"                 // prevent ovelaping text between child process
#define DEAD "/sem_deads10"                  // kill all philos, once sem_wait return form this sem
#define FINISH "/sem_finishs10"             // prevent other child process from continuing execution if the simulation stoped
#define WAIT_PROC "/sem_wait_proccess"     //  wait for all process to be created

typedef struct data t_data;

typedef enum code
{
  SEM_OPEN,
  SEM_CLOSE,
  SEM_POST,
  SEM_WAIT,
  SEM_UNLINK,
} t_code;

typedef enum write_status
{
  GRAB_FORK,
  EAT,
  SLEEP,
  THINK,
  DIE,
} e_status;

typedef enum time
{
  MILLISECOND,
  MICROSECOND,
} e_time;

typedef struct philo
{
  int       id;
  pid_t     pid;
  int       nb_meal_eat;
  size_t    time_last_meal;
  bool      is_full;
  t_data    *data;
  sem_t     **left_fork;
  sem_t     **right_fork;
  char      *sem_meal_name;
  sem_t     *sem_meal;
  pthread_t monitor;
} t_philo;

typedef struct data
{
  int     nb_philo;
  size_t  time_to_eat;
  size_t  time_to_sleep;
  size_t  time_to_die;
  size_t  start_simulation;
  size_t  nb_meal;
  bool    end_simulation;
  bool    is_child;
  char    **sem_fork_name;
  sem_t   *meels;
  sem_t   **sem_fork;
  sem_t   *write;
  sem_t   *dead;
  sem_t   *stop;
  sem_t   *all_created;
  t_philo *philo;
} t_data;

void    send_error(char *message);
void    *allocate_memory(size_t bytes);
int	    ft_atol(char *str);
void    sem_handler(sem_t **sem, t_code opcode, const char *sem_name, int nb);
void    parse_input(int argc, char *argv[], t_data *data);
size_t	get_time(e_time time_code);
void    ft_usleep(long time, t_data *data);
char    *ft_strjoin(char *str1, char *str2);
char    *ft_itoa(int nb);
bool    is_simulation_finished(t_data *data);
void    init_data(t_data *data);
void    start_simulation(t_data *data);
void    kill_all_process(t_data *data);
void    write_status(e_status status, t_philo *philo);
void    init_local_sem(t_philo *philo);
void    assign_fork(t_philo *philo);
void    wait_and_kill(t_data *data);
void    init_philo(t_philo *philo);
void    *monitor(void *data);
bool    eaten_enough(t_philo *philo);
void    clean_data(t_data *data);
void    delay_time(t_data *data);

#endif
