cmake_minimum_required(VERSION 3.5)
project(BundleFusion_Ubuntu)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
set(CUDA_NVCC_FLAGS "-g -G")
find_package(OpenCV REQUIRED)

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_directories(${GLEW_LIBRARIES})
endif()




find_package(CUDA)
#set(CUDA_SOURCE_PROPERTY_FORMAT OBJ)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

find_package(Eigen3 REQUIRED)
link_directories(${PROJECT_NAME}/lib)#https://blog.csdn.net/bigdog_1027/article/details/79113342
#find_library
include_directories(
        ${EIGEN3_INCLUDE_DIR}      
)

#INCLUDE_DIRECTORIES(/home/manifold/OpenNI-Linux-x64-2.3.0.43/Include)
INCLUDE_DIRECTORIES(/home/manifold/ASUS-Linux-x64-OpenNI2.2/Include)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)  
set(CUDA_SOURCE_PROPERTY_FORMAT OBJ)
set(CUDA_SEPARABLE_COMPILATION ON)
include_directories(${CUDA_INCLUDE_DIRS})
set(CUDA_PROPAGATE_HOST_FLAGS OFF)
set(CUDA_NVCC_FLAGS -arch=sm_61;-O3;-G;-g;-std=c++11)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)
include_directories(
${PROJECT_SOURCE_DIR}/external/mLib/include
${PROJECT_SOURCE_DIR}/mLibExternal/include
${PROJECT_SOURCE_DIR}/FriedLiver/Include/cutil/inc
${PROJECT_SOURCE_DIR}/FriedLiver/Include
${PROJECT_SOURCE_DIR}/FriedLiver/Include/Uplink
${PROJECT_SOURCE_DIR}/FriedLiver/Source
${PROJECT_SOURCE_DIR}/FriedLiver/Source/SiftGPU
${PROJECT_SOURCE_DIR}/FriedLiver/Source/Solver
)
add_library(${PROJECT_NAME} SHARED
external/mLib/src/core-base/common.cpp
external/mLib/src/core-util/timer.cpp
external/mLib/src/core-util/utility.cpp
external/mLib/include/mLibDepthCamera.cpp
#external/mLib/src/ext-depthcamera/image.cpp
)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

file(GLOB CURRENT_HEADERS  FriedLiver/Source/*.h )
file(GLOB CURRENT_SOURCES  FriedLiver/Source/*.cpp)


CUDA_ADD_EXECUTABLE(FriedLiver 
FriedLiver/Source/FriedLiver.cpp FriedLiver/Source/ConditionManager.cpp 
FriedLiver/Source/RGBDSensor.cpp
FriedLiver/Source/PrimeSenseSensor.cpp
#FriedLiver/Source/BinaryDumpReader.cpp
FriedLiver/Source/SensorDataReader.cpp
FriedLiver/Source/CUDAImageManager.cpp
FriedLiver/Source/CUDAImageUtil.cu
FriedLiver/Source/TimingLog.cpp
FriedLiver/Source/Bundler.cpp
FriedLiver/Source/OnlineBundler.cu
FriedLiver/Source/OnlineBundler.cpp
FriedLiver/Source/SBA.cu
FriedLiver/Source/SBA.cpp
FriedLiver/Source/CUDACache.cu
FriedLiver/Source/CUDACache.cpp
FriedLiver/Source/GlobalAppState.cpp
FriedLiver/Source/TrajectoryManager.cpp

FriedLiver/Source/SiftGPU/SIFTImageManager.cu
FriedLiver/Source/SiftGPU/SIFTImageManager.cpp
FriedLiver/Source/SiftGPU/SiftGPU.cpp
FriedLiver/Source/SiftGPU/ProgramCU.cu
FriedLiver/Source/SiftGPU/CUDASiftConstant.cu
#FriedLiver/Source/stdafx.cpp
FriedLiver/Source/SiftGPU/SiftMatch.cpp
FriedLiver/Source/SiftGPU/SiftPyramid.cpp
FriedLiver/Source/SiftGPU/GlobalUtil.cpp
FriedLiver/Source/SiftGPU/CuTexImage.cpp
FriedLiver/Source/Solver/CUDASolverBundling.cpp
FriedLiver/Source/Solver/SolverBundling.cu
FriedLiver/Source/DepthSensing/DepthSensing.cpp
FriedLiver/Source/DepthSensing/CUDAMarchingCubesSDF.cu
FriedLiver/Source/DepthSensing/CUDAMarchingCubesHashSDF.cpp
FriedLiver/Source/DepthSensing/CUDASceneRepChunkGrid.cu
FriedLiver/Source/DepthSensing/CUDASceneRepChunkGrid.cpp
FriedLiver/Source/DepthSensing/CUDAConstant.cu
FriedLiver/Source/DepthSensing/CUDASceneRepHashSDF.cu
FriedLiver/Source/DepthSensing/CUDASceneRepHashSDF.cpp
FriedLiver/Source/DepthSensing/TimingLogDepthSensing.cpp
FriedLiver/Source/DepthSensing/ScanCS.cu
FriedLiver/Source/DepthSensing/CUDAScan.cpp
FriedLiver/Source/DepthSensing/CUDARayCastSDF.cu
FriedLiver/Source/DepthSensing/CUDARayCastSDF.cpp
FriedLiver/Source/DepthSensing/CameraUtil.cu

#FriedLiver/Source/DepthSensing/StdOutputLogger.cpp
)
#add_subdirectory(external/mLib)
target_link_libraries(FriedLiver ${PROJECT_NAME} ${CUDA_LIBRARIES}

${OPENGL_LIBRARIES}
${GLEW_LIBRARY}
${GLFW3_LIBRARY}
GLEW
glfw3
X11 Xrandr Xinerama Xi Xxf86vm Xcursor pthread dl
GLU
#${PROJECT_NAME}/lib/libfreeimage.a
#/home/manifold/OpenNI-Linux-x64-2.3.0.43/Redist/libOpenNI2.so
OpenNI2
${OpenCV_LIBS}
)

