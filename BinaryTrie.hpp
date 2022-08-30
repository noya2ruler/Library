#pragma once

namespace noya2{

template<typename T, int MAX_LOG> // T = int/ll, 0 <= x < 2 ^ MAX_LOG
struct BinaryTrie{ // set(NOT multiset) of integer
    struct node{
        node *p;
        array<node*,2> ch;
        int exist;
        int sz; // number of integers exist in the subtree of this node
        node () : p(nullptr), ch({nullptr,nullptr}), exist(0), sz(0) {}
    };
    BinaryTrie () : lazy(T(0)){}
    int size(node *v){
        if (v == nullptr){return 0;}
        return v->sz;
    }
    void insert(T x){
        x ^= lazy;
        node *v = root;
        for (int i = MAX_LOG-1; i >= 0; i--){
            int j = x >> i & 1;
            if (v->ch[j] == nullptr){
                v->ch[j] = new node();
                v->ch[j]->p = v;
            }
            v = v->ch[j];
        }
        v->exist = 1;
        update(v);
        for (int i = 0; i < MAX_LOG; i++){
            v = v->p;
            update(v);
        }
    }
    void erase(T x){
        x ^= lazy;
        node *v = root;
        for (int i = MAX_LOG-1; i >= 0; i--){
            int j = x >> i & 1;
            if (v->ch[j] == nullptr){
                return ;
            }
            v = v->ch[j];
        }
        if (v->exist == 0) return ;
        v->exist = 0;
        update(v);
        for (int i = 0; i < MAX_LOG; i++){
            node *p = v->p;
            if (size(v) == 0){
                if (v == p->ch[0]) p->ch[0] = nullptr;
                else p->ch[1] = nullptr;
                delete v;
            }
            v = p;
            update(v);
        }
    }
    int order(T x){ // number of element which is less than x
        node *v = root;
        int res = 0;
        for (int i = MAX_LOG-1; i >= 0; i--){
            int j = lazy >> i & 1;
            if ((x >> i & 1) == 0){
                v = v->ch[j];
            }
            else {
                res += size(v->ch[j]);
                v = v->ch[j^1];
            }
            if (v == nullptr){
                break;
            }
        }
        return res;
    }
    T get(int k){ // value of kth(0_indexed) element, order(get(k)) = k
        node *v = root;
        T ans = T(0);
        for (int i = MAX_LOG-1; i >= 0; i--){
            int j = lazy >> i & 1;
            if (k < size(v->ch[j])){
                v = v->ch[j];
            }
            else {
                k -= size(v->ch[j]);
                v = v->ch[j^1];
                ans |= T(1) << i;
            }
        }
        return ans;
    }
    void xor_all(T x){lazy ^= x;}
  private:
    T lazy;
    node *root = new node();
    void update(node *v){
        v->sz = v->exist + size(v->ch[0]) + size(v->ch[1]);
    }
};

} // namespace noya2