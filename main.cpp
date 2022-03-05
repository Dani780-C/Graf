#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

ifstream f("graf.in");
ofstream g("graf.out");

typedef pair<int, int> per;
const long long inf = 2000000000;

class Graf{
    int numar_noduri;
    int numar_muchii;
    vector<vector<int>> vecini;
public:
    Graf(){
        numar_muchii = 0;
        numar_noduri = 0;
    };

    void seteaza_nr_noduri(int);
    void seteaza_nr_muchii(int);
    void redimensioneaza(int);
    void adauga_muchie(int,int);
    void bfs(int);
    void dfs(vector<bool>&,int);
    int numar_componente_conexe();
    void dfs_topo(vector<bool>&, int, vector<int>&);
    void sortare_topologica(vector<int>&);
    void componente_tare_conexe(Graf&, int&, vector<vector<int>>&);
    void dfs_comp(vector<bool>&, int, vector<vector<int>>&, int);
    void biconex(int&, vector<vector<int>>&);
    void dfs_biconex(vector<bool>&, int, int, vector<int>&, vector<int>&, stack<int>&, vector<vector<int>>&, int&);
    void muchii_critice(int&, vector<vector<int>>&);
    void dfs_muchii_critice(vector<bool>&,int, int, vector<int>&, vector<int>&, vector<vector<int>>&, int&);
    void dijkstra(vector<vector<per>> &, vector<int> &);
    void prim(vector<vector<per>>&, int&, vector<per>&);
    void roy_floyd(int d[101][101]);
    void dfs_darb(vector<bool> &, int, int , int &, int &);
    void reseteaza_vizitat(vector<bool> &);
    int darb();
};

void Graf::seteaza_nr_noduri(int numar_noduri){
    this->numar_noduri = numar_noduri;
}

void Graf::seteaza_nr_muchii(int numar_muchii){
    this->numar_muchii = numar_muchii;
}

void Graf::redimensioneaza(int n){
    vecini.resize(n+1);
}

void Graf::adauga_muchie(int n1, int n2){
    vecini[n1].push_back(n2);
}

/// rezolvari

void Graf::bfs(int start){
    vector<int> coada;
    vector<int> drum_min(numar_noduri+1,-1);
    vector<bool> vizitat(numar_noduri+1,false);
    drum_min[start] = 0;
    coada.push_back(start);
    vizitat[start] = true;

    int inc = 0, sf = 0;
    while(inc <= sf){
        for(int i = 0; i < vecini[coada[inc]].size(); i++)
            if(vizitat[vecini[coada[inc]][i]] == false){
                coada.push_back(vecini[coada[inc]][i]);
                sf++;
                drum_min[vecini[coada[inc]][i]] = drum_min[coada[inc]] + 1;
                vizitat[vecini[coada[inc]][i]] = true;
            }
        inc++;
    }

    for(int i = 1; i <= numar_noduri; i++)
        g<<drum_min[i]<<" ";
}

/// ----

void Graf::dfs(vector<bool> &vizitat, int start){
    vizitat[start] = true;
    for(int i = 0; i < vecini[start].size(); i++)
        if(vizitat[vecini[start][i]] == false)
            dfs(vizitat,vecini[start][i]);
}

/// ----

int Graf::numar_componente_conexe(){
    vector<bool> vizitat(numar_noduri+1, false);
    int cnt = 0;
    for(int i = 1; i <= numar_noduri; i++)
        if(vizitat[i] == false){
            cnt++; dfs(vizitat,i);
        }
    return cnt;
}

/// -----

void Graf::dfs_topo(vector<bool> &vizitat, int start, vector<int> &stk){
    vizitat[start] = true;
    int i;
    for(i = 0; i < vecini[start].size(); i++)
        if(vizitat[vecini[start][i]] == false)
            dfs_topo(vizitat, vecini[start][i], stk);
    stk.push_back(start);
}

void Graf::sortare_topologica(vector<int> &stk){
    vector<bool> vizitat(numar_noduri+1,false);
    for(int i = 1; i <= numar_noduri; i++){
        if(vizitat[i] == false){
            dfs_topo(vizitat,i,stk);
        }
    }
}

/// ----

