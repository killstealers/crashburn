#include <iostream>
#include <cstdlib>

#include <crashburn/crashburn.h>
#include <crashburn/loaders/md2_loader.h>

void 
usage (char *s)
{
  std::cerr << "Usage: %s <input_name> \n" << s << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
    	usage(argv[0]);
    }

  	const char *filename = argv[1];
    crashburn::MD2MeshLoader mesh(filename);

    return EXIT_SUCCESS;
}
