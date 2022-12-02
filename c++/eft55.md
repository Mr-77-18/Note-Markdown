# 1.让自己习惯C++
## 条款3:尽可能使用const
1. 编译器使用的时bitwise constness，但是你编写程序时应该使用**概念上的常量性(conceptual constness)** 
```c++
//conceptual constness指对于CTextBlock类来说，逻辑上我只要内容不变，就是常量的，即pText指向的内容不变。而类中的其它成员可以依据情况变化（比如加上mutable）
class CTextBlock{
	public:
		...
		std::size_t length() const
	private:
 		char* pText;
 		mutable std::size_t textLength;
 		mutable bool lengthIsValid;//将可以改变的变量声明为mutable
}
std::size_t CTextBlock::length()const
{
	if(!lengthIsValid){
		textLength = std::strlen(pText);
		lengthIsValid = true;
	}
	return textLength;
}
`````

<++>
# 2.构造/析构/赋值运算
<++>
## 条款
const    局部的const    全局的const?

## 条款
编译器在一些情况下不会为你生成copy构造函数...


## 条款10：令operator=返回一个reference to *this

:smile:nothing to say

## 条款11：在operator=中处理自我赋值
处理自我赋值并没有想象中那么简单。\
对于operator=，你应该考虑的不仅有**自我赋值** 问题，还有**异常安全** 问题。\
:fast_forward:一个好的处理方法是使用**copy and swap** 技术

## 条款12:复制对象时勿忘其每一个成分
1. Coping函数要初始化local成员变量，以及要调用base class的Coping函数。
2. 不要尝试在coping函数中调用另外一个coping函数。\
:fast_forward:such as:\
copy constructor当中调用copy assigment function\
or\
copy assigment function中调用copy constructor中的函数

