#include <iostream>
#include <vector>

using namespace std;

class node
{
    vector<pair<int, int>> pts;
    vector<node*> H;
    int m_nod, m, Cant = 0;
    vector<int> l_x;
    vector<int> l_y;
    bool eshoja;
    bool es_padre;
    bool es_raiz = false;
    public:
    node* P = 0;
    node(int m_nod, vector<int> x, vector<int> y);
    node();
    ~node();

    void changeIsLeaf(bool);
    void cambiar_M(int m);
    void c_es_padre(bool);
    void a_lim(pair<int, int> lim);
    void a_point(pair<int, int> punto);
    void a_hijo(node*& hijo);
    void h_raiz();
    void foo();
    int o_cuanto();
    int g_M();
    int g_m();
    pair<int, int> g_punto(int i);
    node* g_hijo(int i);
    node* g_padre();
    pair<int, int> g_limite(size_t i);
    vector<pair<int, int>> g_todos_puntos();
    vector<node*> g_todos_hijos();
    int ampliar(pair<int, int> punto);
    int ampliar(node* n);
    void delete_punto(int i);
    void delete_hijo(int i);
    void borrar();
    bool es_hoja();
    void v_balancear();
    void balancear();

};

node::node(int m_nod, vector<int> x, vector<int> y) {
    this->m_nod = m_nod;
    this->m = ceil((double)m_nod / 2);
    this->l_x = x;
    this->l_y = y;
    this->eshoja = this->es_padre = false;
}

node::node() {
    this->m_nod = 0;
    this->m = 0;
    this->eshoja = this->es_padre = false;
}

node::~node() {
    this->pts.clear();
    this->H.clear();
}

void node::changeIsLeaf(bool X) {
    this->eshoja = X;
}

void node::c_es_padre(bool b) {
    this->es_padre = b;
}

void node::a_lim(pair<int, int> lim) {
    this->l_x.push_back(lim.first);
    this->l_y.push_back(lim.second);
}

void node::a_point(pair<int, int> punto) {
    this->pts.push_back(punto);
    this->Cant = this->Cant + 1;
    this->balancear();
}

void node::a_hijo(node*& hijo) {
    hijo->P = this;
    this->H.push_back(hijo);
    this->Cant = this->Cant + 1;
    if (this->es_raiz)return;
    this->v_balancear();
}

void node::h_raiz() {
    this->es_raiz = true;
}

void node::foo() {
    this->es_raiz = false;
}

int node::o_cuanto() {
    return this->Cant;
}

int node::g_M() {
    return this->m_nod;
}

int node::g_m() {
    return this->m;
}

pair<int, int> node::g_punto(int i) {
    if (i > this->Cant)
        return make_pair(1024, 1024);
    return this->pts[i];
}

node* node::g_hijo(int i) {
    if (i >= this->Cant)
        return 0;
    return this->H[i];
}

pair<int, int> node::g_limite(size_t i) {
    size_t sizeLimite = this->l_x.size();
    if (i > sizeLimite)
        return make_pair(1024, 1024);
    return make_pair(this->l_x[i], this->l_y[i]);
}

void node::delete_punto(int i) {
    if (i >= this->Cant)
        return;
    this->pts.erase(this->pts.begin() + i);
    this->Cant = this->Cant - 1;
}

void node::delete_hijo(int i) {
    if (i >= this->Cant)
        return;
    this->H.erase(this->H.begin() + i);
    this->Cant = this->Cant - 1;
}

bool node::es_hoja() {
    return this->eshoja;
}

vector<pair<int, int>> node::g_todos_puntos() {
    return this->pts;
}

vector<node*> node::g_todos_hijos() {
    return this->H;
}

void node::borrar() {
    this->H.clear();
    this->pts.clear();
    this->Cant = 0;
}

