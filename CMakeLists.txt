cmake_minimum_required(VERSION 3.5.0)
project(test2 VERSION 0.1.0 LANGUAGES C)

add_executable(test2 main.c)

set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 23
    
)

add_executable( ChildProcess process.c )

add_library( targetlibrary SHARED target.c )
