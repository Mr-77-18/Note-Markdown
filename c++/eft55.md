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
`区分` 
```c++
const char* p;//char const* p
char* const p;
const char* const p;
`````
:star:const最具威力的用法是在函数声明时。
1. const作用于函数返回参数或者参数上都可以避免粗心用户导致的错误。
2. const作用于函数(**只能修饰成员函数** )本身`注意` 
```c++
class A{
	...
	void function(int length)const{...}//length并没有被修饰成const
	...
}
`````
对于const修饰的成员函数有一个好处如下：
```c++
class A{
	public:
		//错误的
		int& function()const{
			return length;
		}
		//正确的
		const int& function()const{
			return length;
		}
	private:
		int length;
}
`````

`解释` 即限制了返回要const（如果返回的是对象的成员的话）\
:bangbang:对函数返回值是内置类型(不是引用哦)进行赋值是不合法的，但是对返回值是自定义的类确实合法的。(**只是合法，但这样做没有什么意义** )\
:fast_forward:应用：
```c++
class TextBlock{
	public:
		...
		//有两个版本1：一个const；2：一个non-const
		//但这出现一个问题：即non-const和const中代码重复
		const char& operator[](std::size_t position)const
		{
			...
			...
			return text[position];
		}
		char& operator[](std:size_t position)
		{
			...
			...
			return text[position];
		}
}
`````

`解决方法` :将常量性转除（casting away constness）

```c++
//这里只显示non-const []的改动
char& operator[](std::size_t position){
	return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
}
`````



## 条款4：确定对象被使用前已被初始化
1. 区分初始化和赋值
```c++
class A{
	A(...)
	:member initialization list//这里才是初始化{
		...//这里是赋值了。没有想到吧
	}
}
`````

2. 内置类型的初始化和赋值的开销是一样的。内置类型的初始化必须**手工进行** 

3. c++有着**十分固定** 的成员初始化次序:smile:,class的成员变量总是以声明的次序被初始化。（初值列当中的次序不重要，这点需要**注意哦** :bangbang:\
4. non-local static **and**
local static:fast_forward:**请以local static 对象替换non-local static对象** 
```c++
void function(void){
	static ...//this is local static
}
class A{
	static ...//this is local static
};
static ...//this is local static
`````

:bangbang:编译单元:bangbang::指产出单一目标文件的源码\
对于同一个编译单元当中的non-local static member的初始化顺序c++没有进行保证。**解决办法** :fast_forward:
```c++
static ...//non-local static member
	|
	|
	|变成
void function(){
	static ...//local static member
}
`````

:bangbang:这里利用的规则是：对于local static对象会在"该函数被调用期间""首次遇上该对象之定义式"时被初始化。（:smile:这样就可以控制non-local static member的初始化咯:bangbang:）



# 2.构造/析构/赋值运算

## 条款5: 了解c++默默编写并调用哪些函数
要理解以下几个问题
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
讨论的例子：
```c++
class TimeKeeper{
public:
	TimeKeeper();
	~TimeKeeper();
	...
};
class AtomicClock : public TimeKeeper{...};
class WaterClock : public TimeKeeper{...};
class WristWatch : public TiemKeeper{...};

//当以上述为基础，进行以下操作时，将会发生未定义行为
TimeKeeper* ptk = getTimeKeeper();
...
delete ptk;//你不知道delete的是哪一个哦
`````


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

## 条款13：以对象管理资源


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
## 条款15: 在资源管理类中提供对原始资源的访问

## 条款16: 成对使用new和delete是要采用相同形式

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
std::trl::shared_ptr<Widget>这个传参可能会被编译器改成这样的顺序：
1. new Widget
2. priority()
3. 最后调用tr1::shared_ptr构造函数\
:bangbang:如果priority出现异常，那么可能导致Widget的资源泄露。

# 4.设计与声明
<center>

舞台：让接口容易被正确使用，不容易被误用。\
舞台上有：正确性，高效性，封装性，维护性，延展性，协议一致性
</center>

## 条款18：让接口容易被正确使用，不易被误用
:question:什么是DLL问题。shared_ptr又是怎么解决这个问题的？

