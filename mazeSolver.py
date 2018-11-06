import math
import random
import sys
import numpy as np
import time

#Class Node (recomendado como el profesor)
class Node:
    def __init__(self, state, parent, movement, cost, visited):
        self.state = state #estado del nodo
        self.parent = parent #padre
        self.movement = movement #movimiento
        self.cost = cost #costo
        self.visited = visited #visitado para checar si ya fue visitado el nodo

def createNode(state, parent, movement, cost, visited):
    return Node(state, parent, movement, cost, visited)

#Funcion principal definida por el profesor busquedaNoInformada(edoInicial, edoFinal, algoritmo)

def expandNode(state, matrixMaze, finalX, finalY, colMat, rowMat):
    possibleStates = []
    #D
    if (state[0] != (rowMat - 1) and matrixMaze[state[0]+1][state[1]] != 1 ):
        # print("gdlsagdlka")
        nodeTemp = state.copy()
        nodeTemp[0] += 1
        #calcular el costo a traves de la heuristica dada
        distanceFinal = math.sqrt((finalX - nodeTemp[0])**2 + (finalY - nodeTemp[1])**2)
        cost = distanceFinal
        #appendear el nuevo estado
        possibleStates.append(createNode(nodeTemp, state, "D", cost, 0))
    #U
    if (state[0] != 0 and matrixMaze[state[0]-1][state[1]] != 1): #Si se encuentra en cualquier columna 2 o la 3, lo podemos mover a la izquierda
        nodeTemp = state.copy()
        nodeTemp[0] -= 1
        #calcular el costo a traves de la heuristica dada
        distanceFinal = math.sqrt((finalX - nodeTemp[0])**2 + (finalY - nodeTemp[1])**2)
        cost = distanceFinal
        #appendear el nuevo estado
        possibleStates.append(createNode(nodeTemp, state, "U", cost, 0))
    #L
    if (state[1] != 0 and matrixMaze[state[0]][state[1] - 1] != 1): #Si se encuentra en cualquier fila 1 o 2, lo podemos mover hacia abajo
        nodeTemp = state.copy()
        nodeTemp[1] -= 1
        #calcular el costo a traves de la heuristica dada
        distanceFinal = math.sqrt((finalX - nodeTemp[0])**2 + (finalY - nodeTemp[1])**2)
        cost = distanceFinal
        #appendear el nuevo estado
        possibleStates.append(createNode(nodeTemp, state, "L", cost, 0))
    #R
    if (state[1] != colMat - 1 and matrixMaze[state[0]][state[1] + 1] != 1): #Si se encuentra en cualquier fila 1 o 2, lo podemos mover hacia abajo
        nodeTemp = state.copy()
        nodeTemp[1] += 1
        #calcular el costo a traves de la heuristica dada
        distanceFinal = math.sqrt((finalX - nodeTemp[0])**2 + (finalY - nodeTemp[1])**2)
        cost = distanceFinal
        #appendear el nuevo estado
        possibleStates.append(createNode(nodeTemp, state, "R", cost, 0))
    return possibleStates

def aStarSearch(matrixMaze, initX, initY, finalX, finalY, colMat, rowMat):
    #arreglos que vamos a utilizar
    queue = [] #queue para el AStar
    currentState = [] #estado actual en el que estamos
    fringe = [] #los nodos que vamos a ir sacando
    path = [] #el camino completo
    visitedNodes = [] #los nodos que ya hemos visitado
    finalPath = [] #El camino final
    stringPath = ""

    visitedCount = 0 #cuantos hemos visitado
    cost = [] #costo del camino

    edoInicial = [initX, initY]
    edoFinal = [finalX, finalY]
    # print(edoFinal)
    # print(edoInicial)

    #Nuestro nodo inicial
    queue.append(createNode(edoInicial, None, "", 0, 0))

    while(queue): #mientras haya algo en el queue
        #Sorteamos el queue
        sorted(queue, key=lambda node: node.cost)
        path.append(queue[0]) #metemos a los visitados cualquiera que vaya siguiente en el queue
        currentState = queue.pop(0) #sacamos el primer elemento o nodo en el queue

        if np.array_equal(currentState.state, edoFinal): #Si ya se encontro el estado final
            finalPath = findPath(currentState, path) #Buscar el camino
            path = ''.join(finalPath)
            print(path[::-1])
            break

        for i in range(len(visitedNodes)):
            if(currentState.state == visitedNodes[i].parent):
                currentState.visited = 1 #si existe en los visitados marcamos su visited como 1

        if currentState.visited != 1: #si no ha sido visitado
            fringe = expandNode(currentState.state, matrixMaze, finalX, finalY, colMat, rowMat) #Checamos todas las expansiones posibles
            for i in range(len(fringe)):
                queue.append(fringe[i])
                visitedNodes.append(fringe[i])

        if(len(queue) == 0):
            print("End of queue, no solution")
            break

#Funcion para obtener el backtracking una vez que se encuentra la meta, recibe el estado y un arreglo con todos los nodos
def findPath(currentState, pathQueue):
    path = "" #Arreglo que contendra el path final
    while(currentState.parent):
        path += currentState.movement #Se agrega el movimiento de dicho estado al path resultante
        for i in range(len(pathQueue)):
            parent = currentState.parent
            if(pathQueue):
                temp = pathQueue.pop() #Busca en todos los nodos creados anteriormente
                if(parent == temp.state): #Busca el que el estado concuerde con el último padre
                    currentState = temp
                    path += currentState.movement
            else: break

    return path


if __name__ == '__main__':
    start_time = time.time()
    matrix, matrixMaze = [],[]
    mn = input()
    m, n = int(mn.split(" ")[0]), int(mn.split(" ")[1])

    start = input()
    startX, startY = int(start.split(" ")[0]), int(start.split(" ")[1])

    final = input()
    finalX, finalY = int(final.split(" ")[0]), int(final.split(" ")[1])
    for i in range(int(n)):
        line = input()
        matrix.append(line)

    for i in range(len(matrix)):
        matrixMaze.append([int(i) for i in str(matrix[i])])

    aStarSearch(matrixMaze, ((n-1)-startY), startX, ((n-1)-finalY), finalX, m, n)
    print("Seconds: " , (time.time() - start_time))
