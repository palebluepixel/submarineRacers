#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

using namespace glm;
typedef glm::tquat<float> quaternion;

namespace convert{
    extern quaternion quatFromSTDVec(std::vector<float> v);
    extern vec3 vec3FromSTDVec(std::vector<float> v);
}
