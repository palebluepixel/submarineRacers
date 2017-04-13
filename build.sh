# build all of the necessary dependencies of this project.
# requires cmake.


# glfw setup.

cd lib/glfw
rm -f CMakeCache.txt
cmake .
make

cd ../libpng-1.6.29
rm -f CMakeCache.txt
cmake .
make

