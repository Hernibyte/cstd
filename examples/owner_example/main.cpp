#include <owner_ref.h>
#include <unique_ref.h>
#include <ref.h>
#include <array.h>
#include <array>
#include <iostream>

class Example
{
    public:
    Example() = default;
    explicit Example(const int i) : m_variable(i) {}
    
    void set_variable(const int i) { m_variable = i; }
    
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
    const cstd::owner_ref<int> o{5};
    const cstd::owner_ref<Example> example{10};
    
    std::cout << "=== owner example\n";
    std::cout << o.get() << "\n";
    example->foo();
    example->print_var();
    
    std::cout << "=== unique_ptr example\n";
    cstd::unique_ref<Example> unique_example{2};
    unique_example->print_var();
    
    const cstd::unique_ref<Example> unique_example2 = cstd::unique_ref<Example>::move(unique_example);
    unique_example2->print_var();
    
    {
        std::cout << "=== ref example\n";
        cstd::ref<Example> ref_example{};
        
        ref_example->print_var();
        ref_example->set_variable(10);
        std::cout << "ref_example count: " << ref_example.count() << "\n";
        
        {
            cstd::ref<Example> ref_example0 = ref_example;
            ref_example0->print_var();
            std::cout << "ref_example count: " << ref_example.count() << "\n";
            std::cout << "ref_example0 count: " << ref_example0.count() << "\n";
        }
        
        std::cout << "ref_example count: " << ref_example.count() << "\n";
        
        cstd::ref<Example> ref_example1 = ref_example;
        ref_example1->print_var();
        std::cout << "ref_example count: " << ref_example.count() << "\n";
        std::cout << "ref_example1 count: " << ref_example1.count() << "\n";
    }
    
    std::cout << "=== array example\n";
    cstd::array<Example, 2> arr{Example{1}, Example{2}};
    arr.for_each([](const Example& e) { e.print_var(); });
    
    return 0;
}