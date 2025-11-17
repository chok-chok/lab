#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <time.h>
#include <iomanip>

#include "graph-a.h"

using namespace std;

void test(graph test_graph, int index){ //index отвечает за алгоритм 
    clock_t start_time = 0, end_time = 0;
    vector<int> result;
    vector<vector<int>> result_matrix;
    graph result_graph(test_graph.size());
    start_time = clock();
    switch (index)
    {
    case 1:
        cout << "DFS:" << endl;
        result = test_graph.DFS(0);
        break;
    case 2:
        cout << "BFS:" << endl;
        result = test_graph.BFS(0);
        break;
    case 3:
        cout << "Dijkstra:" << endl;
        result = test_graph.Dijkstra(0);
        break; 
    case 4:
        cout << "Floyd_Warshall:" << endl;
        result_matrix = test_graph.Floyd_Warshall();
        break;
    case 5:
        cout << "Prima:" << endl;
        result_graph = test_graph.Prima();
        break;
    case 6:
        cout << "Kruskal:" << endl;
        result_graph = test_graph.Kruskal();
        break;
          
    default:
        break;
    }

    
    end_time = clock();
    switch (index)
    {
    case 0:
        test_graph.print();
        break;
    case 1:
    case 2:
    case 3:
        for(auto element : result){
            cout << setw(3)<< setfill(' ') << element;
        }
        break; 
    case 4:
        for(auto col : result_matrix){
            for(auto element : col){
                cout << setw(3)<< setfill(' ')<< element  ;
            }
            cout << endl;
        }
        break;
    case 5:
    case 6:
        result_graph.print();
        break;
    default:
        break;
    }
    cout << endl;
    double second  = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    cout << " time: " << endl << second << endl;
    cout << setw(40) << setfill('-') << "-";
    cout << endl;
   
} 



int main(int argc, char* argv[]){

        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);


    size_t size;
    cin >> size;
    bool flag; 
    if(argc != 1){
        flag = true;
    }
    graph test_graph(size);
    cin >> size;
    for(int i = 0; i < size; i++){
        int x, y, weight;
        cin >> x >> y >> weight;
        test_graph.push(x - 1,y - 1,weight,flag);
    }
    for(int i = 0; i < 7; i++){
        test(test_graph, i);
    }
    return 0;
}