void Graf::dfs_comp(vector<bool> &vizitat, int start, vector<vector<int>> &comp, int cnt){
    vizitat[start] = true;
    comp[cnt].push_back(start);
    for(int i = 0; i < vecini[start].size(); i++)
        if(vizitat[vecini[start][i]] == false){
            dfs_comp(vizitat,vecini[start][i],comp,cnt);
        }
}

void Graf::componente_tare_conexe(Graf &transpus, int &cnt, vector<vector<int>> &componente){
    vector<bool> vizitat(numar_noduri+1, false);
    vector<int> stk;
    sortare_topologica(stk);
    for(int i = stk.size() - 1; i >= 0; i--)
        if(vizitat[stk[i]] == false){
            transpus.dfs_comp(vizitat,stk[i],componente,cnt);
            cnt++;
        }
}

/// ----

void Graf::dfs_biconex(vector<bool> &vizitat, int fiu, int tata, vector<int> &nivel, vector<int> &nma, stack<int> &stk, vector<vector<int>> &comp, int &cnt){
    vizitat[fiu] = true;
    stk.push(fiu);
    nivel[fiu] = nivel[tata] + 1;
    nma[fiu] = nivel[fiu];
    int i;
    for(i = 0; i < vecini[fiu].size(); i++)
        if(vecini[fiu][i] != tata){
            if(vizitat[vecini[fiu][i]] == true){
                if(nma[fiu] > nivel[vecini[fiu][i]]) nma[fiu] = nivel[vecini[fiu][i]];
            }
            else{
                dfs_biconex(vizitat, vecini[fiu][i], fiu, nivel, nma, stk, comp, cnt);
                if(nma[fiu] > nma[vecini[fiu][i]]) nma[fiu] = nma[vecini[fiu][i]];

                if(nivel[fiu] <= nma[vecini[fiu][i]]){
                    while(stk.top() != vecini[fiu][i]){
                        comp[cnt].push_back(stk.top());
                        stk.pop();
                    }
                    comp[cnt].push_back(vecini[fiu][i]);
                    stk.pop();
                    comp[cnt].push_back(fiu);
                    cnt++;
                }
            }
        }
}

void Graf::biconex(int &cnt, vector<vector<int>> &componente){
    vector<bool> vizitat(numar_noduri+1,false);
    stack<int> stk;
    vector<int> nivel(numar_noduri+1);
    vector<int> nma(numar_noduri+1);
    nivel[0] = 0;
    dfs_biconex(vizitat, 1, 0, nivel, nma, stk, componente, cnt);
}

/// ----

void Graf::dfs_muchii_critice(vector<bool> &vizitat, int fiu, int tata, vector<int> &nivel, vector<int> &nma, vector<vector<int>> &comp, int &cnt){
    vizitat[fiu] = true;
    nivel[fiu] = nivel[tata] + 1;
    nma[fiu] = nivel[fiu];
    int i;
    for(i = 0; i < vecini[fiu].size(); i++)
        if(vecini[fiu][i] != tata){
            if(vizitat[vecini[fiu][i]] == true){
                if(nma[fiu] > nivel[vecini[fiu][i]]) nma[fiu] = nivel[vecini[fiu][i]];
            }
            else{
                dfs_muchii_critice(vizitat, vecini[fiu][i], fiu, nivel, nma, comp, cnt);
                if(nma[fiu] > nma[vecini[fiu][i]]) nma[fiu] = nma[vecini[fiu][i]];

                if(nivel[fiu] < nma[vecini[fiu][i]]){
                    comp[cnt].push_back(fiu);
                    comp[cnt].push_back(vecini[fiu][i]);
                    cnt++;
                }
            }
        }
}

void Graf::muchii_critice(int &cnt, vector<vector<int>> &componente){
    vector<bool> vizitat(numar_noduri+1,false);
    vector<int> nivel(numar_noduri+1);
    vector<int> nma(numar_noduri+1);
    nivel[0] = 0;
    dfs_muchii_critice(vizitat, 1, 0, nivel, nma, componente, cnt);
}

/// ----

