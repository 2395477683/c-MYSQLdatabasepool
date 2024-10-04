#include "Connpool.h"
#include<thread> 

Connpool* Connpool::getConnpool()
{
    static Connpool pool;
    return &pool;
}

std::shared_ptr <Mysqlconn> Connpool::getconnection()
{
    std::unique_lock<std::mutex>locker(my_mutexQ);
    while (my_connectionQueue.empty()) {
        if (std::cv_status::timeout == my_cond.wait_for(locker, std::chrono::milliseconds(my_timeout)))
        {
            if (my_connectionQueue.empty()) {
                continue;
            }
        }
    }
    std::shared_ptr<Mysqlconn>conn(my_connectionQueue.front(), [this](Mysqlconn* conns) {
        std::lock_guard<std::mutex>locker(my_mutexQ);
        conns->refreshiAlieveTime();
        my_connectionQueue.push(conns);
        });
    my_connectionQueue.pop();
    my_cond.notify_all();
    return conn;
}

Connpool::Connpool() {

    for (int i = 0; i < my_minsize; ++i) {
        Mysqlconn* conn = new Mysqlconn;
        conn->connect(my_user, my_pass, my_dbname, my_ip, my_port);
        conn->refreshiAlieveTime();
        my_connectionQueue.push(conn);
    }
    std::thread producer(&Connpool::producerconnection, this);
    std::thread recycler(&Connpool::recyclerconnection, this);
    producer.detach();
    recycler.detach();
}

Connpool::~Connpool()
{
    while (!my_connectionQueue.empty()) {
        Mysqlconn* conn = my_connectionQueue.front();
        my_connectionQueue.pop();
        delete conn;
    }
}

void Connpool::producerconnection()
{
    while (true) {
        std::unique_lock<std::mutex> locker(my_mutexQ);
        while (my_connectionQueue.size() >= my_minsize) {
            my_cond.wait(locker);
        }
        Mysqlconn* conn = new Mysqlconn;
        conn->connect(my_user, my_pass, my_dbname, my_ip, my_port);
        conn->refreshiAlieveTime();
        my_connectionQueue.push(conn);
        my_cond.notify_all();
    }
}

void Connpool::recyclerconnection()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(500));
        std::lock_guard<std::mutex> locker(my_mutexQ);
        while (my_connectionQueue.size() > my_maxsize) {
            Mysqlconn* conn = my_connectionQueue.front();
            if (conn->getAliveTime() >= my_maxIdleTime) {
                my_connectionQueue.pop();
                delete conn;
            }
            else {
                break;
            }
        }
    }
}
