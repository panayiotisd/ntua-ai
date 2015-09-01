#include <set>
#include <queue>
#include <time.h>
#include <stdlib.h>

#define HEUR heur1

using namespace std;

typedef struct Node {
    struct Node *parent;
    int state;
    int g;
    int h;
    struct Node *next;
    struct Node *r_sibling;
    struct Node *child1;
} SearchGraphNode;

SearchGraphNode *OPEN, *CLOSED;

typedef struct {
    int s;
    int f;
    SearchGraphNode *p;
} tuple;

class mycomparison
{
public:
  bool operator() (const tuple& lhs, const tuple& rhs) const
  {
    return (lhs.f>rhs.f);
  }
};

int heur1(int,int,int,int);
int heur2(int,int,int,int);
void add_neigh(int,int,int *,int *,set<int>,set<int>&,priority_queue<tuple,vector<tuple>,mycomparison>&,SearchGraphNode *);
int up(int,int,int *,int *);
int down(int,int,int *,int *);
int left(int,int,int *,int *);
int right(int,int,int *,int *);
int move_r1();
void move_r2();

int X,Y,x1,y1,x2,y2,s1,s2,*g,*f,count=0;
char **plane;

int main() {
    
    int i,done;
    
    scanf("%d %d",&X,&Y);
    scanf("%d %d",&x1,&y1);
    scanf("%d %d",&x2,&y2);
    
    x1--; x2--; y1--; y2--;
    
    plane=(char**)malloc(Y*sizeof(char*));
    
    for (i=0;i<Y;i++)
        plane[i]=(char*)malloc((X+1)*sizeof(char));
    
    for (i=0;i<Y;i++)
            scanf("%s",plane[i]);
    
    g = (int*) malloc(sizeof(int)*X*Y);
    f = (int*) malloc(sizeof(int)*X*Y);
    
    do {
        move_r2();
        done=move_r1();
    }
    while (!done);
    
    printf("Nodes opened: %d\n",count);
    printf("Time: %.3f\n",(((float)clock())/CLOCKS_PER_SEC));
    
    return 0;
}

int move_r1(){
    //declarations
    int found=0,i,x,y,nx,ny;
    tuple current;
    SearchGraphNode *curr,*temp;
    set<int> openset, closedset;
    set<int>::iterator it;
    priority_queue< tuple, vector<tuple>, mycomparison > frontier;
    
    for (i=0;i<X*Y;i++) g[i]=0;
    
    //main
    s1=y1*X+x1;
    s2=y2*X+x2;
    
    g[s1]=0;
    f[s1]=g[s1]+HEUR(x1,y1,x2,y2);
    
    curr=(SearchGraphNode*)malloc(sizeof(SearchGraphNode));
    
    curr->parent=NULL;
    curr->state=s1;
    curr->g=g[s1];
    curr->h=HEUR(x1,y1,x2,y2);
    curr->next=OPEN;
    curr->r_sibling=NULL;
    curr->child1=NULL;
    
    count++;
    
    tuple t = {s1,f[s1],curr};
    frontier.push(t);
    openset.insert(s1);
    
    while (!openset.empty()) {
          
          do {
              current=frontier.top();
              frontier.pop();
              it=openset.find(current.s);
          }
          while (it==openset.end());
          
          if (current.s==s2) { found=1; break; }
          
          y=(current.s)/X;
          x=(current.s)%X;
          
          openset.erase(current.s);
          closedset.insert(current.s);
          
          if (up(x,y,&nx,&ny)) add_neigh(nx,ny,g,f,closedset,openset,frontier,current.p);
          if (down(x,y,&nx,&ny)) add_neigh(nx,ny,g,f,closedset,openset,frontier,current.p);
          if (left(x,y,&nx,&ny)) add_neigh(nx,ny,g,f,closedset,openset,frontier,current.p);
          if (right(x,y,&nx,&ny))add_neigh(nx,ny,g,f,closedset,openset,frontier,current.p);
    }
    
    if (found) {
       curr=current.p;
       if ((curr->g)<=3) {
                         y1=(curr->state)/X;
           x1=(curr->state)%X;
           return 1;
           }
       else {
           while ((curr)&&((curr->g)>3)) {     
               temp=curr;
               curr=curr->parent;
               free(temp);
           }
           y1=(curr->state)/X;
           x1=(curr->state)%X;
           
           while (curr) {     
               temp=curr;
               curr=curr->parent;
               free(temp);
           }
           return 0;
       }
    }
    else printf("Something went wrong....\n");
    
    return 1;
}

void move_r2() {
     
     int r,flag=0,nx,ny;
     
     plane[y1][x1]='X';
     
     if ((!up(x2,y2,&nx,&ny))&&(!down(x2,y2,&nx,&ny))&&(!left(x2,y2,&nx,&ny))&&(!right(x2,y2,&nx,&ny)))
        return;
     
     plane[y1][x1]='O';
     
     do {
         
         srand(clock()+time(0));
         r=rand()%4;
         
         if (r==0) flag=up(x2,y2,&nx,&ny);
         if (r==1) flag=down(x2,y2,&nx,&ny);
         if (r==2) flag=left(x2,y2,&nx,&ny);
         if (r==3) flag=right(x2,y2,&nx,&ny);
     }
     while ((!flag) || ((nx==x1)&&(ny==y1)));
     
     x2=nx;
     y2=ny;
}

int up(int x, int y, int *nx, int *ny) {

    if ((y!=0)&&(plane[y-1][x]=='O')) {
       *nx=x;
       *ny=y-1;
       return 1;
    }
    else return 0;
}

int down(int x, int y, int *nx, int *ny) {
    
    if ((y!=(Y-1))&&(plane[y+1][x]=='O')) {
       *nx=x;
       *ny=y+1;
       return 1;
    }
    else return 0;
}

int left(int x, int y, int *nx, int *ny) {
    
    if ((x!=0)&&(plane[y][x-1]=='O')) {
       *nx=x-1;
       *ny=y;
       return 1;
    }
    else return 0;
}

int right(int x, int y, int *nx, int *ny) {
    
    if ((x!=(X-1))&&(plane[y][x+1]=='O')) {
       *nx=x+1;
       *ny=y;
       return 1;
    }
    else return 0;
}

int heur1(int x1, int y1, int x2, int y2) {
    return (abs(x1-x2)+abs(y1-y2));
}

int heur2(int x1, int y1, int x2, int y2) {
    return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}

void add_neigh(int x,int y,int *g,int *f,set<int> closedset,set<int>& openset,priority_queue<tuple,vector<tuple>,mycomparison>& frontier,SearchGraphNode *par) {
     
     int ng,s;
     set<int>::iterator it;
     
     s=y*X+x;
     ng=g[par->state]+1;
     
     it=closedset.find(s);
     
     if ((it!=closedset.end()) && (ng>=g[s])) return;
     
     it=openset.find(s);
     
     if ((it==openset.end()) || (ng<g[s])) {
        
        g[s]=ng;
        f[s]=g[s]+HEUR(x,y,x2,y2);
        
        SearchGraphNode *curr=(SearchGraphNode*)malloc(sizeof(SearchGraphNode));
        
        curr->parent=par;
        curr->state=s;
        curr->g=g[s];
        curr->h=HEUR(x,y,x2,y2);
        curr->next=OPEN;
        curr->child1=NULL;
        
        count++;
        
        curr->r_sibling=par->child1;
        par->child1=curr;
        
        tuple t = {s,f[s],curr};
        frontier.push(t);
        openset.insert(s);
        
     }
}
