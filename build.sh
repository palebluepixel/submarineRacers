# build all of the necessary dependencies of this project.
# requires cmake.


# glfw setup.

cd lib/glfw
rm -f CMakeCache.txt
cmake .
make



