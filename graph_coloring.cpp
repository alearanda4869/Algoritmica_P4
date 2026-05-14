#include "graph_coloring.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <set>
#include <string>
using namespace std;

/**
 * @brief Analiza un archivo de grafo y construye una estructura Graph.
 * @param filepath Ruta al archivo de grafo.
 * @return Estructura Graph con vértices y lista de adyacencia.
 */
Graph parse_graph_file(const string& filepath) {
    Graph graph;
    ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Error: Could not open graph file " << filepath << endl;
        return graph;
    }

    string line;
    int num_vertices = -1;
    bool header_parsed = false;

    while (getline(file, line)) {
        // Remove comments and trim whitespace.
        size_t comment_pos = line.find_first_of("#//");
        if (comment_pos != string::npos) {
            line = line.substr(0, comment_pos);
        }

        istringstream iss(line);
        vector<string> tokens;
        string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            continue;
        }

        if (!header_parsed) {
            // First meaningful line contains the number of vertices.
            if (tokens.size() >= 1) {
                num_vertices = stoi(tokens[0]);
            }
            header_parsed = true;
            graph.num_vertices = num_vertices;
            graph.adj.assign(num_vertices, vector<int>());
            continue;
        }

        if (tokens.size() >= 2) {
            int u = stoi(tokens[0]);
            int v = stoi(tokens[1]);
            // Accept either 0-indexed or 1-indexed vertex identifiers.
            if (u >= 1 && u <= num_vertices && v >= 1 && v <= num_vertices) {
                u -= 1;
                v -= 1;
            }
            if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
                cerr << "Warning: edge (" << tokens[0] << ", " << tokens[1] << ") is outside vertex range and will be ignored." << endl;
                continue;
            }
            if (u == v) {
                continue;
            }
            graph.adj[u].push_back(v);
            graph.adj[v].push_back(u);
        }
    }

    // Remove duplicate neighbors.
    for (auto& neighbors : graph.adj) {
        sort(neighbors.begin(), neighbors.end());
        neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }

    return graph;
}

/**
 * @brief Comprueba si asignar un color a un vértice es válido (ningún vértice adyacente tiene el mismo color).
 * @param vertex El vértice a colorear.
 * @param color El color a asignar.
 * @param colors Asignaciones de colores actuales.
 * @param graph La estructura del grafo.
 * @return true si la asignación de color es válida, false en caso contrario.
 */
bool is_color_valid(
    int vertex,
    int color,
    const vector<int>& colors,
    const Graph& graph
) {
    for (int neighbor : graph.adj[vertex]) {
        if (colors[neighbor] == color) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Cuenta el número de colores únicos utilizados en el coloreo.
 * @param colors Vector de asignaciones de colores.
 * @return Número de colores distintos utilizados.
 */
int count_used_colors(const vector<int>& colors) {
    set<int> unique_colors;
    for (int c : colors) {
        if (c >= 0) {
            unique_colors.insert(c);
        }
    }
    return static_cast<int>(unique_colors.size());
}

/**
 * @brief Realiza un coloreo voraz del grafo.
 * @param graph El grafo a colorear.
 * @return Vector de asignaciones de colores (uno por vértice).
 */
vector<int> greedy_coloring(const Graph& graph) {
    int n = graph.num_vertices;
    vector<int> colors(n, -1);
    vector<int> order(n);

    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return graph.adj[a].size() > graph.adj[b].size();
    });

    for (int vertex : order) {
        vector<bool> forbidden(n, false);
        for (int neighbor : graph.adj[vertex]) {
            if (colors[neighbor] != -1) {
                forbidden[colors[neighbor]] = true;
            }
        }
        int color = 0;
        while (color < n && forbidden[color]) {
            color += 1;
        }
        colors[vertex] = color;
    }
    return colors;
}

/**
 * @brief Intenta colorear el grafo usando backtracking con un máximo de max_colors.
 * @param graph El grafo a colorear.
 * @param max_colors Número máximo de colores permitidos.
 * @return ColoringResult con éxito, colores y estadísticas.
 */
ColoringResult backtracking_color_with_k(const Graph& graph, int max_colors) {
    ColoringResult result;
    result.colors.assign(graph.num_vertices, -1);
    // TODO: Implement a recursive backtracking search that assigns colors to each vertex
    // using at most `max_colors`. The algorithm should update `result.nodes_generated`,
    // `result.nodes_pruned` and `result.max_live_nodes`.
    return result;
}

class Solucion {
    public:
    Solucion(vector<vector<int>> & map, vector<int>b, int num_vertices) {
        size = num_vertices;
        colores.resize(size);
        nodos_podados = 0;
        nodos_generados = 0;
        nodos_vivos = 0;
        MdeMapa = map;
    }
    void IniciaComp(int k){
       colores[k] = -1; // valor NULO
    }
    void SigValComp(int k){
        colores[k]++; // Siguiente valor del dominio. -1->0->1->...->N
    }
    bool TodosGenerados(int k) const{
        return colores[k]==size; // END cuando llegue al final del vector (N)
    }
    bool Factible(int p_actual) {
        bool devolver = true;
        for (int i = 0; i < MdeMapa[p_actual].size() && devolver; i++) { //recorre los adyacentes de cada región
            if (colores[MdeMapa[p_actual][i]] == colores[p_actual]) {
                devolver = false;
            }
        }
        return devolver;
    }