int node::ampliar(pair<int, int> Pt) {
    int hijo = this->l_x[0], y = this->l_y[0];
    int X = this->l_x[1], Y = this->l_y[1];

    int areaR = abs(X - hijo) * abs(Y - y);

    if (Pt.first >= X)
        X = Pt.first;
    else if (Pt.first <= hijo)
        hijo = Pt.first;

    if (Pt.second >= Y)
        Y = Pt.second;
    else if (Pt.second <= y)
        y = Pt.second;

    int areaRp = abs(X - hijo) * abs(Y - y);
    return areaRp - areaR;
}

int node::ampliar(node* nod_x) {
    int hijo = this->l_x[0], y = this->l_y[0];
    int X = this->l_x[1], Y = this->l_y[1];

    pair<int, int> n = nod_x->g_limite(0), N = nod_x->g_limite(1);
    int areaR = abs(X - hijo) * abs(Y - y);

    if (N.first >= X)
        X = N.first;
    else if (n.first <= hijo)
        hijo = n.first;

    if (N.second >= Y)
        Y = N.second;
    else if (n.second <= y)
        y = n.second;

    int areaRp = abs(X - hijo) * abs(Y - y);
    return areaRp - areaR;
}

void node::v_balancear() {
    if (this->eshoja)return;
    pair<int, int> r, R, aux;
    r = this->H[0]->g_limite(0);
    R = this->H[0]->g_limite(1);
    for (int i = 1; i < this->Cant; ++i) {
        aux = this->H[i]->g_limite(0);
        if (aux.first < r.first)
            r.first = aux.first;
        if (aux.second < r.second)
            r.second = aux.second;
        aux = this->H[i]->g_limite(1);
        if (aux.first > R.first)
            R.first = aux.first;
        if (aux.second > R.second)
            R.second = aux.second;
    }
    this->l_x.clear();
    this->l_y.clear();
    this->a_lim(r);
    this->a_lim(R);
}

void node::balancear() {
    if (this->es_raiz || this->es_padre) return;
    int hijo = 1024, X = -1000;
    int y = 1024, Y = -1000;
    int px, py;
    for (int i = 0; i < this->Cant; ++i) {
        px = this->pts[i].first;
        py = this->pts[i].second;
        if (px < hijo) hijo = px;
        if (px > X) X = px;
        if (py < y) y = py;
        if (py > Y) Y = py;
    }
    this->l_x.clear();
    this->l_y.clear();
    this->a_lim(make_pair(hijo, y));
    this->a_lim(make_pair(X, Y));
}

void node::cambiar_M(int m) {
    this->m_nod = m;
    this->m = ceil((double)m / 2);
}

node* node::g_padre() {
    return this->P;
}

class r_tree
{
    node* root;

    node* escoger(node*& R, pair<int, int>& Pt);
    node* split_hoja(node*& R);
    node* split_nodo(node*& R);
    node* balancear(node*& R, node*& RR);

    void pickseed_p(vector<pair<int, int>>& Puntos, node*& GroupA, node*& GroupB);
    pair<int, int> picknext_p(vector<pair<int, int>>& Puntos, node*& GroupA, node*& GroupB);

    void pickseed_r(vector<node*>& Regiones, node*& GroupA, node*& GroupB);
    node* picknext_r(vector<node*>& Regiones, node*& GroupA, node*& GroupB);

public:
    r_tree();
    r_tree(int m_nod, int hijo, int X, int y, int Y);
    ~r_tree();

    void insertar(pair<int, int> hijo);
    void search(pair<int, int> hijo);
};

r_tree::r_tree() {
    this->root = 0;
}

r_tree::r_tree(int m_nod, int hijo, int X, int y, int Y) {
    vector<int> Xs = { hijo , X };
    vector<int> Ys = { y , Y };
    this->root = new node(m_nod, Xs, Ys);
    this->root->h_raiz();
    this->root->changeIsLeaf(1);
}

r_tree::~r_tree() {
    this->root->~node();
}

