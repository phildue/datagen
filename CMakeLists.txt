cmake_minimum_required(VERSION 3.5.1)
set (CMAKE_CXX_STANDARD 11)

project(datagen)
set(AIRSIM_ROOT lib/AirSim)
set(CMAKE_BINARY_DIR ../bin)

set(PROJECT_ROOT ./)


LIST(APPEND CMAKE_MODULE_PATH "${AIRSIM_ROOT}/cmake/cmake-modules") 

INCLUDE("${AIRSIM_ROOT}/cmake/cmake-modules/CommonSetup.cmake")
add_subdirectory(${AIRSIM_ROOT}/external/rpclib/rpclib-2.2.1 ${CMAKE_CURRENT_BINARY_DIR}/rpclib)

set(OpenCV_DIR ${PROJECT_ROOT}/lib/opencv/build)
find_package(OpenCV REQUIRED)

CommonSetup()

#override some CommonSetup settings:
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

IncludeEigen()

SetupConsoleBuild()

set(CLIENT_SRC_DIR ${PROJECT_ROOT}/src/cpp/client/)
set(ASC_INCLUDES ${CLIENT_SRC_DIR})
set(ASC_SOURCES 
                        ${CLIENT_SRC_DIR}/airsim/AirSimInterface.cpp
                        ${CLIENT_SRC_DIR}/airsim/Session.cpp
                        ${CLIENT_SRC_DIR}/Image.cpp
                        ${CLIENT_SRC_DIR}/gui/CmdListener.cpp
                        ${CLIENT_SRC_DIR}/annotate/Camera.cpp
                        ${CLIENT_SRC_DIR}/annotate/Annotator.cpp
                        ${CLIENT_SRC_DIR}/annotate/RandomPoseDataGenerator.cpp
                        ${CLIENT_SRC_DIR}/annotate/ObjectFilter.cpp

                )
set(ASC_HEADERS 
                        ${CLIENT_SRC_DIR}/airsim/AirSimInterface.h
                        ${CLIENT_SRC_DIR}/airsim/Session.h
                        ${CLIENT_SRC_DIR}/Image.h
                        ${CLIENT_SRC_DIR}/gui/CmdListener.h
                        ${CLIENT_SRC_DIR}/annotate/Gates.h
                        ${CLIENT_SRC_DIR}/annotate/Camera.h
                        ${CLIENT_SRC_DIR}/annotate/Annotator.h
                        ${CLIENT_SRC_DIR}/annotate/RandomPoseDataGenerator.h
                        ${CLIENT_SRC_DIR}/annotate/ObjectFilter.h
                        ${CLIENT_SRC_DIR}/annotate/BoxObject.h
                        ${CLIENT_SRC_DIR}/annotate/Labels.h
                        ${CLIENT_SRC_DIR}/annotate/Object.h
                        ${CLIENT_SRC_DIR}/gui/InputParser.h
                        ${CLIENT_SRC_DIR}/gui/Args.h
        )


set(FLIGHTSIM_SRC
        ${CLIENT_SRC_DIR}/annotate/AnnotatorFlight.cpp
        ${CLIENT_SRC_DIR}/annotate/AnnotatorFlight.h
        ${CLIENT_SRC_DIR}/annotate/TrajectoryPoseGenerator.h
        ${CLIENT_SRC_DIR}/annotate/TrajectoryPoseGenerator.cpp
        ${CLIENT_SRC_DIR}/sim/Simulation.h
        ${CLIENT_SRC_DIR}/sim/DroneModel.h
        ${CLIENT_SRC_DIR}/sim/Autopilot.h
        ${CLIENT_SRC_DIR}/sim/SensorModel.h
        ${CLIENT_SRC_DIR}/sim/DroneModel.cpp
        ${CLIENT_SRC_DIR}/sim/Simulation.cpp
        ${CLIENT_SRC_DIR}/sim/Autopilot.cpp
        ${CLIENT_SRC_DIR}/sim/SensorModel.cpp
        )
include_directories(
  ${AIRSIM_ROOT}/AirLib/include
  ${RPC_LIB_INCLUDES}
  ${OpenCV_INCLUDE_DIRS}
  ${ASC_INCLUDES}

)
add_definitions(-DTARGET_SIMULATOR=1)
link_directories(${AIRSIM_ROOT}/AirLib/lib/x64/Debug/)

set(SOURCES
                        ${ASC_SOURCES}
                        )
set(HEADERS
        ${ASC_HEADERS}
                    )

add_executable(randomview ${SOURCES} ${HEADERS} src/cpp/datagen/randomview.cpp)

target_link_libraries(randomview ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(randomview AirLib)
target_link_libraries(randomview ${RPC_LIB})
target_link_libraries(randomview ${OpenCV_LIBS})

add_executable(flightsimu ${SOURCES} ${HEADERS} src/cpp/datagen/flightsimu.cpp ${FLIGHTSIM_SRC})

target_link_libraries(flightsimu ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(flightsimu AirLib)
target_link_libraries(flightsimu ${RPC_LIB})
target_link_libraries(flightsimu ${OpenCV_LIBS})





