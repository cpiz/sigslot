/*
* 基于C++模板的signal-slot实现
* 只有一个头文件
*                   by caijw 2014/10/07
*
* 注意：非线程安全
*/

#pragma once

#include <list>
#include <set>

namespace ccutils
{
    namespace sigslot
    {
        class SignalBase;

        // 支持Slot的基类
        class SlotAble
        {
        public:
            virtual ~SlotAble();                        // 销毁SLOT类时，通知已关联的信号断开
            unsigned disConnectAll();                   // 通知已关联的信号断开，返回断开的连接数

            // 以下函数仅允许内部使用
            void connectSignal(SignalBase *signal);     // 连接信号时保存信号指针，用于销毁时通知信号断开
            void disconnectSignal(SignalBase *signal);  // 信号主动断开或信号销毁时，清除保存的信息

        private:
            typedef std::set<SignalBase *> SignalSet;
            SignalSet m_signals;
        };

        class ConnectionBase
        {
        protected:
            ConnectionBase(SlotAble *obj)
                : m_obj(obj)
            {
            }

        public:
            SlotAble* obj() const
            {
                return m_obj;
            }

        private:
            SlotAble *m_obj;
        };

        class SignalBase
        {
        protected:
            SignalBase()
            {
            }

            virtual ~SignalBase()
            {
                disConnectAll();    // 信号销毁时断开所有连接
            }

            void addConnection(ConnectionBase *conn)
            {
                m_connections.push_back(conn);
            }

        public:
            // 断开与SLOT对象的连接，并返回断开的连接数
            unsigned disconnect(SlotAble *obj)
            {
                unsigned retVal = 0;
                for (ConnectionList::iterator it = m_connections.begin();
                    it != m_connections.end();
                    )
                {
                    if ((*it)->obj() == obj)
                    {
                        obj->disconnectSignal(this);
                        it = m_connections.erase(it);
                        ++retVal;
                    }
                    else
                    {
                        ++it;
                    }
                }

                return retVal;
            }

            // 断开与所有SLOT对象的连接，并返回断开的连接数
            unsigned disConnectAll()
            {
                unsigned retVal = 0;
                for (ConnectionList::const_iterator it = m_connections.begin();
                    it != m_connections.end();
                    ++it)
                {
                    (*it)->obj()->disconnectSignal(this);
                    ++retVal;
                }
                m_connections.clear();

                return retVal;
            }

        protected:
            typedef std::list<ConnectionBase*> ConnectionList;
            ConnectionList m_connections;
        };

        SlotAble::~SlotAble()
        {
            disConnectAll();
        }

        // 断开与所有信号的连接
        unsigned SlotAble::disConnectAll()
        {
            unsigned retVal = 0;
            for (SignalSet::iterator it = m_signals.begin();
                it != m_signals.end();
                )
            {
                SignalBase *signal = *it;
                m_signals.erase(it++);
                retVal += signal->disconnect(this);
            }

            return retVal;
        }

        void SlotAble::connectSignal(SignalBase *signal)
        {
            m_signals.insert(signal);
        }

        void SlotAble::disconnectSignal(SignalBase *signal)
        {
            m_signals.erase(signal);
        }

