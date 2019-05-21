







#ifndef LRUCache_hpp
#define LRUCache_hpp

#include <stdio.h>
#include <list>
#include <unordered_map>
#include <iostream>
#include <sstream>

using namespace std;

namespace XP {
    class LRUCache {
    private:
        typedef std::list<int> LRUKey; 
        typedef std::unordered_map<int, std::pair<int, LRUKey::iterator>> Cache; 
        LRUKey _keys;
        Cache _cache;
        int _capacity;
        
    private:
        void touch(Cache::iterator &it) {
            _keys.erase(it->second.second);
            _keys.push_front(it->first);
            it->second.second = _keys.begin();
        }
        
    public:
        LRUCache(int capacity) : _capacity(capacity) {}
        
        int get(int key) {
            auto it = _cache.find(key);
            if (it == _cache.end()) {
                return -1;
            }
            touch(it);
            return it->second.first;
        }
        
        void set(int key, int value) {
            auto it = _cache.find(key);
            if (it != _cache.end()) {
                it->second.first = value;
                touch(it);
                return;
            }
            
            while (_keys.size() >= _capacity) {
                int key = _keys.back();
                _keys.pop_back();
                _cache.erase(key);
            }
            
            _keys.push_front(key);
            _cache[key] = {value, _keys.begin()};
        }
        
        void print() {
            std::cout<<std::endl;
            for (auto key : _keys) {
                std::cout<< "key: " << key <<", value: "<<_cache[key].first<<std::endl;
            }
            std::cout<<std::endl;
        }
        
    public:
        
        
        
        
        
        static void testLRUCache() {
            LRUCache lruCache(2);
            lruCache.set(1, 1);
            lruCache.print();
            lruCache.set(2, 2);
            lruCache.print();
            lruCache.set(3, 3);
            lruCache.print();
            lruCache.set(1, 1);
            lruCache.print();
            std::cout<<lruCache.get(2)<<std::endl;
            lruCache.print();
            std::cout<<lruCache.get(3)<<std::endl;
            lruCache.print();
            std::cout<<"test over!!!"<<std::endl;
        }
    };
};

#endif 
