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
#include <map>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
  vector<Cell> visited;  // list of visited cells (ordered) — return this

  // ===================== WRITE YOUR ALGORITHM HERE =====================

    queue<Cell> qStart, qGoal;
    set<Cell> visStart, visGoal;

    map<Cell, Cell> parentStart;
    map<Cell, Cell> parentGoal;

    qStart.push(start);
    qGoal.push(goal);

    visStart.insert(start);
    visGoal.insert(goal);

    Cell meeting = {-1, -1};

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!qStart.empty() && !qGoal.empty()) {

        // Expand from start side
        int sz = qStart.size();
        while (sz--) {
            Cell cur = qStart.front();
            qStart.pop();

            visited.push_back(cur);

            if (visGoal.count(cur)) {
                meeting = cur;
                break;
            }

            for (int i = 0; i < 4; i++) {
                int nr = cur.first + dr[i];
                int nc = cur.second + dc[i];

                if (!inBounds(nr, nc) || isWall(grid[nr][nc]))
                    continue;

                Cell nxt = {nr, nc};

                if (!visStart.count(nxt)) {
                    visStart.insert(nxt);
                    parentStart[nxt] = cur;
                    qStart.push(nxt);
                }
            }
        }

        if (meeting != Cell{-1, -1})
            break;

        // Expand from goal side
        sz = qGoal.size();
        while (sz--) {
            Cell cur = qGoal.front();
            qGoal.pop();

            visited.push_back(cur);

            if (visStart.count(cur)) {
                meeting = cur;
                break;
            }

            for (int i = 0; i < 4; i++) {
                int nr = cur.first + dr[i];
                int nc = cur.second + dc[i];

                if (!inBounds(nr, nc) || isWall(grid[nr][nc]))
                    continue;

                Cell nxt = {nr, nc};

                if (!visGoal.count(nxt)) {
                    visGoal.insert(nxt);
                    parentGoal[nxt] = cur;
                    qGoal.push(nxt);
                }
            }
        }

        if (meeting != Cell{-1, -1})
            break;
    }

    // Reconstruct path for harness
    if (meeting != Cell{-1, -1}) {

        Cell cur = meeting;

        while (cur != start) {
            came_from[cur] = parentStart[cur];
            cur = parentStart[cur];
        }

        cur = meeting;

        while (cur != goal) {
            Cell nxt = parentGoal[cur];
            came_from[nxt] = cur;
            cur = nxt;
        }
    }

  // =========================================================================
  return visited;
}