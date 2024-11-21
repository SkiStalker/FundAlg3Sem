#include <iostream>

using namespace std;

class IRunnable
{
public:
    virtual void run(int m) = 0;
};

class IWalkable
{
public:
    virtual void walk(int m) = 0;
};

class Cat : public IRunnable, public IWalkable
{
private:
    int age;
    int weight;
    bool gender;
    char color[25];

public:
    void run(int m) override
    {
        cout << "Cat running away " << m << " meters" << endl;
    }

    void walk(int m) override
    {
        cout << "Cat walking away " << m << " meters" << endl;
    }
};
