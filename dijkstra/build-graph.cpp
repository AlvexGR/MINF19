#include "/Users/Alvex/Work/CP/bits/stdc++.h"
using namespace std;
int n, m, maze[1000][1000];
bool isVertex[1000][1000], visited[1000][1000];
int dirX[] = { 0, 0, 1, -1 };
int dirY[] = { 1, -1, 0, 0 };
struct Vertex {
  int id;
  int x, y; // original position
  vector<pair<int, int> > edges;
};
vector<Vertex> graph;
int calIntersection(int u, int v) {
  return (u - 1) * m + (v - 1);
}
void traversal(int curX, int curY, int weight, int from) {
  visited[curX][curY] = true;
  for (int i = 0; i < 4; i++) {
    int nextX = curX + dirX[i];
    int nextY = curY + dirY[i];
    if (maze[nextX][nextY] == 1) {
      if (isVertex[nextX][nextY] && calIntersection(nextX, nextY) != from) {
        int id = calIntersection(nextX, nextY);
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
  // start and end point
  int stPos, enPos;
  cin >> stPos >> enPos;
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= m; j++) {
      // skip the wall
      if (maze[i][j] == 0) {
        continue;
      }
      bool isValid = false;
      if (stPos == calIntersection(i, j) || enPos == calIntersection(i, j)) {
        isValid = true;
      } else {
        int totalWay = 0;
        for (int k = 0; k < 4; k++) {
          totalWay += maze[i + dirX[k]][j + dirY[k]] == 1 ? 1 : 0;
        }
        isValid = totalWay != 2 ? true : false;
      }
      if (isValid) {
        Vertex vt;
        vt.id = calIntersection(i, j);
        vt.x = i;
        vt.y = j;
        graph.push_back(vt);
        isVertex[i][j] = true;
      }
    }
  }
  // explore the maze
  for (auto it : graph) {
    if (!visited[it.x][it.y]) {
      traversal(it.x, it.y, 0, it.id);
    }
  }
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
