cmake_minimum_required(VERSION 3.0)
project(funky_scheduler)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-2 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -gdwarf-2 -O3")

add_definitions(
  -std=c++11
  -pedantic
  -Wall
  -Wextra
  -Werror
  -Wcast-align
  -Wcast-qual
  -Wctor-dtor-privacy
  -Wnon-virtual-dtor
  -Wdisabled-optimization
  -Wformat=2
  -Wlogical-op
  -Wmissing-declarations
  -Wmissing-include-dirs
  -Wnoexcept
  -Wold-style-cast
  -Woverloaded-virtual
  -Wredundant-decls
  -Wshadow
  -Wsign-conversion
  -Wsign-promo
  -Wstrict-null-sentinel
  -Wstrict-overflow=5
  -Wswitch-default
  -Wundef
  -Wzero-as-null-pointer-constant
  -Wuseless-cast
)

set(Boost_USE_MULTITHREADED ON)
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost REQUIRED COMPONENTS log)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(src)

file(GLOB sources src/*.cpp)

add_executable(funky_scheduler ${sources})

target_link_libraries(funky_scheduler
  ${Boost_LIBRARIES}
  yaml-cpp
  pthread
  )