#include"main.h"

const double dnf = 10000;
struct Data {
	vector<int> data;
	vector<int> index;
	int bel;
	Data() {}
	Data(vector<int>  data, int bel) {
		this->data = data;
		this->bel = bel;
		index = vector<int>(data.size());
		for (int i = 0; i < index.size(); i++)
			index[i] = i;
	}
	void del_atr(int num) {
		data.erase(data.begin() + num);
		index.erase(index.begin() + num);
	}
	friend ostream & operator << (ostream & o, const Data & s) { //重载<<, 令其可以使用输出流输出
		o << "[";
		for (auto x : s.data) {
			o << " " << x;
		}
		o << " ]";
		cout << " bel : " << s.bel;
		return o;
	}
};
class ID3_Tree {
private:
	vector<int> blo, he, ne, ver, atr, val;
	int cnt, tot, root;
	void dfs_print(int p, int tab = 0) {
		bool flag = false;
		for (int i = he[p]; i; i = ne[i])
		{
			flag = true;
			for (int i = 0; i < tab; i++) printf("\t");
			printf("IF [%d] ATTRIBUTE IS [%d] THEN:\n", atr[i], val[i]);
			int y = ver[i];
			dfs_print(y, tab + 1);
			for (int i = 0; i < tab; i++) printf("\t");
			printf("END IF\n");
		}
		if (!flag) {
			for (int i = 0; i < tab; i++) printf("\t");
			printf("FINAL DECISION AND THE DATA BELONG TO %d BLOCK.\n", blo[p]);
		}
	}
public:
	void init_tree(int n)
	{
		tot = 1;
		cnt = 0;
		root = 0;
		he = vector<int>((n << 5) + 5);
		ne = vector<int>((n << 5) + 5);
		ver = vector<int>((n << 5) + 5);
		atr = vector<int>((n << 5) + 5);
		val = vector<int>((n << 5) + 5);
		blo = vector<int>((n << 5) + 5);
	}
	void connect_son(int fa, int son, int _atr, int _val) {
		ver[++tot] = son;
		atr[tot] = _atr;
		val[tot] = _val;
		ne[tot] = he[fa];
		he[fa] = tot;
	}
	int new_node() {
		return ++cnt;
	}
	void set_leaf(int node, int bel) {
		blo[node] = bel;
	}
	int get_root() {
		return root;
	}
	void print() {

		dfs_print(root);
	}
};
class ID3_n_column {
private:
	vector<Data> input_set;
	vector<int> limit;
	ID3_Tree tree;
	bool check_data(Data o) {
		if (o.data.size() != limit.size()) return false;
		for (int i = 0; i < limit.size(); i++) {
			if (o.data[i] >= limit[i] || o.data[i] < 0) return false;
		}
		return true;
	}
	double I(const vector<int> &s) {
		int sum = 0;
		for (auto x : s) sum += x;
		double g = 0.0;
		for (auto x : s)
		{
			if (!x) continue;
			g += 1.0 * x / sum * (log(1.0 * x / sum) / log(2.0));
		}
		return -g;
	}
	double che(const vector<Data> & mp, int index, int limit) {
		vector<vector<int> > s; vector<int> sa;
		int mx = 0;
		for (auto x : mp) { mx = max(mx, x.bel); }
		s = vector<vector<int> >(limit + 5);
		for (int i = 0; i <= limit; i++) s[i] = vector<int>(mx + 1, 0);
		sa = vector<int>(mx + 1);
		for (auto x : mp) {
			s[x.data[index]][x.bel]++;
			sa[x.bel]++;
		}
		double ansa = 0.0;
		for (int i = 0; i < limit; i++) {
			double p = 0.0;
			for (int j = 0; j <= mx; j++) {
				p += s[i][j];

			}
			p /= mp.size();
			p *= I(s[i]);
			ansa += p;
		}
		return I(sa) - ansa;
	}
	bool check_leaf(const vector<Data> & s, int p) {
		if (s.size() == 0) return true;
		bool flag = true;
		for (auto x : s) {
			if (x.data != s[0].data) flag = false;
		}
		map<int, int> num;
		int mx = 0, pos;
		for (auto x : s) {
			num[x.bel]++;
			if (num[x.bel] > mx) {
				mx = num[x.bel];
				pos = x.bel;
			}
		}
		tree.set_leaf(p, pos);
		if (flag || num.size() == 1) return true;
		return false;
	}
	void get_tree(int p, const vector<Data> & cur_set, const vector<int> & cur_limit) {
		int g = 0;
		int val = 0;
		if (check_leaf(cur_set, p)) return;
		double mi = dnf; int pos = -1;
		for (int i = 0; i < cur_limit.size(); i++) {
			double ans = che(cur_set, i, cur_limit[i]);
			if (mi > ans) {
				mi = ans; pos = i;
			}
		}
		int gli = cur_limit[pos];
		vector<vector<Data> > sp;
		vector<int> li;
		for (int i = 0; i < cur_limit.size(); i++) {
			if (i == pos) continue;
			li.push_back(cur_limit[i]);
		}
		sp = vector<vector<Data> >(cur_limit[pos] + 5);
		for (auto x : cur_set) {
			int gs = x.data[pos];
			x.del_atr(pos);
			sp[gs].push_back(x);
		}
		for (int i = 0; i < gli; i++) {
			int g = tree.new_node();
			int val;
			//cout << p << " " << g << " " << pos << " " << i << endl;
			tree.connect_son(p, g, cur_set[0].index[pos], i);
			check_leaf(cur_set, g);
			get_tree(g, sp[i], li);
		}
	}
public:
	ID3_n_column() { }
	ID3_n_column(vector<int> limit) {
		this->limit = limit;
	}
	void init() {
		input_set.clear();
	}
	bool add_data(Data o) {
		if (!check_data(o)) return false;

		input_set.push_back(o);
		return true;
	}
	void runID3() {
		tree.init_tree(input_set.size());
		get_tree(tree.get_root(), input_set, limit);
	}
	void print() {
		printf("GET THE TREE\n\n");
		tree.print();
	}
};

void start() {
	int n, m; cin >> n >> m;
	vector<int> limit(m);
	for (int i = 0; i < m; i++)
		scanf("%d", &limit[i]);

	ID3_n_column solver(limit);
	solver.init();
	for (int i = 1; i <= n; i++) {
		vector<int> data;
		for (int i = 1; i <= m; i++) {
			int te; scanf("%d", &te);
			data.push_back(te);
		}
		int bel; cin >> bel;
		solver.add_data(Data(data, bel));
	}
	solver.runID3();
	solver.print();
}

int main() {
	IO::read_f();

	start();

	IO::run_time();
}