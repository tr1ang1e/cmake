// get project version info from CMakeLists.txt = PROJECT(...)
// to get version = @<PROJECTNAME>_VERSION_<TYPE>@
#define VERSION_MAJOR 11
#define VERSION_MINOR 0
#define VERSION_PATCH 0
#define VERSION_TWEAK 0

// get options declared by OPTION() and transform to C/C++ define macros
/* #undef  */
#define mv_LOGGING_on
