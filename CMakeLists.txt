add_subdirectory(mediator)
add_subdirectory(connection)
add_subdirectory(pluginManager)
add_subdirectory(http)

if(PUTILS_BUILD_PSE)
    add_subdirectory(pse)
endif()

if(PUTILS_BUILD_POGRE)
    add_subdirectory(pogre)
endif()

file(GLOB src *.hpp)
add_library(putils INTERFACE)
target_include_directories(putils INTERFACE .)
