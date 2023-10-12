#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <climits>

using namespace std;

// Estrutura para representar uma aresta com peso
struct Edge {
    int to;
    int weight;
};

// Estrutura para representar um grafo
struct Graph {
    int V;  // Número de vértices
    vector<vector<Edge>> adj; // Lista de adjacência
};

// Função para ler o grafo a partir de um arquivo
bool readGraph(const char* filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return false;
    }

    file >> graph.V;
    graph.adj.resize(graph.V);

    int E;
    file >> E;

    for (int i = 0; i < E; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        graph.adj[u - 1].push_back({v - 1, w});
    }

    file.close();
    return true;
}

// Função para encontrar o caminho mínimo usando o algoritmo de Dijkstra
void dijkstra(const Graph& graph, int startVertex, vector<int>& distance) {
    distance.resize(graph.V, INT_MAX);
    distance[startVertex] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, startVertex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int dist_u = pq.top().first;
        pq.pop();

        if (dist_u != distance[u]) {
            continue;
        }

        for (const Edge& edge : graph.adj[u]) {
            int v = edge.to;
            int weight = edge.weight;

            if (distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                pq.push({distance[v], v});
            }
        }
    }
}

int main(int argc, char* argv[]) {
    const char* programName = argv[0];
    const char* filename = nullptr;
    int startVertex = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
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

    vector<int> distance;
    dijkstra(graph, startVertex - 1, distance);

    for (int i = 0; i < graph.V; i++) {
        if (distance[i] == INT_MAX) {
            cout << i + 1 << ":-1 ";
        } else {
            cout << i + 1 << ":" << distance[i] << " ";
        }
    }
    cout << endl;

    return 0;
}