void r_tree::insertar(pair<int, int> hijo) {
    pair<int, int> l = this->root->g_limite(0);
    pair<int, int> L = this->root->g_limite(1);
    node* Rptr = this->root;
    node* Leaff = escoger(Rptr, hijo);
    pair<int, int> lim = Leaff->g_limite(0);
    pair<int, int> Li = Leaff->g_limite(1);
    node* LL = 0;
    Leaff->a_point(hijo);
    if (Leaff->o_cuanto() > Leaff->g_M()) {
        LL = split_hoja(Leaff);
        vector<pair<int, int>> ll = LL->g_todos_puntos();
        vector<pair<int, int>> le = Leaff->g_todos_puntos();
    }
    node* RR = balancear(Leaff, LL);
    if (RR) {
        node* newroot = new node();
        newroot->h_raiz();
        newroot->a_lim(l);
        newroot->a_lim(L);
        newroot->cambiar_M(this->root->g_M());
        newroot->c_es_padre(1);
        newroot->a_hijo(this->root);
        this->root->foo();
        this->root->v_balancear();
        newroot->a_hijo(RR);
        this->root = newroot;
    }
}

node* r_tree::escoger(node*& R, pair<int, int>& Pt) {
    node* Raux = R;
    while (!Raux->es_hoja()) {
        int n = Raux->o_cuanto(), iesimo, major = 361201;
        for (int i = 0; i < n; ++i) {
            int area = Raux->g_hijo(i)->ampliar(Pt);
            if (major >= area) {
                iesimo = i;
                major = area;
            }
        }
        Raux = Raux->g_hijo(iesimo);
    }
    pair<int, int> a = Raux->g_limite(0), b = Raux->g_limite(1);
    return Raux;
}

node* r_tree::split_hoja(node*& R) {
    node* RR = new node();
    RR->cambiar_M(R->g_M());
    vector<pair<int, int>> pts(R->g_todos_puntos());
    R->borrar();
    R->changeIsLeaf(1);
    RR->changeIsLeaf(1);
    pickseed_p(pts, R, RR);
    while (!pts.empty()) {
        pair<int, int> next = picknext_p(pts, R, RR);
        int areaA = R->ampliar(next);
        int areaB = RR->ampliar(next);
        (areaA < areaB) ? R->a_point(next) : RR->a_point(next);
    }
    while (RR->o_cuanto() < RR->g_m()) {
        int aux = R->o_cuanto() - 1;
        pair<int, int> temp = R->g_punto(aux);
        RR->a_point(temp);
        R->delete_punto(aux);
    }
    while (R->o_cuanto() < R->g_m()) {
        int aux = RR->o_cuanto() - 1;
        pair<int, int> temp = RR->g_punto(aux);
        R->a_point(temp);
        RR->delete_punto(aux);
    }
    R->balancear();
    RR->balancear();
    return RR;
}

node* r_tree::split_nodo(node*& R) {
    node* RR = new node();
    RR->cambiar_M(R->g_M());
    vector<node*> sons(R->g_todos_hijos());
    R->borrar();
    R->c_es_padre(1);
    RR->c_es_padre(1);
    pickseed_r(sons, R, RR);
    while (!sons.empty()) {
        node* next = picknext_r(sons, R, RR);
        pair<int, int> a = next->g_limite(0), b = next->g_limite(1);
        int areaA = R->ampliar(next);
        int areaB = RR->ampliar(next);
        (areaA < areaB) ? R->a_hijo(next) : RR->a_hijo(next);
    }
    while (RR->o_cuanto() < RR->g_m()) {
        int aux = R->o_cuanto() - 1;
        node* temp = R->g_hijo(aux);
        RR->a_hijo(temp);
        R->delete_hijo(aux);
    }
    while (R->o_cuanto() < R->g_m()) {
        int aux = RR->o_cuanto() - 1;
        node* temp = RR->g_hijo(aux);
        R->a_hijo(temp);
        RR->delete_hijo(aux);
    }
    return RR;
}

node* r_tree::balancear(node*& R, node*& RR) {
    while (R) {
        node* papa = R->g_padre();
        if (papa == 0)break;
        if (RR) {
            papa->a_hijo(RR);
            if (papa->o_cuanto() > papa->g_M())
                RR = split_nodo(papa);
            else
                RR = 0;
        }
        R = papa;
    }
    return RR;
}

