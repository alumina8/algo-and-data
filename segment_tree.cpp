#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <numeric>
#include <array>
#include <memory>
#include <cmath>
using namespace std;
//変数デバッグ
#define DEB(variable) cout << #variable << '=' << variable << endl

//for簡易表記(引数ミス防止)
#define FOR(LoopVariable,numberOFbegin,numberOFend) for (int LoopVariable = (numberOFbegin); (LoopVariable) < (numberOFend); (LoopVariable)++)
#define DEFOR(LoopVariable,numberOFbegin,numberOFend) for (int LoopVariable = (numberOFbegin)-1; (LoopVariable) >= (numberOFend); (LoopVariable)--)
#define REP(LoopVariable,numberOFend) for(int LoopVariable = 0;(LoopVariable)<(numberOFend);LoopVariable++)

constexpr long long int linf = numeric_limits<long long int>::max()-5;
constexpr int inf= numeric_limits<int>::max()-5;


//通常セグメント木 unique_ptr<Segment_tree<int>> seg(new Segment_tree<int>(n,1));のように呼ぶ
//<numeric>,<memory>,<algorithm>とかを使ってます
//mode分岐を消すと多少早くなります(｀･ω･´)ゞ
template <typename T>
class Segment_tree{
public:
    //サイズを指定し外枠だけつくる
    //leaf_setで初期化すること
    Segment_tree(int origin_data_num,int o_mode){
        mode = o_mode;
        data_num = origin_data_num;

        //幅と深さを得る
        width = 1;
        depth = 1;
        while(width<data_num){
            width *= 2;
            depth++;
        }
        //単位元初期化
        if(mode==0){
            //sum
            unity = 0;
        }else if(mode==1){
            //prod
            unity = 1;
        }else if(mode==2){
            //max
            unity = numeric_limits<T>::min();
        }else if(mode==3){
            //min
            unity = numeric_limits<T>::max();
        }else{
            //sum
            unity = 0;
        }

        nodes.resize(2*width-1,unity);
    }
    //用意しておいたベクトルを用いた初期化
    Segment_tree(vector<T> origin_v,int o_mode){
        mode = o_mode;
        data_num = origin_v.size();
        
        //幅と深さを得る
        width = 1;
        depth = 1;
        //単位元初期化
        if(mode==0){
            //sum
            unity = 0;
        }else if(mode==1){
            //prod
            unity = 1;
        }else if(mode==2){
            //max
            unity = numeric_limits<T>::min();
        }else if(mode==3){
            //min
            unity = numeric_limits<T>::max();
        }else{
            //sum
            unity = 0;
        }
        
        //幅と深さを得る
        width = 1;
        depth = 1;
        while(width<data_num){
            width *= 2;
            depth++;
        }

        //木の初期化
        nodes.resize(2*width-1,unity);
        REP(i,data_num){
            nodes[width - 1 + i] = origin_v[i];
            if(i&1){
                mini_update(i + width - 1);
            }
        }
        if((data_num&1)){
            mini_update(data_num + width - 2);
        }
        
    }
    ~Segment_tree(){}

    void mini_update(int where){
        //木の更新を行う
        int x = where;
        if(mode==0){
            //sum
            while(x > 0) {
                x = (x - 1) / 2;
                nodes[x] = nodes[2*x+1]+ nodes[2*x+2];
            }
        }else if(mode==1){
            //prod
            while(x > 0) {
                x = (x - 1) / 2;
                nodes[x] = nodes[2*x+1]* nodes[2*x+2];
            }
        }else if(mode==2){
            //max
            while(x > 0) {
                x = (x - 1) / 2;
                nodes[x] = max(nodes[2*x+1],nodes[2*x+2]);
            }
        }else if(mode==3){
            //min
            while(x > 0) {
                x = (x - 1) / 2;
                nodes[x] = min(nodes[2*x+1],nodes[2*x+2]);
            }
        }else{
            //sum
            while(x > 0) {
                x = (x - 1) / 2;
                nodes[x] = nodes[2*x+1]+ nodes[2*x+2];
            }
        }
    }
    //葉の値を指定、その後全体を更新
    void leaf_set(int where,T after_value){
        if(where<0||width<where){
            printf("error:out of range!\n");
            return;
        }else if(where>data_num){
            printf("warn:out of originsize\n");
        }
        nodes[width+where-1] = after_value;
        mini_update(width+where-1);
    }
    //任意の要素の値を得る
    T get(int where){
        return nodes[where];
    }
    //葉の要素を得る
    T get_leaf(int where){
        return nodes[width + where - 1];
    }

