cmake_minimum_required(VERSION 3.28)

project(SimpleLogger LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET vulkan_modules
        TYPE CXX_MODULES
        FILES
            src/logger.cxx
)

target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
 
option(INCLUDE_SAMPLES "INCLUDE SAMPLE CODE" OFF)

if(INCLUDE_SAMPLES)
    add_subdirectory(samples)
endif(INCLUDE_SAMPLES)