void r_tree::pickseed_p(vector<pair<int, int>>& Puntos, node*& GroupA, node*& GroupB) {
    size_t n = Puntos.size();
    int i = 0, j = 0, d = -1000;
    for (size_t ii = 0; ii < n - 1; ++ii) {
        for (size_t jj = ii + 1; jj < n; ++jj) {
            int temp = abs(Puntos[ii].first - Puntos[jj].first) * abs(Puntos[ii].second - Puntos[jj].second);
            if (d < temp) {
                d = temp;
                i = ii;
                j = jj;
            }
        }
    }
    GroupA->a_point(Puntos[i]);
    GroupB->a_point(Puntos[j]);
    Puntos.erase(Puntos.begin() + i);
    Puntos.erase(Puntos.begin() + (j - 1));
}

pair<int, int> r_tree::picknext_p(vector<pair<int, int>>& Puntos, node*& GroupA, node*& GroupB) {
    size_t n = Puntos.size();
    int areaA, areaB, d, D = -1000, ii;
    for (size_t i = 0; i < n; ++i) {
        areaA = GroupA->ampliar(Puntos[i]);
        areaB = GroupB->ampliar(Puntos[i]);
        d = abs(areaA - areaB);
        if (D < d) {
            D = d;
            ii = i;
        }
    }
    pair<int, int> entr = Puntos[ii];
    Puntos.erase(Puntos.begin() + ii);
    return entr;
}

void r_tree::pickseed_r(vector<node*>& Regiones, node*& GroupA, node*& GroupB) {
    size_t n = Regiones.size();
    int i = 0, j = 0, d = -1000;

    for (size_t ii = 0; ii < n - 1; ++ii) {
        pair<int, int> r1 = Regiones[ii]->g_limite(0), R1 = Regiones[ii]->g_limite(1);
        int a1 = abs(R1.first - r1.first) * abs(R1.second - r1.second);
        for (size_t jj = ii + 1; jj < n; ++jj) {
            pair<int, int> r2 = Regiones[jj]->g_limite(0), R2 = Regiones[jj]->g_limite(1);
            int a2 = abs(R2.first - r2.first) * abs(R2.second - r2.second);
            int hijo, y, X, Y;
            (r1.first < r2.first) ? hijo = r1.first : hijo = r2.first;
            (R1.first > R2.first) ? X = r1.first : X = r2.first;
            (r1.second < r2.second) ? y = r1.second : y = r2.second;
            (R1.second > R2.second) ? Y = r1.second : Y = r2.second;

            int temp = abs(X - hijo) * abs(Y - y);
            temp = temp - (a1 + a2);
            if (d < temp) {
                d = temp;
                i = ii;
                j = jj;
            }
        }
    }
    GroupA->a_hijo(Regiones[i]);
    GroupB->a_hijo(Regiones[j]);
    Regiones.erase(Regiones.begin() + i);
    Regiones.erase(Regiones.begin() + (j - 1));
}

node* r_tree::picknext_r(vector<node*>& Regiones, node*& GroupA, node*& GroupB) {
    size_t n = Regiones.size();
    int areaA, areaB, d, D = -1000, ii;
    for (size_t i = 0; i < n; ++i) {
        areaA = GroupA->ampliar(Regiones[i]);
        areaB = GroupB->ampliar(Regiones[i]);
        d = abs(areaA - areaB);
        if (D < d) {
            D = d;
            ii = i;
        }
    }
    node* entr = Regiones[ii];
    Regiones.erase(Regiones.begin() + ii);
    return entr;
}

int main() {
    r_tree* Rt;
    int X = 51;
    Rt = new r_tree(X, 0, 100, 0, 100);
    pair<int, int> a(2, 3);
    pair<int, int> b(1, 4);
    Rt->insertar(a);
    Rt->insertar(b);
    /*
    */
    return 0;
}