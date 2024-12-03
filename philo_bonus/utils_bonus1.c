#include "philosopher_bonus.h"


void  send_error(char *message)
{
  printf("%s\n", message);
  exit(1);
}
void  *allocate_memory(size_t bytes)
{
  void  *ptr;

  ptr = malloc(bytes);
  if (!ptr)
    send_error("Malloc Failed To Allocate Memory\n");
  return (ptr);
}

static inline int is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static inline int is_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	ft_atol(char *str)
{
  long	number;

  number = 0;
	while(is_space(*str))
		++str;
	if (*str == '+')
		++str;
	else if (*str == '-')
		send_error("Error! Only positive number allowed\n");
	while (*str)
	{
		if (is_digit(*str))
			number = number * 10 + (*str - '0');
		else if (!is_digit(*str) || number > INT_MAX)
			send_error("number should have only digit and not greater than INT MAX\n");
		++str;
	}
	return (number);
}
