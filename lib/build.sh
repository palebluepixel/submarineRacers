# build all of the necessary dependencies of this project.
# requires cmake.


# SDL setup.

cd SDL2-2.0.5
rm -f CMakeCache.txt
mkdir build
cd build
cmake ..

make

