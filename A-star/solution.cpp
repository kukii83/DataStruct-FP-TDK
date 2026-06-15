// solution.cpp — THIS IS WHAT YOU WILL WORK ON.
//
// Write your algorithm ONLY inside the solve() function below.
// Do not add other functions. Do not modify harness.h.
//
// Compile: g++ -O2 -std=c++17 solution.cpp -o solution
// Run:     ./solution ../maps/soal1.txt output.txt
//          ./solution ../maps/soal2.txt output.txt
//          ./solution ../maps/soal3.txt output.txt
// View:    open engine/index.html, select the question/map, drag output.txt, click Play/Run.

#include "harness.h"
#include <queue>
#include <map>
#include <set>
#include <cmath>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
  vector<Cell> visited;  // list of visited cells (ordered) — return this

  int dr[] = {-1, 1, 0, 0};
  int dc[] = { 0, 0,-1, 1};

  auto heuristic = [&](Cell c) -> int {
    return abs(c.first - goal.first) + abs(c.second - goal.second);
  };

  using Entry = pair<int, Cell>;
  priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

  map<Cell, int> g_cost;
  set<Cell> closed;

  g_cost[start] = 0;
  pq.push({heuristic(start), start});
  came_from[start] = start;

  while (!pq.empty()) {
    auto [f, cur] = pq.top();
    pq.pop();

    if (closed.count(cur)) continue;
    closed.insert(cur);
    visited.push_back(cur);

    if (cur == goal) break;

    for (int k = 0; k < 4; k++) {
      int nr = cur.first  + dr[k];
      int nc = cur.second + dc[k];

      if (!inBounds(nr, nc)) continue;
      if (isWall(grid[nr][nc])) continue;

      Cell nx = {nr, nc};
      if (closed.count(nx)) continue;

      int new_g = g_cost[cur] + cellCost(grid[nr][nc]);

      if (!g_cost.count(nx) || new_g < g_cost[nx]) {
        g_cost[nx] = new_g;
        int f_new = new_g + heuristic(nx);
        came_from[nx] = cur;
        pq.push({f_new, nx});
      }
    }
  }
  return visited;
}
