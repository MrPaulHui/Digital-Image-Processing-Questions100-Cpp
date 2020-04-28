import argparse 

# generate .cpp
parser = argparse.ArgumentParser()
parser.add_argument('--id', dest='cpp_filename', type=str, help="the source cpp file to generate")
opt = parser.parse_args()

with open(f'{opt.cpp_filename}.cpp', 'w') as f:
    f.write('//题目描述：\n//\n'
            + '#include <opencv2/core.hpp>\n'
            + '#include <opencv2/highgui.hpp>\n'
            + '#include <iostream>\n'
            + 'using namespace std;\n'
            + '\n'
            + 'int main(){\n'
            + '}')
