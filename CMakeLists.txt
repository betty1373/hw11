cmake_minimum_required(VERSION 3.10.2)

if (NOT DEFINED ENV{GITHUB_RUN_NUMBER})
    set(ENV{GITHUB_RUN_NUMBER} 1)
endif()
project(bulk_server VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIED)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_library(bulk SHARED ${PROJECT_SOURCE_DIR}/src/CmdReader.cpp 
                         ${PROJECT_SOURCE_DIR}/src/Logger.cpp 
                         ${PROJECT_SOURCE_DIR}/src/Server.cpp)

add_executable(bulk_server ${PROJECT_SOURCE_DIR}/main.cpp)
set_target_properties(bulk bulk_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wunused-parameter;-Wextra;-pedantic;-Werror"
)
target_link_libraries(bulk_server -fsanitize=address
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    bulk
)
install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
