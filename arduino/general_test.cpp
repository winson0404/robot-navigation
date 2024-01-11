#include<iostream>

struct test_packet {
    uint16_t data[32];
    int num_data;
};



test_packet test()
{
    test_packet p2;
    p2.num_data = 3;
    p2.data[0] = 3;
    p2.data[1] = 4;
    // free(p.data)
    
    return p2;
}

int test2(int a[])
{
    std::cout << a[0] << std::endl;
    std::cout << a[1] << std::endl;
    int b[2] = {3, 4};
    
    a[0] = 3;
    a[1] = 4;
}

int main()
{
    test_packet p;
    p.num_data = 2;
    // p.data = (uint16_t *)malloc(sizeof(uint16_t) * p.num_data);
    p.data[0] = 1;
    p.data[1] = 2;
    // test(p);

    std::cout<<p.data[0]<<std::endl;

    std::cout<<p.data[1]<<std::endl;

    p = test();

    std::cout<<"num_data: "<<p.num_data<<std::endl;
    std::cout<<p.data[0]<<std::endl;
    std::cout<<p.data[1]<<std::endl;

    // int a[2] = {1,2};
    // test2(a);
    // std::cout<<a[0]<<std::endl;
    // std::cout<<a[1]<<std::endl;


    // uint8_t test =31;
    // std::cout << test%10 << std::endl;

    return 0;
}