
#include "headers/clusterize.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error : mismatch arguments\n";
		return (0);
	}
	std::string clusterized_data = "clusterized_data.csv";
	clusterize(argv[1], clusterized_data);
	return (0);
}