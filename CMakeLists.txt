cmake_minimum_required(VERSION 3.10)
project(RoomCraft3D CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source and Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/freeglut/include
)

file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

add_executable(RoomCraft3D ${SOURCES} ${HEADERS})

# Link libraries based on compiler (MSVC or MinGW)
if(MSVC)
    target_link_libraries(RoomCraft3D
        ${CMAKE_CURRENT_SOURCE_DIR}/external/freeglut/lib/freeglut.lib
        opengl32
        glu32
    )
else()
    # MinGW / GCC
    target_link_libraries(RoomCraft3D
        ${CMAKE_CURRENT_SOURCE_DIR}/external/freeglut/lib/libfreeglut.dll.a
        opengl32
        glu32
    )
endif()

# Copy libfreeglut.dll into the executable output directory
add_custom_command(TARGET RoomCraft3D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/external/freeglut/bin/libfreeglut.dll"
        $<TARGET_FILE_DIR:RoomCraft3D>
)
