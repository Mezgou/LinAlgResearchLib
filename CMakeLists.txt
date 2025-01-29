cmake_minimum_required(VERSION 3.30)
project(LinAlgResearchLib)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-error=unused-but-set-variable>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-Wno-error=array-bounds>)

set(SRC_MATH Source/Math/SparseMatrix.h)
set(SRC_TESTS Source/Tests/Test.h Source/Tests/TestSparseMatrix.h)
set(INC_DIRS Source/Math Source/Tests Source)

add_subdirectory(Vendor/cunit/CUnit)

include_directories(Vendor/cunit/CUnit ${INC_DIRS})

add_executable(${PROJECT_NAME} Source/Main.cpp Source/Utils.h ${SRC_MATH} ${SRC_TESTS})

target_link_libraries(${PROJECT_NAME} PRIVATE cunit)
