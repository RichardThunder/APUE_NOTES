# CMake

教程: [https://www.hahack.com/codes/cmake/]( https://www.hahack.com/codes/cmake/)

仓库:https://github.com/wzpan/cmake-demo.git

1. 编写 CMake 配置文件 CMakeLists.txt 。
2. 执行命令 `cmake PATH` 或者 `ccmake PATH` 生成 Makefile（`ccmake` 和 `cmake` 的区别在于前者提供了一个交互式的界面）。其中， `PATH` 是 CMakeLists.txt 所在的目录。
3. 使用 `make` 命令进行编译。

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo1)

# 指定生成目标
add_executable(Demo main.cc)
```



## 多文件

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo2)

# 指定生成目标
add_executable(Demo main.cc MathFunctions.cc)



#使用目录变量
#aux_source_directory(<dir> <variable>)
#该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. ALLCPP)
project(demo2)
add_executable(Demo ${ALLCPP})


```

# 多个目录,多文件

需要在根目录和子目录分别编写CMakeLists.txt

将子目录编译成静态库调用

```cmake
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

# 生成链接库
add_library (MathFunctions ${DIR_LIB_SRCS})
#在该文件中使用命令 add_library 将 src 目录中的源文件编译为静态链接库。
```

主目录

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo3)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 添加 math 子目录
add_subdirectory(math)

# 指定生成目标 
add_executable(Demo main.cc)

# 添加链接库
target_link_libraries(Demo MathFunctions)

```



# 自定义编译选项

可以将 MathFunctions 库设为一个可选的库，如果该选项为 `ON` ，就使用该库定义的数学函数来进行运算。否则就调用标准库中的数学函数库。

```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo4)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# 是否使用自己的 MathFunctions 库
option (USE_MYMATH
       "Use provided math implementation" ON)

# 是否加入 MathFunctions 库
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/math")
  add_subdirectory (math)  
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(Demo ${DIR_SRCS})
target_link_libraries (Demo  ${EXTRA_LIBS})
```

# 安装

首先先在 math/CMakeLists.txt 文件里添加下面两行：

```cmake
# 指定 MathFunctions 库的安装路径
install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)
```

指明 MathFunctions 库的安装路径

修改根目录的 CMakeLists 文件，在末尾添加下面几行：

```cmake
# 指定安装路径
install (TARGETS Demo DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
         DESTINATION include)
```

生成的 Demo 文件和 MathFunctions 函数库 libMathFunctions.o 文件将会被复制到 `/usr/local/bin` 中，而 MathFunctions.h 和生成的 config.h 文件则会被复制到 `/usr/local/include` 中



自定义安装路径

```cmake
#需要出现在 project(ProjectName ) 之前
set(CMAKE_INSTALL_PREFIX "/Users/richard/Desktop/unix")
```

```shell
cmake .
make
make install
```



# 测试

```cmake
# 启用测试
enable_testing()

# 测试程序是否成功运行
add_test (test_run Demo 5 2)

#其中 PASS_REGULAR_EXPRESSION 用来测试输出是否包含后面跟着的字符串。
# 测试帮助信息是否可以正常提示
add_test (test_usage Demo)
set_tests_properties (test_usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage: .* base exponent")

# 测试 5 的平方
add_test (test_5_2 Demo 5 2)

set_tests_properties (test_5_2
 PROPERTIES PASS_REGULAR_EXPRESSION "is 25")

# 测试 10 的 5 次方
add_test (test_10_5 Demo 10 5)

set_tests_properties (test_10_5
 PROPERTIES PASS_REGULAR_EXPRESSION "is 100000")

# 测试 2 的 10 次方
add_test (test_2_10 Demo 2 10)

set_tests_properties (test_2_10
 PROPERTIES PASS_REGULAR_EXPRESSION "is 1024")
```

## 使用宏来编写测试用例

```cmake
# 定义一个宏，用来简化测试工作
macro (do_test arg1 arg2 result)

  add_test (test_${arg1}_${arg2} Demo ${arg1} ${arg2})
  set_tests_properties (test_${arg1}_${arg2}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})

endmacro (do_test)
 
# 使用该宏进行一系列的数据测试
do_test (5 2 "is 25")
do_test (10 5 "is 100000")
do_test (2 10 "is 1024")
```

# 支持GDB

让 CMake 支持 gdb 的设置也很容易，只需要指定 `Debug` 模式下开启 `-g` 选项：

```cmake
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
```

之后可以直接对生成的程序使用 gdb 来调试。

# 添加环境检查

**添加 CheckFunctionExists 宏**
首先在顶层 CMakeLists 文件中添加 CheckFunctionExists.cmake 宏，并调用 check_function_exists 命令测试链接器是否能够在链接阶段找到 pow 函数。

```cmake
# 检查系统是否支持 pow 函数
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
check_function_exists (pow HAVE_POW)
将上面这段代码放在 configure_file 命令前
```

### 预定义相关宏变量

接下来修改 [config.h.in](http://config.h.in/) 文件，预定义相关的宏变量。

```
// does the platform provide pow function?
#cmakedefine HAVE_POW
```

### 在代码中使用宏和函数

最后一步是修改 [main.cc](http://main.cc/) ，在代码中使用宏和函数：

```
#ifdef HAVE_POW
    printf("Now we use the standard library. \n");
    double result = pow(base, exponent);
#else
    printf("Now we use our own Math library. \n");
    double result = power(base, exponent);
#endif
```

# 添加版本号

首先修改顶层 CMakeLists 文件，在 `project` 命令之后加入如下两行：

```cmake
set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)
```

分别指定当前的项目的主版本号和副版本号。

为了在代码中获取版本信息，我们可以修改 [config.h.in](http://config.h.in/) 文件，添加两个预定义变量：

```cmake
// the configured options and settings for Tutorial
#define Demo_VERSION_MAJOR @Demo_VERSION_MAJOR@
#define Demo_VERSION_MINOR @Demo_VERSION_MINOR@
```

可以直接在代码中打印版本信息了：

```cpp
printf("%s Version %d.%d\n",
            argv[0],
            Demo_VERSION_MAJOR,
            Demo_VERSION_MINOR);
```

# 生成安装包

配置生成各种平台上的安装包，包括二进制安装包和源码安装包

需要用到 CPack ，它同样也是由 CMake 提供的一个工具，专门用于打包。

首先在顶层的 CMakeLists.txt 文件尾部添加下面几行：

```cmake
# 构建一个 CPack 安装包
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
  "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Demo_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Demo_VERSION_MINOR}")
include (CPack)
```



- 生成二进制安装包：

```cmake
cpack -C CPackConfig.cmake
```

- 生成源码安装包

```cmake
cpack -C CPackSourceConfig.cmake
```