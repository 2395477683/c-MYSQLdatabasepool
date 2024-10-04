#pragma once
#include <iostream>
#include <mysql.h>
#include<string>
#include<chrono>
class Mysqlconn {
public:
	//初始化数据库链接
	Mysqlconn();
	//释放数据库链接
	~Mysqlconn();
	//连接数据库
	bool connect(std::string user, std::string pass, std::string dbname, std::string ip, unsigned short port = 3306);
	//更新数据库
	bool update(std::string sql);
	//查询数据库
	bool query(std::string sql);
	//遍历查询的结果集
	bool next();
	//得到结果集中的字段值
	std::string value(int index);
	//事务操作
	bool transaction();
	//提交事务
	bool commit();
	//回滚事务
	bool rollback();
	//刷新起始的空闲时间点
	void refreshiAlieveTime();
	//计算链接存活的总时长
	long long getAliveTime();
private:
	void freeResult();
	MYSQL* my_conn = nullptr;
	MYSQL_RES* my_result = nullptr;
	MYSQL_ROW my_row = nullptr;
	std::chrono::steady_clock::time_point my_alivetime;
};

