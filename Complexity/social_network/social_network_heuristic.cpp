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
default_random_engine randomGenerator; // required C++11 to run
set<pair<int, int> > existedEdges;
int nCommunity;
int nMember;

void addVertex(Vertex *vertex) {
  vertices.push_back(vertex);
}

bool addEdge(Vertex *from, Vertex *to, double weight) {
  int fromId = from->id;
  int toId = to->id;
  if (fromId > toId) {
    swap(fromId, toId);
  }

  // O(LogN)
  if (existedEdges.count({fromId, toId}) != 0) {
    return false;
  }

  existedEdges.insert({fromId, toId});
  edges[from->id].push_back(new Edge(from, to, weight));
  edges[to->id].push_back(new Edge(to, from, weight));
  return true;
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
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // required C++11 to run
  // Generate nCommunity
  for (int i = 0; i < nCommunity; i++) {
    init(i, nMember);
    makeCommunity(i, nMember);
  }

  // Try to connect randomly for nConnect time
  // BE AWARE: Change this number if the network is too big
  double percentile = 0.15; // Connect communities for 15% of the entire graph
  int nConnect = min(500, (int)(nMember * nCommunity * percentile));

  for (int k = 0; k < nConnect; k++) {
    for (int curCommunity = 0; curCommunity < nCommunity; curCommunity++) {
      // required C++11 to run
      uniform_int_distribution<int> randCommunity(0, nCommunity - 1); // random from 0 to nCommunity
      int someCommunity = randCommunity(randomGenerator);
      while (curCommunity == someCommunity) {
        // two communities must be different
        someCommunity = randCommunity(randomGenerator);
      }

      uniform_int_distribution<int> randMember(0, nMember - 1); // random from 0 to nMember

      int stId = 0, ndId = 0;
      do {
        stId = curCommunity * nMember + randMember(randomGenerator);
        ndId = someCommunity * nMember + randMember(randomGenerator);
      } while (!addEdge(vertices[stId], vertices[ndId], 1));
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

bool isCommunity(vector<Vertex*> &toCheck, int from, int to) {
  if (toCheck.size() < 3 || to - from < 3) {
    return false;
  }

  set<int> setVertexIds;
  for (int i = from; i < to; i++) {
    setVertexIds.insert(toCheck[i]->id);
  }
  for (int i = from; i < to; i++) {
    int counter = 0;
    for (auto edge : edges[toCheck[i]->id]) {
      if (edge->from->id != toCheck[i]->id && setVertexIds.count(edge->from->id) != 0) {
        counter++;
      }
      if (edge->to->id != toCheck[i]->id && setVertexIds.count(edge->to->id) != 0) {
        counter++;
      }
    }

    if (counter != setVertexIds.size() - 1) {
      return false;
    }
  }
  return true;
}

int heuristic(int id, int nTry, int nToCheck, bool goThroughVisited, int increaseNTry) {
  vector<Vertex*> connectedVertices;
  for (auto edge : edges[id]) {
    if (!goThroughVisited && (visited[edge->from->id] || visited[edge->to->id])) {
      continue;
    }
    if (edge->from->id != id) {
      connectedVertices.push_back(edge->from);
    } else {
      connectedVertices.push_back(edge->to);
    }
  }

  while (nToCheck > 2) {
    if (nToCheck <= connectedVertices.size() + 1) {
      for (int i = 0; i < (nToCheck == connectedVertices.size() + 1 ? 1 : nTry); i++) {
        shuffle(connectedVertices.begin(), connectedVertices.end(), randomGenerator);
        bool qualified = true;
        for (int j = connectedVertices.size() - 1, k = 0; j >= 0 && k < nToCheck - 1; j--, k++) {
          if (edges[connectedVertices[j]->id].size() < nToCheck) {
            qualified = false;
            break;
          }
        }
        if (!qualified) {
          continue;
        }
        connectedVertices.push_back(vertices[id]);
        int from = connectedVertices.size() - nToCheck;
        int to = connectedVertices.size();
        if (isCommunity(connectedVertices, from, to)) {
          for (int j = from; j < to; j++) {
            visited[connectedVertices[j]->id] = true;
          }
          return to - from;
        }
        connectedVertices.pop_back();
      }
    }
    nToCheck--;
    nTry += increaseNTry;
  }
  return 0;
}

void countCommunityWithHeuristic(int nToCheck = 10, int nTry = 10, bool goThroughVisited = false, int increaseNTry = 1) {
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // require C++11 to run

  // Init possible community size
  vector<int> communitySize;
  for (int i = 0; i <= nToCheck; i++) {
    communitySize.push_back(0);
  }

  // Init visited vector
  for (auto vertex : vertices) {
    visited.push_back(false);
  }

  int totalCommunity = 0;
  // Loop through non-visited member
  for (auto vertex : vertices) {
    if (!visited[vertex->id]) {
      int cmSize = heuristic(vertex->id, nTry, nToCheck, goThroughVisited, increaseNTry);
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
  nCommunity = 50;
  nMember = 100;
  generateNetwork(nCommunity, nMember);
  // printNetwork();

  // Set up hyperparameters
  int nToCheck = nMember; // How large the community to check
  int nTry = 1000; // How many tries to find
  bool goThroughVisited = false; // allow multiple communities to have multiple common members
  int increaseNTry = 0; // increase nTry after every nToCheck
  countCommunityWithHeuristic(nToCheck, nTry, goThroughVisited, increaseNTry);

  destroy(); // clean up memory
  return 0;
}
