
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <limits>
using namespace std;

/*
  HOSPITAL DSA MANAGEMENT SYSTEM
  C++ Data Structures & Algorithms Semester Project

  Topics demonstrated:
  Arrays, linear/binary search, selection/bubble/insertion sort,
  singly/doubly linked lists, stacks and stack applications,
  queues, circular queue, priority queue, multiple queues,
  graphs (matrix/list), BFS, DFS, Prim, Kruskal,
  binary tree, BST, heap, AVL tree, hashing and file handling.
*/

struct Patient {
    int id;
    string name;
    int age;
    string condition;
    int priority;

    Patient(int i=0,string n="",int a=0,string c="Normal",int p=4)
        : id(i), name(n), age(a), condition(c), priority(p) {}

    void display() const {
        cout << "ID: " << id << " | Name: " << name
             << " | Age: " << age << " | Condition: " << condition
             << " | Priority: " << priority << "\n";
    }
};

int readInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
    return x;
}

string readLine(const string& prompt) {
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string s;
    getline(cin, s);
    return s;
}

Patient inputPatient() {
    int id = readInt("Patient ID: ");
    string name = readLine("Name: ");
    int age = readInt("Age: ");
    string condition = readLine("Condition: ");
    int p = readInt("Priority (1 Critical, 2 Serious, 3 Moderate, 4 Normal): ");
    if (p < 1 || p > 4) p = 4;
    return Patient(id, name, age, condition, p);
}

/* ========================= ARRAY / SEARCH / SORT ========================= */

const int MAX_PATIENTS = 100;

void displayPatients(const Patient a[], int n) {
    if (!n) { cout << "No patients.\n"; return; }
    for (int i=0;i<n;i++) a[i].display();
}

int linearSearch(const Patient a[], int n, int id) {
    for (int i=0;i<n;i++) if (a[i].id == id) return i;
    return -1;
}

int binarySearch(const Patient a[], int n, int id) {
    int l=0,r=n-1;
    while(l<=r) {
        int m=l+(r-l)/2;
        if(a[m].id==id) return m;
        if(a[m].id<id) l=m+1; else r=m-1;
    }
    return -1;
}

void selectionSort(Patient a[], int n) {
    for(int i=0;i<n-1;i++) {
        int m=i;
        for(int j=i+1;j<n;j++) if(a[j].id<a[m].id) m=j;
        swap(a[i],a[m]);
    }
}

void bubbleSort(Patient a[], int n) {
    for(int i=0;i<n-1;i++) {
        bool changed=false;
        for(int j=0;j<n-i-1;j++) if(a[j].id>a[j+1].id) {
            swap(a[j],a[j+1]); changed=true;
        }
        if(!changed) break;
    }
}

void insertionSort(Patient a[], int n) {
    for(int i=1;i<n;i++) {
        Patient key=a[i]; int j=i-1;
        while(j>=0 && a[j].id>key.id) { a[j+1]=a[j]; --j; }
        a[j+1]=key;
    }
}

/* ========================= SINGLY LINKED LIST ========================= */

class SinglyLinkedList {
    struct Node { Patient p; Node* next; Node(const Patient& x):p(x),next(NULL){} };
    Node* head; Node* tail;
public:
    SinglyLinkedList():head(NULL),tail(NULL){}
    ~SinglyLinkedList(){removeAll();}

    void addToHead(const Patient& p) {
        Node* n=new Node(p); n->next=head; head=n;
        if(!tail) tail=n;
    }
    void addToTail(const Patient& p) {
        Node* n=new Node(p);
        if(!tail) head=tail=n;
        else {tail->next=n; tail=n;}
    }
    bool addAfter(int id,const Patient& p) {
        Node* c=head; while(c&&c->p.id!=id)c=c->next;
        if(!c)return false;
        Node*n=new Node(p);n->next=c->next;c->next=n;
        if(tail==c)tail=n;return true;
    }
    bool addBefore(int id,const Patient& p) {
        if(!head)return false;
        if(head->p.id==id){addToHead(p);return true;}
        Node* c=head;
        while(c->next&&c->next->p.id!=id)c=c->next;
        if(!c->next)return false;
        Node*n=new Node(p);n->next=c->next;c->next=n;return true;
    }
    void addSorted(const Patient& p) {
        if(!head||p.id<head->p.id){addToHead(p);return;}
        Node*c=head;
        while(c->next&&c->next->p.id<p.id)c=c->next;
        Node*n=new Node(p);n->next=c->next;c->next=n;
        if(tail==c)tail=n;
    }
    bool remove(int id) {
        if(!head)return false;
        if(head->p.id==id){Node*t=head;head=head->next;if(tail==t)tail=head;delete t;return true;}
        Node*c=head;
        while(c->next&&c->next->p.id!=id)c=c->next;
        if(!c->next)return false;
        Node*t=c->next;c->next=t->next;if(tail==t)tail=c;delete t;return true;
    }
    void removeAll(){while(head){Node*t=head;head=head->next;delete t;}tail=NULL;}
    bool moveToHead(int id) {
        if(!head)return false;if(head->p.id==id)return true;
        Node*prev=head;
        while(prev->next&&prev->next->p.id!=id)prev=prev->next;
        if(!prev->next)return false;
        Node*n=prev->next;prev->next=n->next;if(tail==n)tail=prev;
        n->next=head;head=n;return true;
    }
    bool moveToTail(int id) {
        if(!head||!tail)return false;if(tail->p.id==id)return true;
        Node*prev=head;
        while(prev->next&&prev->next->p.id!=id)prev=prev->next;
        if(!prev->next)return false;
        Node*n=prev->next;prev->next=n->next;n->next=NULL;tail->next=n;tail=n;return true;
    }
    void traverse()const {
        if(!head){cout<<"List empty.\n";return;}
        for(Node*c=head;c;c=c->next)c->p.display();
    }
};

