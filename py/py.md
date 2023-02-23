## 数据类型
字符串；整数；浮点数；
```python
#字符串操作
name = " liu sen hong "
name.title()
name.upper()
name.lower()
name.rstrip()
name.lstrip()
number = 12
new_name = name + str(number) 
`````

## 列表
基本操作
```python
list = ["name" , "12" , "diwjid"]
#访问
list(0)
list(-1)#返回最后一个元素
#插入
list.insert(0 , "liusen")
list.append("sss")#尾部追加
#删除
del list[1]
list.pop(2)
list.remove("12")
#排序
list.sort()
list.sort(reverse = True)
list.reverse()
```
高级操作
```python
#访问元素
for value in list:
	print(value)

#访问一定范围内的元素
for value in list[1:2]:
	print(value)

#列表复制
list_1 = list#这种方式下list_1关联到list,有点像c++当中的引用
list_1 = list[:]#这种方式下list_1和list各自独立

#求数值列表的最大；最小;和
max(list)
min(list)
sum(list)

#列表解析
list_2 = [value for value in list[1:2]]
`````

## 元组
除了元素不能改变之外，跟列表差不多

## 字典
```python
alien = {"color" : "red" , "age" : 12}
print(alien["color"])
alien["name"] = "liu"
`````

## 函数
```python
def function(形参):
	...
`````


## 类
```python
class Class_name():
	def __init__(self , 其它参数)：
		...
`````
## 文件操作
1. 打开文件：善于利用关键词with
```python
with open("文件路径") as file_open:
	...#file_open只有在with里面能够使用

#想要在其它地方使用，需要加文件内容读入
with open("1.txt") as file_open:
	line = file_open.readlines()
`````

2. 文件的读写：
```python
#读操作
with open("1.txt") as file_open:
	read_txt = file_open.read()

with open("1.txt") as file_open:
	line = file_open.readlines()

#写操作
with open("1.txt") as file_open:
	file_open.write("liusenhong")

#以追加的方式写
with open("1.txt" , 'a') as file_open:
	file_open.write("jdiwojdi")
`````

3. json格式用到的函数:
	1. json.dump()
	2. json.load()
```python
import json

with open("1.txt" , 'w') as file_open
	json.dump("dwjidjwi" , file_open)
	#load_txt = json.load(file_open)

`````

<++>
