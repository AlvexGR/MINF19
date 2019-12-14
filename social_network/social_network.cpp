#include "stdc++.h"
using namespace std;
class Vertex {
public:
  int id;

  Vertex(int _id) {
    id = _id;
  }
};

class Edge {
public:
  Vertex *from;
  Vertex *to;
  double weight;

  Edge(Vertex *_from, Vertex *_to, double _weight) {
    from = _from;
    to = _to;
    weight = _weight;
  }
};

vector<Vertex*> vertices;
vector<vector<Edge*> > edges;
default_random_engine randomGenerator; // require C++11 to run
int totalCommunity = 0;

void addVertex(Vertex *vertex) {
  vertices.push_back(vertex);
}

void addEdge(Vertex *from, Vertex *to, double weight) {
  edges[from->id].push_back(new Edge(from, to, weight));
  edges[to->id].push_back(new Edge(to, from, weight));
}

void init(int baseId, int nMember) {
  for (int i = 0; i < nMember; i++) {
    vertices.push_back(new Vertex(baseId * nMember + i));
    vector<Edge*> initEdges;
    edges.push_back(initEdges);
  }
}

void makeCommunity(int baseId, int nMember) {
  for (int i = 0; i < nMember - 1; i++) {
    for (int j = i + 1; j < nMember; j++) {
      addEdge(vertices[baseId * nMember + i], vertices[baseId * nMember + j], 1);
    }
  }
}

void generateNetwork(int nCommunity, int nMember) {
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // require C++11 to run
  // Generate nCommunity
  for (int i = 0; i < nCommunity; i++) {
    init(i, nMember);
    makeCommunity(i, nMember);
  }

  // Connect created communities randomly
  for (int curCommunity = 0; curCommunity < nCommunity; curCommunity++) {
    // require C++11 to run
    uniform_int_distribution<int> randCommunity(0, nCommunity - 1); // random from 0 to nCommunity
    int someCommunity = randCommunity(randomGenerator);
    while (curCommunity == someCommunity) {
      // two communities must be different
      someCommunity = randCommunity(randomGenerator);
    }

    uniform_int_distribution<int> randMember(0, nMember - 1); // random from 0 to nMember
    addEdge(
      vertices[curCommunity * nMember + randMember(randomGenerator)],
      vertices[someCommunity * nMember + randMember(randomGenerator)],
      1
    );
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  cout << "Generate Network with " << nCommunity << " communities x " << nMember << " members";
  cout << " in " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
}

void printNetwork() {
  for (int i = 0; i < edges.size(); i++) {
    cout << "Member " << i << " connects to ";
    for (auto edge : edges[i]) {
      cout << "[" << edge->from->id << " -> " << edge->to->id << "] ";
    }
    cout << endl;
  }
}

bool isCommunity(vector<Vertex*> &toCheck) {
  if (toCheck.size() < 3) {
    return false;
  }

  set<int> setVertexIds;
  for (auto vertex : toCheck) {
    setVertexIds.insert(vertex->id);
  }
  for (auto vertex : toCheck) {
    int counter = 0;
    for (auto edge : edges[vertex->id]) {
      if (edge->from->id != vertex->id && setVertexIds.count(edge->from->id) != 0) {
        counter++;
      }
      if (edge->to->id != vertex->id && setVertexIds.count(edge->to->id) != 0) {
        counter++;
      }
    }
    if (counter != toCheck.size() - 1) {
      return false;
    }
  }
  return true;
}

int generateCombination(vector<Vertex*> &curVertices, int curIndex = 0) {
  int totalCommunity = isCommunity(curVertices) ? 1 : 0;

  // Print out combination
  // for (auto vertex : curVertices) {
  //   cout << vertex->id << " ";
  // }
  // cout << endl;

  for (int i = curIndex; i < vertices.size(); i++) {
    curVertices.push_back(vertices[i]);
    totalCommunity += generateCombination(curVertices, i + 1);
    curVertices.pop_back();
  }
  return totalCommunity;
}

void countCommunityBruteForce() {
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // require C++11 to run
  vector<Vertex*> curVertices;
  int totalCommunity = generateCombination(curVertices);
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  cout << "Found " << totalCommunity << " communities in " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
}

void countCommunityWithHeuristic() {

}

void destroy() {
  cout << "Cleaning up...!!!" << endl;
  for (auto vertex : vertices) {
    for (auto edge : edges[vertex->id]) {
      delete edge;
    }
    delete vertex;
  }
  vertices.clear();
  edges.clear();
  cout << "Done!" << endl;
}

int main() {
  int nCommunity = 5;
  int nMember = 3;
  generateNetwork(nCommunity, nMember);
  //printNetwork();

  //countCommunityBruteForce(); // It's so bad...!!!
  countCommunityWithHeuristic();

  destroy(); // clean up memory
  return 0;
}
