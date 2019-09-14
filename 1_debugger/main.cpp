# include <iostream>



void leak(){
    int i = 42;
    int *ip = new int (42);

    // address on the stack
    //std::cout << "&i: " << &i <<std::endl;
    //std::cout<< "ip: " << ip << std::endl;
    //std::cout << "i: " << i << std::endl;
    //std::cout << "*ip: " << *ip << std::endl;
}

int main(void){
    for (int i = 0; i < 1000000000; i++){
        leak();
    }
}
