# 1.让自己习惯C++
## 条款1：视C++为一个语言联邦
1. C
2. Object-Oriented C++
3. Template C++
4. STL
在不同的场景使用不同的联邦，有着不同的规则。
## 条款2：尽量以const , enum , inline替换 #define
当出错的时候便于找到错误，比如有变量名输出等，但是define的话就没有这样的提示，只会出现令人恼怒的错误。
## 条款3:尽可能使用const
1. 编译器使用的是bitwise constness，但是你编写程序时应该使用**概念上的常量性(conceptual constness)** 
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


# 2.构造/析构/赋值运算

## 条款5: 了解c++默默编写并调用哪些函数
要理解以下几个问题\
1. 在什么情况下会产生default constructor or coping function
2. 这些函数做了什么

:fast_forward:例如对于copy assignment\
当类中存在引用的时候，编译器不会为你产生copy assignment函数（如果你没有声明和定义的话）
```c++
class NamedObject{
	public:
		...
	private:
		std::string& nameValue;//含有一个引用
}
`````

:star:原因是:**C++不允许让reference改指向不同对象** (什么意思呢？)\
:fast_forward:例如
```c++
//你定义了一个引用ra，并且指向了a
int a = 1;
int& ra = a;
//然后你想让ra重新指向b（这是错误的）
int b = 77;
ra = b;
`````

## 条款6: 若不想使用编译器自动生成的函数，就改明确拒绝
有两种方式拒绝之：
1. 将函数在private中进行声明（并且不去定义它）,这种方式下友员类或者函数可以访问，但是因为没有定义它，所以会在链接的时候报错。
2. 继承像Uncopyable这样的类（当base class没有对应的函数的时候,编译器就不会为derived class自动生成了）

## 条款7：为多态基类声明virtual函数
## 条款8：别让异常逃离析构函数
:star:两种解决方法：
1. 吞下它，并做一些记录
2. 结束程序
## 条款9：绝不在构造和析构过程中调用virtual函数
可以调用，但没必要。
:star:不管调用的是继承链中的哪个类的构造或者析构函数，c++都视它们为原本的class，即不会产生多态行为。

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

# 3. 资源管理
## 条款14:在资源管理类中心小心coping行为
初始化shared_ptr时可以指定一个**删除器** 这个删除器默认时delete掉动态分配的内存\
但对于一些内存管理类，它要管理的可能时其它资源的释放，例如mutex的解锁，这个时候可以传递给shared_ptr一个**删除器** 为unlock。
```c++
class lock{
public :
	explicit Lock(Mutex* pm)
	:mutexOtr(pm,unlock)
	{
		lock(mutexPtr.get());
	}
}
`````
## 条款16
## 条款17: 以独立语句将newed对象置入智能指针。
对于资源管理类的初始化要在单独一条语句当中进行\
:no_entry_sign:例如：
```c++
//we have weo function:
int priority();
void processWidget(std:tr1::shared_ptr<Widget> pw , int priority);
//and then we use the function like that:
processWidget(std::trl::shared_ptr<Widget>(new Widget) , priority());
`````
std::trl::shared_ptr<Widget>这个传参可能会被编译器改成这样的顺序：\
1. new Widget
2. priority()
3. 最后调用tr1::shared_ptr构造函数\
:bangbang:如果priority出现异常，那么可能导致Widget的资源泄露。

# 4.设计与声明
## 条款18：让接口容易被正确使用，不易被误用

