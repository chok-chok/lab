#include <iostream>
#include "btree_map.h"

int main() {
    btree_map<int, std::string> map(2); // Минимальная степень B-дерева = 2

    // Вставка значений
    map.insert(10, "ten");
    map.insert(20, "twenty");
    map.insert(5, "five");
    map.insert(6, "six");
    map.insert(12, "twelve");
    map.insert(30, "thirty");
    map.insert(7, "seven");
    map.insert(17, "seventeen");

    // Проверка поиска
    std::cout << "Find 6: " << (map.find(6) ? "Found" : "Not found") << std::endl;
    std::cout << "Find 15: " << (map.find(15) ? "Found" : "Not found") << std::endl;

    // Доступ через operator[]
    std::cout << "Value at 10: " << map[10] << std::endl;
    std::cout << "Value at 7: " << map[7] << std::endl;
    
    // Проверка изменения значения
    map[10] = "TEN";
    std::cout << "Updated value at 10: " << map[10] << std::endl;
    
    // Проверка пустоты
    std::cout << "Map is empty: " << (map.empty() ? "Yes" : "No") << std::endl;
    
    // Очистка
    map.clear();
    std::cout << "Map cleared. Is empty: " << (map.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
