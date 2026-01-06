#include <mutex>

class some_data{
    int a;
    std::string b;
public:
    void do_something()
    {}
};

class data_wrapper {
private:
    some_data data;
    std::mutex m;
public:
    template<typename Function> // 
	requires std::is_invocable_v<Function, some_data&>
    void process_data(Function func)    {
        std::lock_guard<std::mutex> l(m);
        func(data);
    }
};

some_data* unprotected;
void malicious_function(some_data& protected_data){
    unprotected=&protected_data;
}

void nice_function(some_data& protected_data) {
    protected_data.do_something();
}

data_wrapper x;
void foo(){
    x.process_data(malicious_function);
    unprotected->do_something();
}

int main(){
    foo();
}
