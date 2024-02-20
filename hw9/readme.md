# 并行程序设计 第九次作业

` 2100011047 工学院 陈国赐 `

## 文件结构

`source`目录下主要文件(目录)及其功能如下表格所示:

| 文件(目录)名 | 文件(目录)功能                 |
| ------------ | ------------------------------ |
| output/      | 存放输出log的文件夹               |
| input.cpp    | 输入类实现                     |
| input.h      | 输入类声明及接口               |
| mat_demo.h   | 矩阵类声明及实现               |
| mytimer.cpp  | 计时器类实现                   |
| mytimer.h    | 计时器类声明及接口             |
| main.cpp     | 主程序源码                     |
| autotest.py  | 自用测试脚本                  |
| calc.h       | 负责计算与通信的各种函数, 封装在一个文件中 |
| Makefile     | makefile                       |
| INPUT.txt    | 要求的输入文件                       |

## 编译说明

`Makefile`中提供了3个目标`clean`,`mpi`,`runmpi`.

运行`make mpi`会编译MPI版本可执行文件`main`.

运行`make runmpi`会在编译MPI版本的基础上运行`main`.

运行`make clean`会清除生成的可执行文件.

`runmpi`可指定`MPI_PROCESS_NUM`为运行的进程个数,
`MPIRUN_FLAGS`为`mpirun`运行时的附加参数, 如下:

```bash
make runmpi MPIRUN_FLAGS=--oversubscribe MPI_PROCESS_NUM=16
```

(`--oversubscribe`开关可以让运行进程个数大于机器核个数, 在自己电脑上测试需要此开关.)

## 运行说明

在命令行中执行
```bash
./main [FILE]
```
即可以`[FILE]`为输入文件运行程序.

一些约定:
输入矩阵大小为0或者两矩阵大小不一致时输出提示信息, 并且程序返回值为1.

## autotest.py(~~如果需要的话~~)

~~由于是自用所以写的比较狂野而且没啥可拓展性~~
可以用自用的`autotest.py`脚本做一些简单的测试. 运行前要保证矩阵测试数据文件夹matadd(即作业附带的数据包)和`source`文件夹在同一个文件夹下(也就是在`autotest.py`的上一级目录).

脚本会选择几个`alpha`和`beta`值, 在进程数`p=1,2,4,8,16,32,3,7,13`的情况下测试五组数据,
并检查以下内容:
1. `process_i.log`拼接的结果是否和`result.txt`是同一个矩阵.
2. `result.txt`和标准答案是否一致.
并输出每次测试的结果, `AC`通过, `RE`运行时错误, `WA`答案错误.