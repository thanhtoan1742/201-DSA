#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>
#include <map>
using namespace std;

//For testing

class BusParking
{
private:
    struct coup
    {
        int start;
        int end;
        bool operator<(const coup &rhs) const
        {
            return (start < rhs.start) || (start == rhs.start && end < rhs.end);
        }
        bool operator>(const coup &rhs) const
        {
            return (start > rhs.start) || (start == rhs.start && end > rhs.end);
        }
        bool operator>=(const coup &rhs) const
        {
            return !((*this) < rhs);
        }
        bool operator<=(const coup &rhs) const
        {
            return !((*this) > rhs);
        }
        bool operator==(const coup &rhs) const
        {
            return (start == rhs.start && end == rhs.end);
        }
        bool operator!=(const coup &rhs) const
        {
            return !((*this) == rhs);
        }
        coup(int start, int end) : start(start), end(end)
        {
        }
    };
    enum class State
    {
        LH,
        EH,
        RH
    };
    struct Node
    {
        State state;
        int time;  /*Key*/
        int value; /*Bus change at this->time, value = coming - going*/
        int leftMax;
        int rightMax;
        int sumAll;
        int maxRes;
        int going;
        int coming;
        Node *left;
        Node *right;
        Node() = default;
        Node(int time, int value, Node *left = nullptr, Node *right = nullptr)
            : state(State::EH),
              time(time),
              value(value),
              leftMax((value > 0) ? value : 0),
              rightMax((value > 0) ? value : 0),
              sumAll(value),
              maxRes((value > 0) ? value : 0),
              going(0),
              coming(0),
              left(left),
              right(right)
        {
            if (value > 0)
            {
                coming = 1;
            }
            else
            {
                going = 1;
            }
        }
        void update() //Wrong all rhs operand must be max(old, new,....)
        {
            if (left && right)
            {
                this->sumAll = left->sumAll + this->value + right->sumAll;
                this->leftMax = max(left->leftMax, left->sumAll + this->value + right->leftMax);
                this->rightMax = max(right->rightMax, left->rightMax + this->value + right->sumAll);
                this->maxRes = max(max(left->rightMax + this->value + right->leftMax, right->maxRes), left->maxRes);
            }
            else if (left && !right)
            {
                this->sumAll = left->sumAll + this->value;
                this->leftMax = max(left->leftMax, left->sumAll + this->value);
                this->rightMax = max(this->value, left->rightMax + this->value);
                this->maxRes = max(left->rightMax + this->value, left->maxRes);
            }
            else if (!left && right)
            {
                this->sumAll = this->value + right->sumAll;
                this->leftMax = max(this->value, this->value + right->leftMax);
                this->rightMax = max(right->rightMax, this->value + right->sumAll);
                this->maxRes = max(right->maxRes, this->value + right->leftMax);
            }
            else
            {
                this->sumAll = this->value;
                this->leftMax = max(this->value, 0);
                this->rightMax = max(this->value, 0);
                this->maxRes = max(this->value, 0);
            }
        }
    };
    Node *pRoot = nullptr;
    map<coup, int> checkMap;
    int nE;

    //public:
    //    BusParking();
    //
    //    ~BusParking();
    //    void clear();
    //
    //    virtual void add(int start, int end);
    //    virtual void remove(int start, int end);
    //    Node *find(int time);
    //    int size()
    //    {
    //        return nE;
    //    }
    //
    //    virtual bool balanceLeft(Node *&pCur);
    //    virtual bool balanceRight(Node *&pCur);
    //    virtual void rotateLeft(Node *&pCur);
    //    virtual void rotateRight(Node *&pCur);
    //    virtual void rotateRightLeft(Node *&pCur);
    //    virtual void rotateLeftRight(Node *&pCur);
    //
    //    virtual void traverseInfix(std::function<void(int, int)> f);
    //    virtual void traversePrefix(std::function<void(int, int)> f);
    //    virtual void traversePostfix(std::function<void(int, int)> f);
    //
    //public:
    //    int minPark();
    //
    //    void clear(Node *&pCur);
    //    Node *&find(int time, Node *&pCur);
    //
    //    virtual bool add(int time, int value, Node *&pCur);
    //    virtual bool remove(int time, int value, Node *&pRoot);
    //
    //    virtual void traverseInfix(std::function<void(int, int)> f, Node *&pCur);
    //    virtual void traversePrefix(std::function<void(int, int)> f, Node *&pCur);
    //    virtual void traversePostfix(std::function<void(int, int)> f, Node *&pCur);

public:
    BusParking()
        : pRoot(nullptr), nE(0)
    {
    }

