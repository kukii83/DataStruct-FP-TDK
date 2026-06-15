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
#include <set>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {
    vector<Cell> visited;

    const int DR[] = {-1, 1, 0, 0};
    const int DC[] = { 0, 0,-1, 1};

    queue<Cell> q;
    set<Cell> enqueued;

    q.push(start);
    enqueued.insert(start);
    visited.push_back(start);

    while (!q.empty()) {
        Cell cur = q.front();
        q.pop();

        if (cur == goal) break;

        for (int d = 0; d < 4; d++) {
            int nr = cur.first  + DR[d];
            int nc = cur.second + DC[d];

            if (!inBounds(nr, nc))        continue;
            if (isWall(grid[nr][nc]))     continue;
            if (enqueued.count({nr, nc})) continue;

            Cell nx = {nr, nc};

            enqueued.insert(nx);
            came_from[nx] = cur;
            visited.push_back(nx);
            q.push(nx);
        }
    }

    return visited;
}
