#pragma once
#include <iostream>
#include "sigslot.h"

class ClassTest : public ccutils::sigslot::SlotAble
{
public:
    ClassTest(int var)
        :m_var(var)
    {
    }

    virtual ~ClassTest()
    {
    }

    ccutils::sigslot::Signal0<> sig0;
    void slot0()
    {
        std::cout << this << " " <<  __FUNCTION__ 
            << " var = " << m_var 
            << std::endl;
    }

    ccutils::sigslot::Signal1<int> sig1;
    void slot1(int para1)
    {
        (void)para1;
        std::cout << this << " " <<  __FUNCTION__ 
            << " var = " << m_var 
            << ", para1 = " << para1
            << std::endl;
    }

    ccutils::sigslot::Signal2<int, const std::string &> sig2;
    void slot2(int para1, const std::string &para2)
    {
        (void)para1;
        (void)para2;
        std::cout << this << " " <<  __FUNCTION__ 
            << " var = " << m_var 
            << ", para1 = " << para1
            << ", para2 = " << para2
            << std::endl;
    }

    ccutils::sigslot::Signal3<int, const std::string &, void *> sig3;
    void slot3(int para1, const std::string &para2, void *para3)
    {
        (void)para1;
        (void)para2;
        (void)para3;
        std::cout << this << " " <<  __FUNCTION__ 
            << " var = " << m_var 
            << ", para1 = " << para1
            << ", para2 = " << para2
            << ", para3 = " << para3
            << std::endl;
    }

private:
    int m_var;
};