    /*------------------------------------------------------------------------------------------------*/
    ~BusParking()
    {
        clear();
    }

    int size()
    {
        return nE;
    }

    void clear()
    {
        clear(pRoot);
    }

    int minPark()
    {
        return pRoot->maxRes;
    }

    void clear(Node *&pCur)
    {
        if (pCur->left)
        {
            clear(pCur->left);
        }
        if (pCur->right)
        {
            clear(pCur->right);
        }
        delete pCur;
        pCur = nullptr;
    }

    /*------------------------------------------------------------------------------------------------*/
    void add(int start, int end)
    {
        coup in(start, end);
        auto it = checkMap.find(in);
        if (it == checkMap.end())
        {
            add(start, 1, pRoot);
            add(end, -1, pRoot);
            checkMap[in] = 0;
            nE++;
        }
    }

    bool add(int time, int value, Node *&pCur)
    {
        if (!pCur)
        {
            pCur = new Node(time, value);
            return true;
        }
        if (time == pCur->time)
        {
            pCur->value += value;
            if (value < 0)
            {
                pCur->going++;
            }
            else
            {
                pCur->coming++;
            }
            pCur->update();
            return false;
        }
        else if (time < pCur->time)
        {
            if (add(time, value, pCur->left))
            {
                bool isTaller = balanceLeft(pCur);
                if (pCur->left)
                {
                    pCur->left->update();
                }
                if (pCur->right)
                {
                    pCur->right->update();
                }
                pCur->update();
                return isTaller;
            }
            pCur->update();
            return false;
        }
        else
        {
            if (add(time, value, pCur->right))
            {
                bool isTaller = balanceRight(pCur);
                if (pCur->left)
                {
                    pCur->left->update();
                }
                if (pCur->right)
                {
                    pCur->right->update();
                }
                pCur->update();
                return isTaller;
            }
            pCur->update();
            return false;
        }
    }

    Node *find(int time)
    {
        return find(time, pRoot);
    }

    Node *&find(int time, Node *&pCur)
    {
        if (!pCur)
        {
            return pCur;
        }
        else if (pCur->time == time)
        {
            return pCur;
        }
        else if (time < pCur->time)
        {
            return find(time, pCur->left);
        }
        else
        {
            return find(time, pCur->right);
        }
    }

    void remove(int start, int end)
    {
        coup in(start, end);
        auto it = checkMap.find(in);
        if (it != checkMap.end())
        {
            remove(start, 1, pRoot);
            remove(end, -1, pRoot);
            checkMap.erase(it);
            nE--;
        }
    }

