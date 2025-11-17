#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <stack>

using namespace std;

class graph{
    private:
        struct Vertex{
            bool visit;  
            list<pair<int,int>> path; // Тип <Куда:Вес> 
        };
        vector<Vertex> peaks; // Массив вершин, реализация через связанный список 
        
        void DFS_privat(int index,auto result){
            peaks[index].visit = true; // Помечаем текущую вершину как посещённую
            *result = index;
            for(auto element: peaks[index].path){
                if(!peaks[element.first].visit){ //если вершина еще не посещена 
                    DFS_privat(element.first, result++);
                }
            }
        }

        auto min(auto a, auto b){
            return a > b ? b : a;
        }

        auto max(auto a, auto b){
            return a > b ? a : b;
        } 

    public:
        graph(size_t new_size){
            peaks.resize(new_size);
        }

        vector<int> DFS(int index) {
            vector<int> result;          // Вектор для хранения результата обхода
            stack<int> buffer;           // Стек 
            buffer.push(index);          // Помещаем начальную вершину в стек
            
            while(!buffer.empty()) {     
                int next = buffer.top(); // Берем вершину с вершины стека
                buffer.pop();            // Удаляем ее из стека
                
                if(!peaks[next].visit) { // Если вершина еще не посещена
                    peaks[next].visit = true;  
                    result.push_back(next);    
                    
                    // Проходим по всем смежным вершинам
                    for(auto element : peaks[next].path) {
                        if(!peaks[element.first].visit) {  // Если смежная вершина не посещена
                            buffer.push(element.first);    // Добавляем в стек для обработки
                        }
                    }
                }
            }
            return result;  
        }

        vector<int> BFS(int index) {
            vector<int> result(peaks.size(), 0);
            if(!peaks.empty()) {
                peaks[index].visit = true;
                result[0] = index;
                auto link = result.begin(); // Создаем итератор для записи следующих вершин

                // Проходим по всем элементам вектора result
                for(auto element: result) {
                    // Для текущей вершины проверяем всех соседей
                    for(auto path: peaks[element].path) {
                        // Если соседняя вершина еще не посещена
                        if(!peaks[path.first].visit) {
                            // Перемещаем итератор на следующую позицию
                            link++;
                            
                            // Записываем индекс соседней вершины в result
                            *link = path.first;
                            
                            // Помечаем вершину как посещенную
                            peaks[path.first].visit = true;
                        }
                    }
                }
                // Сбрасываем флаги посещения для всех вершин
                for(auto peak: peaks) {
                    peak.visit = false;
                }
            }
            return result;
        }

        vector<int> Dijkstra(int index) {
            vector<int> result(peaks.size(), __INT_MAX__);
            if(!peaks.empty()) {
                result[index] = 0;
                peaks[index].visit = false;
                
                for(int k = 0; k < result.size(); k++) {
                    int min_el = __INT_MAX__;  
                    int id = -1;               

                    for(int j = 0; j < result.size(); j++) {
                        if (min_el >= result[j] && !peaks[j].visit) {
                            min_el = result[j];  
                            id = j;             
                        }
                    }
                    peaks[id].visit = true;
                    
                    // Обновляем расстояния до всех соседей выбранной вершины
                    for(auto element : peaks[id].path) {
                        // Выбираем минимум между текущим расстоянием и новым маршрутом
                        result[element.first] = min(result[element.first], result[id] + element.second);
                    }
                }
                for(auto peak: peaks) {
                    peak.visit = false;
                }
            }
            return result;
        }
    
        vector<vector<int>> Floyd_Warshall(){
            vector<vector<int>> result(peaks.size(),vector<int>(peaks.size(),20000));
            
            for(int i = 0; i < peaks.size(); i++){
                for(auto element : peaks[i].path){
                    result[i][element.first] = element.second;
                    result[element.first][i] = element.second;
                }
            }
            //заполение диагонали нулями
            for(int i = 0; i < peaks.size(); i++){
                for(int j = 0; j < peaks.size(); j++){
                    if( i == j)
                        result[i][j] = 0;
                }   
            }
            //проходим по всем промежуточным вершинам 
            for(int k = 0; k < peaks.size(); k++){
                for(int i = 0; i < peaks.size(); i++){
                    for(int j = 0; j < peaks.size(); j++){
                        result[i][j] = min(result[i][j], result[i][k] + result[k][j]);
                    }
                }
            }
            return result; 
        }

        graph Prima(){
            graph result(peaks.size()); 
            vector<pair<int,pair<int,int>>> buffer; 

            if(!peaks.empty()){
                int index = 0;
                for(int i = 0; i < peaks.size() - 1; i++){
                    peaks[index].visit = true;
                    for(auto element : peaks[index].path){
                        if(!peaks[element.first].visit)//Если вершина ещё не посещалась
                            buffer.push_back(make_pair(element.second, make_pair(index, element.first))); //добавляем ребро в buffer (вес, откуда, куда)
                    }

                    sort(buffer.begin(),buffer.end(),[](const auto& a, const auto& b ){ //Сортируем буфер по весу рёбер (по возрастанию)
                        return a.first < 0 ? (b.first < 0 ? false : true) : (b.first < 0 ? false : a.first < b.first);
                    });
                    
                    for(auto element : buffer){
                        if(element.first < 0){
                            break;
                        }
                        if(peaks[element.second.second].visit){
                            element.first = -1;
                        }else{
                            result.peaks[element.second.first].path.push_back(make_pair(element.second.second, element.first));
                            result.peaks[element.second.second].path.push_back(make_pair(element.second.first, element.first));
                            index = element.second.second;
                            break;
                        }
                    }
                }
            }
            return result;
        }

        graph Kruskal(){
            graph result(peaks.size()); 
            vector<pair<int,pair<int,int>>> buffer; //pair<вес, pair<из, в>>
            vector<int> colors(peaks.size());
            //Заполняем buffer рёбрами 
            for(int i = 0; i < peaks.size(); i++){
                colors[i] = i;
                // Проходим по всем смежным вершинам (рёбрам) текущей вершины i
                for(auto element : peaks[i].path){
                    // Чтобы избежать дублирования рёбер (A-B и B-A), добавляем только если i > element.first
                    if(i > element.first){
                        buffer.push_back(make_pair(element.second, make_pair(i, element.first)));
                    }
                }
            }
            sort(buffer.begin(),buffer.end());

            for(auto element : buffer){
                if(colors[element.second.first] != colors[element.second.second]){ // вершины в разных компонентах → можно добавить ребро
                    result.peaks[element.second.first].path.push_back(make_pair(element.second.second, element.first));
                    result.peaks[element.second.second].path.push_back(make_pair(element.second.first, element.first));
                    int temp = colors[element.second.first];
                    
                    for(int i = 0; i < colors.size(); i ++){
                        if( temp == colors[i]){
                            colors[i] = colors[element.second.second];
                        }
                        
                    }
                    
                }
            }
            return result;
        }

        void print(){
            for(int i = 0; i < peaks.size(); i++){
                cout << i << " : ";  
                for(auto element : peaks[i].path){
                    cout << "( "<< element.first <<" | "<< element.second << "),";
                }
                cout << endl;
            }
        }

        void push(int input, int output, int weight = 0, bool flag = false ){ // Флаг отвечает за оринтированность(true)/не оринтированность(false)
            peaks[input].path.push_back(make_pair(output,weight));
            if (!flag){
                peaks[output].path.push_back(make_pair(input,weight));
            }
        }

        size_t size(){
            return peaks.size();
        }

};


