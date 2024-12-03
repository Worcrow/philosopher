//42 header

#include "philosopher_bonus.h"


int main(int argc, char *argv[])
{
  t_data  data;
  sem_t   *sem;

  if (argc != 5 && argc != 6)
  {
    send_error("Invalid arguments\n");
  }
  parse_input(argc, argv, &data);
  init_data(&data);
  start_simulation(&data);
  clean_data(&data);
  return (0);
}
