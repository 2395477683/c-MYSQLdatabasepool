#include<iostream>
#include"Mysqlpool.h"

#include"connpool.h"

//测试sql语句
int query() {
	Mysqlconn conn;
	//连接数据库
	conn.connect("root", "macan344", "test", "localhost");
	//开启事务
	conn.transaction();
	//测试更新数据库
	std::string sql = "update account_2 set name='lisi' where name='zhangsan'; ";
	bool flag = conn.update(sql);
	//测试查询数据库
	std::string sql2 = "select * from account_2";
	bool flag2 = conn.query(sql2);
	while (conn.next()) {
		std::cout << conn.value(0) << "," << conn.value(1) << "," << conn.value(2) << std::endl;
	}
	std::cout << flag << std::endl;
	std::cout << flag2 << std::endl;
	//事务回滚
	conn.rollback();
	flag2 = conn.query(sql2);
	while (conn.next()) {
		std::cout << conn.value(0) << "," << conn.value(1) << "," << conn.value(2) << std::endl;
	}
	return 0;
}

void op1(int begin, int end)
{
	for (int i = begin; i < end; ++i)
	{
		Mysqlconn conn;
		conn.connect("root", "macan344", "test", "localhost");
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into account_2 values(%d, 'man', 0)", i);
		conn.update(sql);
		std::cout << i;
	}
}

void op2(Connpool* pool, int begin, int end)
{
	for (int i = begin; i < end; ++i)
	{
		std::shared_ptr<Mysqlconn> conn = pool->getconnection();
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into account_2 values(%d, 'man', 0)", i);
		conn->update(sql);
		std::cout << i << std::endl;
	}
}
void test1() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	op1(0, 5000);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto length = end - begin;
	std::cout << "非连接池,单线程，用时：" << length.count() / 1000000 << "毫秒" << std::endl;
}
void test2() {
	Connpool* pool = Connpool::getConnpool();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	op2(pool, 0, 5000);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto length = end - begin;
	std::cout << "连接池,单线程，用时：" << length.count() / 1000000 << "毫秒" << std::endl;
}
void test3() {
	Mysqlconn conn;
	conn.connect("root", "macan344", "test", "localhost");
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::thread t1(op1, 0, 1000);
	std::thread t2(op1, 1000, 2000);
	std::thread t3(op1, 2000, 3000);
	std::thread t4(op1, 3000, 4000);
	std::thread t5(op1, 4000, 5000);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto length = end - begin;
	std::cout << "非连接池,多线程，用时：" << length.count() / 1000000 << "毫秒" << std::endl;
}
void test4() {
	Connpool* pool = Connpool::getConnpool();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::thread t1(op2, pool, 0, 1000);
	std::thread t2(op2, pool, 1000, 2000);
	std::thread t3(op2, pool, 2000, 3000);
	std::thread t4(op2, pool, 3000, 4000);
	std::thread t5(op2, pool, 4000, 5000);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	auto length = end - begin;
	std::cout << "连接池,多线程，用时：" << length.count() / 1000000 << "毫秒" << std::endl;
}
int main() {
	//query();
	test4();
	return 0;
}