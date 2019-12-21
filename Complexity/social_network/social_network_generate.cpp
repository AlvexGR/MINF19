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
default_random_engine randomGenerator; // required C++11 to run
int nCommunity;
int nMember;

void addVertex(Vertex *vertex) {
  vertices.push_back(vertex);
}

void addEdge(Vertex *from, Vertex *to, double weight) {
  edges[from->id].push_back(new Edge(from, to, weight));
  edges[to->id].push_back(new Edge(to, from, weight));
}

void init(int baseId, int nMember, vector<Vertex*> &tmpVertices) {
  for (int i = 0; i < nMember; i++) {
    vertices.push_back(new Vertex(baseId * nMember + i));
    tmpVertices.push_back(vertices.back());
    vector<Edge*> initEdges;
    edges.push_back(initEdges);
  }
}

void makeCommunity(vector<Vertex*> &tmpVertices, int from, int to) {
  for (int i = from; i < to - 1; i++) {
    for (int j = i + 1; j < to; j++) {
      addEdge(vertices[tmpVertices[i]->id], vertices[tmpVertices[j]->id], 1);
    }
  }
}

void generateNetwork(int nCommunity, int nMember) {
  chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); // required C++11 to run
  // Generate nCommunity
  vector<Vertex*> tmpVertices;
  for (int i = 0; i < nCommunity; i++) {
    init(i, nMember, tmpVertices);
  }

  // Shuffle the entire vector randomly
  shuffle(tmpVertices.begin(), tmpVertices.end(), randomGenerator);

  // Make community for continuous segments of nMember
  for (int i = 0; i < vertices.size(); i += nMember) {
    makeCommunity(tmpVertices, i, i + nMember);
  }

  // Store existed edges
  set<pair<int, int> > existedEdges;

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
        if (stId > ndId) {
          swap(stId, ndId);
        }
        // retry until find non existed edge O(LogN)
      } while (existedEdges.count({stId, ndId}) > 0);

      existedEdges.insert({stId, ndId});
      addEdge(vertices[stId], vertices[ndId], 1);
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

bool checkDuplicateEdges() {
  set<pair<int, int> > existedEdges;
  for (auto vertex : vertices) {
    for (auto edge : edges[vertex->id]) {
      if (existedEdges.count({edge->from->id, edge->to->id}) == 0) {
        existedEdges.insert({edge->from->id, edge->to->id});
      } else {
        return false;
      }
    }
  }
  return true;
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
  // set up random seed
  randomGenerator.seed((unsigned)time(0));

  nCommunity = 50;
  nMember = 50;
  generateNetwork(nCommunity, nMember);
  //printNetwork();

  // string dup = checkDuplicateEdges() ? "YES" : "NO";
  // cout << "Duplicate: " << dup << endl;

  destroy(); // clean up memory
  return 0;
}
