1) Create a new project (make sure it's a C++ project not Blueprint!) or open your existing Unreal project
2) Open "YourGame/Sources/YourGame/YourGame.Build.cs".
3) Delete "PrivateDependencyModuleNames.AddRange(new string[] { "" });"
4) Paste "PrivateDependencyModuleNames.AddRange(new string[] { "Http", "Json", "JsonUtilities" });"
5) Paste the Private/Public folder inside the zip into your "YourGame/Sources/YourGame/" folder
6) Now you should have the SteamAttributionActorComponent.h/SteamAttributionActorComponent.cpp files
7) The SteamAttributionActorComponent.cpp file contains the http(s) POST request inside the BeginPlay function
8) Modify the .h/.cpp file to your needs

Testing the code
9) Create a new Empty Actor to your World (in the World Outliner)
10) Click on the +Add Component and search for "Steam Attribution Actor" component and add it to the empty actor
11) Hit Play and the BeginPlay() function of the SteamAttributionActorComponent will be called, this will do the POST call