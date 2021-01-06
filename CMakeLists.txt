# cmake version
cmake_minimum_required(VERSION 2.8)

project( NoobCar )

find_package( OpenCV REQUIRED )
find_package(WiringPi REQUIRED)
find_package (Threads REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${WIRINGPI_INCLUDE_DIRS})

# add main cpp
add_executable( NoobCar main.cpp motion.cpp vision.cpp)

target_link_libraries(NoobCar ${OpenCV_LIBS})
target_link_libraries(NoobCar ${WIRINGPI_LIBRARIES}) 
target_link_libraries (NoobCar ${CMAKE_THREAD_LIBS_INIT})