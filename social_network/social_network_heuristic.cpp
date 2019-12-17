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
vector<bool> visited;
set<pair<int, int> > existedEdges;
default_random_engine randomGenerator; // required C++11 to run
int nCommunity;
int nMember;

void addVertex(Vertex *vertex) {
  vertices.push_back(vertex);
}

void addEdge(Vertex *from, Vertex *to, double weight) {
  int fromId = from->id;
  int toId = to->id;

  if (fromId > toId) {
    swap(fromId, toId);
  }

  if (existedEdges.count({fromId, toId}) > 0) {
    return;
  } else {
    existedEdges.insert({fromId, toId});
  }

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

void generateNetwork(int nCommunity, int nMember, int nTryConnect = 20) {
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // required C++11 to run
  // Generate nCommunity
  for (int i = 0; i < nCommunity; i++) {
    init(i, nMember);
    makeCommunity(i, nMember);
  }

  // Connect created communities randomly - try k times
  for (int k = 0; k < nTryConnect; k++) {
    for (int curCommunity = 0; curCommunity < nCommunity; curCommunity++) {
      // required C++11 to run
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

int heuristic(int id, int nTry, int nToCheck, bool goThroughVisisted, int increaseNTry) {
  vector<Vertex*> connectedVertices;
  for (auto edge : edges[id]) {
    if (!goThroughVisisted && (visited[edge->from->id] || visited[edge->to->id])) {
      continue;
    }
    if (edge->from->id != id) {
      connectedVertices.push_back(edge->from);
    } else {
      connectedVertices.push_back(edge->to);
    }
  }
  while (nToCheck > 1) {
    for (int i = 0; i < nTry; i++) {
      shuffle(connectedVertices.begin(), connectedVertices.end(), randomGenerator);
      connectedVertices.push_back(vertices[id]);
      vector<Vertex*> toCheck;
      for (int j = connectedVertices.size() - 1, k = 0; j >= 0 && k < nToCheck; j--, k++) {
        toCheck.push_back(connectedVertices[j]);
      }
      if (isCommunity(toCheck)) {
        for (auto vertex : toCheck) {
          visited[vertex->id] = true;
        }
        return toCheck.size();
      }
      connectedVertices.pop_back();
    }
    nToCheck--;
    increaseNTry += increaseNTry;
  }
  return 0;
}

void countCommunityWithHeuristic(int nToCheck = 10, int nTry = 10, bool randomStart = true, bool goThroughVisisted = false, int increaseNTry = 1) {
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // require C++11 to run

  // Init possible community size
  vector<int> communitySize;
  for (int i = 0; i <= nToCheck; i++) {
    communitySize.push_back(0);
  }

  vector<int> vertexIds;

  // Init visited vector
  for (auto vertex : vertices) {
    visited.push_back(false);
    vertexIds.push_back(vertex->id);
  }

  int totalCommunity = 0;

  if (randomStart) {
    shuffle(vertexIds.begin(), vertexIds.end(), randomGenerator);
  }

  // Loop through non-visited member
  for (auto vertex : vertexIds) {
    if (!visited[vertex]) {
      int cmSize = heuristic(vertex, nTry, nToCheck, goThroughVisisted, increaseNTry);
      if (cmSize >= 3) {
        communitySize[cmSize]++;
        totalCommunity++;
      }
    }
  }

  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  cout << "Found " << totalCommunity << " communities in " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
  int topSize = 0, top = 0;
  for (int i = communitySize.size() - 1, k = 0; i >= 0 && k < 3; i--, k++) {
    if (communitySize[i] == 0) {
      k--;
      continue;
    }
    topSize += communitySize[i];
    top++;
  }
  cout << "Top " << top << ": " << topSize << endl;
  for (int i = communitySize.size() - 1; i >= 0; i--) {
    if (communitySize[i] == 0) {
      continue;
    }
    cout << "Community with " << i << " members: " << communitySize[i] << endl; 
  }
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
  // Set up random seed
  randomGenerator.seed((unsigned)time(0));

  // Input
  nCommunity = 200;
  nMember = 15;
  int nTryConnect = 50;
  generateNetwork(nCommunity, nMember, nTryConnect);
  // printNetwork();

  // Set up hyperparameters
  int nToCheck = nMember;
  int nTry = 500;
  bool randomStart = true;
  bool goThroughVisisted = false;
  int increaseNTry = 5;
  countCommunityWithHeuristic(nToCheck, nTry, randomStart, goThroughVisisted, increaseNTry);

  destroy(); // clean up memory
  return 0;
}
