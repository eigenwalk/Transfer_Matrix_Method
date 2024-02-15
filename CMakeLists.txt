cmake_minimum_required(VERSION 3.6.0)
project(TMM VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenMP REQUIRED)
  
include(CTest) 
enable_testing()
  
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/)  
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/) 
find_library(yaml_lib NAMES libyaml-cpp.a HINTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
find_library(openblas_lib NAMES libopenblas.lib HINTS ${CMAKE_CURRENT_SOURCE_DIR}/lib_win64/)
###find_library(armarillo_lib NAMES libarmadillo.dll.a HINTS "lib/")
  
add_executable(TMM main.cpp) 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/manager)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/optics)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/manager)
#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/optics)
target_link_libraries(${PROJECT_NAME} 
    job_manager_lib
    tmm_lib
    ${yaml_lib}
    ${openblas_lib}
    ${OpenMP_CXX_FLAGS}
    #${armadillo_lib}
)
 
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS})
#target_link_libraries(${PROJECT_NAME} PRIVATE "${OpenMP_CXX_FLAGS}")
#target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_FLAGS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(THIRD_PARTY_DLLS
   ${CMAKE_CURRENT_SOURCE_DIR}/lib_win64/libopenblas.dll
   #${CMAKE_CURRENT_SOURCE_DIR}/lib/libyaml-cpp.dll
)

# Copy dll files
foreach(file_i ${THIRD_PARTY_DLLS})
    add_custom_command(
        TARGET TMM POST_BUILD
        COMMAND ${CMAKE_COMMAND} ARGS -E copy 
        ${file_i} ${OUTPUT_PATH})
endforeach(file_i)

include(CPack)
