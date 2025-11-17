#ifndef BTREE_MAP_H
#define BTREE_MAP_H

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

// Корень содержит от 1 до 2t-1 ключей. Любой другой узел содержит от t-1 до 2t-1 ключей. 
// Листья не являются исключением из этого правила. Здесь t — параметр дерева
template <typename Key>
struct comparator {
    bool operator()(const Key &a, const Key &b) const {
        return a < b;
    }
};

// Структура узла B-дерева
template <typename Key, typename Value, typename Compare = comparator<Key>>
struct Node {
    bool leaf;                        // флаг листа
    std::vector<std::pair<Key, Value>> keys; // Ключи и значения
    std::vector<Node*> children;      // Дочерние узлы

    Node(bool isLeaf) : leaf(isLeaf) {}
};

// Класс B-дерева
template <typename Key, typename Value, typename Compare = comparator<Key>>
class btree_map {
private:
    Node<Key, Value, Compare>* root;  // Корневой узел
    int t;                            // Минимальная степень
    Compare comp;                     // Компаратор

    //разделение дочернего переполненного узла 
    void splitChild(Node<Key, Value, Compare>* parent, int i, Node<Key, Value, Compare>* child) {
        Node<Key, Value, Compare>* newNode = new Node<Key, Value, Compare>(child->leaf); //для правой части
        for (int j = 0; j < t - 1; j++)
            newNode->keys.push_back(child->keys[j + t]); //Перенос t - 1 правых ключей 
        if (!child->leaf) { //Если узел не лист, переносим t правых дочерних узлов.
            for (int j = 0; j < t; j++)
                newNode->children.push_back(child->children[j + t]);
        }
        parent->keys.insert(parent->keys.begin() + i, child->keys[t - 1]); 
        parent->children.insert(parent->children.begin() + i + 1, newNode); 
        child->keys.resize(t - 1); // Удаляем из узла правые перенесенные ключи
        child->children.resize(t); //сокр потомков до t
    }

    // Вставка в неполный узел
    void insertNonFull(Node<Key, Value, Compare>* node, const Key& key, const Value& value) {
        int i = node->keys.size() - 1;
        if (node->leaf) { //узел - лист
            node->keys.emplace_back(key, value); 
            std::sort(node->keys.begin(), node->keys.end(), [&](const auto& a, const auto& b) {
                return comp(a.first, b.first); //сортируем элементы, тк,например, 15 добавляется -> [10,20,15]
            });
        } else { //узел - не лист 
            
            while (i >= 0 && comp(key, node->keys[i].first)) 
                i--;
            i++; 
            // если узел переполнен
            if (node->children[i]->keys.size() == 2 * t - 1) {
                splitChild(node, i, node->children[i]);
                if (comp(node->keys[i].first, key))
                    i++;
            }
            insertNonFull(node->children[i], key, value);
        }
    }

    // Рекурсивный поиск ключа
    Node<Key, Value, Compare>* search(Node<Key, Value, Compare>* node, const Key& key) const {
        if (!node) return nullptr;
        for (const auto& pair : node->keys) {
            if (pair.first == key)
                return node;
        }
        return node->leaf ? nullptr : search(node->children[0], key);
    }

    // Рекурсивное удаление узлов
    void destroyTree(Node<Key, Value, Compare>* node) {
        if (node) {
            if (!node->leaf) {
                for (auto child : node->children)
                    destroyTree(child);
            }
            delete node;
        }
    }

public:
    // Конструктор
    btree_map(int min_degree) : t(min_degree), root(nullptr) {}
    // Деструктор
    ~btree_map() { clear(); }

    // Проверка на пустоту
    bool empty() const { return root == nullptr; }

    // Очистка дерева
    void clear() {
        destroyTree(root);
        root = nullptr;
    }

    // Вставка элемента
    void insert(const Key& key, const Value& value) {
        if (!root) {
            root = new Node<Key, Value, Compare>(true);
            root->keys.emplace_back(key, value);
        } else {    
            if (root->keys.size() == 2 * t - 1) { //заполнен ли корневой узел. max число ключей - 2t-1
                Node<Key, Value, Compare>* newRoot = new Node<Key, Value, Compare>(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0, root);
                root = newRoot;
            }
            insertNonFull(root, key, value);
        }
    }

    // Оператор []
    Value& operator[](const Key& key) {
        Node<Key, Value, Compare>* node = search(root, key);
        if (node) {
            for (auto& pair : node->keys) {
                if (pair.first == key)
                    return pair.second;
            }
        }
        insert(key, Value());
        return (*this)[key];
    }


    // Поиск ключа
    bool find(const Key& key) const {
        return search(root, key) != nullptr;
    }
};


#endif
