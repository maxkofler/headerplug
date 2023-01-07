# Creating a plugin for use with headerplug
This file will explain the creation of a plugin that is loadable by the exaple in the main README.md.
You can paste these code snippets into a file called `myplugin.cpp` to compile them later.
### Creating the classes
In this piece, we just declare our base class and create a derived class that represents our plugin.
```c++
#include <iostream>

//Declare the class the plugin derives of
class MyPlugin{
public:
	virtual std::string getName() = 0;
};

//Create a class that derives from the plugin class
class SamplePlugin : public MyPlugin{
public:
	std::string getName(){
		return "SamplePlugin";
	}

	SamplePlugin(){
		std::cout << "SamplePlugin got created!" << std::endl;
	}

	~SamplePlugin(){
		std::cout << "SamplePlugin got deleted!" << std::endl;
	}
};
```
### Creation and deletion
For the plugin system to work, we need to expose two functions:
 - one to create an instance of the plugin function
 - one to delete the instance later
The names for those functions can be variable, but they need to be adjusted in the call to `Plugin<T>::load`. The example works with the default names:
```c++
extern "C" SamplePlugin* create_plugin(){
	return new SamplePlugin();
}

//This function is responsible for deleting an instance of SamplePlugin
extern "C" void delete_plugin(SamplePlugin* pl){
	delete pl;
}
```
These functions need to be marked with `extern "C"` to prevent the C++ compiler from performing name mangling.
### Compilation
To compile this plugin, issue the following call to the compiler:
```bash
c++ -shared myplugin.cpp -o myplugin.so
```
The `-shared` option tells the compiler to create a shared library.
