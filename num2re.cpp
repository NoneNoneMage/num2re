//
// Created by Mage on 2019/10/28.
//

#include "num2re.h"
#include <bitset>
#include <cmath>
#include <iostream>

const int CHECK[10] = {0x01,0x01 << 1,0x01 << 2,0x01 << 3,0x01 << 4,0x01 << 5,0x01 << 6,0x01 << 7,0x01 << 8,0x01 << 9};
const int BASE = 0x3ff;

#define OFFSET(num) (num % 10)
typedef std::list<int> NumberList;
typedef std::list<int>* pNumberList;
typedef std::pair<int,NumList*> NumPair;

class NumList{
public:
    NumList():_father(NULL),_key(0x0),_base(0){
        _nlist = new NumberList();
    }

    virtual ~NumList(){
        if (_nlist) {
            _nlist->clear();
            delete _nlist;
            _nlist = NULL;
        }
        for(std::list<NumList *>::iterator it = _childs.begin();it != _childs.end();it++){
            delete (*it);
        }
        _childs.clear();
    }

public:
    void updateKey(int offset){
        _key = _key | 0x01 << offset;
    }

    void setBase(int base){_base = base;}

    void addChild(NumList* child){
        _childs.push_back(child);
    }

    void setFather(NumList *father){_father = father;};

    int deep(){
        return _childs.empty() ? 1:  _childs.front()->deep() + 1;
    }

    std::string tab(int len){
        std::string tab;
        for(int i = 0; i< len;i++)
            tab.append("\t");
        return tab;
    }

    std::string to_string(int i = 1){
        std::string out;
        out.append("base:").append(std::to_string(_base)).append(" deep:").append(std::to_string(deep())).append(" flag:").append(std::bitset<10>(_key).to_string());
        std::list<NumList*>::iterator begin = _childs.begin();
        for(std::list<NumList*>::iterator it = begin;it!=_childs.end();it++){
            out.append("\n").append(tab(i)).append((*it)->to_string(i+1));
        }
        return out;
    }

public:
    pNumberList _nlist;
    std::list<NumList*> _childs;
    NumList * _father;
    int _key;
    int _base;
};

Num2Re::Num2Re(){

}
Num2Re::~Num2Re() {
    for(NumMapiter it = _numsMap.begin();it!=_numsMap.end();it++){
        delete it->second;
    }
    _numsMap.clear();
}
void Num2Re::addNum(int num) {
    NumMapiter lastIter;
    NumList* nList;
    int offset = OFFSET(num);
    if(_numsMap.empty())
        goto NEW;
    lastIter = _numsMap.end();
    lastIter--;
    if(num - lastIter->first > 9 || num - lastIter->first < 0)
        goto NEW;

    nList = ((NumList*) (lastIter->second));
    lastIter->second->updateKey(offset);
    goto PUSH;

    NEW:
    nList = new NumList();
    nList->updateKey(offset);
    nList->setBase(num - offset);
    //nList->_key_len = (int)log10(num) + 1;
    _numsMap.insert(NumPair(num-offset,nList));

    PUSH:
    nList->_nlist->push_back(num);
}

static bool checkBreak(NumMap map){
    if(map.size() == 1 )
        return false;
    NumMapiter begin = map.begin();
    NumMapiter end = map.end();
    int hash[256] = {0};
    for(NumMapiter it = begin;it !=end;it++){
        if(it->first == 0)
            return false;
        int key = (int)log10(it->first) + 1;
        if(hash[key] == 1)
            return true;
        hash[key] = 1;
    }
    return false;
}

