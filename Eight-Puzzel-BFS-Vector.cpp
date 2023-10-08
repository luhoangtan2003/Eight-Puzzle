#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
using namespace std;

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MAXLENGTH 500

const char *Action[] = {
    "First state","Move cell to UP","Move cell to DOWN","Move cell to LEFT","Move cell to RIGHT"
    };

struct State{
    int EightPuzze[ROWS][COLS];
    int Emptyrow;
    int Emptycol;
};

void Printstate(State State){
    printf("----------\n");
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            printf("|%d ",State.EightPuzze[Row][Col]);
        }
        printf("|\n");
    }
    printf("----------\n");
}

int Comparestates(State S1, State S2){
    if(S1.Emptyrow!=S2.Emptyrow||S1.Emptycol!=S2.Emptycol){
        return 0;
    }
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            if(S1.EightPuzze[Row][Col]!=S2.EightPuzze[Row][Col]){
                return 0;
            }
        }
    }
    return 1;
}

int Goalcheck(State S, State Goal){
    return Comparestates(S,Goal);
}

int Up(State S, State *Result){
    *Result = S;
    int EmptyRowCurrent = S.Emptyrow;
    int EmptyColCurrent = S.Emptycol;
    if(EmptyRowCurrent>0){
        Result->Emptyrow = EmptyRowCurrent - 1;
        Result->Emptycol = EmptyColCurrent;
        Result->EightPuzze[EmptyRowCurrent][EmptyColCurrent] = S.EightPuzze[EmptyRowCurrent-1][EmptyColCurrent];
        Result->EightPuzze[EmptyRowCurrent-1][EmptyColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int Down(State S, State *Result){
    *Result = S;
    int EmptyRowCurrent = S.Emptyrow;
    int EmptyColCurrent = S.Emptycol;
    if(EmptyRowCurrent<2){
        Result->Emptyrow = EmptyRowCurrent + 1;
        Result->Emptycol = EmptyColCurrent;
        Result->EightPuzze[EmptyRowCurrent][EmptyColCurrent] = S.EightPuzze[EmptyRowCurrent+1][EmptyColCurrent];
        Result->EightPuzze[EmptyRowCurrent+1][EmptyColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int Left(State S, State *Result){
    *Result = S;
    int EmptyRowCurrent = S.Emptyrow;
    int EmptyColCurrent = S.Emptycol;
    if(EmptyColCurrent>0){
        Result->Emptycol = EmptyColCurrent - 1;
        Result->Emptyrow = EmptyRowCurrent;
        Result->EightPuzze[EmptyRowCurrent][EmptyColCurrent] = S.EightPuzze[EmptyRowCurrent][EmptyColCurrent-1];
        Result->EightPuzze[EmptyRowCurrent][EmptyColCurrent-1] = EMPTY;
        return 1;
    }
    return 0;
}

int Right(State S, State *Result){
    *Result = S;
    int EmptyRowCurrent = S.Emptyrow;
    int EmptyColCurrent = S.Emptycol;
    if(EmptyColCurrent<2){
        Result->Emptycol = EmptyColCurrent + 1;
        Result->Emptyrow = EmptyRowCurrent;
        Result->EightPuzze[EmptyRowCurrent][EmptyColCurrent] = S.EightPuzze[EmptyRowCurrent][EmptyColCurrent+1];
        Result->EightPuzze[EmptyRowCurrent][EmptyColCurrent+1] = EMPTY;
        return 1;
    }
    return 0;
}

int Calloperator(State S, State *Result, int Option){
    switch(Option){
        case 1: return Up(S,Result);
        case 2: return Down(S,Result);
        case 3: return Left(S,Result);
        case 4: return Right(S,Result);
        default: printf("Lỗi"); break;
    }
    return 0;
}

int Heuristic_A(State S, State Goal){
    int Count = 0;
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            if(S.EightPuzze[Row][Col]!=Goal.EightPuzze[Row][Col]){
                Count++;
            }
        }
    }
    return Count;
}

struct Node{
    State State;
    Node *Dad;
    int Order;
    int Heuristic;
};

Node *Findstate(State State, vector<Node*> List, vector<Node*>::iterator *Position){
    vector<Node*>::iterator It = List.begin();
    if(List.size()==0){
        return NULL;
    }
    while(It != List.end()){
        if(Comparestates((*It)->State,State)){
            *Position = It;
            return *It;
        }
        It = List.erase(It);
    }
    return NULL;
}

bool CompareHeuristic(Node *A, Node *B){
    return A->Heuristic > B->Heuristic;
}

Node *BFS(State S, State Goal){
    vector<Node*> Open_BFS(MAXLENGTH);
    vector<Node*> Closed_BFS(MAXLENGTH);
    Open_BFS.clear();
    Closed_BFS.clear();
    Node *Root = (Node*)malloc(sizeof(Node));
    Root->State = S;
    Root->Dad = NULL;
    Root->Order = 0;
    Root->Heuristic = Heuristic_A(Root->State,Goal);
    Open_BFS.push_back(Root);
    while(!Open_BFS.empty()){
        Node *NODE = Open_BFS.back();
        Open_BFS.pop_back();
        Closed_BFS.push_back(NODE);
        if(Goalcheck(NODE->State,Goal)){
            return NODE;
        }
        for(int Option = 1; Option <= MAX_OPERATOR; Option++){
            State Newstate;
            Newstate = NODE->State;
            if(Calloperator(NODE->State,&Newstate,Option)){
                Node *Newnode = (Node*)malloc(sizeof(Node));
                Newnode->State = Newstate;
                Newnode->Dad = NODE;
                Newnode->Order = Option;
                Newnode->Heuristic = Heuristic_A(Newstate,Goal);
                vector<Node*>::iterator Pos_Open, Pos_Closed;
                Node *Existed_Open = Findstate(Newstate,Open_BFS,&Pos_Open);
                Node *Existed_Close = Findstate(Newstate,Closed_BFS,&Pos_Closed);
                if(Existed_Open == NULL && Existed_Close == NULL) Open_BFS.push_back(Newnode);
                else if(Existed_Open != NULL && Existed_Open->Heuristic > Newnode->Heuristic){
                    Open_BFS.erase(Pos_Open);
                    Open_BFS.push_back(Newnode);
                }
                else if(Existed_Close != NULL && Existed_Close->Heuristic > Newnode->Heuristic){
                    Closed_BFS.erase(Pos_Closed);
                    Open_BFS.push_back(Newnode);
                }
            }
        }
        sort(Open_BFS.begin(),Open_BFS.end(),CompareHeuristic);
    }
    return NULL;
}

void Print_WayToGetGoal(Node *N){
    vector<Node*> List;
    List.clear();
    while(N->Dad!=NULL){
        List.push_back(N);
        N = N->Dad;
    }
    List.push_back(N);
    int Order = 0, Pos;
    for(Pos=List.size()-1;Pos>=0;Pos--){
        printf("Action %d: %s.\n",Order,Action[List.at(Pos)->Order]);
        Printstate(List.at(Pos)->State);
        Order++;
    }
}

int main(int argc, char const *argv[]){
    State S;
    S.Emptyrow = 1;
    S.Emptycol = 1;
    S.EightPuzze[0][0] = 3;
    S.EightPuzze[0][1] = 4;
    S.EightPuzze[0][2] = 5;
    S.EightPuzze[1][0] = 1;
    S.EightPuzze[1][1] = 0;
    S.EightPuzze[1][2] = 2;
    S.EightPuzze[2][0] = 6;
    S.EightPuzze[2][1] = 7;
    S.EightPuzze[2][2] = 8;

    State Goal;
    Goal.Emptyrow = 0;
    Goal.Emptycol = 0;
    Goal.EightPuzze[0][0] = 0;
    Goal.EightPuzze[0][1] = 1;
    Goal.EightPuzze[0][2] = 2;
    Goal.EightPuzze[1][0] = 3;
    Goal.EightPuzze[1][1] = 4;
    Goal.EightPuzze[1][2] = 5;
    Goal.EightPuzze[2][0] = 6;
    Goal.EightPuzze[2][1] = 7;
    Goal.EightPuzze[2][2] = 8;
    Node *P = BFS(S,Goal);
    Print_WayToGetGoal(P);
}