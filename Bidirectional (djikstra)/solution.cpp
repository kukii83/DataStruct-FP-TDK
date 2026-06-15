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
#include <vector>
#include <functional>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
  vector<Cell> visited;  // list of visited cells (ordered) — return this

  // ===================== WRITE YOUR ALGORITHM HERE =====================

    map<Cell,int> distS, distG;
    map<Cell,Cell> parentS, parentG;

    priority_queue<
        pair<int,Cell>,
        vector<pair<int,Cell>>,
        greater<pair<int,Cell>>
    > pqS, pqG;

    distS[start] = 0;
    distG[goal] = 0;

    pqS.push({0,start});
    pqG.push({0,goal});

    set<Cell> doneS, doneG;

    Cell meet = {-1,-1};

    int dr[] = {-1,1,0,0};
    int dc[] = {0,0,-1,1};

    while (!pqS.empty() && !pqG.empty()) {

        // Forward Dijkstra
        auto [dS, curS] = pqS.top();
        pqS.pop();

        if (doneS.count(curS))
            continue;

        doneS.insert(curS);
        visited.push_back(curS);

        if (doneG.count(curS)) {
            meet = curS;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = curS.first + dr[i];
            int nc = curS.second + dc[i];

            if (!inBounds(nr,nc) || isWall(grid[nr][nc]))
                continue;

            Cell nxt = {nr,nc};

            int nd = dS + cellCost(grid[nr][nc]);

            if (!distS.count(nxt) || nd < distS[nxt]) {
                distS[nxt] = nd;
                parentS[nxt] = curS;
                pqS.push({nd,nxt});
            }
        }

        // Backward Dijkstra
        auto [dG, curG] = pqG.top();
        pqG.pop();

        if (doneG.count(curG))
            continue;

        doneG.insert(curG);
        visited.push_back(curG);

        if (doneS.count(curG)) {
            meet = curG;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = curG.first + dr[i];
            int nc = curG.second + dc[i];

            if (!inBounds(nr,nc) || isWall(grid[nr][nc]))
                continue;

            Cell nxt = {nr,nc};

            int nd = dG + cellCost(grid[nr][nc]);

            if (!distG.count(nxt) || nd < distG[nxt]) {
                distG[nxt] = nd;
                parentG[nxt] = curG;
                pqG.push({nd,nxt});
            }
        }
    }

    // Reconstruct for harness
    if (meet != Cell{-1,-1}) {

        Cell cur = meet;

        while (cur != start) {
            came_from[cur] = parentS[cur];
            cur = parentS[cur];
        }

        cur = meet;

        while (cur != goal) {
            Cell nxt = parentG[cur];
            came_from[nxt] = cur;
            cur = nxt;
        }
    }

  // =========================================================================
  return visited;
}