## 条款19: 设计class犹如设计type
:star:这里提到的条款有（方便总结）：4,6,7,13,15,23,24,29,34,36,46

## 条款20：宁以pass-by-reference-to-const替换pass-by-value
一个合理的假设是：“pass-by-value并不昂贵”的唯一对象就是内置类型和STL的迭代器和函数对象。\
:question:为什么STL...也是呢？
`个人理解` :因为STL的设计者（设计class时）应该考虑了pass-by-value的性能问题。设计的比较好。


## 条款21：必须返回对象时，别妄想返回其reference
:bangbang:提示我们：不是return-by-reference就是好的\
:star:我觉得这个条款应该说返回"新对象时"\
在一个函数中，如果要返回新对象，这个新对象的创建方式有：
1. 在stack中
2. 在heap中(return-by-reference的话可能会造成内存泄露，即用户不会去delete)
3. 在local static当中\
不过以上三种都不是你return-by-reference的好理由。请以return-by-value的方式返回新对象

## 条款22：将成员变量声明为private:
## 条款23：宁以non-member , non-friend替换member函数
<font color=red>成员函数的数量越少，类的弹性就越大</font>\
non-member,non-friends函数的一个好处是可以分布在不同的头文件当中但却利用同一个命名空间这个技术使得可以操纵其它头文件当中的类的方法。标准库就是这样的。
## 条款24： 若所有参数都要类型转换，请为此采用non-member函数
<font color=red>只有当参数被列为参数列内，这个参数才是隐式类型转换的合格参与者</font>**我认为这也是本条款提出来的原因** 
## 条款25：考虑写出一个不抛出异常的swap函数

# 5.实现
**太快定义变量，过度使用转型，返回对象内部数据handles，未考虑异常，过度使用inlining,过度耦合** 都不可取
## 条款26：尽可能延后变量定义式的出现时间
```c++
void function(){
	std::string st;
	if(...)
		throw(...)
	//do something for st;
	st = ...
}
//我们应该延后st的定义
void function(){
	if(...)]
		throw(...)
		std::string st;
		//都 something for st;
		st = ...
}
`````

以上代码其实还有一个可优化的就是，变量声明应该延后到确定要使用它的时候，甚至延后到能定义它的时候，即不是像上面一样先调用default构造函数,而是**声明即定义** 
```c++
void function(){
	if(...)
		throw(...)
	std::string st(...);//直接给出初值
}
`````



## 条款27: 尽量少做转型动作
`对于向下转型` :目标类型只能是指针和引用（尚未验证正确性，不过大概率是对的）的理解:我认为根本原因是不能通过base class 构造 derived class的原因(base class拥有的东西少啊)\
`对于向上转型` 则没有上面的限制

## 条款28：避免返回handles指向对象内部成分
## 条款29：为“异常安全”而努力是值得的
**异常安全包括两个条件** 
1. 不泄露任何资源:fast_forward:以对象管理资源
2. 不允许数据败坏\
:bangbang:一个错误的例子如下：
```c++
void function(...){
	lock(&mutex);
	delete bgImage;
	++imageChanges;
	bgImage = new Image(imgSrc);
	unlock(&mutex);
}
`````

当new出现错误的时候，以上两个问题都会出现

**异常安全函数的保证包括** 
1. 基本保证
2. 强烈保证(执行这种函数，只有可能导致两种状态：失败:fast_forward:原来的状态；成功:fast_forward:成功的状态)
3. 不抛出异常

声明是怎么样的并不能影响函数本身具备怎样的异常安全性。
:fast_forward:void function()nothrow;即使你写着nothrow,并不是就不会抛出异常，一切都要看你怎么写这个函数的。
## 条款30：透彻了解inlining的里里外外
1. 对于编译器想要inline的函数，还是有可能产生函数实体，比如通过函数指针去调用这个函数。
2. 构造和析构函数虽然是在class内被隐式inline，但编译器依然可能产生outline的函数本体，因为编译器可能通过函数指针去调用这些构造和析构函数。**即用函数指针去调用函数的并不止程序员，编译器也会这样干（当有这个需要的时候）** 
3. inline和template一样，需要完整的放在头文件当中，因为如果它们被调用，需要inline(inline函数)或者实例化的话（template class）,你需要提供完成的定义。
4. inline函数将不能被调试器跟踪，所以这个点通常是你考虑的地方。为了便于调试，可能你要放弃inline