    //int Solucion::Decision(int k) const;
    //Obtener valor componente k, return X[k]
    void ProcesaSolucion() {
        float coste = 0.0, beneficio = 0.0;
        cout << "{";
        for (int i = 0; i < size; i++) {
            cout << " " << (char)(i+'A');
            coste += c[i];
            beneficio += benef[i];
        }
        cout << " }\tcoste: " << coste << " " << "beneficio: " << beneficio << endl;
    }
    // Representa el proceso que se realiza cuando se alcanza una solución.
    //Permite quedarnos con la mejor solución


    void imprimeNodos() {
        cout << endl << "Nodos podados: " << nodos_podados << endl;
        cout << "Nodos generados: " << nodos_generados << endl;
    }

    int getSize() {
        return size;
    }

    void VueltaAtras(int k) {
        if (pais == colores.size()) {
            return;
        }
        colores[k] = 0;
    }

    void ActualizaSolucion() {

    }

    private:
        vector<int> colores; // X, aka soluciones posibles
        vector<vector<int>> MdeMapa; // coste
        int nodos_podados, nodos_generados, nodos_vivos;
        int size;
};

void bb_coloreo_grafos(Solucion & sol, int k) {
    queue<Solucion> cola;
    fin = false;
    cola.insert(sol);

    do {
        sol
    } while (!cola.empty());

    if (k == sol.getSize()) {
        sol.ProcesaSolucion();
    }
    else {
        sol.IniciaComp(k);
        sol.SigValComp(k);
        while (!sol.TodosGenerados(k)) {
            if (sol.Factible(k)) {
                bb_coloreo_grafos(sol, k);
                sol.VueltaAtras(k+1);
            }
            sol.SigValComp(k);
        }
    }
}


/**
 * @brief Encuentra el número mínimo de colores necesarios para colorear el grafo usando branch and bound.
 * @param graph El grafo a colorear.
 * @return ColoringResult con coloreo óptimo y estadísticas.
 */
ColoringResult branch_and_bound_min_colors(const Graph& graph) {
    ColoringResult result;
    result.colors.assign(graph.num_vertices, -1);
    // TODO: Implement a branch and bound search that finds the smallest number of colors
    // needed to color the graph. Use a greedy upper bound and a lower bound based on the
    // current partial assignment to prune the search.

    queue<Solucion> cola;
    fin = false;
    cola.insert(sol);
    /*
     * Metemos la raíz en la cola,
     * En el do-while, hacemos un nodo = raiz (sol = cola.front)
     * Si la raiz es factible (función de factibilidad)
     * pais k = sol.comp() (que sería el país primero a recorrer, asumo yo)
     * k++ (inicializa a 0)
     * for (mapa[0][i], mientras tenga paises adyacentes, i++)
     * si es factible (funcion de factibilidad): ver si es solución.
     * Si es solución, sol.actualizaSolucion() hace que las cotas se ajusten
     * si no es solución, se inserta como solución parcial en la cola.
     */
    do {
        sol = cola.front(); // definir
        if (sol.Factible()) { // factible distinto (no pos)
            k = sol.CompENodo();
            k++;
            for (sol.PrimerValorComp(k); sol.HayMasValores(k); sol.SigValComp(k)) {
                if (sol.Factible()) {
                    if (sol.NumComponentes()==sol.size()) {
                        sol.ActualizaSolucion();
                    } else {
                        cola.insert(sol);
                    }
                }
            }
        }
        cola.pop_front();
    } while (!cola.empty());

    return result;
}

/**
 * @brief Imprime la solución de coloreo en la consola.
 * @param result El ColoringResult a imprimir.
 * @param caption Título para la sección de salida.
 */
void print_coloring_solution(const ColoringResult& result, const string& caption) {
    cout << "=== " << caption << " ===" << endl;
    if (!result.success) {
        cout << "No valid coloring found." << endl;
        return;
    }
    cout << "Colors used: " << result.colors_used << endl;
    cout << "Nodes generated: " << result.nodes_generated << endl;
    cout << "Nodes pruned: " << result.nodes_pruned << endl;
    cout << "Max live nodes: " << result.max_live_nodes << endl;
    cout << "Color assignment:" << endl;
    for (int i = 0; i < static_cast<int>(result.colors.size()); ++i) {
        cout << "  vertex " << i << " -> color " << result.colors[i] << endl;
    }
}

/**
 * @brief Extrae el nombre de instancia de una ruta de archivo.
 * @param path La ruta del archivo.
 * @return El nombre base del archivo sin extensión.
 */
string get_instance_name(const string& path) {
    size_t last_slash = path.find_last_of("/\\");
    string filename = (last_slash == string::npos) ? path : path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of('.');
    return (last_dot == string::npos) ? filename : filename.substr(0, last_dot);
}