void Graf::dijkstra(vector<vector<per>> &vecini_cost, vector<int> &d){
    priority_queue<per, vector<per>, greater<per>> pq;
    vector<bool> vizitat(numar_noduri+1,false);
    vizitat.resize(numar_noduri+1);
    d[1] = 0;
    vizitat[1] = true;

    for(int i = 0; i < vecini_cost[1].size(); i++){
        d[vecini_cost[1][i].second] = vecini_cost[1][i].first;
        pq.push(vecini_cost[1][i]);
    }

    while(pq.size() != 0){
        int nod_curent = pq.top().second;
        pq.pop();
        if(vizitat[nod_curent] == false){
            for(int i = 0; i < vecini_cost[nod_curent].size(); i++)
                if(d[nod_curent] + vecini_cost[nod_curent][i].first < d[vecini_cost[nod_curent][i].second]){
                    d[vecini_cost[nod_curent][i].second] = d[nod_curent] + vecini_cost[nod_curent][i].first;
                    pq.push(make_pair(d[vecini_cost[nod_curent][i].second],vecini_cost[nod_curent][i].second));
                }
        }
        vizitat[nod_curent] = true;
    }
}

/// ----

void Graf::prim(vector<vector<per>> &vecini_cost, int &suma, vector<per> &rez){
    struct tuplu{
        int a;
        int cost;
        int b;
        tuplu(int a, int cost, int b):a(a),cost(cost),b(b){
        }
    };

    struct selector{
        bool operator()(tuplu const& t1, tuplu const& t2) {
            return t1.cost > t2.cost;
        }
    };

    priority_queue<tuplu, vector<tuplu>, selector> pq;

    vector<int> part(numar_noduri+1,0);
    int start = 1;

    for(int i = 0; i < vecini_cost[start].size(); i++)
        pq.push(tuplu(start,vecini_cost[start][i].first,vecini_cost[start][i].second));

    part[1] = 1;

    while(pq.size() != 0){
        int nod_curent = pq.top().b;
        int start = pq.top().a;
        if(part[nod_curent] == 0){
            suma += pq.top().cost;
            rez.push_back(make_pair(start,nod_curent));
            pq.pop();
            for(int i = 0; i < vecini_cost[nod_curent].size(); i++){
                pq.push(tuplu(nod_curent,vecini_cost[nod_curent][i].first, vecini_cost[nod_curent][i].second));
            }
            part[nod_curent] = 1;
        }else{
            pq.pop();
        }
    }
}

/// ----

void Graf::roy_floyd(int d[101][101]){
    for(int k = 1; k <= numar_noduri; k++)
        for(int i = 1; i <= numar_noduri; i++)
            for(int j = 1; j <= numar_noduri; j++)
                if(d[i][k] && d[k][j])
                    if(d[i][j] > d[i][k] + d[k][j] || d[i][j] == 0 and (i-j)*(j-k)*(i-k))
                        d[i][j] = d[i][k] + d[k][j];
}

/// ----

void Graf::dfs_darb(vector<bool> &vizitat, int nod, int nivel, int &nivel_maxim, int &_nod)
{
    vizitat[nod] = true;
    if(nivel > nivel_maxim){
        nivel_maxim = nivel;
        _nod = nod;
    }

    for(int i = 0; i < vecini[nod].size(); i++){
        if(!vizitat[vecini[nod][i]])
            dfs_darb(vizitat,vecini[nod][i], nivel + 1, nivel_maxim, _nod);
    }
}

void Graf::reseteaza_vizitat(vector<bool> &vizitat){
    for(int i = 0; i <= numar_noduri; i++)
        vizitat[i] = false;
}

int Graf::darb(){

    vector<bool> vizitat(numar_noduri+1,false);

    int _nod, nivel_maxim = 0;

    dfs_darb(vizitat,1,0,nivel_maxim,_nod);
    reseteaza_vizitat(vizitat);
    nivel_maxim = 0;
    dfs_darb(vizitat,_nod, 0,nivel_maxim,_nod);

    return nivel_maxim + 1;
}

/// ----

