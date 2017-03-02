#include <iostream>
#include "color.h"
#include "test.h"

#define ECHO_LINE(x) std::cout << x << std::endl
#define ECHO_CHECK(x, y)                                                                     \
{                                                                                           \
    int rst = (x);                                                                          \
    if (rst == y)                                                                           \
        ECHO_LINE(GREEN << "Check passed, expected " << y << " and got " << rst << " at " << __FILE__ << "(" << __LINE__ << ")" << WHITE);   \
    else                                                                                    \
        ECHO_LINE(RED << "Check failed, expected " << y << " but got " << rst << " at " << __FILE__ << "(" << __LINE__ << ")" << WHITE);   \
}

void test0()
{
    ClassTest c(1);

    ECHO_LINE("验证空信号");
    {
        ECHO_CHECK(c.sig0(), 0);
        ECHO_CHECK(c.sig1(1111), 0);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 0);
        ECHO_CHECK(c.sig3(3333, "Hello world", &c), 0);
    }

    ECHO_LINE("验证正确连接");
    {
        ECHO_CHECK(c.sig0.connect(&c, &ClassTest::slot0), 1);
        ECHO_CHECK(c.sig1.connect(&c, &ClassTest::slot1), 1);
        ECHO_CHECK(c.sig2.connect(&c, &ClassTest::slot2), 1);
        ECHO_CHECK(c.sig3.connect(&c, &ClassTest::slot3), 1);

        ECHO_CHECK(c.sig0(), 1);
        ECHO_CHECK(c.sig1(1111), 1);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 1);
        ECHO_CHECK(c.sig3(3333, "Hello world", &c), 1);
    }
}

void test1()
{
    ClassTest c(1);

    ECHO_LINE("验证正确连接");
    {
        ECHO_CHECK(c.sig0.connect(&c, &ClassTest::slot0), 1);
        ECHO_CHECK(c.sig1.connect(&c, &ClassTest::slot1), 1);
        ECHO_CHECK(c.sig2.connect(&c, &ClassTest::slot2), 1);

        ECHO_CHECK(c.sig0(), 1);
        ECHO_CHECK(c.sig1(1111), 1);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 1);
    }

    ECHO_LINE("验证断开连接 1");
    {
        ECHO_CHECK(c.sig0.disconnect(&c), 1);
        ECHO_CHECK(c.sig1.disconnect(&c), 1);
        ECHO_CHECK(c.sig2.disconnect(&c), 1);

        ECHO_CHECK(c.sig0(), 0);
        ECHO_CHECK(c.sig1(1111), 0);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 0);
    }

    ECHO_LINE("验证重新连接");
    {
        ECHO_CHECK(c.sig0.connect(&c, &ClassTest::slot0), 1);
        ECHO_CHECK(c.sig1.connect(&c, &ClassTest::slot1), 1);
        ECHO_CHECK(c.sig2.connect(&c, &ClassTest::slot2), 1);

        ECHO_CHECK(c.sig0(), 1);
        ECHO_CHECK(c.sig1(1111), 1);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 1);
    }

    ECHO_LINE("验证断开连接 2");
    {
        ECHO_CHECK(c.disConnectAll(), 3);

        ECHO_CHECK(c.sig0(), 0);
        ECHO_CHECK(c.sig1(1111), 0);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 0);
    }
}

void test2()
{
    ClassTest c(1);

    ECHO_LINE("验证重复连接");
    {
        ECHO_CHECK(c.sig0.connect(&c, &ClassTest::slot0), 1);
        ECHO_CHECK(c.sig0.connect(&c, &ClassTest::slot0), 1);
        ECHO_CHECK(c.sig1.connect(&c, &ClassTest::slot1), 1);
        ECHO_CHECK(c.sig1.connect(&c, &ClassTest::slot1), 1);
        ECHO_CHECK(c.sig2.connect(&c, &ClassTest::slot2), 1);
        ECHO_CHECK(c.sig2.connect(&c, &ClassTest::slot2), 1);

        ECHO_CHECK(c.sig0(), 2);
        ECHO_CHECK(c.sig1(1111), 2);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 2);
    }

    ECHO_LINE("验证断开连接");
    {
        ECHO_CHECK(c.disConnectAll(), 6);

        ECHO_CHECK(c.sig0(), 0);
        ECHO_CHECK(c.sig1(1111), 0);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 0);
    }
}

void test3()
{
    ClassTest c(1);

    ECHO_LINE("验证SIGNAL对象提前销毁");
    {
        ClassTest s(1);
        ECHO_CHECK(s.sig0.connect(&c, &ClassTest::slot0), 1);
        ECHO_CHECK(s.sig1.connect(&c, &ClassTest::slot1), 1);
        ECHO_CHECK(s.sig2.connect(&c, &ClassTest::slot2), 1);

        ECHO_CHECK(s.sig0(), 1);
        ECHO_CHECK(s.sig1(1111), 1);
        ECHO_CHECK(s.sig2(2222, "Hello world"), 1);
    }

    ECHO_LINE("验证SLOT对象提前销毁");
    {
        ClassTest s(1);
        ECHO_CHECK(c.sig0.connect(&s, &ClassTest::slot0), 1);
        ECHO_CHECK(c.sig1.connect(&s, &ClassTest::slot1), 1);
        ECHO_CHECK(c.sig2.connect(&s, &ClassTest::slot2), 1);

        ECHO_CHECK(c.sig0(), 1);
        ECHO_CHECK(c.sig1(1111), 1);
        ECHO_CHECK(c.sig2(2222, "Hello world"), 1);
    }
}

void test4()
{
    ClassTest c(1);

    ECHO_LINE("验证非法连接参数");
    {
        // 无效OBJ
        ECHO_CHECK(c.sig0.connect((ClassTest *)NULL, &ClassTest::slot0), 0);
        ECHO_CHECK(c.sig1.connect((ClassTest *)NULL, &ClassTest::slot1), 0);

        // 无效SLOT
        ECHO_CHECK(c.sig0.connect(&c, (void (ClassTest::*)())NULL), 0);
        ECHO_CHECK(c.sig1.connect(&c, (void (ClassTest::*)(int))NULL), 0);

        ECHO_CHECK(c.sig0(), 0);
        ECHO_CHECK(c.sig1(1111), 0);
    }
}

int main(int argc, char* argcv[])
{
    (void)argc;
    (void)argcv;

    test0();
    test1();
    test2();
    test3();
    test4();
}
