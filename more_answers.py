import shutil
import argparse 

# generate new cpp for more answers
parser = argparse.ArgumentParser()
parser.add_argument('--id', dest='cpp_filename', type=str, help="the source cpp file to generate")
opt = parser.parse_args()

shutil.copyfile(f'{opt.cpp_filename}.cpp', f'{opt.cpp_filename}_a.cpp')
