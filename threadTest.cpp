#include <iostream>
#include <thread>

using namespace std;

class person {
public:
    int m_age;
    person(int a) :m_age(a) {
        cout << "person �Ĺ��캯��,����Ϊ��" << std::this_thread::get_id() << endl;
    }
    person(const person& p) :m_age(p.m_age) {
        cout << "person �ĸ��ƹ��캯�� " << std::this_thread::get_id() << endl;
    }
    ~person() {
        cout << "person���������� " << std::this_thread::get_id() << endl;
    }

};
void myprin(const int& num, char* p[]) {
    cout << "���ڵ��߳��ǣ� " << std::this_thread::get_id() << endl;
    cout << "����Ĳ����ǣ�" << endl;
    cout << num << "  " << p << endl;
}
void print_class(person p)
{
    cout << "���ڵ��߳��ǣ� " << std::this_thread::get_id() << endl;
    cout << "print class: " << p.m_age << endl;
}

int nomain()
{

    std::cout << "���߳�: " << std::this_thread::get_id() << endl;

    int num = 1;
    //person student(num);
    thread obj(print_class, person(10));

    return 0;
}