    //要素全てを出力
    void all_out(){
        
        cout << width <<" "<< depth << endl;
        
        REP(i,depth){
            REP(j,pow(2,i)-1){
                cout << nodes[(int)pow(2,i)-1+j] << " ";
            }
            cout <<nodes[(int)pow(2,i+1)-2];
            printf("\n");    
        }
        cout<<endl;
    }
    //範囲を指定して値を得る
    T get_val(int a, int b, int k=0, int l=0, int r=-1) {


        if(mode==0){
            //sum
            return get_val_sum(a,b,k,l,r);
        }else if(mode==1){
            //prod
            return get_val_prod(a,b,k,l,r);
        }else if(mode==2){
            //max
            return get_val_max(a,b,k,l,r);
        }else if(mode==3){
            //min
            return get_val_min(a,b,k,l,r);
        }else{
            //sum
            return get_val_sum(a,b,k,l,r);
        }
    }
    T get_val_sum(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) {
            r = width;
        }
        if(r <= a || b <= l) {return unity;}
        if(a <= l && r <= b) {return nodes[k];}
        T vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        T vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return vl+ vr;
    }
    T get_val_prod(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) {
            r = width;
        }
        if(r <= a || b <= l) {return unity;}
        if(a <= l && r <= b) {return nodes[k];}
        T vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        T vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return vl* vr;
    }
    T get_val_max(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) {
            r = width;
        }
        if(r <= a || b <= l) {return unity;}
        if(a <= l && r <= b) {return nodes[k];}
        T vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        T vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return max(vl, vr);
    }
    T get_val_min(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) {
            r = width;
        }
        if(r <= a || b <= l) {return unity;}
        if(a <= l && r <= b) {return nodes[k];}
        T vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        T vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
private:
    int width,data_num,depth,mode;//木の横幅、元となったデータの数、木の深さ、木のモード(sum(+),prod(*),max,min))
    vector<T> nodes;//それぞれの要素の値を保持するベクトル
    T unity;//単位元・コンストラクタで初期化される sum: 0,prod: 1,max: numeric_limits<T>::min(),min: numeric_limits<T>::max();
};

int main(){
    int n=20;

    
    unique_ptr<Segment_tree<int>> seg_sum(new Segment_tree<int>(n,0));
    REP(i,n){
        seg_sum->leaf_set(i , i+1);
    }
    seg_sum->all_out();
    unique_ptr<Segment_tree<long long int>> seg_prod(new Segment_tree<long long int>(n,1));
    REP(i,n){
        seg_prod->leaf_set(i , i+1);
    }
    seg_prod->all_out();
    unique_ptr<Segment_tree<int>> seg_max(new Segment_tree<int>(n,2));
    REP(i,n){
        seg_max->leaf_set(i , i+1);
    }
    seg_max->all_out();
    unique_ptr<Segment_tree<int>> seg_min(new Segment_tree<int>(n,3));
    REP(i,n){
        seg_min->leaf_set(i , i+1);
    }
    seg_min->all_out();

    vector<int> v(n);
    REP(i,n){
        v[i]=i+1;
    }
    unique_ptr<Segment_tree<int>> seg_sumv(new Segment_tree<int>(v,0));
    seg_sumv->all_out();
    unique_ptr<Segment_tree<int>> seg_prodv(new Segment_tree<int>(v,1));
    seg_prodv->all_out();
    unique_ptr<Segment_tree<int>> seg_maxv(new Segment_tree<int>(v,2));
    seg_maxv->all_out();
    unique_ptr<Segment_tree<int>> seg_minv(new Segment_tree<int>(v,3));
    seg_minv->all_out();
    cout << "sum " << seg_sum->get_val(0, n) <<" "<< seg_sumv->get_val(0, n) << "\n"
         << "prod "<< seg_prod->get_val(0, n)<<" "<< seg_prodv->get_val(0, n) << "\n"
         << "max " << seg_max->get_val(0, n) <<" "<< seg_maxv->get_val(0, n) << "\n"
         << "min " << seg_min->get_val(0, n) <<" "<< seg_minv->get_val(0, n) << endl;
    return 0;
}
