#include<iostream>
#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string>
#include<fstream>
#include<vector>
#include<cstddef>
#include<sstream>
// int main()
// {
//     std::cout << "Content-type: text/html" << std::endl << std::endl;
//     std::cout << "<html><head><title>CGI</title></head><body><h1>Hello, CGI!</h1></body></html>";
//     return 0;
// }
int main(){
    time_t start_time = time(NULL);
    int i = 0;
    while(true)
    {
        sleep(2);
        break;
    }
    std::cout << difftime(time(NULL), start_time) << std::endl;
}