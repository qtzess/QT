#include <string>


using CST = std::string;
class demo_in
{
private:
    /* data */
    CST name;
    
public:
    demo_in(CST name);
  

    ~demo_in();
};

demo_in::demo_in(CST name)
{
    this->name = name;
}

demo_in::~demo_in()
{
}

class demo:public demo_in
{

    demo(CST name);


};

demo::demo(CST name):demo_in(name)
{
    
}



int main()
{

}
