mkdir -p ../build;
Clang main.c -L ../lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL ../lib/libraylib.a -o ../build/Point\ \&\ Click\ Game -g -O0 -Wall;
