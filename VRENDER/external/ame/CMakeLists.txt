cmake_minimum_required(VERSION 3.20)

# Create Library
add_library(ame INTERFACE)

target_include_directories(ame INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/ame/include
)

target_compile_features(ame INTERFACE cxx_std_20)