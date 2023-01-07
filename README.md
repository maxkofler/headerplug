# headerplug
A simple header-only plugin loader system
# Integration
To integrate this system into your project, clone the submodule and include the `plugin.h` file into your cpp-files.
### Example
If you cloned this repo into the subdirectory `headerplug`, add the following to you .cpp files:
```c++
#include "headerplug/plugin.h"
```

# Usage:
> **Note**
>
> If you want to know how to create a plugin, refer to the [Create a plugin](CREATE_A_PLUGIN.md) file.

If you have created a plugin named `myplugin.so`, that inherits from the class `MyPlugin`, that defines the function `getName()`, the loading works as follows:
```c++
//Include the library
#include "headerplug/plugin.h"

//For outputting results and errors
#include <iostream>

//Declare the class the plugin derives of
class MyPlugin{
public:
	virtual std::string getName() = 0;
};

int main(){
	//Define the path to the .so file
	std::string soPath = "myplugin.so";

	try {
		//Load the plugin derived from the class MyPlugin
		Plugin<MyPlugin> plugin = Plugin<MyPlugin>::load(soPath);

		//Crate a new instance of the class provided in the plugin
		auto instance = plugin.instanciate();

		//And output its name
		std::cout << "Plugin name: " << instance->getName() << std::endl;

	} catch (PluginException& e) {
		//Output any errors
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
```
