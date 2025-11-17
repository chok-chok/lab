#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <utility>

template <typename Key, typename Value>
struct Node {
    std::pair<Key, Value> data;
    Node* left;
    Node* right;
    Node* parent;
    int height;
    
    Node(Key key, Value value)
        : data(std::make_pair(key, value)), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
};

template <typename Key, typename Value, typename Comparator = std::less<Key>>
class Map {
private:
    Node<Key, Value>* root;
    Comparator comp;

    // получение высоты узла
    int getHeight(Node<Key, Value>* node) {
        return node ? node->height : 0; //возвращает высоту узла 
    }

    void updateHeight(Node<Key, Value>* node) {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right)); //обновляет высоту узла 
        }
    }
    
    // Возвращает разницу высот левого и правого поддеревьев.
    int getBalance(Node<Key, Value>* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

//      y
//     /
//    x
//   /
//  z

    //поворот вправо
    Node<Key, Value>* rotateRight(Node<Key, Value>* y) {
        Node<Key, Value>* x = y->left;      // Запоминаем левого потомка y (x)
        y->left = x->right;                 // Перемещаем правое поддерево x в левое поддерево y
        if (x->right) x->right->parent = y; // Обновляем родителя правого поддерева x
        x->right = y;                       // Делаем y правым потомком x
        x->parent = y->parent;              // Обновляем родителя x
        y->parent = x;                      // Обновляем родителя y
        updateHeight(y);                    // Обновляем высоту y
        updateHeight(x);                    // Обновляем высоту x
        return x;                           // Возвращаем новый корень поддерева
    }


    // x
    //  \
    //   y
    //   \
    //    z
    
    //поворот влево
    Node<Key, Value>* rotateLeft(Node<Key, Value>* x) {
        Node<Key, Value>* y = x->right;     // Запоминаем правого потомка x (y)
        x->right = y->left;                 // Перемещаем левое поддерево y в правое поддерево x
        if (y->left) y->left->parent = x;   // Обновляем родителя левого поддерева y
        y->left = x;                        // Делаем x левым потомком y
        y->parent = x->parent;              // Обновляем родителя y
        x->parent = y;                      // Обновляем родителя x
        updateHeight(x);                    // Обновляем высоту x
        updateHeight(y);                    // Обновляем высоту y
        return y;                           // Возвращаем новый корень поддерева
    }

    //
    Node<Key, Value>* balance(Node<Key, Value>* node) {
        updateHeight(node);
        int balanceFactor = getBalance(node); //разница между высотой левого поддерева и высотой правого поддерева:

        if (balanceFactor > 1) {
            if (getBalance(node->left) < 0) { //правое поддерево левого потомка больше
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if (balanceFactor < -1) { //левое поддерево больше правого
            if (getBalance(node->right) > 0) { //левое поддерево правого потомка больше
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    //добавляет узел в дерево
    Node<Key, Value>* insert(Node<Key, Value>* node, Key key, Value value) {
        if (!node) return new Node<Key, Value>(key, value);
        if (comp(key, node->data.first)) {
            node->left = insert(node->left, key, value);
            node->left->parent = node;
        } else if (comp(node->data.first, key)) {
            node->right = insert(node->right, key, value);
            node->right->parent = node;
        } else {
            node->data.second = value;
        }
        return balance(node);
    }

    //поиск узла
    Node<Key, Value>* find(Node<Key, Value>* node, Key key) const {
        if (!node || node->data.first == key) return node;
        if (comp(key, node->data.first)) return find(node->left, key);
        return find(node->right, key);
    }

    //удаление дерева   
    void destroy(Node<Key, Value>* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    Map() : root(nullptr) {}
    ~Map() { clear(); }
    
    void insert(Key key, Value value) {
        root = insert(root, key, value);
    }
    
    bool empty() const { return root == nullptr; }
    
    void clear() {
        destroy(root);
        root = nullptr;
    }
    
    Value& operator[](Key key) {
        Node<Key, Value>* node = find(root, key);
        if (!node) {
            insert(key, Value());
            node = find(root, key);
        }
        return node->data.second;
    }
    
    bool contains(Key key) const {
        return find(root, key) != nullptr;
    }
};

#endif