/* ========================= DOUBLY LINKED LIST ========================= */

class DoublyLinkedList {
    struct Node { Patient p; Node* prev; Node* next; Node(const Patient&x):p(x),prev(NULL),next(NULL){} };
    Node* head; Node* tail;
public:
    DoublyLinkedList():head(NULL),tail(NULL){}
    ~DoublyLinkedList(){removeAll();}
    void addToHead(const Patient&p){Node*n=new Node(p);n->next=head;if(head)head->prev=n;else tail=n;head=n;}
    void addToTail(const Patient&p){Node*n=new Node(p);n->prev=tail;if(tail)tail->next=n;else head=n;tail=n;}
    bool addAfter(int id,const Patient&p){
        Node*c=head;while(c&&c->p.id!=id)c=c->next;if(!c)return false;
        if(c==tail){addToTail(p);return true;}Node*n=new Node(p);
        n->prev=c;n->next=c->next;c->next->prev=n;c->next=n;return true;
    }
    bool addBefore(int id,const Patient&p){
        Node*c=head;while(c&&c->p.id!=id)c=c->next;if(!c)return false;
        if(c==head){addToHead(p);return true;}Node*n=new Node(p);
        n->next=c;n->prev=c->prev;c->prev->next=n;c->prev=n;return true;
    }
    void addSorted(const Patient&p){
        if(!head||p.id<head->p.id){addToHead(p);return;}
        Node*c=head;while(c->next&&c->next->p.id<p.id)c=c->next;
        if(c==tail){addToTail(p);return;}addAfter(c->p.id,p);
    }
    bool remove(int id){
        Node*c=head;while(c&&c->p.id!=id)c=c->next;if(!c)return false;
        if(c->prev)c->prev->next=c->next;else head=c->next;
        if(c->next)c->next->prev=c->prev;else tail=c->prev;
        delete c;return true;
    }
    void removeAll(){while(head){Node*t=head;head=head->next;delete t;}tail=NULL;}
    bool moveToHead(int id){
        Node*c=head;while(c&&c->p.id!=id)c=c->next;if(!c)return false;if(c==head)return true;
        if(c->prev)c->prev->next=c->next;if(c->next)c->next->prev=c->prev;if(c==tail)tail=c->prev;
        c->prev=NULL;c->next=head;head->prev=c;head=c;return true;
    }
    bool moveToTail(int id){
        Node*c=head;while(c&&c->p.id!=id)c=c->next;if(!c)return false;if(c==tail)return true;
        if(c->prev)c->prev->next=c->next;if(c->next)c->next->prev=c->prev;if(c==head)head=c->next;
        c->next=NULL;c->prev=tail;tail->next=c;tail=c;return true;
    }
    void forward()const{if(!head){cout<<"List empty.\n";return;}for(Node*c=head;c;c=c->next)c->p.display();}
    void backward()const{if(!tail){cout<<"List empty.\n";return;}for(Node*c=tail;c;c=c->prev)c->p.display();}
};

/* ========================= STACK ========================= */

class ArrayStack {
    static const int MAX=100; char a[MAX]; int top;
public:
    ArrayStack():top(-1){}
    bool push(char c){if(top==MAX-1)return false;a[++top]=c;return true;}
    bool pop(char&c){if(top<0)return false;c=a[top--];return true;}
    bool peek(char&c)const{if(top<0)return false;c=a[top];return true;}
    bool isEmpty()const{return top<0;} bool isFull()const{return top==MAX-1;}
};

class LinkedStack {
    struct Node{char c;Node*next;Node(char x,Node*n):c(x),next(n){}};
    Node*top;
public:
    LinkedStack():top(NULL){}~LinkedStack(){char c;while(pop(c)){}}
    bool push(char c){top=new Node(c,top);return true;}
    bool pop(char&c){if(!top)return false;Node*t=top;c=t->c;top=t->next;delete t;return true;}
    bool peek(char&c)const{if(!top)return false;c=top->c;return true;}
    bool isEmpty()const{return !top;}
};

int precedence(char c){if(c=='^')return 3;if(c=='*'||c=='/')return 2;if(c=='+'||c=='-')return 1;return 0;}
bool op(char c){return c=='+'||c=='-'||c=='*'||c=='/'||c=='^';}
string noSpaces(const string&s){string r;for(char c:s)if(!isspace((unsigned char)c))r+=c;return r;}

string reverseString(const string&s){ArrayStack st;for(char c:s)st.push(c);char x;string r;while(st.pop(x))r+=x;return r;}

