#include<bits/stdc++.h>
using namespace std;

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define OPERATOR 4
#define LENGTH 10000

struct State{
    int Eight_Puzzle[ROWS][COLS];
    int Empty_Row;
    int Empty_Col;
};

struct Node{
    State State;
    Node *Dad;
    int Order, G, H, F = G + H;
};

struct List{
    Node *Element[LENGTH];
    int Size;
};

int Compare_States(State S1, State S2){
    if(S1.Empty_Row != S2.Empty_Row || S1.Empty_Col != S2.Empty_Col){
        return 0;
    }
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            if(S1.Eight_Puzzle[Row][Col]!=S2.Eight_Puzzle[Row][Col]){
                return 0;
            }
        }
    }
    return 1;
}

int Is_Goal(State S, State Goal){
    return Compare_States(S,Goal);
}

int Push_U(State S, State *Result){
    *Result = S;
    int Empty_Row = S.Empty_Row;
    int Empty_Col = S.Empty_Col;
    if(Empty_Row>0){
        Result->Empty_Row = Empty_Row - 1;
        Result->Empty_Col = Empty_Col;
        Result->Eight_Puzzle[Empty_Row][Empty_Col] = S.Eight_Puzzle[Empty_Row-1][Empty_Col];
        Result->Eight_Puzzle[Empty_Row-1][Empty_Col] = EMPTY;
        return 1;
    }
    return 0;
}

int Pull_D(State S, State *Result){
    *Result = S;
    int Empty_Row = S.Empty_Row;
    int Empty_Col = S.Empty_Col;
    if(Empty_Row<2){
        Result->Empty_Row = Empty_Row + 1;
        Result->Empty_Col = Empty_Col;
        Result->Eight_Puzzle[Empty_Row][Empty_Col] = S.Eight_Puzzle[Empty_Row+1][Empty_Col];
        Result->Eight_Puzzle[Empty_Row+1][Empty_Col] = EMPTY;
        return 1;
    }
    return 0;
}

int Left(State S, State *Result){
    *Result = S;
    int Empty_Row = S.Empty_Row;
    int Empty_Col = S.Empty_Col;
    if(Empty_Col>0){
        Result->Empty_Col = Empty_Col - 1;
        Result->Empty_Row = Empty_Row;
        Result->Eight_Puzzle[Empty_Row][Empty_Col] = S.Eight_Puzzle[Empty_Row][Empty_Col-1];
        Result->Eight_Puzzle[Empty_Row][Empty_Col-1] = EMPTY;
        return 1;
    }
    return 0;
}

int Right(State S, State *Result){
    *Result = S;
    int Empty_Row = S.Empty_Row;
    int Empty_Col = S.Empty_Col;
    if(Empty_Col<2){
        Result->Empty_Col = Empty_Col + 1;
        Result->Empty_Row = Empty_Row;
        Result->Eight_Puzzle[Empty_Row][Empty_Col] = S.Eight_Puzzle[Empty_Row][Empty_Col+1];
        Result->Eight_Puzzle[Empty_Row][Empty_Col+1] = EMPTY;
        return 1;
    }
    return 0;
}

int Call_Operator(State S, State *Result, int Option){
    switch(Option){
        case 1: return Push_U(S,Result);
        case 2: return Pull_D(S,Result);
        case 3: return Left(S,Result);
        case 4: return Right(S,Result);
        default: printf("Error!!!"); break;
    }
    return 0;
}

void Make_Null_List(List *List){
    List->Size = 0;
}

int Empty_List(List List){
    return List.Size == 0;
}

int Full_List(List List){
    return List.Size == LENGTH;
}

Node *Element(List List, int Pos){
    int Index = Pos - 1;
    return List.Element[Index];
}

void Push_List(Node *X,int Pos, List *List){
    if(!Full_List(*List)){
        if(Pos<1||Pos>List->Size+1){
            printf("Pos is impossible to add.\n");
        }else{
            for(int Q=List->Size;Q>=Pos;Q--){
                List->Element[Q] = List->Element[Q-1];
            }
            List->Element[Pos-1] = X;
            List->Size++;
        }
    }else{
        printf("List is full of the elements.\n");
    }
}

void Delete_List(int Pos, List *List){
    if(Empty_List(*List)){
        printf("List is empty,cannot delete.\n");
    }else if(Pos<1||Pos>List->Size){
        printf("Pos is impossible to delete.\n");
    }else{
        for(int i=Pos-1;i<List->Size;i++){
            List->Element[i] = List->Element[i+1];
        }
        List->Size--;
    }
}

Node *Find_state(State State, List List, int *Pos){
    for(int i=1;i<=List.Size;i++){
        if(Compare_States(Element(List,i)->State,State)){
            *Pos = i;
            return Element(List,i);
        }
    }
    return NULL;
}
void Sort_List(List *List){
    for(int i=0;i<List->Size-1;i++){
        for(int j=i+1;j<List->Size;j++){
            if(List->Element[i]->G>List->Element[j]->G){
                swap(List->Element[i],List->Element[j]);
            }
        }
    }
}

