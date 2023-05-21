#include <iostream>
#include <thread>

using namespace std;

class person {
public:
    int m_age;
    person(int a) :m_age(a) {
        cout << "person 的构造函数,进程为：" << std::this_thread::get_id() << endl;
    }
    person(const person& p) :m_age(p.m_age) {
        cout << "person 的复制构造函数 " << std::this_thread::get_id() << endl;
    }
    ~person() {
        cout << "person的析构函数 " << std::this_thread::get_id() << endl;
    }

};
void myprin(const int& num, char* p[]) {
    cout << "现在的线程是： " << std::this_thread::get_id() << endl;
    cout << "传入的参数是：" << endl;
    cout << num << "  " << p << endl;
}
void print_class(person p)
{
    cout << "现在的线程是： " << std::this_thread::get_id() << endl;
    cout << "print class: " << p.m_age << endl;
}

int nomain()
{

    std::cout << "主线程: " << std::this_thread::get_id() << endl;

    int num = 1;
    //person student(num);
    thread obj(print_class, person(10));

    return 0;
}