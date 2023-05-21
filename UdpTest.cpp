/*测试用,与主程序无关*/

#include <iostream>

using namespace std;

class Bigbox;

class Box
{
    double width;
public:
    friend void printWidth(Box box);
    friend class BigBox;
    void setWidth(double wid);

private:
    int a = 1;

    void getbiga() {
        //Bigbox b;

    }
};

class BigBox
{
public:
    friend class Box;
    void Print(int width, Box& box)
    {
        // BigBox��Box����Ԫ�࣬������ֱ�ӷ���Box����κγ�Ա
        box.setWidth(width);
        cout << "Width of box : " << box.width << endl;
    }

    void geta() {
        Box b;
        cout << b.a;
    }

private:
    int biga = 3;
};

// ��Ա��������
void Box::setWidth(double wid)
{
    width = wid;
}

// ��ע�⣺printWidth() �����κ���ĳ�Ա����
void printWidth(Box box)
{
    /* ��Ϊ printWidth() �� Box ����Ԫ��������ֱ�ӷ��ʸ�����κγ�Ա */
    cout << "Width of box : " << box.width << endl;
}

class Another
{
    void geta() {
        Box b;
    }
};

// �����������
//int main()
//{
//    Box box;
//    BigBox big;
//
//    // ʹ�ó�Ա�������ÿ���
//    box.setWidth(10.0);
//
//    // ʹ����Ԫ�����������
//    printWidth(box);
//
//    // ʹ����Ԫ���еķ������ÿ���
//    big.Print(20, box);
//
//    big.geta();
//
//    getchar();
//    return 0;
//}