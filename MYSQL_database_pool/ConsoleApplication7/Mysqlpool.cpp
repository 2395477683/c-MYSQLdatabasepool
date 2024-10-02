#include<iostream>
#include "Mysqlpool.h"

Mysqlconn::Mysqlconn()
{
	my_conn = mysql_init(nullptr);
	mysql_set_character_set(my_conn, "utf8");
}

Mysqlconn::~Mysqlconn()
{
	if (my_conn != nullptr) {
		mysql_close(my_conn);
	}
	freeResult();
}

bool Mysqlconn::connect(std::string user, std::string pass, std::string dbname, std::string ip, unsigned short port)
{
	MYSQL* ptr=mysql_real_connect(my_conn,ip.c_str(),user.c_str(),pass.c_str(),dbname.c_str(),port,nullptr,0);
	return ptr!=nullptr;
}

bool Mysqlconn::update(std::string sql)
{
	if (mysql_query(my_conn, sql.c_str())) {
		return false;
	}
	return true;
}

bool Mysqlconn::query(std::string sql){

	freeResult();
	if (mysql_query(my_conn, sql.c_str())) {
	return false;
	}
	my_result = mysql_store_result(my_conn);
	return true;
}

bool Mysqlconn::next()
{
	if (my_result != nullptr) {
		my_row=mysql_fetch_row(my_result);
		if (my_row != nullptr) {
			return true;
		}
	}
	return false;
}

std::string Mysqlconn::value(int index)
{
	int zicount = mysql_num_fields(my_result);
	if (index >=zicount || index < 0) {
		return std::string();
	}

		char* val = my_row[index];

	unsigned long length = mysql_fetch_lengths(my_result)[index];

	return std::string(val,length);
}

bool Mysqlconn::transaction()
{
	return mysql_autocommit(my_conn,false);
}

bool Mysqlconn::commit()
{
	return mysql_commit(my_conn);
}

bool Mysqlconn::rollback()
{
	return mysql_rollback(my_conn);
}

void Mysqlconn::refreshiAlieveTime()
{
	my_alivetime = std::chrono::steady_clock::now();
}

long long Mysqlconn::getAliveTime()
{
	std::chrono::nanoseconds res = std::chrono::steady_clock::now() - my_alivetime;
	std::chrono::milliseconds millsec = std::chrono::duration_cast<std::chrono::milliseconds>(res);
	return millsec.count();
}

void Mysqlconn::freeResult()
{
	if (my_result) {
		mysql_free_result(my_result);
		my_result = nullptr;
	}
}