    bool remove(int time, int value, Node *&pCur)
    {
        if (!pCur)
            return false;
        if (pCur->time == time)
        {
            pCur->value -= value;
            if (value < 0)
            {
                pCur->going--;
            }
            else if (value > 0)
            {
                pCur->coming--;
            }
            if (pCur->coming == 0 && pCur->going == 0)
            {
                if (pCur->left == nullptr && pCur->right == nullptr)
                {
                    delete pCur;
                    pCur = nullptr;
                    return true;
                }
                else if (pCur->left == nullptr)
                {
                    Node *pDel = pCur;
                    pCur = pCur->right;
                    pCur->update();
                    delete pDel;
                    return true;
                }
                else if (pCur->right == nullptr)
                {
                    Node *pDel = pCur;
                    pCur = pCur->left;
                    pCur->update();
                    delete pDel;
                    return true;
                }
                else
                {
                    Node *pNew = pCur->right;
                    while (pNew->left)
                    {
                        pNew = pNew->left;
                    }
                    pCur->time = pNew->time;
                    pCur->value = pNew->value;
                    pCur->going = pNew->going;
                    pCur->coming = pNew->coming;
                    pCur->update();
                    pNew->going = 0;
                    pNew->coming = 0;
                    pNew->value = 0;
                    if (remove(pNew->time, 0, pCur->right))
                    {
                        bool isShorter = !balanceLeft(pCur);
                        if (pCur->left)
                        {
                            pCur->left->update();
                        }
                        if (pCur->right)
                        {
                            pCur->right->update();
                        }
                        pCur->update();
                        return isShorter;
                    }
                }
            }
            pCur->update();
            return false;
        }
        else if (time < pCur->time)
        {
            if (remove(time, value, pCur->left))
            {
                bool isShorter = !balanceRight(pCur);
                if (pCur->left)
                {
                    pCur->left->update();
                }
                if (pCur->right)
                {
                    pCur->right->update();
                }
                pCur->update();
                return isShorter;
            }
            pCur->update();
            return false;
        }
        else
        {
            if (remove(time, value, pCur->right))
            {
                bool isShorter = !balanceLeft(pCur);
                if (pCur->left)
                {
                    pCur->left->update();
                }
                if (pCur->right)
                {
                    pCur->right->update();
                }
                pCur->update();
                return isShorter;
            }
            pCur->update();
            return false;
        }
    }

    /*------------------------------------------------------------------------------------------------*/

    bool balanceLeft(Node *&pCur)
    {
        if (pCur->state == State::RH)
        {
            pCur->state = State::EH;
            return false;
        }
        else if (pCur->state == State::EH)
        {
            pCur->state = State::LH;
            return true;
        }
        else
        {
            if (pCur->left->state == State::LH)
            {
                rotateRight(pCur);
                pCur->state = State::EH;
                pCur->right->state = State::EH;
                return false;
            }
            else if (pCur->left->state == State::EH)
            {
                rotateRight(pCur);
                pCur->state = State::RH;
                pCur->right->state = State::LH;
                return true;
            }
            else
            {
                rotateLeftRight(pCur);
                if (pCur->state == State::EH)
                {
                    pCur->left->state = pCur->right->state = State::EH;
                }
                else if (pCur->state == State::LH)
                {
                    pCur->right->state = State::RH;
                    pCur->left->state = pCur->state = State::EH;
                }
                else
                {
                    pCur->left->state = State::LH;
                    pCur->right->state = pCur->state = State::EH;
                }
                return false;
            }
        }
    }

    bool balanceRight(Node *&pCur)
    {
        if (pCur->state == State::LH)
        {
            pCur->state = State::EH;
            return false;
        }
        else if (pCur->state == State::EH)
        {
            pCur->state = State::RH;
            return true;
        }
        else
        {
            if (pCur->right->state == State::RH)
            {
                rotateLeft(pCur);
                pCur->state = State::EH;
                pCur->left->state = State::EH;
                return false;
            }
            else if (pCur->right->state == State::EH)
            {
                rotateLeft(pCur);
                pCur->state = State::LH;
                pCur->left->state = State::RH;
                return true;
            }
            else
            {
                rotateRightLeft(pCur);
                if (pCur->state == State::EH)
                {
                    pCur->left->state = pCur->right->state = State::EH;
                }
                else if (pCur->state == State::RH)
                {
                    pCur->left->state = State::LH;
                    pCur->right->state = pCur->state = State::EH;
                }
                else
                {
                    pCur->right->state = State::RH;
                    pCur->left->state = pCur->state = State::EH;
                }
                return false;
            }
        }
    }

    void rotateLeft(Node *&pCur)
    {
        Node *pTemp = pCur;
        pCur = pCur->right;
        pTemp->right = pCur->left;
        pCur->left = pTemp;
    }

    void rotateRight(Node *&pCur)
    {
        Node *pTemp = pCur;
        pCur = pCur->left;
        pTemp->left = pCur->right;
        pCur->right = pTemp;
    }

