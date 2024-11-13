// 42 header 
//
#include "philosopher.h"


int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc != 5 && argc != 6)
		StreamOut("Invalid Argument\n", 2);
	parse_input(&data, argv);
	data_init(&data);
	start_simulation(&data);
	free_data(&data);
	return (0);
}
