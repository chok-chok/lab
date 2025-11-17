#include "map.h"
#include <iostream>
#include <cassert>

int main() {
    Map<int, std::string> myMap;
    
    // Проверка на пустоту
    assert(myMap.empty());
    std::cout << "Map is initially empty: PASSED\n";
    
    // Добавление элементов
    myMap.insert(1, "one");
    myMap.insert(2, "two");
    myMap.insert(3, "three");
    
    assert(!myMap.empty());
    std::cout << "Insertion works correctly: PASSED\n";
    
    // Доступ по ключу
    assert(myMap[1] == "one");
    assert(myMap[2] == "two");
    assert(myMap[3] == "three");
    std::cout << "Access by key works correctly: PASSED\n";
    
    // Изменение значения
    myMap[2] = "updated_two";
    assert(myMap[2] == "updated_two");
    std::cout << "Updating values works correctly: PASSED\n";
    
    // Поиск элемента
    assert(myMap.contains(1));
    assert(myMap.contains(2));
    assert(!myMap.contains(4));
    std::cout << "Search works correctly: PASSED\n";
    
    // Очистка контейнера
    myMap.clear();
    assert(myMap.empty());
    std::cout << "Clear function works correctly: PASSED\n";
    
    std::cout << "All tests PASSED!\n";
    return 0;
}

