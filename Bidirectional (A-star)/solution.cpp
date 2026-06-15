#include "harness.h"
#include <queue>
#include <set>
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

    map<Cell, int>  g_fwd,  g_bwd;
    map<Cell, Cell> par_fwd, par_bwd;
    map<Cell, bool> closed_fwd, closed_bwd;
    MinPQ open_fwd, open_bwd;
    g_fwd[start] = 0;

    open_fwd.push({ max(h(start,goal), 0), start });

    g_bwd[goal] = 0;
    open_bwd.push({ max(h(goal,start), 0), goal });

    int best = INT_MAX;
    Cell meet = {-1, -1};

    auto priority_mm = [&](Cell n, int gn, Cell target) -> int {
        return max(gn + h(n, target), 2 * gn);
    };

    auto expand_one = [&](
        MinPQ&          open,
        map<Cell,int>&  g_mine,
        map<Cell,Cell>& par_mine,
        map<Cell,bool>& closed_mine,
        map<Cell,int>&  g_other,
        map<Cell,bool>& closed_other,
        Cell            my_origin,
        Cell            my_target
    ) -> bool {
        if (open.empty()) return false;

        auto [p, cur] = open.top();
        open.pop();

        if (closed_mine[cur]) return false;
        closed_mine[cur] = true;
        visited.push_back(cur);

        if (g_other.count(cur)) {
            int candidate = g_mine[cur] + g_other[cur];
            if (candidate < best) {
                best = candidate;
                meet = cur;
            }
        }

        for (int d = 0; d < 4; d++) {
            int nr = cur.first  + DR[d];
            int nc = cur.second + DC[d];
            if (!inBounds(nr, nc) || isWall(grid[nr][nc])) continue;

            Cell nb = {nr, nc};
            if (closed_mine[nb]) continue;

            int new_g = g_mine[cur] + cellCost(grid[nr][nc]);

            if (!g_mine.count(nb) || new_g < g_mine[nb]) {
                g_mine[nb]  = new_g;
                par_mine[nb] = cur;
                int p_nb = priority_mm(nb, new_g, my_target);
                open.push({p_nb, nb});

                if (g_other.count(nb)) {
                    int candidate = new_g + g_other[nb];
                    if (candidate < best) {
                        best = candidate;
                        meet = nb;
                    }
                }
            }
        }
        return true;
    };

    while (!open_fwd.empty() || !open_bwd.empty()) {

        int top_fwd = open_fwd.empty() ? INT_MAX : open_fwd.top().first;
        int top_bwd = open_bwd.empty() ? INT_MAX : open_bwd.top().first;
        int top_min = min(top_fwd, top_bwd);

        if (best != INT_MAX && top_min >= best) break;

        if (top_fwd <= top_bwd) {
            expand_one(open_fwd, g_fwd, par_fwd, closed_fwd,
                    g_bwd, closed_bwd, start, goal);
        } else {
            expand_one(open_bwd, g_bwd, par_bwd, closed_bwd,
                    g_fwd, closed_fwd, goal, start);
        }
    }

    if (meet.first != -1) {
        {
            Cell cur = meet;
            while (par_fwd.count(cur)) {
                Cell par = par_fwd[cur];
                came_from[cur] = par;
                cur = par;
            }
        }

        {
            Cell cur = meet;
            while (par_bwd.count(cur)) {
                Cell par = par_bwd[cur];
                came_from[par] = cur;
                cur = par;
            }
        }
    }
    return visited;
}
