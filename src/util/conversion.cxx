#include "conversion.hxx"

quaternion convert::quatFromSTDVec(std::vector<float> v) {
    return glm::angleAxis(v.at(0), vec3(v.at(1), v.at(2), v.at(3)));
}

vec3 convert::vec3FromSTDVec(std::vector<float> v) {
    return vec3(v.at(0), v.at(1), v.at(2));
}
