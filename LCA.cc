#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

int n, u, v;
vector<int> parent, depth;
vector<vector<int>> nodes;

class LCA {
public:
  int k;
  vector<int> parent, depth;
  vector<vector<int>> exp_parent;
  LCA(vector<int> p, vector<int> d) : parent(p), depth(d) {
    exp_parent.assign((int)parent.size(), vector<int>(21, -1));

    for (int i = 0 ; i < (int)parent.size() ; ++i) {
      exp_parent[i][0] = parent[i];
    }

    k = 0;
    for (int i = 0 ; (1 << i) < (int)parent.size() ; ++i) {
      ++k;
    }

    for (int i = 0 ; i < k ; ++i) {
      for (int j = 0 ; j < (int)parent.size() ; ++j) {
        if (exp_parent[j][i] == -1) {
          exp_parent[j][i + 1] = -1;
        } else {
          exp_parent[j][i + 1] = exp_parent[exp_parent[j][i]][i];
        }
      }
    }
  }

  int query(int u, int v) {
    if (depth[u] > depth[v]) {
      swap(u, v);
    }

    int diff = depth[v] - depth[u];
    for (int i = 0 ; i < k ; ++i) {
      if (diff & (1 << i)) {
        v = exp_parent[v][i];
      }
    }

    if (u == v) {
      return u;
    }

    for (int i = k - 1 ; i >=0 ; --i) {
      if (exp_parent[u][i] != exp_parent[v][i]) {
        u = exp_parent[u][i];
        v = exp_parent[v][i];
      }
    }

    return exp_parent[u][0];
  }
};

void dfs(int here) {
  for (int i = 0 ; i < nodes[here].size() ; ++i) {
    int there = nodes[here][i];
    if (depth[there] == -1) {
      depth[there] = depth[here] + 1;
      parent[there] = here;
      dfs(there);
    }
  }
}

int main() {
  scanf("%d", &n);
  nodes.resize(n);
  parent.assign(n, -1);
  depth.assign(n, -1);
  for (int i = 0 ; i < n - 1 ; ++i) {
    scanf("%d%d", &u, &v);
    nodes[u - 1].push_back(v - 1);
    nodes[v - 1].push_back(u - 1);
  }

  depth[0] = 0;
  dfs(0);

  LCA lca = LCA(parent, depth);
  scanf("%d", &n);
  while (n--) {
    scanf("%d%d", &u, &v);
    printf("%d\n", lca.query(u - 1, v - 1) + 1);
  }
}
