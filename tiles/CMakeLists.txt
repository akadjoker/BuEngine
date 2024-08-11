project(tiles)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)




file(GLOB SOURCES "src/*.cpp")
add_executable(tiles   ${SOURCES})


target_include_directories(tiles PUBLIC  include src)



if(CMAKE_BUILD_TYPE MATCHES Debug)

 target_compile_options(tiles PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g -Winvalid-pch -D_DEBUG)
  target_link_options(tiles PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g -Winvalid-pch -D_DEBUG) 
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(tiles PRIVATE -O3 -march=native -flto -funroll-loops -DNDEBUG)
    target_link_options(tiles PRIVATE -O3 -march=native -flto -funroll-loops -DNDEBUG)
endif()

target_link_libraries(tiles buengine raylib)

if (WIN32)
    target_link_libraries(tiles Winmm.lib)
endif()


if (UNIX)
    target_link_libraries(tiles  m )
endif()
