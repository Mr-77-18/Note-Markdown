# 条款
const    局部的const    全局的const?

# 条款
编译器在一些情况下不会为你生成copy构造函数...


# 条款10：令operator=返回一个reference to *this

:smile:nothing to say

# 条款11：在operator=中处理自我赋值
处理自我赋值并没有想象中那么简单。\
对于operator=，你应该考虑的不仅有**自我赋值** 问题，还有**异常安全** 问题。\
:fast_forward:一个好的处理方法是使用**copy and swap** 技术

# 条款12:复制对象时勿忘其每一个成分
1. Coping函数要初始化local成员变量，以及要调用base class的Coping函数。
2. 不要尝试在coping函数中调用另外一个coping函数。\
:fast_forward:such as:\
copy constructor当中调用copy assigment function\
or\
copy assigment function中调用copy constructor中的函数

