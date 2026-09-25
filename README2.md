# dwJSONlib
2nd JSON libs

## Solution File
	JSON_test2.sln
	
## Files
  - #include "dwJSONlib/json_type.h"
  - dwJSONlib  추가.

## usage

(1) example1)
```cpp
	
	JsonFile json;
    CreateJSON(&json, "tilemap_0_0.json");

    json.Print(stdout);

    const char* cc = (const char*) json["address"]["state"];
    printf("\n");
    printf("address: %s \n", cc);
    // printf("address: %s \n", (const char*)json["city"] );

    Array_t* arr = (Array_t *)json["tiles"];

    printf("\n");

    float ff[18];
    for (int y = 0; y < 16; ++y) {
        Array_t* arr1 = arr->getY(y);
        int cnt = arr1->getX(ff);
        for(int x=0; x<cnt; ++x)
            printf("%.f,", ff[x]);
        printf("\n");
    }
    printf("\n");
```


  
