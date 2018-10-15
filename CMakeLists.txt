cmake_minimum_required(VERSION 3.5)

project(ops)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

find_package(Boost COMPONENTS system thread regex REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

find_package(libbsoncxx REQUIRED)
find_package(libmongocxx REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(ops ${SRCS})

include_directories(${LIBMONGOCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR})

target_compile_features(ops PUBLIC cxx_std_17)

target_link_libraries(ops PUBLIC ${Boost_LIBRARIES})
target_link_libraries(ops PUBLIC ${LIBBSONCXX_LIBRARIES})
target_link_libraries(ops PUBLIC ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(ops PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(ops PRIVATE cpprestsdk::cpprest)
