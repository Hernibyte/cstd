#include <owner.h>
#include <unique_ptr.h>
#include <iostream>

class Example
{
    public:
    Example() = default;
    explicit Example(const int i) : m_variable(i) {}
    
    void foo() const
    {
        std::cout << "foo()\n";
    }
    
    void print_var() const
    {
        std::cout << m_variable << "\n";
    }
    
private:
    int m_variable;
    
};

int main()
{
    const cstd::owner<int> o{5};
    const cstd::owner<Example> example{10};
    
    //std::cout << o.get() << "\n";
    //example->foo();
    //example->print_var();
    
    cstd::unique_ptr<Example> unique_example{2};
    unique_example->print_var();
    
    const cstd::unique_ptr<Example> unique_example2 = cstd::unique_ptr<Example>::move(unique_example);
    unique_example2->print_var();
    
    return 0;
}