#include <iostream>
#include <cstdlib>

#include <crashburn/crashburn.h>
#include <crashburn/loaders/md2_loader.h>

void usage(const char* s)
{
  std::cerr << "Usage: %s <input_name>\n" << s << std::endl;

  exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    if (argc!=2)
    	usage(argv[0]);

    crashburn::MD2MeshLoader mesh(argv[1]);

    return EXIT_SUCCESS;
}
