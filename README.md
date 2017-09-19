# CD-Database

## 简介

一个CD管理系统，用于搜集唱片，记录其演奏者及乐曲。部分代码来自于《Linux程序设计》（作者Neil Matthew, Richard Stones），对其进行功能完善。
前端界面使用GTK+及gnome编写，后端使用mysql数据库存储数据


## 运行截图示例
![example](https://github.com/RaynorRaider/CD-Database/blob/master/app_example.png)

## 主要功能
* 添加唱片
* 编辑曲目
* 统计CD，唱片，及艺术家信息
* 删除唱片


### 备注
需要自行使用mysql创建可使用的用户，以及名为blpcd的database，可以使用仓库中自带的sql文件创建表及插入内容用以测试。