void Print_State(State State){
    printf("----------\n");
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            printf("|%d ",State.Eight_Puzzle[Row][Col]);
        }
        printf("|\n");
    }
    printf("----------\n");
}

const char *Action[] = {
    "Trạng thái bắt đầu",
    "Đẩy ô lên trên",
    "Kéo ô xuống dưới",
    "Đẩy ô qua trái",
    "Đẩy ô qua phải"
};

void Print_WayToGetGoal(Node *Node){
    List List;
    Make_Null_List(&List);
    while(Node->Dad!=NULL){
        Push_List(Node,List.Size+1,&List);
        Node = Node->Dad;
    }
    Push_List(Node,List.Size+1,&List);
    int No_Action = 0, Pos;
    for(Pos=List.Size;Pos>0;Pos--){
        printf("Bước %d:%s.\n",No_Action,Action[Element(List,Pos)->Order]);
        Print_State(Element(List,Pos)->State);
        No_Action++;
    }
}

int Heuristic_A(State S, State Goal){
    int Count = 0;
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            if(S.Eight_Puzzle[Row][Col] != Goal.Eight_Puzzle[Row][Col]){
                Count++;
            }
        }
    }
    return Count;
}

Node *A_Star(State Start_State, State Goal_State){
    List IsOpen,Closed;
    Make_Null_List(&IsOpen);
    Make_Null_List(&Closed);
    Node *Root = (Node*)malloc(sizeof(Node));
    Root->State = Start_State;
    Root->Dad = NULL;
    Root->Order = 0;
    Root->G = 0;
    Root->H = Heuristic_A(Root->State,Goal_State);
    Push_List(Root,IsOpen.Size+1,&IsOpen);
    while(!Empty_List(IsOpen)){
        Node *Top = Element(IsOpen,1);
        Delete_List(1,&IsOpen);
        Push_List(Top,Closed.Size+1,&Closed);
        if(Is_Goal(Top->State,Goal_State)){
            Print_WayToGetGoal(Top);
            return 0;
        }
        for(int Option=1;Option<=OPERATOR;Option++){
            State Child_State;
            if(Call_Operator(Top->State,&Child_State,Option)){
                Node *Child_Node = (Node*)malloc(sizeof(Node));
                Child_Node->State = Child_State;
                Child_Node->Dad = Top;
                Child_Node->Order = Option;
                Child_Node->G = Top->G+1;
                Child_Node->H = Heuristic_A(Child_State,Goal_State);
                int Pos_IsOpen, Pos_Closed;
                Node *Existed_IsOpen = Find_state(Child_State,IsOpen,&Pos_IsOpen);
                Node *Existed_Closed = Find_state(Child_State,Closed,&Pos_Closed);
                if(Existed_IsOpen == NULL && Existed_Closed == NULL){
                    Push_List(Child_Node,IsOpen.Size+1,&IsOpen);
                } else if(Existed_IsOpen != NULL && Existed_IsOpen->G > Child_Node->G){
                    Delete_List(Pos_IsOpen,&IsOpen);
                    Push_List(Child_Node,Pos_IsOpen,&IsOpen);
                } else if(Existed_Closed != NULL && Existed_Closed->G > Child_Node->G){
                    Delete_List(Pos_Closed,&Closed);
                    Push_List(Child_Node,IsOpen.Size+1,&IsOpen);
                }
            }
        }
        Sort_List(&IsOpen);
    }
    return NULL;
}

int main(int argc, char const *argv[]){
    State First;
    First.Empty_Row = 1;
    First.Empty_Col = 1;
    First.Eight_Puzzle[0][0] = 1;
    First.Eight_Puzzle[0][1] = 2;
    First.Eight_Puzzle[0][2] = 3;
    First.Eight_Puzzle[1][0] = 8;
    First.Eight_Puzzle[1][1] = 0;
    First.Eight_Puzzle[1][2] = 4;
    First.Eight_Puzzle[2][0] = 7;
    First.Eight_Puzzle[2][1] = 6;
    First.Eight_Puzzle[2][2] = 5;

    State Goal;
    Goal.Empty_Row = 1;
    Goal.Empty_Col = 0;
    Goal.Eight_Puzzle[0][0] = 2;
    Goal.Eight_Puzzle[0][1] = 8;
    Goal.Eight_Puzzle[0][2] = 1;
    Goal.Eight_Puzzle[1][0] = 0;
    Goal.Eight_Puzzle[1][1] = 4;
    Goal.Eight_Puzzle[1][2] = 3;
    Goal.Eight_Puzzle[2][0] = 7;
    Goal.Eight_Puzzle[2][1] = 6;
    Goal.Eight_Puzzle[2][2] = 5;
    Node *Result = A_Star(First,Goal);
}