bool symbolsBalanced(const string&s){
    ArrayStack st;char x;
    for(char c:s){
        if(c=='('||c=='['||c=='{')st.push(c);
        else if(c==')'||c==']'||c=='}'){
            if(!st.pop(x))return false;
            if((c==')'&&x!='(')||(c==']'&&x!='[')||(c=='}'&&x!='{'))return false;
        }
    }
    return st.isEmpty();
}

string infixToPostfix(string s){
    s=noSpaces(s);ArrayStack st;char x;string out;
    for(char c:s){
        if(isalnum((unsigned char)c))out+=c;
        else if(c=='(')st.push(c);
        else if(c==')'){while(st.peek(x)&&x!='('){st.pop(x);out+=x;}st.pop(x);}
        else if(op(c)){
            while(st.peek(x)&&x!='('&&precedence(x)>=precedence(c)){st.pop(x);out+=x;}
            st.push(c);
        }
    }
    while(st.pop(x))if(x!='(')out+=x;return out;
}

string infixToPrefix(string s){
    s=noSpaces(s);reverse(s.begin(),s.end());
    for(char&c:s){if(c=='(')c=')';else if(c==')')c='(';}
    string p=infixToPostfix(s);reverse(p.begin(),p.end());return p;
}

string postfixToInfix(string s){
    vector<string> st;
    for(char c:noSpaces(s)){
        if(isalnum((unsigned char)c))st.push_back(string(1,c));
        else if(op(c)&&st.size()>=2){string b=st.back();st.pop_back();string a=st.back();st.pop_back();st.push_back("("+a+c+b+")");}
        else return "";
    }
    return st.size()==1?st[0]:"";
}

string postfixToPrefix(string s){
    vector<string>st;
    for(char c:noSpaces(s)){
        if(isalnum((unsigned char)c))st.push_back(string(1,c));
        else if(op(c)&&st.size()>=2){string b=st.back();st.pop_back();string a=st.back();st.pop_back();st.push_back(string(1,c)+a+b);}
        else return "";
    }
    return st.size()==1?st[0]:"";
}

string prefixToInfix(string s){
    vector<string>st;s=noSpaces(s);
    for(int i=(int)s.size()-1;i>=0;i--){char c=s[i];
        if(isalnum((unsigned char)c))st.push_back(string(1,c));
        else if(op(c)&&st.size()>=2){string a=st.back();st.pop_back();string b=st.back();st.pop_back();st.push_back("("+a+c+b+")");}
        else return "";
    }
    return st.size()==1?st[0]:"";
}

string prefixToPostfix(string s){
    vector<string>st;s=noSpaces(s);
    for(int i=(int)s.size()-1;i>=0;i--){char c=s[i];
        if(isalnum((unsigned char)c))st.push_back(string(1,c));
        else if(op(c)&&st.size()>=2){string a=st.back();st.pop_back();string b=st.back();st.pop_back();st.push_back(a+b+c);}
        else return "";
    }
    return st.size()==1?st[0]:"";
}

double evaluatePostfix(const string&s,bool&ok){
    vector<double>st;ok=true;stringstream ss(s);string t;
    if(s.find(' ')!=string::npos){
        while(ss>>t){
            if(t.size()==1&&op(t[0])){
                if(st.size()<2){ok=false;return 0;}double b=st.back(); st.pop_back(); double a=st.back(); st.pop_back();
                char c=t[0];if(c=='+')st.push_back(a+b);else if(c=='-')st.push_back(a-b);else if(c=='*')st.push_back(a*b);
                else if(c=='/'){if(b==0){ok=false;return 0;}st.push_back(a/b);}else st.push_back(pow(a,b));
            }else st.push_back(atof(t.c_str()));
        }
    }else{
        for(char c:noSpaces(s)){
            if(isdigit((unsigned char)c))st.push_back(c-'0');
            else if(op(c)){
                if(st.size()<2){ok=false;return 0;}double b=st.back(); st.pop_back(); double a=st.back(); st.pop_back();
                if(c=='+')st.push_back(a+b);else if(c=='-')st.push_back(a-b);else if(c=='*')st.push_back(a*b);
                else if(c=='/'){if(b==0){ok=false;return 0;}st.push_back(a/b);}else st.push_back(pow(a,b));
            }else{ok=false;return 0;}
        }
    }
    if(st.size()!=1)ok=false;return ok?st[0]:0;
}

/* ========================= QUEUES ========================= */

class ArrayQueue {
    static const int MAX=20;Patient a[MAX];int front,rear,count;
public:
    ArrayQueue():front(0),rear(-1),count(0){}
    bool enqueue(const Patient&p){if(count==MAX)return false;a[++rear]=p;count++;return true;}
    bool dequeue(Patient&p){if(!count)return false;p=a[front++];count--;if(!count){front=0;rear=-1;}return true;}
    bool isEmpty()const{return count==0;}bool isFull()const{return count==MAX;}
    void display()const{if(!count){cout<<"Queue empty.\n";return;}for(int i=front;i<=rear;i++)a[i].display();}
};

class LinkedQueue {
    struct Node{Patient p;Node*next;Node(const Patient&x):p(x),next(NULL){}};
    Node*front;Node*rear;
public:
    LinkedQueue():front(NULL),rear(NULL){}~LinkedQueue(){Patient p;while(dequeue(p)){}}
    bool enqueue(const Patient&p){Node*n=new Node(p);if(!rear)front=rear=n;else{rear->next=n;rear=n;}return true;}
    bool dequeue(Patient&p){if(!front)return false;Node*t=front;p=t->p;front=t->next;if(!front)rear=NULL;delete t;return true;}
    bool isEmpty()const{return !front;}
    void display()const{if(!front){cout<<"Queue empty.\n";return;}for(Node*n=front;n;n=n->next)n->p.display();}
};