    void rotateRightLeft(Node *&pCur)
    {
        rotateRight(pCur->right);
        rotateLeft(pCur);
    }

    void rotateLeftRight(Node *&pCur)
    {
        rotateLeft(pCur->left);
        rotateRight(pCur);
    }

    /*------------------------------------------------------------------------------------------------*/

    // void traverseInfix(function<void(int, int)> f)
    // {
    //     traverseInfix(f, pRoot);
    // }

    // void traverseInfix(function<void(int, int)> f, Node*& pCur)
    // {
    //     if (!pCur)
    //         return;
    //     traverseInfix(f, pCur->left);
    //     f(pCur->time, pCur->value);
    //     traverseInfix(f, pCur->right);
    // }

    // void traversePrefix(function<void(int, int)> f)
    // {
    //     traversePrefix(f, pRoot);
    // }

    // void traversePrefix(function<void(int, int)> f, Node*& pCur)
    // {
    //     if (!pCur)
    //         return;
    //     f(pCur->time, pCur->value);
    //     traversePrefix(f, pCur->left);
    //     traversePrefix(f, pCur->right);
    // }

    // void traversePostfix(function<void(int, int)> f)
    // {
    //     traversePostfix(f, pRoot);
    // }

    // void traversePostfix(function<void(int, int)> f, Node*& pCur)
    // {
    //     if (!pCur)
    //         return;
    //     traversePostfix(f, pCur->left);
    //     traversePostfix(f, pCur->right);
    //     f(pCur->time, pCur->value);
    // }
};

