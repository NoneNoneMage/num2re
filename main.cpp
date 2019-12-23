#include <iostream>
#include "num2re.h"
bool roll(int base){
    int roll = random() % base;
    return roll != 0;
}
int main() {
    std::cout << "Hello, World!" << std::endl;
    Num2Re nr;
    int begin = 60000;
    int end = 70000;
//    nr.addNum(4001);
//    nr.addNum(9001);
    int count = 0;
    for(int i=begin;i<=end;i++ ){
        if(roll(3)) {
            nr.addNum(i);
//            std::cout << i << " ";
            count++;
        }
    }

    std::list<std::string> slist= nr.toRegexs();
    for(std::list<std::string>::iterator it = slist.begin();it != slist.end();it++)
        std::cout << *it << std::endl;
    std::cout << "Hello, World! end:" << count << "re:" << slist.size() <<  std::endl;
    return 0;
}
