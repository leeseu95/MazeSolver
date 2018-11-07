#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <list>

using namespace std;

//Estructura de nodo
struct Node {
    short x;
    short y;
    int cost;
    Node* parent;
    char movement;
    float heuristic;
    float score;
    Node(short x_, short y_, Node* parent_, char movement_, float heuristic_, int cost_) {
        x = x_;
        y = y_;
        parent = parent_;
        movement = movement_;
        heuristic = heuristic_;
        cost = cost_;
        score = heuristic_ + cost_;
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

void expandNode(Node currentNode, vector<Node> &openSet, vector<Node> &closedSet, map<string, string> &cameFrom, Matrix maze, short finalX, short finalY) {
    // cout << maze(currentNode.x+1, currentNode.y) << endl;
    //Down
    if(currentNode.x != (maze.rows - 1) && maze(currentNode.x+1, currentNode.y) != 1 ) { //Si no esta esta hasta abajo y abajo no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x+1, 2) + pow(finalY - currentNode.y, 2));
        Node tempNode(currentNode.x+1, currentNode.y, &currentNode, 'D', distanceFinal, currentNode.cost + 1);
        bool visitedFlag = false;
        for(int i = 0; i < closedSet.size(); i++) {
            if(closedSet[i].x == tempNode.x && closedSet[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        // Si no fue visitada y es un nodo que no ha sido ya descubierto
        if(visitedFlag == false) {
            bool foundFlag = false;
            int index = 0;
            for(int i = 0; i < openSet.size(); i++) {
                if(openSet[i].x == tempNode.x && openSet[i].y == tempNode.y) {
                    foundFlag = true;
                    index = i;
                    break;
                }
            }
            if (!foundFlag) {
                openSet.push_back(tempNode);
                string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                cameFrom[key] = value;
            }
            else {
                // Si no es un nuevo descubrimiento, pero tenemos mejor score que el que 
                // habíamos descubierto, lo actualizamos
                if (tempNode.score < openSet[index].score) {
                    string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                    string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                    cameFrom[key] = value;
                }
            }
        }
    }
    //Up
    if(currentNode.x != 0 && maze(currentNode.x-1, currentNode.y) != 1 ) { //Si no esta esta hasta arriba y arriba no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x-1, 2) + pow(finalY - currentNode.y, 2));
        Node tempNode(currentNode.x-1, currentNode.y, &currentNode, 'U', distanceFinal, currentNode.cost + 1);
        bool visitedFlag = false;
        for(int i = 0; i < closedSet.size(); i++) {
            if(closedSet[i].x == tempNode.x && closedSet[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        // Si no fue visitada y es un nodo que no ha sido ya descubierto
        if(visitedFlag == false) {
            bool foundFlag = false;
            int index = 0;
            for(int i = 0; i < openSet.size(); i++) {
                if(openSet[i].x == tempNode.x && openSet[i].y == tempNode.y) {
                    foundFlag = true;
                    index = i;
                    break;
                }
            }
            if (!foundFlag) {
                openSet.push_back(tempNode);
                string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                cameFrom[key] = value;
            }
            else {
                // Si no es un nuevo descubrimiento, pero tenemos mejor score que el que 
                // habíamos descubierto, lo actualizamos
                if (tempNode.score < openSet[index].score) {
                    string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                    string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                    cameFrom[key] = value;
                }
            }
        }
    }
    //Right
    if(currentNode.y != (maze.cols-1) && maze(currentNode.x, currentNode.y+1) != 1 ) { //Si no esta hasta la derecha y derecha no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x, 2) + pow(finalY - currentNode.y+1, 2));
        Node tempNode(currentNode.x, currentNode.y+1, &currentNode, 'R', distanceFinal, currentNode.cost + 1);
        bool visitedFlag = false;
        for(int i = 0; i < closedSet.size(); i++) {
            if(closedSet[i].x == tempNode.x && closedSet[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        // Si no fue visitada y es un nodo que no ha sido ya descubierto
        if(visitedFlag == false) {
            bool foundFlag = false;
            int index = 0;
            for(int i = 0; i < openSet.size(); i++) {
                if(openSet[i].x == tempNode.x && openSet[i].y == tempNode.y) {
                    foundFlag = true;
                    index = i;
                    break;
                }
            }
            if (!foundFlag) {
                openSet.push_back(tempNode);
                string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                cameFrom[key] = value;
            }
            else {
                // Si no es un nuevo descubrimiento, pero tenemos mejor score que el que 
                // habíamos descubierto, lo actualizamos
                if (tempNode.score < openSet[index].score) {
                    string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                    string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                    cameFrom[key] = value;
                }
            }
        }
    }
    //Right
    if(currentNode.y != 0 && maze(currentNode.x, currentNode.y-1) != 1 ) { //Si no esta hasta la izquierda e izquierda no es 1
        //Calcular el costo de la heuristica dada
        float distanceFinal = sqrt(pow(finalX - currentNode.x, 2) + pow(finalY - currentNode.y-1, 2));
        Node tempNode(currentNode.x, currentNode.y-1, &currentNode, 'L', distanceFinal, currentNode.cost + 1);
        bool visitedFlag = false;
        for(int i = 0; i < closedSet.size(); i++) {
            if(closedSet[i].x == tempNode.x && closedSet[i].y == tempNode.y) {
                visitedFlag = true;
                break;
            }
        }
        // Si no fue visitada y es un nodo que no ha sido ya descubierto
        if(visitedFlag == false) {
            bool foundFlag = false;
            int index = 0;
            for(int i = 0; i < openSet.size(); i++) {
                if(openSet[i].x == tempNode.x && openSet[i].y == tempNode.y) {
                    foundFlag = true;
                    index = i;
                    break;
                }
            }
            if (!foundFlag) {
                openSet.push_back(tempNode);
                string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                cameFrom[key] = value;
            }
            // Si no es un nuevo descubrimiento, pero tenemos mejor score que el que 
            // habíamos descubierto, lo actualizamos
            else if (tempNode.score < openSet[index].score) {
                string key = to_string(tempNode.x) + "-" + to_string(tempNode.y);
                string value = to_string(currentNode.x) + "-" + to_string(currentNode.y);
                cameFrom[key] = value;
            }
        }
    }
}

string findPath(Node currentNode, map<string, string> &cameFrom) {
    //Definicion de string temporal
    string key = to_string(currentNode.x) + "-" + to_string(currentNode.y);
    string value = cameFrom[key];
    string path = key + "\n";

    while (value != "START") {
        path += value + "\n";
        key = value;
        value = cameFrom[key];
    }

    cout << "path:" << endl;
    cout << path << endl;

    return path;
}

//Funcion para hacer el sort
bool sortQueue (Node a, Node b) { return (a.score < b.score); }

/* void printCamefrom(map<string, string> &cameFrom) { */
/* } */

void aStarSearch(Matrix maze, short initialX, short initialY, short finalX, short finalY) {
    //Arreglos que vamos a utilizar
    vector<Node> closedSet; // Set of nodes already evaluated

    //Creamos el nodo inicial
    float heuristic = sqrt(pow(finalX - initialX, 2) + pow(finalY - initialY, 2));
    Node initialNode(initialX, initialY, nullptr, 'H', heuristic, 0);

    // The set of currently discovered nodes that are not evaluated yet.
    // Initially, only the start node is known.
    vector<Node> openSet;
    openSet.push_back(initialNode);

    // For each node, which node it can most efficiently be reached from.
    // If a node can be reached from many nodes, cameFrom will eventually contain the
    // most efficient previous step.
    // cameFrom := an empty map
    // el key es el elemento y el value es de donde vino.
    map<string, string> cameFrom;
    string key = to_string(initialNode.x) + "-" + to_string(initialNode.y);
    cameFrom[key] = "START";

    // For each node, the cost of getting from the start node to that node.
    /* gScore := map with default value of Infinity */
    // Es 1 el costo para todos en nuestro caso

    while(!openSet.empty()) {
        // Sorteamos los nodos dependiendo del score
        sort(openSet.begin(), openSet.end(), sortQueue);
        Node currentNode = openSet.front();

        // Checamos si llegamos al goal
        if (currentNode.x == finalX && currentNode.y == finalY) {
            ofstream myfile;
            myfile.open ("./public/solutions/generated-solution.txt");
            myfile << findPath(currentNode, cameFrom);
            myfile.close();
        }

        move(openSet.begin(), openSet.begin() + 1, back_inserter(closedSet));
        openSet.erase(openSet.begin());

        expandNode(currentNode, openSet, closedSet, cameFrom, maze, finalX, finalY);
    }
    cout << "End search" << endl;
}

int main(int argc, char * argv[]) {
    //Tenemos por default el nombre del txt
    char * mazeText = "./public/mazes/generated-maze.txt";

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
    return 0;
}
