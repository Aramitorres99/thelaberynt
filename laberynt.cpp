#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;







// Definimos las dimensiones del laberinto
const int WIDTH = 21;  // Usamos dimensiones impares para mejor resultado
const int HEIGHT = 21;

// Definimos los caracteres para representar el laberinto
const char WALL = '#';
const char OPEN = ' ';
const char START = 'S';
const char END = 'X';
const char PATH = '*';  // Para marcar el camino recorrido por el jugador

// Estructura para representar una celda del laberinto
struct Cell { // el struct define una estructura en c++ y cell es el nombre de esa estructura
    int x, y; // son las coordenadas 
    Cell(int _x, int _y) : x(_x), y(_y) {} //aca la estrucura cell toma esas coordenadas y representa de esta manera un laberinto
};

// Función para imprimir el laberinto
void printMaze(const vector<vector<char>>& maze) { //es una constante de vector de vectores de caracteres y maze signfica que no va a modificar el laberinto solo imprimirlo
    for (const auto& row : maze) { //itera sobre maze y row es la fila en ese maze
        for (char cell : row) { //esto va a ejecutar una vez por cada caracter cell en row
            cout << cell << ' '; //esto imprime el valor de cell y un espacio despues de cada caracter para que sea mejor visualizado
        }
        cout << endl;
    }
}

// Verificar si la celda está dentro del laberinto
bool isValid(int x, int y) {  //bool indica que esta funcion devuelve un valor booleano, true or false, x e y son las coordenadas que son parametros de la funcion
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT); //aca se verifica si la coordenada tanto x como y estan o no dentro del laberinto 
}

void generateMaze(vector<vector<char>>& maze, int startX, int startY) { //funcion que toma como parametros el laberinto creado y las coordenadas de inicio del mismo
    queue<Cell> q; // cola que almacena las celdas que se estan procesando durante la generacion del laberinto 
    q.push(Cell(startX, startY)); //incerta en la cola la celda inicial que son las coordenadas starx y starty
    maze[startX][startY] = OPEN; // marca la celda inicial del laberinto marcandola como open o abierto

    // Movimientos posibles (arriba, abajo, izquierda, derecha)
    vector<pair<int, int>> directions = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    // Motor de números aleatorios
    random_device rd; //se usa para obtener numeros aleatorios 
    mt19937 g(rd());

    while (!q.empty()) { //se ejecuta mientras la cola no este vacia 
        Cell current = q.front(); //obtiene la celda en el frente de la cola q y la asigna a la variable current o actual
        q.pop(); //elimina la celda en el frente de la cola q

        // Mezclar las direcciones para aleatoriedad
        shuffle(directions.begin(), directions.end(), g); 

        for (auto dir : directions) { //itera en cada direccion 
            int newX = current.x + dir.first; //calcula la nueva coordenada horizontal y suma la coordenada horizontal actuaL con el primer elemnto de la direccion 
            int newY = current.y + dir.second; //calcula la nueva coordenada vertical y suma la coordenada vertical actual con el segundo elemento de la direccion

            // Verificar si la nueva posición está dentro del laberinto y no ha sido visitada
            if (isValid(newX, newY) && maze[newX][newY] == WALL) {
                // Calcular la posición intermedia para romper el muro
                int midX = current.x + dir.first / 2; //Calcula la coordenada horizontal intermedia para romper el muro entre la celda actual y la nueva celda.
                int midY = current.y + dir.second / 2; //Calcula la coordenada vertical intermedia para romper el muro entre la celda actual y la nueva celda.

                maze[midX][midY] = OPEN;  // Romper el muro
                maze[newX][newY] = OPEN;  // Marcar la nueva celda como parte del laberinto

                q.push(Cell(newX, newY)); // Añadir la nueva celda a la cola
            }
        }
    }
}

// Función para encontrar y marcar el camino desde el inicio hasta la salida
bool solveMaze(vector<vector<char>>& maze, Cell start, Cell end) { //toma como parametros el vector de vectores de caracteres y la celda de inicio y el final
    queue<Cell> q;
    q.push(start);

    // Movimientos posibles (arriba, abajo, izquierda, derecha)
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    vector<vector<Cell>> parent(WIDTH, vector<Cell>(HEIGHT, Cell(-1, -1)));
    parent[start.x][start.y] = start;

    while (!q.empty()) {
        Cell current = q.front();
        q.pop();

        if (current.x == end.x && current.y == end.y) {
            // Marcar el camino desde la salida al inicio usando los padres
            Cell step = end;
            while (!(step.x == start.x && step.y == start.y)) {
                maze[step.x][step.y] = PATH;
                step = parent[step.x][step.y];
            }
            maze[start.x][start.y] = START;  // Asegurar que el inicio y la salida estén correctos
            maze[end.x][end.y] = END;
            return true;
        }

        for (auto dir : directions) {
            int newX = current.x + dir.first;
            int newY = current.y + dir.second;

            if (isValid(newX, newY) && maze[newX][newY] == OPEN && parent[newX][newY].x == -1) {
                parent[newX][newY] = current;
                q.push(Cell(newX, newY));
            }
        }
    }
    return false;
}

int main() {
    srand(time(0)); // Semilla para la generación de números aleatorios

    // Inicializar el laberinto con muros
    vector<vector<char>> maze(WIDTH, vector<char>(HEIGHT, WALL));

    // Definir punto de inicio en el borde superior y punto de salida en el borde inferior
    int startX = 0; // Primera fila
    int startY = rand() % (WIDTH / 2) * 2 + 1; // Posición aleatoria en el borde superior (impar)

    int endX = HEIGHT - 1; // Última fila
    int endY = rand() % (WIDTH / 2) * 2 + 1; // Posición aleatoria en el borde inferior (impar)

    // Generar el laberinto desde una celda dentro del laberinto
    generateMaze(maze, 1, startY);

    // Asegurar el inicio y la salida están abiertos
    maze[startX][startY] = START;
    maze[1][startY] = OPEN;  // Celda dentro del laberinto para BFS

    maze[endX][endY] = END;
    maze[HEIGHT-2][endY] = OPEN;  // Celda dentro del laberinto para BFS

    // Resolver el laberinto
    if (!solveMaze(maze, Cell(1, startY), Cell(HEIGHT-2, endY))) {
        cout << "No se pudo encontrar un camino del inicio a la salida." << endl;
    }

    // Imprimir el laberinto generado
    printMaze(maze);

    return 0;
}
