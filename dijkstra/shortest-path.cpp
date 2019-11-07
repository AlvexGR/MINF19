#include "/Users/Alvex/Work/CP/bits/stdc++.h"
using namespace std;
struct Vertex {
  int id;
  vector<pair<Vertex*, int> > edges;
};
priority_queue<pair<int, Vertex*> > pq; // processing queue
map<Vertex*, int> dist;
map<Vertex*, Vertex*> parent;
vector<Vertex*> vertices;
Vertex* createVertex(int id) {
  Vertex *vertex = new Vertex;
  vertex->id = id;
  return vertex;
}
void buildGraph(Vertex* source, string s) {
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '(') {
      int nextVertex = 0;
      int weight = 0;
      i++;
      while (s[i] >= '0' && s[i] <= '9') {
        nextVertex *= 10;
        nextVertex += (int)(s[i] - '0');
        i++;
      }
      i++;
      while (s[i] >= '0' && s[i] <= '9') {
        weight *= 10;
        weight += (int)(s[i] - '0');
        i++;
      }
      for (auto vertex : vertices) {
        if (vertex->id == nextVertex) {
          source->edges.push_back({ vertex, weight });
          break;
        }
      }
    }
  }
}
void destroyGraph() {
  for (auto vertex : vertices) {
    delete vertex;
  }
  vertices.clear();
}
void dijkstra(Vertex* source, Vertex* destination) {
  pq.push({ 0, source });
  dist[source] = 0;
  parent[source] = source;
  while (!pq.empty()) {
    Vertex* currentVertex = pq.top().second;
    pq.pop();
    for (auto edges : currentVertex->edges) {
      Vertex* nextVertex = edges.first;
      int weight = edges.second;
      if (dist[nextVertex] > dist[currentVertex] + weight) {
        dist[nextVertex] = dist[currentVertex] + weight;
        parent[nextVertex] = currentVertex;
        pq.push({ -dist[nextVertex], nextVertex });
      }
    }
  }
}
vector<int> shortestPath;
void getPath(Vertex* vertex) {
  shortestPath.push_back(vertex->id);
  if (vertex->id == parent[vertex]->id) {
    return;
  }
  getPath(parent[vertex]);
}
int main() {
  // Read graph file
  freopen("graph.txt", "r", stdin);
  freopen("shortest-path.txt", "w", stdout);
  string input;
  bool hasDash = false;
  vector<string> graph;
  while (getline(cin, input)) {
    if (input == "-") {
      break;
    }
    graph.push_back(input);
  }
  int stPos, enPos;
  cin >> stPos >> enPos;
  Vertex* source, * destination;
  for (auto it : graph) {
    int vt = 0;
    for (auto c : it) {
      if (c == ' ') {
        break;
      }
      vt *= 10;
      vt += (int)(c - '0');
    }
    Vertex* vertex = createVertex(vt);
    vertices.push_back(vertex);
    dist[vertex] = 9999999;
    if (vertex->id == stPos) {
      source = vertex;
    }
    if (vertex->id == enPos) {
      destination = vertex;
    }
  }
  for (int i = 0; i < vertices.size(); i++) {
    buildGraph(vertices[i], graph[i]);
  }
  dijkstra(source, destination);
  getPath(destination);
  reverse(shortestPath.begin(), shortestPath.end());
  for (int i = 0; i < shortestPath.size(); i++) {
    cout << shortestPath[i];
    if (i != shortestPath.size() - 1) {
      cout << '-';
    } else {
      cout << endl;
    }
  }
  cout << dist[destination] << endl;
  destroyGraph();
  return 0;
}
