#include <string>
#include <ent/Entity.hxx>

namespace fileio{
	extern char* load_file(const char* path);
        extern Entity* load_Entity(char* raw);
};
