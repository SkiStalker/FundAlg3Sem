#include <cstdint>
#include <iostream>
#include <vector>
#include <map>

class IWalk
{
public: 
    virtual std::size_t walk() = 0;
};

class WrongWalker : public IWalk {

};

class IBreath {
public:
    virtual void breath() = 0; 
};

class Cat: public IBreath, public IWalk
{
public:
    std::size_t walk() override {
        return 10;
    }

    void run() {
        
    }

    void breath() override {
        std::cout << "meew" << std::endl;
    }
};


class Dog: public IBreath, public IWalk
{
public:
    std::size_t walk() override {
        return 15;
    }

    void breath() override {
        std::cout << "bark" << std::endl;
    }
};


int main() {

    // IWalk* w = new IWalk();   // !!!!errors!!!!

    // WrongWalker ww{};  // !!!!errors!!!!

    std::vector<IWalk*> walkers;
    std::map<int, IBreath*> breathers; 


    walkers.push_back(new Cat());
    walkers.push_back(new Dog());

    breathers.emplace(0, new Cat());
    breathers.emplace(1, new Dog());


    int walked_dist = 0;
    for(const auto& iter: walkers) {
        walked_dist += iter->walk();
        int c = 10;
    }

    std::cout << walked_dist << std::endl;

    for (std::map<int, IBreath*>::iterator iter = breathers.begin(); iter != breathers.end(); iter++) {
        (*iter).second->breath();
    }


    return 0;
}