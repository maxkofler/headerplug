/*

A simple header-only plugin system for C++
Copyright (C) 2023 Max Kofler (https://maxkofler.eu)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include <dlfcn.h>
#include <string>

class PluginException {

public:
    PluginException(const std::string& msg) : _msg(msg) {
    }

    const char *what(){
        return _msg.c_str();
    }

private:
    std::string         _msg;

};

template <class T> class PluginInstance;

template <class T>
class Plugin{

public:
    static Plugin<T> load (std::string path, std::string createFun = "create_plugin", std::string deleteFun = "delete_plugin"){
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (handle == nullptr)
            throw PluginException("Failed to load library at " + path + ": " + dlerror());

        T* (*create)(void);
        void (*del)(T*);

        create = (T*(*)(void)) dlsym(handle, createFun.c_str());
        if (create == nullptr){
            dlclose(handle);
            throw PluginException("Failed to find function '" + createFun + "' in library " + path);
        }

        del = (void(*)(T*)) dlsym(handle, deleteFun.c_str());
        if (del == nullptr){
            dlclose(handle);
            throw PluginException("Failed to find function '" + deleteFun + "' in library " + path);
        }

        return Plugin<T>(handle, create, del);
    }

    ~Plugin(){
        if (_dlHandle != nullptr)
            dlclose(_dlHandle);
    }

    PluginInstance<T> instanciate() const{
        return PluginInstance<T>(_plugin_create(), _plugin_delete);
    }

private:
    Plugin(){}
    Plugin(void* dlHandle, T*(plugin_create)(void), void(plugin_delete)(T*)) : _dlHandle(dlHandle), _plugin_create(plugin_create), _plugin_delete(plugin_delete) {}

    void*               _dlHandle;

    T*                  (*_plugin_create)(void);
    void                (*_plugin_delete)(T* p);
};

template <class T>
class PluginInstance{

public:
    ~PluginInstance()   {_plugin_delete(_instance);}
    T*                  get() const { return _instance; }

    T* operator -> () const {
        return _instance;
    }

    template <class U> friend class Plugin;

private:
    PluginInstance(){}
    PluginInstance(T* instance, void (*plugin_delete)(T* p)) : _instance(instance), _plugin_delete(plugin_delete) {}

    void                (*_plugin_delete)(T* p);
    T*                  _instance;
};

#endif
