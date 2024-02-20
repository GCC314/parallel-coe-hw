# 并行程序设计 第六次作业

` 2100011047 工学院 陈国赐 `

## 文件结构

文件包中主要文件和文件功能如下表格所示：

| 文件名       | 文件功能                       |
|--------------|--------------------------------|
| mytimer.h    | 计时器类的头文件               |
| mytimer.cpp  | 计时器类的变量及函数的定义     |
| mat_demo.h   | 矩阵类的头文件，用于性能测试   |
| mat_demo.cpp | 矩阵类函数的定义，用于性能测试 |
| demo.cpp     | 演示计时器效果的主程序         |
| Makefile     | Makefile 脚本                  |

## 编译说明

程序采用`c++`编写，并部署了`Makefile`。在根目录下执行
```bash
make main
```
即可完成编译，获得可执行文件`main`。

```bash
make test
```
在编译完成后执行`./main`，进行计时器效果演示。

```bash
make debugtest
```
在编译时开启宏`DEBUG`，并执行`./main`。（用于调试）

## 运行说明

在编译完成后可以用`make test`或者`./main`运行演示程序。
程序调用作业5中编写的矩阵库，调用一定次数的矩阵加法、数乘、求最值等运算。统计以下函数的执行时间及次数等相关信息并在最后输出：
* `Mat_Demo::MatDemo`
* `Mat_Demo::operator *=`
* `Mat_Demo::operator =`
* `Mat_Demo::operator +`
* `Mat_Demo::operator -`
* `Mat_Demo::Max`
* `Mat_Demo::Min`