class CircularQueue {
    static const int MAX=10;Patient a[MAX];int front,rear,count;
public:
    CircularQueue():front(0),rear(-1),count(0){}
    bool enqueue(const Patient&p){if(count==MAX)return false;rear=(rear+1)%MAX;a[rear]=p;count++;return true;}
    bool dequeue(Patient&p){if(!count)return false;p=a[front];front=(front+1)%MAX;count--;return true;}
    bool isEmpty()const{return count==0;}bool isFull()const{return count==MAX;}
    void display()const{if(!count){cout<<"Circular queue empty.\n";return;}for(int i=0;i<count;i++)a[(front+i)%MAX].display();}
};

class PriorityQueue {
    struct Node{Patient p;Node*next;Node(const Patient&x):p(x),next(NULL){}};Node*front;
public:
    PriorityQueue():front(NULL){}~PriorityQueue(){Patient p;while(dequeue(p)){}}
    bool enqueue(const Patient&p){
        Node*n=new Node(p);
        if(!front||p.priority<front->p.priority){n->next=front;front=n;return true;}
        Node*c=front;while(c->next&&c->next->p.priority<=p.priority)c=c->next;
        n->next=c->next;c->next=n;return true;
    }
    bool dequeue(Patient&p){if(!front)return false;Node*t=front;p=t->p;front=t->next;delete t;return true;}
    void display()const{if(!front){cout<<"Priority queue empty.\n";return;}for(Node*n=front;n;n=n->next)n->p.display();}
};

/* ========================= GRAPH ========================= */

class Graph {
    static const int V=6;int m[V][V];vector<int> adj[V];
    void dfsRec(int v,bool seen[])const{seen[v]=true;cout<<v<<" ";for(int x:adj[v])if(!seen[x])dfsRec(x,seen);}
public:
    Graph(){for(int i=0;i<V;i++)for(int j=0;j<V;j++)m[i][j]=0;}
    void addEdge(int u,int v,int w){m[u][v]=m[v][u]=w;adj[u].push_back(v);adj[v].push_back(u);}
    void matrix()const{cout<<"Adjacency Matrix:\n";for(int i=0;i<V;i++){for(int j=0;j<V;j++)cout<<m[i][j]<<" ";cout<<"\n";}}
    void list()const{cout<<"Adjacency List:\n";for(int i=0;i<V;i++){cout<<i<<" -> ";for(int x:adj[i])cout<<x<<" ";cout<<"\n";}}
    void bfs(int s)const{bool seen[V]={};queue<int>q;q.push(s);seen[s]=true;cout<<"BFS: ";while(!q.empty()){int v=q.front();q.pop();cout<<v<<" ";for(int x:adj[v])if(!seen[x])seen[x]=true,q.push(x);}cout<<"\n";}
    void dfs(int s)const{bool seen[V]={};cout<<"DFS: ";dfsRec(s,seen);cout<<"\n";}
    void prim()const{
        const int INF=1000000000;int key[V],par[V];bool used[V]={};
        for(int i=0;i<V;i++)key[i]=INF,par[i]=-1;key[0]=0;int total=0;
        for(int c=0;c<V;c++){int u=-1;for(int i=0;i<V;i++)if(!used[i]&&(u==-1||key[i]<key[u]))u=i;used[u]=true;
            if(par[u]!=-1){cout<<par[u]<<" - "<<u<<" ("<<m[par[u]][u]<<")\n";total+=m[par[u]][u];}
            for(int v=0;v<V;v++)if(m[u][v]&&!used[v]&&m[u][v]<key[v])key[v]=m[u][v],par[v]=u;
        }cout<<"Prim total: "<<total<<"\n";
    }
    void kruskal()const{
        struct E{int u,v,w;};vector<E>e;for(int i=0;i<V;i++)for(int j=i+1;j<V;j++)if(m[i][j])e.push_back({i,j,m[i][j]});
        sort(e.begin(),e.end(),[](E a,E b){return a.w<b.w;});int p[V],r[V]={};for(int i=0;i<V;i++)p[i]=i;
        auto findRoot=[&](int x){while(p[x]!=x)x=p[x];return x;};int total=0,c=0;
        for(E x:e){int a=findRoot(x.u),b=findRoot(x.v);if(a!=b){p[b]=a;cout<<x.u<<" - "<<x.v<<" ("<<x.w<<")\n";total+=x.w;if(++c==V-1)break;}}
        cout<<"Kruskal total: "<<total<<"\n";
    }
};

/* ========================= BINARY TREE / BST ========================= */

