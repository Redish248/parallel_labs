cmake_minimum_required(VERSION 3.23)
project(lab6_cuda CUDA)

set(CMAKE_CUDA_STANDARD 14)

add_executable(lab6_cuda main.cu lab6_print.cu)

set_target_properties(lab6_cuda PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
