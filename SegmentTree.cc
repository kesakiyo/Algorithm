#include <stdio.h>
#include <algorithm>
#include <vector>
#include <limits>
#include <functional>

namespace SegmentTree {
	template <typename T> class Tree {
	public:
		int idx;
		T LIMIT;
		std::vector<T> node;
		std::function<T(T, T)> eval;

		Tree(int n) {
			this->idx = 1;
			while (this->idx < n) {
				this->idx <<= 1;
			}
		}

		T query(int lo, int hi) {
			int x = lo + this->idx, y = hi + this->idx;
			T ret = this->LIMIT;
			while (x <= y) {
				if (x % 2 == 1) {
					ret = this->eval(ret, this->node[x++]);
				}
				if (y % 2 == 0) {
					ret = this->eval(ret, this->node[y--]);
				}
				x >>= 1;
				y >>= 1;
			}
			return ret;
		}

		void update(int pos, T val) {
			int x = pos + this->idx;
			this->node[x] = val;
			x >>= 1;
			while (x) {
				this->node[x] = this->eval(this->node[x * 2], this->node[x * 2 + 1]);
				x >>= 1;
			}
		}
	};

	template<typename T> class Min: public Tree<T> {
	public:
		Min(int n): Tree<T>(n) {
			this->LIMIT = std::numeric_limits<T>::max();
			this->node.assign(this->idx * 4, this->LIMIT);
			this->eval = [](T a, T b) {
				return std::min(a, b);
			};
		}
	};

	template<typename T> class Max: public Tree<T> {
	public:
		Max(int n): Tree<T>(n) {
			this->LIMIT = std::numeric_limits<T>::min();
			this->node.assign(this->idx * 4, this->LIMIT);
			this->eval = [](T a, T b) {
				return std::max(a, b);
			};
		}
	};

	template<typename T> class Sum: public Tree<T> {
	public:
		Sum(int n): Tree<T>(n) {
			this->LIMIT = 0;
			this->node.assign(this->idx * 4, this->LIMIT);
			this->eval = [](T a, T b) {
				return a + b;
			};
		}
	};
}

int main() {
	int n, m;
	scanf("%d%d", &n, &m);

	auto minSegmentTree = new SegmentTree::Min<int>(n);
	auto maxSegmentTree = new SegmentTree::Max<int>(n);

	for (int i = 0 ; i < n ; ++i) {
		int val;
		scanf("%d", &val);
		minSegmentTree->update(i, val);
		maxSegmentTree->update(i, val);
	}

	for (int i = 0 ; i < m ; ++i) {
		int lo, hi;
		scanf("%d%d", &lo, &hi);
		printf("%d %d\n", minSegmentTree->query(lo - 1, hi - 1), maxSegmentTree->query(lo - 1, hi - 1));
	}
}
