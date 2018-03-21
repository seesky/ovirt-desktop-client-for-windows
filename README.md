# ovirt-desktop-client-for-windows
a desktop client software for windows 

Description:
    这是一个ovirt的用户客户端软件，通过ovirt提供的restful api与engine进行通信。
    1.首先程序会通过restful api进行认证并获取token，保存到本地
    2.然后根据域中的权限获取对应的虚拟机列表
    3.根据获取到的虚拟机信息组装SPICE或RDP的连接脚本
    4.调用本地的RDP客户端或Virt Viewer连接虚拟机（virt-viewer的地址我是在代码里写死的哦，这个要注意，版本也是写死的，开发的时候使用的是V6.0-256）

Attention:
    我们已经在生产环境中使用了这个客户端软件，环境中大约有200个虚拟桌面的用户，总体来说软件还是很稳定的。但是有几个已知的Bug，我已知抽不出时间解决。
    (1)这个客户端软件在登陆后会一遍一遍的从ovirt engine获取虚拟机的列表以及每个虚拟机的状态，这个过程将持续5分钟（这个时间是在代码中写死的），然后程序就会退出到登陆界面，这个过程将会给engine服务器带来很大的压力。因为客户端需要不断的获取虚拟机的当前状态，如果用户多了，对api的访问量可想而知。
    (2)手动设置IP地址的功能兼容性很差呀，也没空改，唉....
    
Compliling environment:
    1.QT 5.9.0
    
Running environment：
    1.windows
    2.virt-viewer V6.0-256（正确的做法应当是把virt-viewer的exe位置选择做到配置界面里的）




# ovirt-desktop-client-for-windows
a desktop client software for windows 

Description:
    This is a client software for user to connect ovirt engine with restful api.
    1.Authentication by restful api
    2.Get VM list from ovirt engine by permission
    3.create connect script for SPICE or RDP
    4.connect vm

Attention:
    We have use this client in production environment,about 200 users,on the whole,it is stable.But it has few known bug,I have no time to fix it.
    (1)The client will request vm list again and again from ovirt engine,this process will last 5 minutes, and then,it will exit
       to  the logining interface.This will affect engine's performance.
    (2)The functional compatibility of manually setting IP addresses is poort....
    
Compliling environment:
    1.QT 5.9.0

Running environment:
    1.windows
    2.virt-viewer V6.0-256
