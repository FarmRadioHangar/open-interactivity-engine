cmake_minimum_required(VERSION 3.5)

project(polls)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

find_package(Boost COMPONENTS system thread REQUIRED)

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

#add_executable(server ${SRCS})
add_executable(server src/main.cpp src/model.cpp src/server.cpp)

include_directories(${LIBMONGOCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIRS} ${LIBMONGOCXX_INCLUDE_DIR})

target_link_libraries(server PUBLIC ${Boost_LIBRARIES})
target_link_libraries(server PUBLIC ${LIBBSONCXX_LIBRARIES})
target_link_libraries(server PUBLIC ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(server PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(server PRIVATE cpprestsdk::cpprest)
