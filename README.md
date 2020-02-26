# cmdCalc
simple homework#1

*****************************
**  关于commandLineCalc.c  **
*****************************
简单的命令行电费计算器...(闭门造车..做的很粗糙就是了)
version: v0.2.0
工具：Notepad++7.8.0 & gcc8.1.0 (& Win10 cmd)


v0.1.0实现的功能：
1. PPT上要求的4个函数以及一个额外的help函数
2. 不区分大小写
3. 不受空格数量的影响，不受输入开始和末尾的空格的影响
4. 自动检测传入参数的个数（例如：如果给elecfee函数的参数数量不为1就会报错）
5. 彩色的界面
6. 检测elecfee是否曾被成功调用（否则不能调用feedetail和tocurrency）

v0.2.0实现的功能：
1.改进了交互，现在有更详细的错误反馈
2.增加了简单运算的功能(+-*/%=)，支持优先级和括号
3.重构了一下代码(未完成)

关于代码：
1. v0.1.0是交bonus task的时候写的，基本上的界面功能是实现了，不过代码写得。。挺乱的
   没有C99,C11特性，单文件可以直接编译
   
2. v0.2.0是研讨课前临时改的，稍微整理了一下，把一大堆函数打包成了库，把错误处理打包成了函数
   所有文件请放在同一文件夹下编译
   源文件里只有开放给用户的函数
   加入的运算功能全写在了SYA_Calc.h里，不过为了写的顺手，用了一些C99和C11的特性 
   （在opstack.h里用了C11的泛型选择和匿名联合，有些地方用了C99风格的for循环）//实际上都可以不用的
   //你完全可以用C++来重写SYA_Calc.h  //不过毕竟是C语言课，还是自己实现stack吧..
   
3. #pragma c11 on 是为了让低版本的编译器编译它的时候自动启用C11标准,不过你得确保gcc版本在4.7以上
   可以在cmd(或者其它命令行终端)输入gcc --version 来查看gcc版本
   高版本的gcc可能默认使用C11标准来编译(比如我电脑上的gcc8.1.0)
   在unix环境下(比如可以用git bash)输入 gcc -posix -E -dM - </dev/null后会出来一大堆东西
   可以找到类似#define __STDC_VERSION__ 201710L的一行，表明它默认适用哪一年的标准 
   其中199901L=C99 201112L=C11 201710L=C18(C18没有新特性，只是修了一下C11的bug)
   
4. 现在cmdInterpreter上开了一堆洞放指针，丑，而且不利于扩展(但是我懒得改)
   要改的话可以考虑把电量，电费之类的和电费函数绑到一起(就像在opstack.h里做的一样)
   ///或者不要把函数绑进去，面向对象也未必好...
   然后只需要传一个结构指针给cmdInterpreter就行了
   
5. 本来应该把错误处理和计算的部分放进一个函数里的..
   现在直接裸露出来，不太清晰，而且把cmdInterpreter撑得过长了..
   
6. 代码的总量：v0.1.0有不到300行，v0.2.0四个文件总计有接近700行(栈和运算的实现占了一半的代码量)

已知的若干bug：(研讨课结束后我就没有debug/重构的动力了..)
1. 读入的时候会对空格进行处理，但是不会对其它的空白字符(如[tab])做处理
   所以会无法识别含有这些字符的命令
   可能的解决方案：重写一下cmd_gets(),把' '换成isspace()之类的

2. 不能识别负数，因为在写SYA_Calc.h的时候没有考虑单目运算的关系..
   可能的解决方案：加入单目运算逻辑？还有左右结合性好像也得考虑..好复杂X_X
   
3. 因为运算是直接使用double类型，然后用%g打印的，所以..
   数字大一点的时候就会变成指数显示，精度也没法保证(虽说算电费绝对够了)
   可能的解决方案：采用高精度计算   //...这样的话是不是用Cpp的重载会更方便一点
   
   
   
