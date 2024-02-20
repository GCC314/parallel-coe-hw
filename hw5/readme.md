# 并行程序设计 第五次作业

` 2100011047 工学院 陈国赐 `

## 文件结构

| 文件名       | 文件功能                       |
|--------------|--------------------------------|
| input.h      | 输入类的头文件               |
| input.cpp    | 输入类的变量及函数的定义     |
| mat_demo.h   | 矩阵类的头文件               |
| mat_demo.cpp | 矩阵类函数的定义             |
| main.cpp     | 主程序源代码                  |
| Makefile     | Makefile 脚本                  |
| a.in / b.in  | 示例输入文件                  |

## 编译说明

程序采用`c++`编写，并部署了`Makefile`。在根目录下执行
```bash
make main
```
即可完成编译，获得可执行文件`main`/`main.exe`。

## 运行说明

运行时在命令行执行
```bash
    ./hw5 [FILE]
```

其中`FILE`是指定的输入文件。

## 运行效果

程序`main`按照题目要求解析输入文件，按照文件内容初始化一个对应的矩阵`matA`。
为了演示，我们同时创建一个大小相等且元素全被初始化为1的矩阵`matB`。

程序的输出由输入文件的`matrix_print`属性决定：此处规定若`matrix_print`为`0`，则我们将所有输出内容输出到标准输出流；反之，我们将输出内容输出到`log-yyyy-mm-dd-hh-mm-ss.log`文件中，其中`yyyy-mm-dd-hh-mm-ss`是当前系统的时间。

程序在构造矩阵`matA`和`matB`后做如下操作：
1. 将`matB`的第0行第0列的元素修改为2。
2. 输出`Input`类收集到的属性值
3. 求元素max
4. 求元素min
5. 求`calculations`指定的方法的结果
6. 计算`matA + matB`并输出

## 测试

文件包中还提供了用于测试的输入文件`a.in`, `b.in`。
`Makefile`中也给出了用于测试的`target`。
直接执行
```bash
make all
```
能在编译之后自动将`a.in`和`b.in`作为输入进行测试。

## 附加的Makefile选项

`test`: 只测试，不编译。
`clean`: 删除可执行文件。
`clean_log`: 删除所有`.log`输出文件。