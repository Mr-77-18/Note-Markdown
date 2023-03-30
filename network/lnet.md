:smile:《深入理解Linux网络技术内幕》
# 第一部分：基础背景
### 第一章：简介
### 第二章：关键数据结构:star::star:
### sk_buff
一个封包就存储在这里。所有网络分层都会使用这个结构来存储其报头、有关用户数据的信息（有效荷载），以及用来协调其工作的其他内部信息。\
sk_buff结构字段类型：
* 布局\
	sk_buff管理着一个缓冲区，这个缓冲区是用来存放帧的	
* 通用
* 功能专用
	需要开启，例如防火墙，QoS之类的
* 管理函数: skb_reserve ; skb_put ; skb_push ; skb_pull
---
:smile:关于sk_buff的一些关键词:sock(套接字),对齐
### net_device
在Linux内核中每种网络设备都用这个数据结构表示，包括软硬件的配置信息。
包含的字段包括：
1. 配置
1. 统计数据
1. 设备状态
1. 列表管理
1. **流量控制** 
	每一个设备都会有一个输入输出的队列，里面都是需要发送的包,虚拟设备是没有的，它的流量控制由真实的物理设备去管理
	<font color=red>思考为什么流量控制信息是在net_device当中，而不是在sk_buff当中</font>
1. 功能专用
1. 通用
1. 函数指针\
	例如帧的发送什么的
---
:smile:关于net_device的一些关键词:流量控制，输入输出队列，虚拟设备，MTU, 链路层多播

**全局数据结构** :sk_buff由全局数据结构sk_buff_head管理，它们形成了一个双向列表\
net_device在内核中有两种组织方式：
* 由net_base指针指向，形成一个队列
* net_device由哈希表管理

### 第三章：用户空间与内核的接口
### 概论
### procfs与sysctl
### ioctl
### Netlink
---

# 第二部分:系统初始化
### 第四章：通知链
### 第五章：网络设备初始化
### 第六章：PCI层和网络接口卡
这一层涉及到的数据结构包括：
1. pci_device;(PCI设备都会有)
1. net_device;(网络设备才会有),注意，这个数据结构是在驱动程序加载并且与设备匹配上之后执行probe()函数的时候才会建立
1. pci_driver;其中包含着pci_driver_id
1. pci_driver_id;这个pci_driver_id可以表示一个设备
<font color=red>关于PCI还有一个比较重要的是数据库：总线描述符</font>，其中包含设备列表与驱动程序列表

### 第八章：设备注册和初始化
<font color=red>几个关键</font>
1. 分清楚注册/除名；开启/关闭
1. 这一章讲的内容都从probe()函数开始

关键的函数\
XXX_probe()-->alloc_ethdev(){参数包含一个函数指针：XXX_setup()}-->XXX_setup()-->register_netdev

---

# 第三部分:传输与接收
### 第九章 中断与网络驱动程序
### 第十章 帧的接受
<font color=red>在解释219页的代码</font>\
大概的感觉是这样的：中断调用（驱动）->vortex_rz(dev)->netif_rx()->软中断net_rx_action->netif_receice_skb(skb)将帧网上传\
帧会被传输到cpu的私有队列当中

### 第十一章 帧的传输
### 第十三章 协议处理函数
这一章主要涉及到的全局数据结构有：
1. ptype_all
2. ptye_base
涉及到的函数有：
1. netif_receive_skb()
1. 不同L3层协议对应的处理函数