//BusParking::BusParking()
//    : pRoot(nullptr), nE(0)
//{
//}
//
///*------------------------------------------------------------------------------------------------*/
//BusParking::~BusParking()
//{
//    clear();
//}
//
//void BusParking::clear()
//{
//    clear(pRoot);
//}
//
//int BusParking::minPark()
//{
//    return pRoot->maxRes;
//}
//
//void BusParking::clear(Node *&pCur)
//{
//    if (pCur->left)
//    {
//        clear(pCur->left);
//    }
//    if (pCur->right)
//    {
//        clear(pCur->right);
//    }
//    delete pCur;
//    pCur = nullptr;
//}
//
///*------------------------------------------------------------------------------------------------*/
//void BusParking::add(int start, int end)
//{
//    coup in(start, end);
//    auto it = checkMap.find(in);
//    if (it == checkMap.end())
//    {
//        add(start, 1, pRoot);
//        add(end, -1, pRoot);
//        checkMap[in] = 0;
//        nE++;
//    }
//}
//
//bool BusParking::add(int time, int value, typename BusParking::Node *&pCur)
//{
//    if (!pCur)
//    {
//        pCur = new Node(time, value);
//        return true;
//    }
//    if (time == pCur->time)
//    {
//        pCur->value += value;
//        if (value < 0)
//        {
//            pCur->going++;
//        }
//        else
//        {
//            pCur->coming++;
//        }
//        pCur->update();
//        return false;
//    }
//    else if (time < pCur->time)
//    {
//        if (add(time, value, pCur->left))
//        {
//            bool isTaller = balanceLeft(pCur);
//            if (pCur->left)
//            {
//                pCur->left->update();
//            }
//            if (pCur->right)
//            {
//                pCur->right->update();
//            }
//            pCur->update();
//            return isTaller;
//        }
//        pCur->update();
//        return false;
//    }
//    else
//    {
//        if (add(time, value, pCur->right))
//        {
//            bool isTaller = balanceRight(pCur);
//            if (pCur->left)
//            {
//                pCur->left->update();
//            }
//            if (pCur->right)
//            {
//                pCur->right->update();
//            }
//            pCur->update();
//            return isTaller;
//        }
//        pCur->update();
//        return false;
//    }
//}
//
//typename BusParking::Node *BusParking::find(int time)
//{
//    return find(time, pRoot);
//}
//
//typename BusParking::Node *&BusParking::find(int time, typename BusParking::Node *&pCur)
//{
//    if (!pCur)
//    {
//        return pCur;
//    }
//    else if (pCur->time == time)
//    {
//        return pCur;
//    }
//    else if (time < pCur->time)
//    {
//        return find(time, pCur->left);
//    }
//    else
//    {
//        return find(time, pCur->right);
//    }
//}
//
//void BusParking::remove(int start, int end)
//{
//    coup in(start, end);
//    auto it = checkMap.find(in);
//    if (it != checkMap.end())
//    {
//        remove(start, 1, pRoot);
//        remove(end, -1, pRoot);
//        checkMap.erase(it);
//        nE--;
//    }
//}
//
//bool BusParking::remove(int time, int value, typename BusParking::Node *&pCur)
//{
//    if (!pCur)
//        return false;
//    if (pCur->time == time)
//    {
//        pCur->value -= value;
//        if (value < 0)
//        {
//            pCur->going--;
//        }
//        else if (value > 0)
//        {
//            pCur->coming--;
//        }
//        if (pCur->coming == 0 && pCur->going == 0)
//        {
//            if (pCur->left == nullptr && pCur->right == nullptr)
//            {
//                delete pCur;
//                pCur = nullptr;
//                return true;
//            }
//            else if (pCur->left == nullptr)
//            {
//                Node *pDel = pCur;
//                pCur = pCur->right;
//                pCur->update();
//                delete pDel;
//                return true;
//            }
//            else if (pCur->right == nullptr)
//            {
//                Node *pDel = pCur;
//                pCur = pCur->left;
//                pCur->update();
//                delete pDel;
//                return true;
//            }
//            else
//            {
//                Node *pNew = pCur->right;
//                while (pNew->left)
//                {
//                    pNew = pNew->left;
//                }
//                pCur->time = pNew->time;
//                pCur->value = pNew->value;
//                pCur->going = pNew->going;
//                pCur->coming = pNew->coming;
//                pCur->update();
//                pNew->going = 0;
//                pNew->coming = 0;
//                pNew->value = 0;
//                if (remove(pNew->time, 0, pCur->right))
//                {
//                    bool isShorter = !balanceLeft(pCur);
//                    if (pCur->left)
//                    {
//                        pCur->left->update();
//                    }
//                    if (pCur->right)
//                    {
//                        pCur->right->update();
//                    }
//                    pCur->update();
//                    return isShorter;
//                }
//            }
//        }
//        pCur->update();
//        return false;
//    }
//    else if (time < pCur->time)
//    {
//        if (remove(time, value, pCur->left))
//        {
//            bool isShorter = !balanceRight(pCur);
//            if (pCur->left)
//            {
//                pCur->left->update();
//            }
//            if (pCur->right)
//            {
//                pCur->right->update();
//            }
//            pCur->update();
//            return isShorter;
//        }
//        pCur->update();
//        return false;
//    }
//    else
//    {
//        if (remove(time, value, pCur->right))
//        {
//            bool isShorter = !balanceLeft(pCur);
//            if (pCur->left)
//            {
//                pCur->left->update();
//            }
//            if (pCur->right)
//            {
//                pCur->right->update();
//            }
//            pCur->update();
//            return isShorter;
//        }
//        pCur->update();
//        return false;
//    }
//}
//
///*------------------------------------------------------------------------------------------------*/
//
//bool BusParking::balanceLeft(Node *&pCur)
//{
//    if (pCur->state == State::RH)
//    {
//        pCur->state = State::EH;
//        return false;
//    }
//    else if (pCur->state == State::EH)
//    {
//        pCur->state = State::LH;
//        return true;
//    }
//    else
//    {
//        if (pCur->left->state == State::LH)
//        {
//            rotateRight(pCur);
//            pCur->state = State::EH;
//            pCur->right->state = State::EH;
//            return false;
//        }
//        else if (pCur->left->state == State::EH)
//        {
//            rotateRight(pCur);
//            pCur->state = State::RH;
//            pCur->right->state = State::LH;
//            return true;
//        }
//        else
//        {
//            rotateLeftRight(pCur);
//            if (pCur->state == State::EH)
//            {
//                pCur->left->state = pCur->right->state = State::EH;
//            }
//            else if (pCur->state == State::LH)
//            {
//                pCur->right->state = State::RH;
//                pCur->left->state = pCur->state = State::EH;
//            }
//            else
//            {
//                pCur->left->state = State::LH;
//                pCur->right->state = pCur->state = State::EH;
//            }
//            return false;
//        }
//    }
//}
//
//bool BusParking::balanceRight(Node *&pCur)
//{
//    if (pCur->state == State::LH)
//    {
//        pCur->state = State::EH;
//        return false;
//    }
//    else if (pCur->state == State::EH)
//    {
//        pCur->state = State::RH;
//        return true;
//    }
//    else
//    {
//        if (pCur->right->state == State::RH)
//        {
//            rotateLeft(pCur);
//            pCur->state = State::EH;
//            pCur->left->state = State::EH;
//            return false;
//        }
//        else if (pCur->right->state == State::EH)
//        {
//            rotateLeft(pCur);
//            pCur->state = State::LH;
//            pCur->left->state = State::RH;
//            return true;
//        }
//        else
//        {
//            rotateRightLeft(pCur);
//            if (pCur->state == State::EH)
//            {
//                pCur->left->state = pCur->right->state = State::EH;
//            }
//            else if (pCur->state == State::RH)
//            {
//                pCur->left->state = State::LH;
//                pCur->right->state = pCur->state = State::EH;
//            }
//            else
//            {
//                pCur->right->state = State::RH;
//                pCur->left->state = pCur->state = State::EH;
//            }
//            return false;
//        }
//    }
//}
//
//void BusParking::rotateLeft(Node *&pCur)
//{
//    Node *pTemp = pCur;
//    pCur = pCur->right;
//    pTemp->right = pCur->left;
//    pCur->left = pTemp;
//}
//
//void BusParking::rotateRight(Node *&pCur)
//{
//    Node *pTemp = pCur;
//    pCur = pCur->left;
//    pTemp->left = pCur->right;
//    pCur->right = pTemp;
//}
//
//void BusParking::rotateRightLeft(Node *&pCur)
//{
//    rotateRight(pCur->right);
//    rotateLeft(pCur);
//}
//
//void BusParking::rotateLeftRight(Node *&pCur)
//{
//    rotateLeft(pCur->left);
//    rotateRight(pCur);
//}
//
///*------------------------------------------------------------------------------------------------*/
//
//void BusParking::traverseInfix(function<void(int, int)> f)
//{
//    traverseInfix(f, pRoot);
//}
//
//void BusParking::traverseInfix(function<void(int, int)> f, typename BusParking::Node *&pCur)
//{
//    if (!pCur)
//        return;
//    traverseInfix(f, pCur->left);
//    f(pCur->time, pCur->value);
//    traverseInfix(f, pCur->right);
//}
//
//void BusParking::traversePrefix(function<void(int, int)> f)
//{
//    traversePrefix(f, pRoot);
//}
//
//void BusParking::traversePrefix(function<void(int, int)> f, typename BusParking::Node *&pCur)
//{
//    if (!pCur)
//        return;
//    f(pCur->time, pCur->value);
//    traversePrefix(f, pCur->left);
//    traversePrefix(f, pCur->right);
//}
//
//void BusParking::traversePostfix(function<void(int, int)> f)
//{
//    traversePostfix(f, pRoot);
//}
//
//void BusParking::traversePostfix(function<void(int, int)> f, typename BusParking::Node *&pCur)
//{
//    if (!pCur)
//        return;
//    traversePostfix(f, pCur->left);
//    traversePostfix(f, pCur->right);
//    f(pCur->time, pCur->value);
//}