class BST {
    struct Node{int key;Node*l,*r;Node(int k):key(k),l(NULL),r(NULL){}};Node*root;
    Node*ins(Node*n,int k){if(!n)return new Node(k);if(k<n->key)n->l=ins(n->l,k);else if(k>n->key)n->r=ins(n->r,k);return n;}
    Node*find(Node*n,int k)const{if(!n||n->key==k)return n;return k<n->key?find(n->l,k):find(n->r,k);}
    Node*minNode(Node*n)const{while(n&&n->l)n=n->l;return n;}
    Node*del(Node*n,int k){if(!n)return NULL;if(k<n->key)n->l=del(n->l,k);else if(k>n->key)n->r=del(n->r,k);else{if(!n->l){Node*t=n->r;delete n;return t;}if(!n->r){Node*t=n->l;delete n;return t;}Node*t=minNode(n->r);n->key=t->key;n->r=del(n->r,t->key);}return n;}
    void in(Node*n)const{if(n){in(n->l);cout<<n->key<<" ";in(n->r);}}
    void pre(Node*n)const{if(n){cout<<n->key<<" ";pre(n->l);pre(n->r);}}
    void post(Node*n)const{if(n){post(n->l);post(n->r);cout<<n->key<<" ";}}
    void clear(Node*n){if(n){clear(n->l);clear(n->r);delete n;}}
public:
    BST():root(NULL){}~BST(){clear(root);}
    void insert(int k){root=ins(root,k);}bool search(int k)const{return find(root,k)!=NULL;}void remove(int k){root=del(root,k);}
    void inorder()const{in(root);cout<<"\n";}void preorder()const{pre(root);cout<<"\n";}void postorder()const{post(root);cout<<"\n";}
};

class Heap {
    static const int MAX=100;int maxH[MAX],minH[MAX],maxN,minN;
    void downMax(int i){int l=2*i+1,r=l+1,b=i;if(l<maxN&&maxH[l]>maxH[b])b=l;if(r<maxN&&maxH[r]>maxH[b])b=r;if(b!=i)swap(maxH[i],maxH[b]),downMax(b);}
    void downMin(int i){int l=2*i+1,r=l+1,b=i;if(l<minN&&minH[l]<minH[b])b=l;if(r<minN&&minH[r]<minH[b])b=r;if(b!=i)swap(minH[i],minH[b]),downMin(b);}
public:
    Heap():maxN(0),minN(0){}
    void insertMax(int x){if(maxN==MAX)return;int i=maxN++;maxH[i]=x;while(i&&maxH[(i-1)/2]<maxH[i])swap(maxH[i],maxH[(i-1)/2]),i=(i-1)/2;}
    void insertMin(int x){if(minN==MAX)return;int i=minN++;minH[i]=x;while(i&&minH[(i-1)/2]>minH[i])swap(minH[i],minH[(i-1)/2]),i=(i-1)/2;}
    void deleteMax(){if(!maxN)return;cout<<"Deleted max: "<<maxH[0]<<"\n";maxH[0]=maxH[--maxN];if(maxN)downMax(0);}
    void deleteMin(){if(!minN)return;cout<<"Deleted min: "<<minH[0]<<"\n";minH[0]=minH[--minN];if(minN)downMin(0);}
    void show()const{cout<<"Max Heap: ";for(int i=0;i<maxN;i++)cout<<maxH[i]<<" ";cout<<"\nMin Heap: ";for(int i=0;i<minN;i++)cout<<minH[i]<<" ";cout<<"\n";}
};

/* ========================= AVL ========================= */

class AVL {
    struct Node{int k,h;Node*l,*r;Node(int x):k(x),h(1),l(NULL),r(NULL){}};Node*root;
    int h(Node*n){return n?n->h:0;}int bal(Node*n){return n?h(n->l)-h(n->r):0;}
    Node*rr(Node*y){Node*x=y->l,*t=x->r;x->r=y;y->l=t;y->h=1+max(h(y->l),h(y->r));x->h=1+max(h(x->l),h(x->r));return x;}
    Node*rl(Node*x){Node*y=x->r,*t=y->l;y->l=x;x->r=t;x->h=1+max(h(x->l),h(x->r));y->h=1+max(h(y->l),h(y->r));return y;}
    Node*ins(Node*n,int k){if(!n)return new Node(k);if(k<n->k)n->l=ins(n->l,k);else if(k>n->k)n->r=ins(n->r,k);else return n;n->h=1+max(h(n->l),h(n->r));int b=bal(n);
        if(b>1&&k<n->l->k)return rr(n);if(b<-1&&k>n->r->k)return rl(n);if(b>1&&k>n->l->k){n->l=rl(n->l);return rr(n);}if(b<-1&&k<n->r->k){n->r=rr(n->r);return rl(n);}return n;}
    Node*mn(Node*n){while(n->l)n=n->l;return n;}
    Node*del(Node*n,int k){if(!n)return NULL;if(k<n->k)n->l=del(n->l,k);else if(k>n->k)n->r=del(n->r,k);else{if(!n->l||!n->r){Node*t=n->l?n->l:n->r;if(!t){delete n;return NULL;}*n=*t;delete t;}else{Node*t=mn(n->r);n->k=t->k;n->r=del(n->r,t->k);}}
        n->h=1+max(h(n->l),h(n->r));int b=bal(n);if(b>1&&bal(n->l)>=0)return rr(n);if(b>1){n->l=rl(n->l);return rr(n);}if(b<-1&&bal(n->r)<=0)return rl(n);if(b<-1){n->r=rr(n->r);return rl(n);}return n;}
    void in(Node*n){if(n){in(n->l);cout<<n->k<<" ";in(n->r);}}void clear(Node*n){if(n){clear(n->l);clear(n->r);delete n;}}
public:
    AVL():root(NULL){}~AVL(){clear(root);}void insert(int k){root=ins(root,k);}void remove(int k){root=del(root,k);}void inorder(){in(root);cout<<"\n";}
};

