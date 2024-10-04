#pragma once
#include<queue>
#include<mutex>
#include"Mysqlpool.h"
#include<condition_variable>
class Connpool
{
public:
	static Connpool* getConnpool();
	Connpool(const Connpool& obj) = delete;
	Connpool& operator=(const Connpool& obj) = delete;
	std::shared_ptr<Mysqlconn> getconnection();
private:
	Connpool();
	~Connpool();
	void producerconnection();
	void recyclerconnection();

	std::string my_ip = "localhost";
	std::string my_user = "root";
	std::string my_pass = "macan344";
	std::string my_dbname = "test";
	unsigned short my_port = 3306;
	int my_minsize = 100;
	int my_maxsize = 1024;
	int my_timeout = 1000;
	int my_maxIdleTime = 5000;

	std::queue<Mysqlconn*> my_connectionQueue;

	std::mutex my_mutexQ;

	std::condition_variable my_cond;
};