void printOutput(BusParking *b)
{
    cout << "Minimum Bus Parking Plot: " << b->minPark() << "|"
         << " Number of Bus: " << b->size() << endl;
}

void TestCase()
{
    cout << "*|---------------------------------------------------------------|*" << endl;
    cout << "*|---------------------FIRST TEST CASE---------------------------|*" << endl;
    cout << "*|---------------------------------------------------------------|*" << endl;
    {
        BusParking *b = new BusParking();
        b->add(1, 5);
        printOutput(b);
        b->add(5, 8);
        printOutput(b);
        b->add(3, 5);
        printOutput(b);
        delete b;
    }

    cout << "*|---------------------------------------------------------------|*" << endl;
    cout << "*|---------------------SIMPLE TEST CASE--------------------------|*" << endl;
    cout << "*|---------------------------------------------------------------|*" << endl;
    {
        BusParking *b = new BusParking();
        b->add(2, 6);
        printOutput(b);
        b->add(8, 12);
        printOutput(b);
        b->add(8, 12);
        printOutput(b);
        b->add(4, 9);
        printOutput(b);
        b->add(0, 6);
        printOutput(b);
        b->add(5, 9);
        printOutput(b);
        b->add(5, 10);
        printOutput(b);
        b->remove(4, 9);
        printOutput(b);
        b->remove(2, 6);
        printOutput(b);
        delete b;
    }

    cout << "*|---------------------------------------------------------------|*" << endl;
    cout << "*|--------------------RANDOM SMALL TEST 1------------------------|*" << endl;
    cout << "*|---------------------------------------------------------------|*" << endl;
    {
        BusParking *b = new BusParking();
        vector<pair<int, int>> v;
        for (int i = 0; i < 25; i++)
        {
            int s = 0;
            int t = 0;
            do
            {
                s = rand() % 9;
                t = rand() % 9;
            } while (s >= t);
            int size = b->size();
            cout << "Insert: " << s << " " << t << endl;
            if (s == 0 && t == 2)
            {
                //int baca = 0;
            }
            b->add(s, t);
            printOutput(b);
            if (size < b->size())
            {
                pair<int, int> p(s, t);
                v.push_back(p);
            }
            if (b->size() % 5 == 3)
            {
                cout << "*|--------------------REMOVE PHASE-------------------------|*" << endl;
                int times = rand() % (b->size() / 2);
                while (times--)
                {
                    if (v.size() > 0)
                    {
                        int idx = rand() % v.size();
                        if (v[idx].first == 0 && v[idx].second == 1)
                        {
                            //int baca = 0;
                        }
                        b->remove(v[idx].first, v[idx].second);
                        cout << "Delete: " << v[idx].first << " " << v[idx].second << endl;
                        v.erase(v.begin() + idx);
                        printOutput(b);
                    }
                }
                cout << "*|--------------------INSERT PHASE-------------------------|*" << endl;
            }
        }
        delete b;
    }

    cout << "*|---------------------------------------------------------------|*" << endl;
    cout << "*|--------------------RANDOM LARGE TEST 1------------------------|*" << endl;
    cout << "*|---------------------------------------------------------------|*" << endl;
    {
       BusParking* b = new BusParking();
       vector<pair<int, int>> v;
       for (int i = 0; i < 5000; i++)
       {
           int s = 0; int t = 0;
           do
           {
               s = rand() % 100;
               t = rand() % 100;
           } while (s >= t);
           int size = b->size();
           cout << "Insert: " << s << " " << t << endl;
           if (s == 0 && t == 2)
           {
               //int baca = 0;
           }
           b->add(s, t);
           printOutput(b);
           if (size < b->size())
           {
               pair<int, int> p(s, t);
               v.push_back(p);
           }
           if (b->size() % 47 == 13)
           {
               cout << "*|--------------------REMOVE PHASE-------------------------|*" << endl;
               int times = rand() % (b->size() / 2);
               while (times--)
               {
                   if (v.size() > 0)
                   {
                       int idx = rand() % v.size();
                       if (v[idx].first == 0 && v[idx].second == 1)
                       {
                           //int baca = 0;
                       }
                       b->remove(v[idx].first, v[idx].second);
                       cout << "Delete: " << v[idx].first << " " << v[idx].second << endl;
                       v.erase(v.begin() + idx);
                       printOutput(b);
                   }
               }
               cout << "*|--------------------INSERT PHASE-------------------------|*" << endl;
           }
       }
       delete b;
    }
}

int main()
{
    freopen("ans.txt", "w", stdout);
    TestCase();
}