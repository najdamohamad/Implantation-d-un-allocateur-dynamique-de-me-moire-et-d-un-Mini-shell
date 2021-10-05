# CMake generated Testfile for 
# Source directory: /home/corentin/Documents/sepc/ensimag-malloc
# Build directory: /home/corentin/Documents/sepc/ensimag-malloc/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTestsAllocator "alloctest")
set_tests_properties(AllTestsAllocator PROPERTIES  _BACKTRACE_TRIPLES "/home/corentin/Documents/sepc/ensimag-malloc/CMakeLists.txt;77;add_test;/home/corentin/Documents/sepc/ensimag-malloc/CMakeLists.txt;0;")
subdirs("gtest")
