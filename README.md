### 编译运行

在目标assignment文件夹下，运行下面的指令序列进行编译，需要将freeglut的目录修改成你本地环境下freeglut的目录，并确保freeglut的lib是使用MinGW编译的。

``` sh
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DOPENGL_LIBRARY_DIR="C:\Program Files (x86)\freeglut\lib" -DGLUT_INCLUDE_DIR="C:\Program Files (x86)\freeglut\include" -DCMAKE_BUILD_TYPE=Release
make
```

### 