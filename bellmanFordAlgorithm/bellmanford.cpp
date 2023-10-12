#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <climits>

using namespace std;

// Estrutura para representar uma aresta com peso
struct Edge {
    int from;
    int to;
    int weight;
};

// Estrutura para representar um grafo
struct Graph {
    int V;  // Número de vértices
    vector<Edge> edges; // Lista de arestas
};

// Função para ler o grafo a partir de um arquivo
bool readGraph(const char* filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return false;
    }

    file >> graph.V;

    int E;
    file >> E;
    graph.edges.resize(E);

    for (int i = 0; i < E; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        graph.edges[i] = {u - 1, v - 1, w};
    }

    file.close();
    return true;
}

// Função para encontrar os menores caminhos usando o algoritmo de Bellman-Ford
bool bellmanFord(Graph& graph, int startVertex, vector<long long>& dist, vector<int>& parent) {
    dist.assign(graph.V, LLONG_MAX);
    dist[startVertex] = 0;
    parent.assign(graph.V, -1);

    for (int i = 0; i < graph.V - 1; i++) {
        for (const Edge& edge : graph.edges) {
            int u = edge.from;
            int v = edge.to;
            int weight = edge.weight;

            if (dist[u] != LLONG_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Verifica se há ciclos negativos
    for (const Edge& edge : graph.edges) {
        int u = edge.from;
        int v = edge.to;
        int weight = edge.weight;

        if (dist[u] != LLONG_MAX && dist[u] + weight < dist[v]) {
            return false; // Ciclo negativo
        }
    }

    return true;
}

// Imprimir os menores caminhos
void printShortestPaths(const vector<long long>& dist, const vector<int>& parent, int startVertex) {
    for (int i = 0; i < dist.size(); i++) {
        if (i != startVertex) {
            cout << "Caminho de " << startVertex + 1 << " para " << i + 1 << ": ";
            vector<int> path;
            int cur = i;
            while (cur != -1) {
                path.push_back(cur);
                cur = parent[cur];
            }
            for (int j = path.size() - 1; j >= 0; j--) {
                cout << path[j] + 1;
                if (j > 0) {
                    cout << " -> ";
                }
            }
            cout << " (Custo: " << (dist[i] == LLONG_MAX ? -1 : dist[i]) << ")" << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    const char* programName = argv[0];
    const char* filename = nullptr;
    const char* outputFilename = nullptr;
    int startVertex = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0) {
            outputFilename = argv[i + 1];
        } else if (strcmp(argv[i], "-i") == 0) {
            startVertex = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-h") == 0) {
            cout << "Possíveis argumentos: " << programName << " -f input.dat \n[-o output.dat] \n[-s] \n[-i <vértice inicial>] \n[-h]" << endl;
            return 0;
        }
    }

    if (filename == nullptr || startVertex == -1) {
        cerr << "Argumentos inválidos. Use -f <arquivo> -i <vértice inicial>" << endl;
        return 1;
    }

    Graph graph;
    if (!readGraph(filename, graph)) {
        return 1;
    }

    vector<long long> dist;
    vector<int> parent;

    if (bellmanFord(graph, startVertex - 1, dist, parent)) {
        if (outputFilename != nullptr) {
            ofstream outputFile(outputFilename);
            if (outputFile.is_open()) {
                printShortestPaths(dist, parent, startVertex - 1);
                outputFile.close();
            } else {
                cerr << "Erro ao abrir o arquivo de saída." << endl;
                return 1;
            }
        } else {
            printShortestPaths(dist, parent, startVertex - 1);
        }
    } else {
        cout << "Ciclo negativo detectado. O algoritmo de Bellman-Ford não pode ser aplicado neste grafo." << endl;
    }

    return 0;
}
