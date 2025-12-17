add_library(can_interface
    src/SocketCANInterface.cpp
)

target_include_directories(can_interface
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(can_interface
    PUBLIC
        cxx_std_17
)