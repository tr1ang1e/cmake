# description : project for cmake learning
#   > project dir = main project structure and cmake abilities
#   > external dir = creating and providing relocatable cmake modules
#
# follow instructions below to build all of parts 
# read 'project' or 'external' root CMakeLists.txt for info
#
# current version of /cmake/project/external/CMakeLists.txt file
# deals only with Windows libraries (paths = <path>/<file>.lib)
# change extension for '.a' or wait for me doing this part crossplatform 

# get project
> git clone <current_repo>
$ cd cmake

# build 'external' (project which provides libraries)
$ cd external
$ mkdir cmake 						# create ...
$ cd cmake							# ... and go to folder for cmake untilities
$ cmake ..							# generate cmake cache
$ cmake --build . --config Release			# build project, all output is in cmake/external/out/build

# provide built libraries
# 1 variant
$ cmake --install . --prefix <path>/out/install   				# install project output, <path> = absolute path to 'external' 
$ cpack											# generate installable package (location ./exportable-<version>-<system>)
> install package to cmake/project/external/exportable-<version>		# place necessary for project-consumer files to the required place
# 2 variant
cmake --install . --prefix <path>/external/exportable-<version>		# direct installation, <path> = absolute path to 'project'

# build 'project'
$ cd ../../project
$ mkdir cmake 						# create ...
$ cd cmake							# ... and go to folder for cmake untilities
$ cmake ..							# generate cmake cache
$ cmake --build . --config Release			# build project, all output is in cmake/project/out
> run main executable