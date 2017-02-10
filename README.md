这是一个demo工程，需要添加到你的CCS工作区内，导入该工程即可使用。

// Release Version: test verison  
// Release Date   ：20:37 2016/11/30

#                            使用说明

	更多的使用说明参见 ./src/myPeripherals/myPeripherals_config.h
	这是工程平台的配置文件，具体使用参考文件内说明。
## 文件说明 ##
### common 文件夹:
---------------------------------------------------------------------------
// 此文件下是搭建DSP运行平台所必需的所有文件，必须保留。除非你清楚你在干什么，否则不要修改。  
// 下面逐个文件夹进行介绍   
// -header 文件夹:  
// --lib        : 库函数。禁止修改  
//  
// --peripheral : 即外设，里面包含DSP外设的各种definitions 以及实现的头文件, 宏定义是根据硬件实际情况定义的。禁止修改。  
// ---DSP2833x_Examples.h: 里面包含CPU主频的选择（分频参数）等，此外不建议修改。  
// ---DSP2833x_Device.h  : 可以更改器件型号，此外不建议修改。  
//    
// --user       : 可以修改。   
// ---my_project.h：可以将自己写的头文件#include到这里，以后在自己的工程里只用包含这一个头文件就够了。    
//  
//     
 
### -source 文件夹:  
----
// --peripherals: 顾名思义，即外设功能函数实现的定义。根据实际的配置情况**修改**初始化函数Init()。  
//  
// --cmd: 针对是否为BIOS的应用，选择相应的cmd。  
//  
//  

### TargetConfigurations 文件夹: 看英文就知道了，目标板的配置文件，下载用的。  
----
//  
