#include "stdc++.h"
using namespace std;
int n, m, maze[1000][1000];
bool isVertex[1000][1000], visited[1000][1000];
int dirX[] = { 0, 0, 1, -1 }; // direction for x
int dirY[] = { 1, -1, 0, 0 }; // direction for y
struct Vertex {
  int id;
  int x, y; // original position
  vector<pair<int, int> > edges;
};
vector<Vertex> graph;
int calVertexId(int u, int v) {
  return (u - 1) * m + (v - 1); // calculate the id
}
void traversal(int curX, int curY, int weight, int from) {
  visited[curX][curY] = true;
  for (int i = 0; i < 4; i++) {
    int nextX = curX + dirX[i];
    int nextY = curY + dirY[i];
    if (maze[nextX][nextY] == 1) {
      if (isVertex[nextX][nextY] && calVertexId(nextX, nextY) != from) {
        int id = calVertexId(nextX, nextY);
        for (auto &it : graph) {
          if (it.id == id) {
            bool added = false;
            for (auto jt: it.edges) {
              if (jt.first == from) {
                added = true;
                break;
              }
            }
            if (!added) {
              it.edges.push_back({ from, weight + 1 });
            }
          }
          if (it.id == from) {
            bool added = false;
            for (auto jt : it.edges) {
              if (jt.first == id) {
                added = true;
                break;
              }
            }
            if (!added) {
              it.edges.push_back({ id, weight + 1 });
            }
          }
        }
        if (!visited[nextX][nextY]) {
          traversal(nextX, nextY, 0, id);
        }
      } else if (!visited[nextX][nextY]) {
        traversal(nextX, nextY, weight + 1, from);
      }
    }
  }
}
int main() {
  freopen("input.txt", "r", stdin);
  freopen("graph.txt", "w", stdout);
  cin >> n >> m;
  string s;
  int i, j;
  // . => 1 and # => 0
  for (i = 1; i <= n; i++) {
    cin >> s;
    for (j = 1; j <= m; j++) {
      if (s[j - 1] == '.') {
        maze[i][j] = 1;
      }
    }
  }
  // start and end points
  int stPos, enPos;
  cin >> stPos >> enPos;
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= m; j++) {
      // skip the wall
      if (maze[i][j] == 0) {
        continue;
      }
      bool isValid = false;
      // a valid vertex is either stPos or enPos or an intersection or the end of the tunnel
      if (stPos == calVertexId(i, j) || enPos == calVertexId(i, j)) {
        isValid = true; // stPos or enPos
      } else {
        // Intersection or end of the tunnel
        int totalWay = 0;
        for (int k = 0; k < 4; k++) {
          totalWay += maze[i + dirX[k]][j + dirY[k]] == 1 ? 1 : 0;
        }
        isValid = totalWay != 2 ? true : false;
      }
      if (isValid) { // a valid vertex => create and save in graph
        Vertex vt;
        vt.id = calVertexId(i, j); // calculate ID
        vt.x = i; // store original x
        vt.y = j; // store original y
        graph.push_back(vt); // store in graph vector
        isVertex[i][j] = true;
      }
    }
  }
  // explore the graph
  for (auto it : graph) {
    if (!visited[it.x][it.y]) { // traversal only the non visited
      traversal(it.x, it.y, 0, it.id);
    }
  }

  // print out the result
  for (auto it : graph) {
    cout << it.id << " ";
    for (auto jt: it.edges) {
      cout << "(" << jt.first << "," << jt.second << ") ";
    }
    cout << endl;
  }
  cout << "-" << endl; // separator
  cout << stPos << endl << enPos << endl;
  return 0;
}
