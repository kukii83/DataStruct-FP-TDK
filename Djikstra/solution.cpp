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
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
  vector<Cell> visited;  // list of visited cells (ordered) — return this

  // ===================== WRITE YOUR ALGORITHM HERE =====================

    // Distance table
    map<Cell, int> dist;

    // Min heap: {distance, cell}
    priority_queue<
        pair<int, Cell>,
        vector<pair<int, Cell>>,
        greater<pair<int, Cell>>
    > pq;

    dist[start] = 0;
    pq.push({0, start});

    // 4-direction movement
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto [curDist, cur] = pq.top();
        pq.pop();

        // Ignore outdated entries
        if (curDist > dist[cur])
            continue;

        visited.push_back(cur);

        if (cur == goal)
            break;

        for (int i = 0; i < 4; i++) {
            int nr = cur.first + dr[i];
            int nc = cur.second + dc[i];

            if (!inBounds(nr, nc))
                continue;

            if (isWall(grid[nr][nc]))
                continue;

            Cell next = {nr, nc};

            int newDist = curDist + cellCost(grid[nr][nc]);

            if (!dist.count(next) || newDist < dist[next]) {
                dist[next] = newDist;
                came_from[next] = cur;
                pq.push({newDist, next});
            }
        }
    }

  // =========================================================================
  return visited;
}