/* ========================= HASHING ========================= */

class HashTable {
    static const int SIZE=11;
    int lp[SIZE],qp[SIZE],ls[SIZE],qs[SIZE];
    struct Node{int k;Node*next;Node(int x):k(x),next(NULL){}};Node*chain[SIZE];
    int divHash(int k)const{return abs(k)%SIZE;}
    int multHash(int k)const{double A=.6180339887,x=abs(k)*A;return int(SIZE*(x-floor(x)));}
    int midHash(int k)const{long long s=1LL*k*k;return int(abs((s/10)%100)%SIZE);}
    int foldHash(int k)const{int sum=0;k=abs(k);while(k){sum+=k%100;k/=100;}return sum%SIZE;}
public:
    HashTable(){for(int i=0;i<SIZE;i++)ls[i]=qs[i]=0,chain[i]=NULL;}
    ~HashTable(){for(int i=0;i<SIZE;i++){Node*n=chain[i];while(n){Node*t=n;n=n->next;delete t;}}}
    void functions(int k){cout<<"Division: "<<divHash(k)<<"\nMultiplication: "<<multHash(k)<<"\nMid-square: "<<midHash(k)<<"\nFolding: "<<foldHash(k)<<"\n";}
    void linearInsert(int k){int h=divHash(k);for(int i=0;i<SIZE;i++){int p=(h+i)%SIZE;if(ls[p]!=1){lp[p]=k;ls[p]=1;return;}}}
    void quadraticInsert(int k){int h=divHash(k);for(int i=0;i<SIZE;i++){int p=(h+i*i)%SIZE;if(qs[p]!=1){qp[p]=k;qs[p]=1;return;}}}
    void chainInsert(int k){int h=divHash(k);Node*n=new Node(k);n->next=chain[h];chain[h]=n;}
    void show(){cout<<"Linear Probing:\n";for(int i=0;i<SIZE;i++)cout<<i<<": "<<(ls[i]==1?to_string(lp[i]):"EMPTY")<<"\n";
        cout<<"Quadratic Probing:\n";for(int i=0;i<SIZE;i++)cout<<i<<": "<<(qs[i]==1?to_string(qp[i]):"EMPTY")<<"\n";
        cout<<"Separate Chaining:\n";for(int i=0;i<SIZE;i++){cout<<i<<": ";for(Node*n=chain[i];n;n=n->next)cout<<n->k<<" -> ";cout<<"NULL\n";}}
};

/* ========================= FILE HANDLING ========================= */

void savePatients(const Patient a[],int n){
    ofstream f("data/patients.txt");
    if(!f){cout<<"Could not open file.\n";return;}
    for(int i=0;i<n;i++)f<<a[i].id<<"|"<<a[i].name<<"|"<<a[i].age<<"|"<<a[i].condition<<"|"<<a[i].priority<<"\n";
    cout<<"Patient data saved.\n";
}

void loadPatients(Patient a[],int&n){
    ifstream f("data/patients.txt");if(!f)return;
    string line;n=0;
    while(getline(f,line)&&n<MAX_PATIENTS){
        stringstream ss(line);string t;Patient p;
        if(getline(ss,t,'|'))p.id=atoi(t.c_str());else continue;
        if(getline(ss,p.name,'|')){}else continue;
        if(getline(ss,t,'|'))p.age=atoi(t.c_str());else continue;
        if(getline(ss,p.condition,'|')){}else continue;
        if(getline(ss,t,'|'))p.priority=atoi(t.c_str());else continue;
        a[n++]=p;
    }
}

/* ========================= DEMONSTRATION SYSTEM ========================= */

class HospitalSystem {
    Patient patients[MAX_PATIENTS];int n;
    SinglyLinkedList singly;DoublyLinkedList doubly;
    ArrayQueue aq;LinkedQueue lq;CircularQueue cq;PriorityQueue pq;
    Graph graph;BST bst;Heap heap;AVL avl;HashTable hash;
public:
    HospitalSystem():n(0){
        graph.addEdge(0,1,4);graph.addEdge(0,3,3);graph.addEdge(1,2,2);
        graph.addEdge(1,5,5);graph.addEdge(2,4,4);graph.addEdge(3,4,6);graph.addEdge(4,5,3);
        patients[n++]=Patient(101,"Ali",20,"Normal",4);
        patients[n++]=Patient(105,"Ahmed",35,"Critical",1);
        patients[n++]=Patient(110,"Usman",28,"Serious",2);
        patients[n++]=Patient(103,"Sara",22,"Moderate",3);
        for(int i=0;i<n;i++){singly.addToTail(patients[i]);doubly.addToTail(patients[i]);aq.enqueue(patients[i]);lq.enqueue(patients[i]);cq.enqueue(patients[i]);pq.enqueue(patients[i]);bst.insert(patients[i].id);avl.insert(patients[i].id);hash.linearInsert(patients[i].id);hash.quadraticInsert(patients[i].id);hash.chainInsert(patients[i].id);}
    }

