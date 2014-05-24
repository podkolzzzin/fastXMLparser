#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <string>

#include "xml.cpp"
using namespace std;
string getCode()
{
    ifstream file("level1id1.xml");
    string rez,temp;
    while(!file.fail())
    {
        file>>temp;
        if(!file.fail())
            rez+=temp+" ";
    }
    return rez;
}
int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    string rez=getCode();
    int start=clock();
    XMLDocument a("<o><html>1<p>2</p>3</html></o>");
    cout<<a.getHead()->toString();
    int end=clock();
    cout<<end-start<<"ms"<<endl; 
}