project(PS2)

set(CMAKE_MINIMUM_REQUIRED_VERSION 3.16.3)

set(CXX_STANDARD c++23)

set(SOURCES src/loader/nro.cpp
            src/main.cpp
            src/hle/memory/memory.cpp
            src/hle/kernel/KObject.cpp
            src/hle/kernel/KThread.cpp
            src/hle/kernel/Scheduler.cpp
            src/aarch64/arm.cpp)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(suitchi ${SOURCES})
set(TARGET_NAME suitchi)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -std=c++23)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})