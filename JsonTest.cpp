#include <iostream>
#include <fstream>
#include <streambuf>
#include "json\JsonFile2.h"

using namespace std;

int main(int argc, char** argv)
{

    dw1::JsonFile2 jsonFile;
    jsonFile.Parse("Game.json");
    jsonFile.Print();
    
    std::cout << "\n -------------------" << std::endl;

    dw1::jsonKeyValue* kv = jsonFile.Find("tileCount");
    if (kv) kv->Print(2);
    else 
        std::cout << "\n실패!!" << std::endl;

}