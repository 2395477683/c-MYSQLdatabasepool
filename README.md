# MYSQLdatabasepool

## 参考链接
https://www.bilibili.com/video/BV1Fr4y1s7w4/?spm_id_from=333.337.search-card.all.click

## 环境配置
本人使用的数据库为MYSQL数据库的8.0.36版本，使用的编程软件为Visual Studio Community 2022版本，使用windows系统，使用c++编码.

## 配置环境
下载代码直接解压即可在Visual Studio Community 2022上运行，但需要更改数据库的连接，该项目未使用json所以需要更改Connpool.h文件中的
  
     std::string my_ip = "localhost";
     std::string my_user = "root";
     std::string my_pass = "macan344";
     std::string my_dbname = "test";
    ip为你的主机地址，如果使用localhost即为本机地址
    user为你的数据库用户账号
    pass为你的数据库用户密码
    dbname为你的数据库使用的数据库名
**不要忘记更改main中测试函数中连接数据库函数的参数

## 疑问
如不能运行或报错请在议题中提出！

## 结束
如果有用 请给个star吧 ~~QVQ. 
