#include "harness.h"
#include <stack>
#include <set>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
  vector<Cell> visited;  // list of visited cells (ordered) — return this

  // Four movement directions (up, down, left, right):
  int dr[] = {-1, 1, 0, 0};
  int dc[] = { 0, 0,-1, 1};

  // Stack untuk DFS: menyimpan cell yang akan diproses
  stack<Cell> stk;
  // Set untuk melacak cell yang sudah dikunjungi agar tidak diproses ulang
  set<Cell> seen;

  stk.push(start);
  seen.insert(start);

  while (!stk.empty()) {
    Cell cur = stk.top();
    stk.pop();

    // Catat cell yang sedang diproses ke dalam trace eksplorasi
    visited.push_back(cur);

    // Jika sudah mencapai goal, berhenti
    if (cur == goal) break;

    // Eksplorasi 4 arah tetangga (atas, bawah, kiri, kanan)
    // Dibalik agar urutan eksplorasi sesuai (kanan diproses lebih dulu karena stack LIFO)
    for (int k = 3; k >= 0; k--) {
      int nr = cur.first  + dr[k];
      int nc = cur.second + dc[k];

      // Lewati jika di luar batas, tembok, atau sudah dikunjungi
      if (!inBounds(nr, nc)) continue;
      if (isWall(grid[nr][nc])) continue;
      if (seen.count({nr, nc})) continue;

      Cell nx = {nr, nc};
      seen.insert(nx);         // tandai sebagai sudah ditemukan
      came_from[nx] = cur;     // catat parent untuk rekonstruksi path
      stk.push(nx);            // masukkan ke stack untuk diproses nanti
    }
  }
  return visited;
}
