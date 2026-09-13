#include <iostream>
#include <fstream>
#include <streambuf>
#include "json\JsonFile2.h"

using namespace std;

int main(int argc, char** argv)
{

    {
        dw1::JsonFile2 jsonFile;
        jsonFile.Parse("Game.json");
        jsonFile.Print();


        std::cout << "\n ---------debug info----------" << std::endl;
        std::cout << "\t Array : " << dw1::jsonArray::debug_created_count << std::endl;
        std::cout << "\t Object : " << dw1::jsonObject::debug_created_count << std::endl;
        std::cout << "\t KeyValue : " << dw1::jsonKeyValue::debug_created_count << std::endl;


        std::cout << "\n -------------------" << std::endl;

        dw1::jsonKeyValue* kv = jsonFile.Find("tileCount");
        if (kv) kv->Print(2);
        else
            std::cout << "\n실패!!" << std::endl;

    }

    std::cout << "\n ---------debug info----------" << std::endl;
    std::cout << "\t Array : " << dw1::jsonArray::debug_created_count << std::endl;
    std::cout << "\t Object : " << dw1::jsonObject::debug_created_count << std::endl;
    std::cout << "\t KeyValue : " << dw1::jsonKeyValue::debug_created_count << std::endl;

}