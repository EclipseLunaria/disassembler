echo "cleaning build directory"
rm -r build/*
cd build
echo "running cmake"
cmake ../tests -DCMAKE_BUILD_TYPE=Debug
echo "executing make script"
make
echo "running tests"
./run_tests