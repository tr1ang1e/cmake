# CMake generated Testfile for 
# Source directory: C:/Users/tr1an/Projects/cmake/tutorial
# Build directory: C:/Users/tr1an/Projects/cmake/tutorial/out/build/x64-Debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_run "C:/Users/tr1an/Projects/cmake/tutorial/out/build/x64-Debug/main.exe")
set_tests_properties(test_run PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/tr1an/Projects/cmake/tutorial/CMakeLists.txt;133;ADD_TEST;C:/Users/tr1an/Projects/cmake/tutorial/CMakeLists.txt;0;")
subdirs("libraries/static")
