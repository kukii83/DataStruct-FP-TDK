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
#include <climits>
#include <cmath>

static int h(Cell a, Cell b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

static const int DR[] = {-1, 1, 0, 0};
static const int DC[] = { 0, 0,-1, 1};

using PQItem = pair<int, Cell>;
using MinPQ  = priority_queue<PQItem, vector<PQItem>, greater<PQItem>>;

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
    vector<Cell> visited;

    map<Cell,int> g_fwd, g_bwd;
    map<Cell,Cell> par_fwd, par_bwd;
    map<Cell,bool> closed_fwd, closed_bwd;

    MinPQ open_fwd, open_bwd;

    g_fwd[start] = 0;
    g_bwd[goal] = 0;

    open_fwd.push({h(start, goal), start});
    open_bwd.push({h(goal, start), goal});

    int bestCost = INT_MAX;
    Cell meet = {-1,-1};

    while (!open_fwd.empty() && !open_bwd.empty()) {

        int fwdTop = open_fwd.top().first;
        int bwdTop = open_bwd.top().first;

        if (bestCost != INT_MAX &&
            fwdTop + bwdTop >= bestCost)
            break;

        // ---------- FORWARD ----------
        if (!open_fwd.empty()) {

            auto [f, cur] = open_fwd.top();
            open_fwd.pop();

            if (!closed_fwd[cur]) {

                closed_fwd[cur] = true;
                visited.push_back(cur);

                if (g_bwd.count(cur)) {
                    int candidate =
                        g_fwd[cur] + g_bwd[cur];

                    if (candidate < bestCost) {
                        bestCost = candidate;
                        meet = cur;
                    }
                }

                for (int d = 0; d < 4; d++) {

                    int nr = cur.first + DR[d];
                    int nc = cur.second + DC[d];

                    if (!inBounds(nr,nc) ||
                        isWall(grid[nr][nc]))
                        continue;

                    Cell nb = {nr,nc};

                    int newG =
                        g_fwd[cur] +
                        cellCost(grid[nr][nc]);

                    if (!g_fwd.count(nb) ||
                        newG < g_fwd[nb]) {

                        g_fwd[nb] = newG;
                        par_fwd[nb] = cur;

                        int fScore =
                            newG + h(nb, goal);

                        open_fwd.push({fScore, nb});
                    }
                }
            }
        }

        // ---------- BACKWARD ----------
        if (!open_bwd.empty()) {

            auto [f, cur] = open_bwd.top();
            open_bwd.pop();

            if (!closed_bwd[cur]) {

                closed_bwd[cur] = true;
                visited.push_back(cur);

                if (g_fwd.count(cur)) {
                    int candidate =
                        g_fwd[cur] + g_bwd[cur];

                    if (candidate < bestCost) {
                        bestCost = candidate;
                        meet = cur;
                    }
                }

                for (int d = 0; d < 4; d++) {

                    int nr = cur.first + DR[d];
                    int nc = cur.second + DC[d];

                    if (!inBounds(nr,nc) ||
                        isWall(grid[nr][nc]))
                        continue;

                    Cell nb = {nr,nc};

                    int newG =
                        g_bwd[cur] +
                        cellCost(grid[nr][nc]);

                    if (!g_bwd.count(nb) ||
                        newG < g_bwd[nb]) {

                        g_bwd[nb] = newG;
                        par_bwd[nb] = cur;

                        int fScore =
                            newG + h(nb, start);

                        open_bwd.push({fScore, nb});
                    }
                }
            }
        }
    }

    if (meet.first != -1) {

        Cell cur = meet;

        while (par_fwd.count(cur)) {
            came_from[cur] = par_fwd[cur];
            cur = par_fwd[cur];
        }

        cur = meet;

        while (par_bwd.count(cur)) {
            Cell nxt = par_bwd[cur];
            came_from[nxt] = cur;
            cur = nxt;
        }
    }

    return visited;
}