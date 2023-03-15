https://blog.csdn.net/m0_72813904/article/details/128228191?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-128228191-blog-97016534.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-128228191-blog-97016534.pc_relevant_default&utm_relevant_index=2





虚拟机环境配置：（ubuntu20.04）编译鸿蒙3.2beta4

1. 镜像源配置：
   1. 查看配置源：
   2. 设置最新配置源：
2. 安装工具：
   1. apt install build-essential
3. /mnt下没有共享文件夹：
   1. sudo apt-get install open-vm-tools（似乎还可以改变窗口）
   2. [mkdir](https://so.csdn.net/so/search?q=mkdir&spm=1001.2101.3001.7020) /mnt/hgfs
   3. vmhgfs-fuse .host:/ /mnt/hgfs
4. securecrt连接：
   1. 安装sshd，查看端口开放情况：netstat -aptn



