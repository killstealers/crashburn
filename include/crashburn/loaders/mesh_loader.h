#include <string>

#include "crashburn/mesh/mesh.h"

class MeshLoader 
{ 
	virtual bool load(Mesh* mesh, std::string filename); 
};