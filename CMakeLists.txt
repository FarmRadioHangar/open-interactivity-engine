cmake_minimum_required(VERSION 2.6)

project(polls)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -D_GLIBCXX_USE_CXX11_ABI=0")

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

add_executable(polls src/main.cpp src/server.cpp src/model.cpp)

include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")
include_directories("/usr/local/lib")

target_link_libraries(polls ${Boost_LIBRARIES})
target_link_libraries(polls ${LIBBSONCXX_LIBRARIES})
target_link_libraries(polls ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(polls ${OPENSSL_LIBRARIES})
target_link_libraries(polls PRIVATE cpprestsdk::cpprest)