## 条款31：将文件间的编译依存关系降至最低
前置声明，接口实现类中含着细目实现类的指针（pImpl*）
# 6.继承与面向对象设计
## 条款32：确定你的public继承塑膜出is-a关系
## 条款33：避免遮掩继承而来的名称
对于public继承：是is-a的关系，所以在derived class中不能覆盖掉base class的函数。\
为了将被遮盖的名称再见天日，可以使用using声明式或转交函数（forwarding functions）:star:它们的区别是using将同名的所有函数重见天日，不管参数什么的，而转交函数则可以选择性的开放。这在private当中很有用
## 条款34: 区分接口继承和实现继承
不同的写法有不同的暗示：**我们以base class中的内容讲解** 
```c++
class Shape{
public:
	virtual void draw() const = 0;//暗示derived class继承接口
	virtual void error()(const std::string& msg);//暗示derived class继承接口额和缺省实现
	int objectID() const;//暗示derived class继承接口和一份强制性实。这里可以延申一下：条款36：绝不重新定义继承而来的non-virtual函数
	...
};
`````
对于non-pure virtual function会有问题就是：derived class忘记去实现接口从而出现一些问题（如文中所提到的飞机的不同飞发问题）:fast_forward:解决方法是可以把它定义成pure virtual function，并且给一个缺省实现（虽然pure virtual function一般不提供实现）


## 条款35: 考虑virtual函数以外的其他选择 
场景：不同的人物可能有着不同的方式计算它们的健康指数)\
最常用的方法那当然是在base class当中设置一个virtual函数，:star::star:那么有什么其它方法吗？
1. Non-virtual Interface(NVI)方法
"何时调用virtual函数的权力保存在class当中" ; derived class保存定义virtual函数的权力
2. 函数指针
3. tr1::function(有点类似于泛化指针)完成的Strategy模式,其中还提到一个std::tr1::bind()的用法
4. 古典的Strategy模式


## 条款36: 绝不重新定义继承而来的non-virtual函数 
条款7是一个本条款的一个特里，都是在说不要重新定义继承而来的non-virtual函数

## 条款37：绝不重新定义继承而来的缺省参数值
产生这个条款的原因是：对于virtual函数，缺省参数值是静态绑定的。所以你在derived class中重新设置缺省参数值是没有意义的。解决方法是使用替换策略（条款35有很多关于virtual函数的替换策略:如NVI技术）
## 条款38：通过复合塑膜出has-a或“根据某物实现出”
:star::star:教会我们怎么区分is-a , has-a , is-implemented-in-terms-of(根据某物实现出)
## 条款39：明智而审慎地使用private继承
private并不表示is-a,has-a的关系，它并不描述两个类之间的关系，它只是解决问题的一种实现上的技术。是is-implemented-in-terms-of。我们要明智的使用，在应该使用的收使用。\
:fast_forward:考虑这样一个例子：一个class A想要借助现有class B的实现,一个可以的方法是借用条款38当中说到的复合之（is-implemented-in-terms-of）的方法。但是如果想要借助的函数是clas B当中的protected函数，以这种方法是无效的，因为class A访问不了。修然以public的方式继承的话可以访问这个protected成员函数，但由于class A和class B之间不是has-a的关系，所以不能采用这种方式。**最终** 便采用private继承的方法去实现is-implemeted-in-terms-of
```c++
class B{//现有的实现
...
protected:
	void function(){...}
}
//想要借助class B实现class A一个方法是：复合
class A{
	B b;//但是以这种方式访问不了function()
	...
}
//采用另外一种方式：
class A : private B{
...//这样就可以使用class B当中的function()函数了
}
`````
关于继承当中的关系:
![Image](https://github.com/Mr-77-18/Note-Markdown/tree/master/Pic/2.png) 


## 条款40：明智而审慎地使用多重继承



