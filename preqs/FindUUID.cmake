#vi /usr/share/cmake-3.x/Modules/FindUUID.cmake

find_library(UUID_LIBRARIES NAMES uuid)
find_path(UUID_INCLUDE_DIRS NAMES uuid.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(uuid DEFAULT_MSG UUID_LIBRARIES UUID_INCLUDE_DIRS)