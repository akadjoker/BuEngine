
project(buengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti -fno-strict-aliasing -DUSE_GRAPHICS ")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



add_compile_options(
    -Wextra
    -Wall
    -Wno-missing-field-initializers
    -Wundef
    -Wuninitialized
    -Wunused
    -Wno-unused-parameter
    -Wwrite-strings
    -Wchar-subscripts
    -funsigned-char
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(external/raylib)

file(GLOB SOURCES "src/*.cpp")
add_library(buengine STATIC  ${SOURCES})


target_include_directories(buengine PUBLIC include src)
target_precompile_headers(buengine PRIVATE include/pch.h)

if(CMAKE_BUILD_TYPE MATCHES Debug)

 target_compile_options(buengine PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g -Winvalid-pch -D_DEBUG)
  target_link_options(buengine PRIVATE -fsanitize=address -fsanitize=undefined -fsanitize=leak -g -Winvalid-pch -D_DEBUG) 
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(buengine PRIVATE -O3 -march=native -flto -funroll-loops -DNDEBUG)
    target_link_options(buengine PRIVATE -O3 -march=native -flto -funroll-loops -DNDEBUG)
endif()