    void patientMenu(){
        int c;do{
            cout<<"\n--- PATIENT ARRAY ---\n1 Add  2 Display  3 Linear Search  4 Binary Search  5 Save File  6 Load File  0 Back\n";
            c=readInt("Choice: ");
            if(c==1&&n<MAX_PATIENTS)patients[n++]=inputPatient();
            else if(c==2)displayPatients(patients,n);
            else if(c==3){int id=readInt("ID: ");cout<<(linearSearch(patients,n,id)>=0?"Found":"Not Found")<<"\n";}
            else if(c==4){Patient t[MAX_PATIENTS];for(int i=0;i<n;i++)t[i]=patients[i];insertionSort(t,n);int id=readInt("ID: ");cout<<(binarySearch(t,n,id)>=0?"Found":"Not Found")<<"\n";}
            else if(c==5)savePatients(patients,n);
            else if(c==6)loadPatients(patients,n);
        }while(c);
    }

    void sortMenu(){
        Patient t[MAX_PATIENTS];for(int i=0;i<n;i++)t[i]=patients[i];
        int c=readInt("\n1 Selection Sort\n2 Bubble Sort\n3 Insertion Sort\nChoice: ");
        if(c==1)selectionSort(t,n);else if(c==2)bubbleSort(t,n);else if(c==3)insertionSort(t,n);else return;
        displayPatients(t,n);
    }

    void singlyMenu(){
        int c;do{
            cout<<"\n--- SINGLY LINKED LIST ---\n1 AddHead 2 AddTail 3 AddAfter 4 AddBefore 5 AddSorted 6 Remove 7 RemoveAll 8 MoveHead 9 MoveTail 10 Traverse 0 Back\n";
            c=readInt("Choice: ");
            if(c==1)singly.addToHead(inputPatient());else if(c==2)singly.addToTail(inputPatient());
            else if(c==3){int id=readInt("After ID: ");cout<<(singly.addAfter(id,inputPatient())?"Done":"Not found")<<"\n";}
            else if(c==4){int id=readInt("Before ID: ");cout<<(singly.addBefore(id,inputPatient())?"Done":"Not found")<<"\n";}
            else if(c==5)singly.addSorted(inputPatient());
            else if(c==6)cout<<(singly.remove(readInt("ID: "))?"Removed":"Not found")<<"\n";
            else if(c==7)singly.removeAll();else if(c==8)cout<<(singly.moveToHead(readInt("ID: "))?"Moved":"Not found")<<"\n";
            else if(c==9)cout<<(singly.moveToTail(readInt("ID: "))?"Moved":"Not found")<<"\n";else if(c==10)singly.traverse();
        }while(c);
    }

    void doublyMenu(){
        int c;do{
            cout<<"\n--- DOUBLY LINKED LIST ---\n1 AddHead 2 AddTail 3 AddAfter 4 AddBefore 5 AddSorted 6 Remove 7 RemoveAll 8 MoveHead 9 MoveTail 10 Forward 11 Backward 0 Back\n";
            c=readInt("Choice: ");
            if(c==1)doubly.addToHead(inputPatient());else if(c==2)doubly.addToTail(inputPatient());
            else if(c==3){int id=readInt("After ID: ");cout<<(doubly.addAfter(id,inputPatient())?"Done":"Not found")<<"\n";}
            else if(c==4){int id=readInt("Before ID: ");cout<<(doubly.addBefore(id,inputPatient())?"Done":"Not found")<<"\n";}
            else if(c==5)doubly.addSorted(inputPatient());else if(c==6)cout<<(doubly.remove(readInt("ID: "))?"Removed":"Not found")<<"\n";
            else if(c==7)doubly.removeAll();else if(c==8)cout<<(doubly.moveToHead(readInt("ID: "))?"Moved":"Not found")<<"\n";
            else if(c==9)cout<<(doubly.moveToTail(readInt("ID: "))?"Moved":"Not found")<<"\n";
            else if(c==10)doubly.forward();else if(c==11)doubly.backward();
        }while(c);
    }

    void stackMenu(){
        int c;do{
            cout<<"\n--- STACK & APPLICATIONS ---\n1 Array/Linked Stack 2 Reverse String 3 Symbol Balance 4 Infix->Postfix 5 Infix->Prefix 6 Postfix->Infix 7 Postfix->Prefix 8 Prefix->Infix 9 Prefix->Postfix 10 Evaluate Postfix 0 Back\n";
            c=readInt("Choice: ");
            if(c==1){string s=readLine("String: ");ArrayStack a;LinkedStack l;for(char x:s)a.push(x),l.push(x);char z;cout<<"Array pop: ";while(a.pop(z))cout<<z;cout<<"\nLinked pop: ";while(l.pop(z))cout<<z;cout<<"\n";}
            else if(c==2)cout<<reverseString(readLine("String: "))<<"\n";
            else if(c==3)cout<<(symbolsBalanced(readLine("Expression: "))?"Balanced":"Not Balanced")<<"\n";
            else if(c==4)cout<<infixToPostfix(readLine("Infix: "))<<"\n";
            else if(c==5)cout<<infixToPrefix(readLine("Infix: "))<<"\n";
            else if(c==6)cout<<postfixToInfix(readLine("Postfix: "))<<"\n";
            else if(c==7)cout<<postfixToPrefix(readLine("Postfix: "))<<"\n";
            else if(c==8)cout<<prefixToInfix(readLine("Prefix: "))<<"\n";
            else if(c==9)cout<<prefixToPostfix(readLine("Prefix: "))<<"\n";
            else if(c==10){bool ok;double v=evaluatePostfix(readLine("Postfix: "),ok);cout<<(ok?to_string(v):"Invalid expression")<<"\n";}
        }while(c);
    }

