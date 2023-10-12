#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring> // Para usar a função strcmp

using namespace std;

struct Edge {
    int from;
    int to;
    int weight;
};

struct Graph {
    int V;  // Número de vértices
    vector<Edge> edges;
};

bool readGraph(const char* filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return false;
    }

    file >> graph.V;

    int E;
    file >> E;

    for (int i = 0; i < E; ++i) {
        Edge edge;
        file >> edge.from >> edge.to >> edge.weight;
        graph.edges.push_back(edge);
    }

    file.close();
    return true;
}

struct DisjointSet {
    vector<int> parent, rank;

    DisjointSet(int n) {
        parent.resize(n);
        rank.assign(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unionSets(int x, int y) {
        x = find(x);
        y = find(y);

        if (x != y) {
            if (rank[x] < rank[y]) {
                swap(x, y);
            }
            parent[y] = x;
            if (rank[x] == rank[y]) {
                rank[x]++;
            }
        }
    }
};

bool edgeComparator(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

void kruskalMST(Graph& graph, vector<Edge>& minimumSpanningTree) {
    sort(graph.edges.begin(), graph.edges.end(), edgeComparator);
    
    DisjointSet ds(graph.V);

    for (Edge edge : graph.edges) {
        int from = edge.from;
        int to = edge.to;

        if (ds.find(from) != ds.find(to)) {
            minimumSpanningTree.push_back(edge);
            ds.unionSets(from, to);
        }
    }
}

void printMST(const vector<Edge>& minimumSpanningTree) {
    for (Edge edge : minimumSpanningTree) {
        cout << "(" << edge.from + 1 << "," << edge.to + 1 << ") ";
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

    if (filename == nullptr) {
        cerr << "Argumentos inválidos. Use -f <arquivo>" << endl;
        return 1;
    }

    Graph graph;
    if (!readGraph(filename, graph)) {
        return 1;
    }

    vector<Edge> minimumSpanningTree;
    kruskalMST(graph, minimumSpanningTree);

    if (outputFilename != nullptr) {
        ofstream outputFile(outputFilename);
        if (outputFile.is_open()) {
            if (isPrintMST) {
                printMST(minimumSpanningTree);
            }
            outputFile.close();
        } else {
            cerr << "Erro ao abrir o arquivo de saída." << endl;
            return 1;
        }
    } else {
        if (isPrintMST) {
            printMST(minimumSpanningTree);
        }
    }

    return 0;
}