static int bfsList(NumList *nlist,int* result){
    //BFS
    std::list<NumList*> tmp;
    std::list<NumList*> deep1_tmp;
    tmp.push_back(nlist);
    int ret = nlist->deep();
    while(!tmp.empty()) {
        NumList *tmpList = tmp.front();
        tmp.pop_front();
        bool push_childs = false;
        if (result[tmpList->deep() + 1] == 0) {
            result[tmpList->deep()] = tmpList->_key;
            push_childs = true;
        } else {
            if (CHECK[tmpList->_base / 10 % 10] & result[tmpList->deep() + 1]) {
                push_childs = true;
                if(tmpList->deep() == 1){
                    //记录叶子节点，遍历完后，叶子结点减去已经过滤数字；
                    deep1_tmp.push_back(tmpList);
                }
                if (result[tmpList->deep()] == 0)
                    result[tmpList->deep()] = tmpList->_key;
                else if(tmpList->_key != 0)
                    result[tmpList->deep()] = result[tmpList->deep()] & tmpList->_key;
            }
        }
        if(push_childs){
            for (std::list<NumList *>::iterator it = tmpList->_childs.begin(); it != tmpList->_childs.end(); it++) {
                tmp.push_back((*it));
            }
        }
    }
    for (std::list<NumList *>::iterator it = deep1_tmp.begin(); it != deep1_tmp.end(); it++) {
        (*it)->_key -= result[1];
        if((*it)->_key == 0){
            NumList * son = (*it);
            NumList * father = (*it)->_father;
            while (father != NULL){
                if(son->_key == 0) {
                    father->_key &= (BASE & ~CHECK[son->_base / 10 % 10]);
                    son = father;
                    father = son->_father;
                } else {
                    break;
                }
            }
        }

    }
    return ret;
}
static std::string toReString(int flag){
    std::string out="[";
    for(int i = 0;i < 10;i++){
        if(CHECK[i] & flag) {
            out.append(std::to_string(i));
#ifdef DEBUG
            std::cout << std::bitset<10>(flag).to_string() << "-" << std::bitset<10>(CHECK[i]).to_string();
#endif
            flag -= CHECK[i];
#ifdef DEBUG
            std::cout << "=" << std::bitset<10>(flag).to_string() << ";";
#endif
            if ((flag & BASE) != 0 && i != 9)
                out.append(",");
            else
                break;
        }
    }
    out.append("]");
    return out;
}
void Num2Re::prepare() {
    NumMap tmpMap;
    NumMapiter begin = _numsMap.begin();
    NumMapiter end = _numsMap.end();
    bool isBreak = true;
    if(checkBreak(_numsMap)) {
        for (NumMapiter iter = begin; iter != end; iter++) {
            int num = iter->first / 10;
            if (0 == num) {
                tmpMap.insert(NumPair(iter->first, iter->second));
                continue;
            }
            isBreak = false;
            int offset = OFFSET(num);
            int key = num - offset;
            NumMapiter iterKey = tmpMap.find(key);
            if (iterKey == tmpMap.end()) {
                NumList *nlist = new NumList();
                nlist->addChild(iter->second);
                iter->second->setFather(nlist);
                nlist->updateKey(offset);
                nlist->setBase(key);
                tmpMap.insert(NumPair(key, nlist));
            } else {
                NumList *nlist = iterKey->second;
                nlist->addChild(iter->second);
                iter->second->setFather(nlist);
                nlist->updateKey(offset);
            }
        }
    }
    if(!isBreak) {
        _numsMap.swap(tmpMap);
        prepare();
    }else {
#ifdef DEBUG
        for (NumMapiter iter = begin; iter != end; iter++) {
            std::cout << iter->second->to_string() << std::endl << std::endl;
        }
#endif
    }
}
std::list<std::string> Num2Re::toRegexs() {
    prepare();
    NumMapiter begin = _numsMap.begin();
    NumMapiter end = _numsMap.end();
    std::list<std::string> slist;
    for(NumMapiter iter = begin;iter != end;iter++){
        std::list<NumList*> tmp;

        NumList *nlist = iter->second;
        tmp.push_back(nlist);

        while(!tmp.empty()) {
            int rt[15] = {0};
            NumList *tmpList = tmp.front();
            tmp.pop_front();

            int deep = bfsList(tmpList, rt);
            if ((rt[1] & BASE) != 0) {
                int base = tmpList->_base / 10;

                std::string res = base ? std::to_string(base) : "";
                for (int i = deep; i > 0; i--) {
#ifdef DEBUG
                    std::cout << i << ":";
#endif
                    res.append(toReString(rt[i]));
#ifdef DEBUG
                    std::cout << std::bitset<10>(rt[i]).to_string() << " ";
#endif
                }
                slist.push_back(res);
#ifdef DEBUG
                std::cout << "re: " << res << std::endl;
#endif
            }

            for (std::list<NumList *>::iterator it = tmpList->_childs.begin(); it != tmpList->_childs.end(); it++) {
                tmp.push_back((*it));
            }
        }
    }
    return slist;
}
