cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

find_package(Boost COMPONENTS system thread regex REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

find_package(GTest REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(libmongocxx REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)

include(ExternalProject)

ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${LIBMONGOCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR})

enable_testing()

file(GLOB_RECURSE SRCS ../src/*.cpp)
list(FILTER SRCS EXCLUDE REGEX ".*main.cpp$")

file(GLOB TEST_SRCS *.cpp)

add_executable(opstest ${SRCS} ${TEST_SRCS})

target_compile_features(opstest PUBLIC cxx_std_17)

target_link_libraries(opstest PUBLIC ${GTEST_BOTH_LIBRARIES})
target_link_libraries(opstest PUBLIC ${Boost_LIBRARIES})
target_link_libraries(opstest PUBLIC ${LIBBSONCXX_LIBRARIES})
target_link_libraries(opstest PUBLIC ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(opstest PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(opstest PRIVATE cpprestsdk::cpprest)
