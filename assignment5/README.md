### 编译运行

``` sh
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DOPENGL_LIBRARY_DIR="C:\Program Files (x86)\freeglut\lib" -DGLUT_INCLUDE_DIR="C:\Program Files (x86)\freeglut\include" -DCMAKE_BUILD_TYPE=Debug
make
```

### 可能存在的问题

为了达到和网站上一样的效果，ray_tracer的traceRay返回的color不能乘上weight，但根据我个人的理解，secondary light对像素的影响应该小一点，所以是需要乘上weight的，代码中我按照我个人的理解进行了修改
