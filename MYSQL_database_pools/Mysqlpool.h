#pragma once
#include <iostream>
#include <mysql.h>
#include<string>
#include<chrono>
class Mysqlconn {
public:
	//��ʼ�����ݿ�����
	Mysqlconn();
	//�ͷ����ݿ�����
	~Mysqlconn();
	//�������ݿ�
	bool connect(std::string user, std::string pass, std::string dbname, std::string ip, unsigned short port = 3306);
	//�������ݿ�
	bool update(std::string sql);
	//��ѯ���ݿ�
	bool query(std::string sql);
	//������ѯ�Ľ����
	bool next();
	//�õ�������е��ֶ�ֵ
	std::string value(int index);
	//�������
	bool transaction();
	//�ύ����
	bool commit();
	//�ع�����
	bool rollback();
	//ˢ����ʼ�Ŀ���ʱ���
	void refreshiAlieveTime();
	//�������Ӵ�����ʱ��
	long long getAliveTime();
private:
	void freeResult();
	MYSQL* my_conn = nullptr;
	MYSQL_RES* my_result = nullptr;
	MYSQL_ROW my_row = nullptr;
	std::chrono::steady_clock::time_point my_alivetime;
};