    void queueMenu(){
        int c;do{
            cout<<"\n--- QUEUES ---\n1 Array Queue 2 Linked Queue 3 Circular Queue 4 Priority Queue 0 Back\n";
            c=readInt("Choice: ");Patient p;
            if(c==1){aq.display();if(readInt("1 Enqueue 2 Dequeue: ")==1)aq.enqueue(inputPatient());else if(aq.dequeue(p))p.display();}
            else if(c==2){lq.display();if(readInt("1 Enqueue 2 Dequeue: ")==1)lq.enqueue(inputPatient());else if(lq.dequeue(p))p.display();}
            else if(c==3){cq.display();if(readInt("1 Enqueue 2 Dequeue: ")==1)cq.enqueue(inputPatient());else if(cq.dequeue(p))p.display();}
            else if(c==4){pq.display();if(readInt("1 Enqueue 2 Dequeue: ")==1)pq.enqueue(inputPatient());else if(pq.dequeue(p))p.display();}
        }while(c);
    }

    void graphMenu(){
        int c;do{
            cout<<"\n--- GRAPH ---\n1 Matrix 2 Adjacency List 3 BFS 4 DFS 5 Prim MST 6 Kruskal MST 0 Back\n";
            c=readInt("Choice: ");
            if(c==1)graph.matrix();else if(c==2)graph.list();else if(c==3)graph.bfs(readInt("Start 0-5: "));
            else if(c==4)graph.dfs(readInt("Start 0-5: "));else if(c==5)graph.prim();else if(c==6)graph.kruskal();
        }while(c);
    }

    void treeMenu(){
        int c;do{
            cout<<"\n--- BST / TREE ---\n1 Insert 2 Search 3 Delete 4 Inorder 5 Preorder 6 Postorder 0 Back\n";
            c=readInt("Choice: ");
            if(c==1)bst.insert(readInt("Key: "));else if(c==2)cout<<(bst.search(readInt("Key: "))?"Found":"Not Found")<<"\n";
            else if(c==3)bst.remove(readInt("Key: "));else if(c==4)bst.inorder();else if(c==5)bst.preorder();else if(c==6)bst.postorder();
        }while(c);
    }

    void heapMenu(){
        int c;do{
            cout<<"\n--- HEAP ---\n1 Insert Max 2 Delete Max 3 Insert Min 4 Delete Min 5 Display 0 Back\n";
            c=readInt("Choice: ");if(c==1)heap.insertMax(readInt("Value: "));else if(c==2)heap.deleteMax();else if(c==3)heap.insertMin(readInt("Value: "));
            else if(c==4)heap.deleteMin();else if(c==5)heap.show();
        }while(c);
    }

    void avlMenu(){
        int c;do{
            cout<<"\n--- AVL TREE ---\n1 Insert 2 Delete 3 Inorder 0 Back\n";c=readInt("Choice: ");
            if(c==1)avl.insert(readInt("Key: "));else if(c==2)avl.remove(readInt("Key: "));else if(c==3)avl.inorder();
        }while(c);
    }

    void hashMenu(){
        int c;do{
            cout<<"\n--- HASHING ---\n1 Hash Functions 2 Linear Probing 3 Quadratic Probing 4 Separate Chaining 0 Back\n";c=readInt("Choice: ");
            if(c==1)hash.functions(readInt("Key: "));
            else if(c==2){int k=readInt("Key: ");hash.linearInsert(k);cout<<"Inserted using linear probing.\n";}
            else if(c==3){int k=readInt("Key: ");hash.quadraticInsert(k);cout<<"Inserted using quadratic probing.\n";}
            else if(c==4){int k=readInt("Key: ");hash.chainInsert(k);cout<<"Inserted using separate chaining.\n";}
        }while(c);
    }

public:
    void run(){
        int c;do{
            cout<<"\n====================================================\n";
            cout<<"        HOSPITAL DSA MANAGEMENT SYSTEM\n";
            cout<<"====================================================\n";
            cout<<"1 Patient/Array/Search\n2 Sorting\n3 Singly Linked List\n4 Doubly Linked List\n";
            cout<<"5 Stack & Applications\n6 Queues\n7 Graph Algorithms\n8 Binary Search Tree\n9 Heap\n10 AVL Tree\n11 Hashing\n0 Exit\n";
            cout<<"====================================================\n";
            c=readInt("Choice: ");
            switch(c){case 1:patientMenu();break;case 2:sortMenu();break;case 3:singlyMenu();break;case 4:doublyMenu();break;
            case 5:stackMenu();break;case 6:queueMenu();break;case 7:graphMenu();break;case 8:treeMenu();break;case 9:heapMenu();break;case 10:avlMenu();break;case 11:hashMenu();break;case 0:cout<<"Goodbye!\n";break;default:cout<<"Invalid choice.\n";}
        }while(c);
    }
};

int main(){
    HospitalSystem system;
    system.run();
    return 0;
}
