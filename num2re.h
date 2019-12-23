#ifndef LIBNUM2RE_LIBRARY_H
#define LIBNUM2RE_LIBRARY_H

//
// Created by Mage on 2019/10/28.
//

#include <map>
#include <string>
#include <list>


class NumList;
typedef std::map<int, NumList*> NumMap;
typedef std::map<int, NumList*>::iterator NumMapiter;

class Num2Re {
public:
    Num2Re();
    virtual ~Num2Re();
public:
    void addNum(int num);
    std::list<std::string> toRegexs();

private:
    void prepare();
private:
    NumMap _numsMap;
};

#endif //LIBNUM2RE_LIBRARY_H