        // 以下是Signal的实现，为了方便拓展参数数目，定义了一个宏
#define DEFINE_SIGNAL(paraCount, comma, typenameParatypes, paratypes, paratypeAndParas, paras)  \
    template <typenameParatypes>                                                                \
    class Connection##paraCount##Base : public ConnectionBase                                   \
    {                                                                                           \
    public:                                                                                     \
        Connection##paraCount##Base(SlotAble *obj)                                              \
            : ConnectionBase(obj)                                                               \
                {                                                                               \
                }                                                                               \
                                                                                                \
        virtual void callSlot(paratypes) = 0;                                                   \
    };                                                                                          \
                                                                                                \
    template <class slotcls, typenameParatypes>                                                 \
    class Connection##paraCount: public Connection##paraCount##Base<paratypes>                  \
    {                                                                                           \
    public:                                                                                     \
        typedef void (slotcls::*slot)(paratypes);                                               \
                                                                                                \
        Connection##paraCount(slotcls *o, slot s)                                               \
            : Connection##paraCount##Base<paratypes>(o)                                         \
            , m_slot(s)                                                                         \
        {                                                                                       \
        }                                                                                       \
                                                                                                \
        void callSlot(paratypeAndParas)                                                         \
        {                                                                                       \
            ((static_cast<slotcls *>(this->obj()))->*m_slot)(paras);                            \
        }                                                                                       \
                                                                                                \
    private:                                                                                    \
        slot m_slot;                                                                            \
    };                                                                                          \
                                                                                                \
    template <typenameParatypes>                                                                \
    class Signal##paraCount: public SignalBase                                                  \
    {                                                                                           \
    public:                                                                                     \
        /* emit 动作 */                                                                         \
        unsigned operator()(paratypeAndParas)                                                   \
        {                                                                                       \
            unsigned retVal = 0;                                                                \
                                                                                                \
            for (SignalBase::ConnectionList::const_iterator it = m_connections.begin();         \
                it != m_connections.end();                                                      \
                ++it)                                                                           \
            {                                                                                   \
                (static_cast<Connection##paraCount##Base<paratypes> *>(*it))->callSlot(paras);  \
                retVal++;                                                                       \
            }                                                                                   \
                                                                                                \
            return retVal;                                                                      \
        }                                                                                       \
                                                                                                \
        /* 将信号与目标相连接 */                                                                \
        template <class objcls>                                                                 \
        bool connect(objcls *obj, void (objcls::*slt)(paratypes))                               \
        {                                                                                       \
            if (!obj || !slt) return false;                                                     \
                                                                                                \
            addConnection(new Connection##paraCount<objcls comma paratypes>(obj, slt));         \
            obj->connectSignal(this);                                                           \
                                                                                                \
            return true;                                                                        \
        }                                                                                       \
    };                                                                                          \

        // 定义支持 0 ~ n 个参数的Signal
#define _ ,
        DEFINE_SIGNAL(0, , typename paratype1 = void, , , )
            DEFINE_SIGNAL(1, _, typename paratype1, paratype1, paratype1 p1, p1)
            DEFINE_SIGNAL(2, _, typename paratype1 _ typename paratype2, paratype1 _ paratype2, paratype1 p1 _ paratype2 p2, p1 _ p2)
            DEFINE_SIGNAL(3, _, typename paratype1 _ typename paratype2 _ typename paratype3, paratype1 _ paratype2 _ paratype3, paratype1 p1 _ paratype2 p2 _ paratype3 p3, p1 _ p2 _ p3)
            DEFINE_SIGNAL(4, _, typename paratype1 _ typename paratype2 _ typename paratype3 _ typename paratype4, paratype1 _ paratype2 _ paratype3 _ paratype4, paratype1 p1 _ paratype2 p2 _ paratype3 p3 _ paratype4 p4, p1 _ p2 _ p3 _ p4)
            DEFINE_SIGNAL(5, _, typename paratype1 _ typename paratype2 _ typename paratype3 _ typename paratype4 _ typename paratype5, paratype1 _ paratype2 _ paratype3 _ paratype4 _ paratype5, paratype1 p1 _ paratype2 p2 _ paratype3 p3 _ paratype4 p4 _ paratype4 p5, p1 _ p2 _ p3 _ p4 _ p5)
            DEFINE_SIGNAL(6, _, typename paratype1 _ typename paratype2 _ typename paratype3 _ typename paratype4 _ typename paratype5 _ typename paratype6, paratype1 _ paratype2 _ paratype3 _ paratype4 _ paratype5 _ paratype6, paratype1 p1 _ paratype2 p2 _ paratype3 p3 _ paratype4 p4 _ paratype4 p5 _ paratype4 p6, p1 _ p2 _ p3 _ p4 _ p5 _ p6)
#undef _
#undef DEFINE_SIGNAL

    };
};
