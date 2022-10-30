#include"main.h"
struct Data {
    vector<double> data;
    int belong;
    Data () {}
    Data (vector<double> s, int bel) {
        data = s;
        belong = bel;
    }
    double dis(const Data & o, int p) const {
        double dis = 0;
        if (data.size() != o.data.size()) return -1;
        for (int i = 0; i < data.size(); i++) {
            dis = pow( (data[i] - o.data[i]), p);
        }
        return pow(dis, 1.0/p);
    }
    friend ostream & operator << (ostream & o, const Data & s) { //重载<<, 令其可以使用输出流输出
        o << "[";
        for (auto x : s.data) {
            o << " " << x;
        }
        o << " ]";
        return o;
    }
    bool operator < (const Data & o) const { // 重载小于号, 以便存入set. 
        return data < o.data;
    }
};
class KNN_n_column {
private:
    vector<Data> data_set; // 训练数据集
    multiset<Data> near_set; // N集
    map<int, int> belong_num; // 记录各个分类的数量
    int dis_pow;
    multiset<Data>::iterator che(Data t, double diss) { // 检查一个更优数据中心
        auto it = near_set.begin();
        for (; it!=near_set.end(); it++) {
            if (t.dis(*it, dis_pow) > diss) {
                return it;
            }
        }
        return near_set.end();
    }
public:
    KNN_n_column(){dis_pow = 2;}
    KNN_n_column (vector<Data> s) {
        data_set = s;
        dis_pow = 2;
    }
    void init() {data_set.clear(); near_set.clear();}
    void add(Data d) {data_set.push_back(d);} 
    void set_dis_pow(int p) { dis_pow = p;  }
    int get_belong(Data & t, int k) {
        for (auto d : data_set) { // 执行算法
            if (near_set.size() <= k) {
                near_set.insert(d); // 数量小于k, 直接加入near集
            }
            else {
                auto it = che(t, d.dis(t, dis_pow)); // 寻找一个更优数据中心
                if ( it != near_set.end() ) { // 如果存在最优数据中心
                    near_set.erase(it); // 删除
                    near_set.insert(d); // 添加
                }
            }
        }
        belong_num.clear();
        int mx = 0, belong = -1;
        for (auto x : near_set) { // 寻找near集中,最多的元素.
            int g = (++belong_num[x.belong]);
            if (g > mx) {
                mx = g; belong = x.belong;
            }
        }
        t.belong = belong;
        return belong;
    }
};


void start() {
    KNN_n_column solver; // 定义solver;
    solver.init(); //初始化solver
    solver.set_dis_pow(2); //设定明可夫斯基距离指数.
    int n, k; cin >> n >> k; //读入训练集数量和每个数据项的属性
    for (int i = 1; i <= n; i++) {
        vector<double> s(k);
        for (int j = 0; j < k; j++) cin >> s[j];
        int belong; cin >> belong;
        solver.add( Data(s, belong) ); // 向solver中添加数据
    }
    int m, c; cin >> m >> c; // 设定需要分类的数据个数以及k值
    while(m--) {
        vector<double> s(k);
        for (int j = 0; j < k; j++) cin >> s[j];
        Data ss;
        int g = solver.get_belong(ss = Data(s, 0), c); // 开始执行kNN算法分类
        cout << "Check " << ss << " " << "belong to class : " << ss.belong << endl; // 输出结果
    }
}


//此main函数将从程序目录下的0.in文件读入数据, 并将结果及程序运行时间输出至同目录下的0.out文件中.
int main() {
    IO::read_f();
    // srand(time(NULL));
    // cout << 500 << " " << 7 << endl;
    // for (int i = 1; i <= 500; i++) {
    //     for (int i = 1; i <= 7; i++)
    //         cout << rand() << " ";
    //     cout << rand() % 40 << endl;
    // }
    // cout << 20 << " " << 300 << endl;
    // for (int i = 0; i < 20; i++)
    // {
    //     for (int i = 1; i <= 7; i++)
    //         cout << rand() << " ";
    //     cout << endl;
    // }
    // return 0;
    start();

    IO::run_time();
}