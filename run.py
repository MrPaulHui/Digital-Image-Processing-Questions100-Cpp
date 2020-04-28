import os
import argparse 

# generate CMakeLists.txt
parser = argparse.ArgumentParser()
parser.add_argument('--id', dest='cpp_filename', type=str, default='t0', help="the source cpp file for run")
opt = parser.parse_args()
with open('CMakeLists.txt', 'r+') as f:
    f.write('cmake_minimum_required(VERSION 2.8)\n'+
            'set(CMAKE_CXX_FLAGS "-std=c++11 -O3")\n'+
            'project(DIP100)\n'+
            'find_package(OpenCV REQUIRED)\n'+
            f'add_executable({opt.cpp_filename} {opt.cpp_filename}.cpp)\n'+
            f'target_link_libraries({opt.cpp_filename} ' + '${OpenCV_LIBS})')

# cmake and make operation
os.system(f'cd build && cmake .. && make && ./{opt.cpp_filename}')
