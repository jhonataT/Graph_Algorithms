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
        graph.adj[v - 1].push_back({u - 1, w});
    }

    file.close();
    return true;
}

// Função para encontrar a AGM usando o algoritmo de Prim
long long primMST(Graph& graph, int startVertex, vector<int>& parent) {
    vector<bool> inMST(graph.V, false);
    vector<long long> key(graph.V, LONG_LONG_MAX);

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    key[startVertex] = 0;
    pq.push({0, startVertex});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        inMST[u] = true;

        for (const Edge& edge : graph.adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    long long totalWeight = 0;
    for (int i = 0; i < graph.V; i++) {
        if (i != startVertex) {
            totalWeight += key[i];
        }
    }

    return totalWeight;
}

// Função para imprimir a árvore geradora mínima
void printMST(const vector<int>& parent, const Graph& graph) {
    for (int i = 0; i < graph.V; i++) {
        if (parent[i] != -1) {
            cout << "(" << i + 1 << "," << parent[i] + 1 << ") ";
        }
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    const char* programName = argv[0];
    const char* filename = nullptr;
    const char* outputFilename = nullptr;
    bool isPrintMST = false;
    int startVertex = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0) {
            outputFilename = argv[i + 1];
        } else if (strcmp(argv[i], "-s") == 0) {
            isPrintMST = true;
        } else if (strcmp(argv[i], "-i") == 0) {
            startVertex = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-h") == 0) {
            cout << "Possíveis argumentos: " << programName << " -f input.dat \n[-o output.dat] \n[-s] \n[-i <vértice inicial>] \n[-h]" << endl;
            return 0;
        }
    }

    if (filename == nullptr || startVertex == -1) {
        cerr << "Argumentos inválidos. Use -f <arquivo> -i <vértice inicial> [-s]" << endl;
        return 1;
    }

    Graph graph;
    if (!readGraph(filename, graph)) {
        return 1;
    }

    vector<int> parent(graph.V, -1);
    long long cost = primMST(graph, startVertex - 1, parent);

    if (outputFilename != nullptr) {
        ofstream outputFile(outputFilename);
        if (outputFile.is_open()) {
            if (!isPrintMST) {
                outputFile << "Custo da AGM: " << cost << endl;
            }
            if (isPrintMST) {
                outputFile << "Arestas da AGM:" << endl;
                for (int i = 0; i < graph.V; i++) {
                    if (parent[i] != -1) {
                        outputFile << "(" << i + 1 << "," << parent[i] + 1 << ") ";
                    }
                }
                outputFile << endl;
            }
            outputFile.close();
        } else {
            cerr << "Erro ao abrir o arquivo de saída." << endl;
            return 1;
        }
    } else {
        if (!isPrintMST) {
            cout << "Custo da AGM: " << cost << endl;
        }
        if (isPrintMST) {
            cout << "Arestas da AGM:" << endl;
            for (int i = 0; i < graph.V; i++) {
                if (parent[i] != -1) {
                    cout << "(" << i + 1 << "," << parent[i] + 1 << ") ";
                }
            }
            cout << endl;
        }
    }

    return 0;
}
