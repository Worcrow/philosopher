//42 header

#include "philosopher.h"

void	StreamOut(char *message, int stream)
{
	int	i;

	i = -1;
	while (message[++i])
		write(stream, message + i, 1);
	if (stream == 2)
		exit(EXIT_FAILURE);
}

static inline int is_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static inline int is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atol(char *str)
{
	long	number;

	while(is_space(*str))
		++str;
	if (*str == '+')
		++str;
	else if (*str == '-')
		StreamOut("Error! Only positive number allowed\n", 2);
	while (*str)
	{
		if (is_digit(*str))
			number = number * 10 + (*str - '0');
		else if (!is_digit(*str) || number > INT_MAX)
			StreamOut("number should have only digit and not greater than INT MAX\n", 2);
		++str;
	}
	return (number);
}

void	*allocate_memory(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		StreamOut("Can't Allocate Memory\n", 2);
	return (ret);
}
