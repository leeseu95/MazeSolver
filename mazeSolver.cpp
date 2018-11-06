#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

//Estructura de nodo
struct Node {
    short x;
    short y;
    Node* parent;
    char movement;
    float heuristic;
    Node(short x_, short y_, Node* parent_, char movement_, float heuristic_) {
        x = x_;
        y = y_;
        parent = parent_;
        movement = movement_;
        heuristic = heuristic_;
    }
    void printCoords () {
        cout << x << " , " << y << endl;
    }
    void printMe() {
        cout << endl << "x: " << x << endl;
        cout << "y: " << y << endl;
        cout << "parent: " << parent << endl;
        cout << "movement: " << movement << endl;
        cout << "heuristic: " << heuristic << endl << endl;
    }
};

//Estructura de matriz (maze)
struct Matrix {
    short rows;
    short cols;
    vector<short> data;
    Matrix(short rows_, short cols_):rows(rows_), cols(cols_), data(rows*cols) {

    }
    short & operator()(size_t row, size_t col)
    {
        return data[row*cols+col];
    } 
    short operator()(size_t row, size_t col) const
    {
        return data[row*cols+col];
    } 
};

void expandNode(Node currentNode, vector<Node> &queue, vector<Node> &visitedNodes, Matrix maze, short finalX, short finalY) {
    // cout << maze(currentNode.x+1, currentNode.y) << endl;
    //Down
    if(currentNode.x != (maze.rows - 1) && maze(currentNode.x+1, currentNode.y) != 1 ) { //Si no esta esta hasta abajo y abajo no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x+1, 2) + pow(finalY - currentNode.y, 2));
        Node tempNode(currentNode.x+1, currentNode.y, &currentNode, 'D', distanceFinal);
        bool visitedFlag = false;
        for(int i = 0; i < visitedNodes.size(); i++) {
            if(visitedNodes[i].x == tempNode.x && visitedNodes[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        if(visitedFlag == false) //Si no fue visitada
            queue.push_back(tempNode);
    }
    //Up
    if(currentNode.x != 0 && maze(currentNode.x-1, currentNode.y) != 1 ) { //Si no esta esta hasta arriba y arriba no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x-1, 2) + pow(finalY - currentNode.y, 2));
        Node tempNode(currentNode.x-1, currentNode.y, &currentNode, 'U', distanceFinal);
        bool visitedFlag = false;
        for(int i = 0; i < visitedNodes.size(); i++) {
            if(visitedNodes[i].x == tempNode.x && visitedNodes[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        if(visitedFlag == false) //Si no fue visitada
            queue.push_back(tempNode);
    }
    //Right
    if(currentNode.y != (maze.cols-1) && maze(currentNode.x, currentNode.y+1) != 1 ) { //Si no esta hasta la derecha y derecha no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x, 2) + pow(finalY - currentNode.y+1, 2));
        Node tempNode(currentNode.x, currentNode.y+1, &currentNode, 'R', distanceFinal);
        bool visitedFlag = false;
        for(int i = 0; i < visitedNodes.size(); i++) {
            if(visitedNodes[i].x == tempNode.x && visitedNodes[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        if(visitedFlag == false) //Si no fue visitada
            queue.push_back(tempNode);
    }
    //Right
    if(currentNode.y != 0 && maze(currentNode.x, currentNode.y-1) != 1 ) { //Si no esta hasta la izquierda e izquierda no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x, 2) + pow(finalY - currentNode.y-1, 2));
        Node tempNode(currentNode.x, currentNode.y-1, &currentNode, 'L', distanceFinal);
        bool visitedFlag = false;
        for(int i = 0; i < visitedNodes.size(); i++) {
            if(visitedNodes[i].x == tempNode.x && visitedNodes[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        if(visitedFlag == false) //Si no fue visitada
            queue.push_back(tempNode);
    }
    // Metemos a visitedNodes, nuestro nodo
    visitedNodes.push_back(currentNode);
}

string findPath(Node currentNode) {
    //Definicion de string temporal
    string path = "";

    //Empezamos igualando el primer nodo que si llego al path y luego apuntamos hacia su padre
    path = path + currentNode.movement;
    //Apuntamos hacia el padre
    Node* currentParent = currentNode.parent;
    while(currentParent != nullptr) {
        path = path + currentParent->movement;
        currentParent = currentParent->parent;
    }

    return path;
}

//Funcion para hacer el sort
bool sortQueue (Node a, Node b) { return (a.heuristic < b.heuristic); }

void aStarSearch(Matrix maze, short initialX, short initialY, short finalX, short finalY) {
    //Arreglos que vamos a utilizar
    vector<Node> queue; //Queue
    vector<Node> visitedNodes; //Visited
    string finalPath = ""; //String del path

    //Creamos el nodo inicial
    Node initialNode(initialX, initialY, nullptr, 'H', 0);

    //Lo metemos al queue
    queue.push_back(initialNode);
    while(!queue.empty()) {
        //Sorteamos los Nodos dependiendo de su heuristica
        sort(queue.begin(), queue.end(), sortQueue);
        //Se tiene que hacer un back y luego un pop_back(), no se porque pero asi funciona
        Node currentNode = queue.front();
        queue.erase(queue.begin()); //Popeamos el primer nodo

        // cout << currentNode.x << " , " << currentNode.y << endl;

        //Si ya llegamos al estado final
        if(currentNode.x == finalX && currentNode.y == finalY) {
            cout << currentNode.parent->x<< " , " << currentNode.parent->y << endl;
            // finalPath = findPath(currentNode);

            for(int i = finalPath.length()-1; i >= 0; i--) {
                cout << finalPath[i];
            }
            break;
        }

        expandNode(currentNode, queue, visitedNodes, maze, finalX, finalY);

        //Si ya pasamos por todos los nodos posibles
        if(queue.size() == 0) {
            cout << "There is no solution for this problem" << endl;
        }
    }
}

int main(int argc, char * argv[]) {
    //Tenemos por default el nombre del txt
    char * mazeText = "maze.txt";

    if(argc == 2) { //Si nos dieron los file names
        mazeText = argv[1];
    }

    //Abrimos el file
    FILE* file_ptr = fopen(mazeText, "r");
    
    if(file_ptr == NULL) {
        cout << "ERROR : Unable to open file " << endl;
        exit(EXIT_FAILURE);
    }

    //Inicializamos variables
    short rows, cols, initialX, initialY, finalX, finalY;
    fscanf(file_ptr, "%hu %hu %hu %hu %hu %hu", &rows, &cols, &initialX, &initialY, &finalX, &finalY);

    //Debug print
    // cout << initialX << " , " << initialY << endl;
    // cout << finalX << " , " << finalY << endl;

    //Iteramos a traves de la matriz para poner los valores
    Matrix maze(rows, cols);
    for(int i = 0; i < maze.rows; i++) {
        for(int j = 0; j < maze.cols; j++) {
            fscanf(file_ptr, "%hu", &maze(i, j));
        }
    }

    //Debug print
    for(int i = 0; i < maze.rows; i++) {
        for(int j = 0; j < maze.cols; j++) {
            cout << maze(i, j) << " ";
        }
        cout << endl;
    }

    aStarSearch(maze, initialX, initialY, finalX, finalY);

    //Debug
    // vector<Node> queueNodes;
    // Node initialNode(2, 3, nullptr, 'H', 0);
    // queueNodes.push_back(initialNode);

    // vector<Node> queue; //Queue
    // vector<Node> visitedNodes; //Visited

    // // Creamos el nodo inicial
    // Node initialNode(0, 0, nullptr, 'H', 0);
    // Node node2(2, 2, &initialNode, 'U', 5);
    // Node node3(4, 4, &node2, 'D', 45);
    // // cout << node2.parent->x << endl;

    // string path = "";

    // //Empezamos igualando el primer nodo que si llego al path y luego apuntamos hacia su padre
    // path = path + node3.movement;
    // //Apuntamos hacia el padre
    // Node* currentParent = node3.parent;
    // while(currentParent != nullptr) {
    //     path = path + currentParent->movement;
    //     currentParent = currentParent->parent;
    // }
    // cout << path << endl;

    // //Lo metemos al queue
    // queue.push_back(node3);
    // queue.push_back(initialNode);
    // queue.push_back(node2);

    // sort(queue.begin(), queue.end(), sortQueue);

    // // cout << queue[0].heuristic << endl << queue[1].heuristic << endl << queue[2].heuristic << endl;
    // Node currentNode = queue.front();
    // cout << queue.size() << endl;
    // queue.erase(queue.begin());

    // // cout << endl << currentNode.heuristic << endl;
    // cout << queue.size() << endl;

    // string path = "UDUDUDUDUDLLRRURURURUR";
    // for(int i = path.length()-1; i >= 0; i--) {
    //     cout << path[i];
    // }

    // Matrix maze(10, 10);
    // maze(0, 0) = 3;
    // maze(1, 0) = 3;
    // maze(9, 9) = 2;
    // cout << maze(5, 5) << endl;
    // for(int i = 0; i < maze.rows; i++) {
    //     for(int j = 0; j < maze.cols; j++) {
    //         cout << maze(i, j) << " ";
    //     }
    //     cout << endl;
    // }

}
