# dw1_Json
Simple JSON in c++

I created this code for personal use,
It was made simply.

(1) example1)
```cpp
    dw1::JsonFile2 jsonfile;
    jsonfile.Parse("Content\\Game.json");
    
    auto& dxWin = jsonfile["DxWindow"];
    g_Dx11.width = int(dxWin["screenWidth"]);
    g_Dx11.height = int(dxWin["screenHeight"]);
    g_Dx11.titleBarText = (char*)(dxWin["title"]);
```

(2) example2
```cpp
    dw1::JsonFile2 jsonfile;    
    g_Dx11.width = int(jsonfile["DxWindow"]["screenWidth"]);
    g_Dx11.height = int(jsonfile["DxWindow"]["screenHeight"]);
    g_Dx11.titleBarText = (char*)(jsonfile["DxWindow"]["title"]);
```

  (3) example3
  ```cpp
  	dw1::JsonFile2 jfile;
	jfile.Parse(fileName);

	countX = int(jfile["x"]);
	countY = int(jfile["y"]);
	basePos.x = int(jfile["base_x"]);
	basePos.y = int(jfile["base_y"]);

	auto& flags = jfile["tiles"];

	for (int y = 0; y < countY; ++y) {
		for (int x = 0; x < countX - 1; ++x) {
			int f = flags.array_x_y(x, y);
      ...
		}
	}
```

  --- json file
  ```json
  {
	"x" : 16, 
	"y" : 16, 
	"base_x" : 16, 
	"base_y" : 16, 
	"tiles" : 
	  [ 
		 [3,3,1,2,1,1,1,1,1,1,0,0,0,0,0,0],
		 [1,4,2,2,3,1,1,1,1,1,0,0,0,0,0,0],
		 [1,1,6,4,1,2,3,1,1,1,0,0,0,0,0,0],
		 [1,1,2,2,2,8,6,1,1,1,0,0,5,0,0,0],
		 [1,1,2,6,11,19,7,1,1,1,0,0,0,0,0,0],
		 [1,1,1,15,22,20,5,1,1,1,0,0,0,0,0,0],
		 [1,1,1,15,23,21,9,1,1,1,0,4,0,0,0,0],
		 [1,1,1,4,12,11,12,11,1,1,1,1,1,0,0,0],
		 [1,1,23,1,1,2,120,1,1,1,0,3,4,1,0,0],
		 [23,22,22,1,1,1,1,1,1,2,1,23,1,0,4,0],
		 [0,0,0,0,0,0,0,0,0,2,0,21,0,0,0,0],
		 [0,0,0,0,0,0,0,0,23,0,5,5,0,0,0,0],
		 [0,0,0,4,0,0,0,0,23,0,2,2,5,0,0,0],
		 [0,0,0,2,4,0,0,0,0,2,0,0,0,0,0,0],
		 [0,0,0,10,1,0,0,0,0,0,1,3,0,4,0,0],
		 [0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0]
	  ] 
}
```

  