int main()
{
    Graf a;

    int numar_noduri, numar_muchii;

/// bfs
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    int nod_start;
    f >> nod_start;

    int n1,n2;
    a.redimensioneaza(numar_noduri+1);

    for(int i = 0; i < numar_muchii; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
    }

    a.bfs(nod_start); */

/// numar_componente_conexe
    /* f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    int n1,n2;
    a.redimensioneaza(numar_noduri+1);

    for(int i = 0; i < numar_muchii; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
        a.adauga_muchie(n2,n1);
    }

    g<<a.numar_componente_conexe(); */

/// sortare_topologica
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    int n1,n2;
    a.redimensioneaza(numar_noduri+1);

    for(int i = 0; i < numar_muchii; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
    }

    vector<int> stk;

    a.sortare_topologica(stk);

    for(int i = stk.size() - 1; i >= 0; i--)
        g<<stk[i]<<" "; */

/// componente_tare_conexe
/*    Graf b;

    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);
    b.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);
    b.seteaza_nr_muchii(numar_muchii);

    int n1,n2;
    a.redimensioneaza(numar_noduri+1);
    b.redimensioneaza(numar_noduri+1);

    for(int i = 0; i < numar_muchii; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
        b.adauga_muchie(n2,n1);
    }

    int cnt = 0;

    vector<vector<int>> componente;
    componente.resize(numar_noduri+1);
    a.componente_tare_conexe(b,cnt,componente);

    g<<cnt<<'\n';
    for(int i = 0; i < cnt; i++){
        for(int j = 0; j < componente[i].size(); j++)
            g<<componente[i][j]<<" ";
        g<<'\n';
    } */

/// componente_biconexe
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    int n1,n2;
    a.redimensioneaza(numar_noduri+1);

    for(int i = 0; i < numar_muchii; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
        a.adauga_muchie(n2,n1);
    }

    vector<vector<int>> componente;
    componente.resize(numar_noduri+1);
    int cnt = 0;

    a.biconex(cnt,componente);

    g<<cnt<<'\n';
    for(int i = 0; i < cnt; i++){
        for(int j = 0; j < componente[i].size(); j++)
            g<<componente[i][j]<<" ";
        g<<'\n';
    } */

/// muchii_critice
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    int n1,n2;
    a.redimensioneaza(numar_noduri+1);

    for(int i = 0; i < numar_muchii; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
        a.adauga_muchie(n2,n1);
    }

    vector<vector<int>> componente;
    componente.resize(numar_noduri+1);
    int cnt = 0;

    a.muchii_critice(cnt,componente);

    g<<cnt<<'\n';
    for(int i = 0; i < cnt; i++){
        for(int j = 0; j < componente[i].size(); j++)
            g<<componente[i][j]<<" ";
        g<<'\n';
    } */

/// dijkstra
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    vector<vector<per>> vecini_cost;
    vecini_cost.resize(numar_noduri+1);

    for(int i = 1; i <= numar_muchii; i++){
        int nod1; f>>nod1;
        int nod2; f>>nod2;
        int cost; f>>cost;
        vecini_cost[nod1].push_back(make_pair(cost,nod2));
    }

    vector<int> d(numar_noduri+1,inf);

    a.dijkstra(vecini_cost,d);

    for(int i = 2; i <= numar_noduri; i++){
        if(d[i] == inf) g<<0<<" ";
        else g<<d[i]<<" ";
    } */

/// prim
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    f >> numar_muchii;
    a.seteaza_nr_muchii(numar_muchii);

    int suma = 0;
    vector<vector<per>> vecini_cost;
    vecini_cost.resize(numar_noduri+1);
    vector<per> rez;

    for(int i = 1; i <= numar_muchii; i++){
        int nod1; f>>nod1;
        int nod2; f>>nod2;
        int cost; f>>cost;
        vecini_cost[nod1].push_back(make_pair(cost,nod2));
        vecini_cost[nod2].push_back(make_pair(cost,nod1));
    }

    a.prim(vecini_cost, suma, rez);

    g<<suma<<'\n';
    g<<rez.size()<<'\n';
    for(int i = 0; i < rez.size(); i++)
        g<<rez[i].first<<" "<<rez[i].second<<'\n'; */

/// roy-floyd
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);

    int d[101][101];
    for(int i = 1; i <= numar_noduri; i++)
        for(int j = 1; j <= numar_noduri; j++)
            f>>d[i][j];


    a.roy_floyd(d);

    for(int i = 1; i <= numar_noduri; i++){
        for(int j = 1; j <= numar_noduri; j++)
            g<<d[i][j]<<" ";
        g<<'\n';
    } */

/// darb
/*    f >> numar_noduri;
    a.seteaza_nr_noduri(numar_noduri);
    a.seteaza_nr_muchii(numar_noduri-1);
    a.redimensioneaza(numar_noduri+1);

    int n1, n2;
    for(int i = 1; i < numar_noduri; i++){
        f >> n1 >> n2;
        a.adauga_muchie(n1,n2);
        a.adauga_muchie(n2,n1);
    }

    g<<a.darb(); */

    return 0;
}
