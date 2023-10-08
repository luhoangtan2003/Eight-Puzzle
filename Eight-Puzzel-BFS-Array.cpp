#include<stdio.h>
#include<stdlib.h>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MAXLENGTH 500

const char *Action[] = {
    "First state","Move cell to UP","Move cell to DOWN","Move cell to LEFT","Move cell to RIGHT"
    };

struct State{
    int Eight_Puzzel[ROWS][COLS];
    int Emptyrow;
    int Emptycol;
};

void Printstate(State State){
    printf("----------\n");
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            printf("|%d ",State.Eight_Puzzel[Row][Col]);
        }
        printf("|\n");
    }
    printf("----------\n");
}

// Hàm so sánh hai trạng thái
int Comparestates(State S1, State S2){
    if(S1.Emptyrow!=S2.Emptyrow||S1.Emptycol!=S2.Emptycol){
        return 0;
    }
    for(int Row=0;Row<ROWS;Row++){
        for(int Col=0;Col<COLS;Col++){
            if(S1.Eight_Puzzel[Row][Col]!=S2.Eight_Puzzel[Row][Col]){
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
        Result->Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent] = S.Eight_Puzzel[EmptyRowCurrent-1][EmptyColCurrent];
        Result->Eight_Puzzel[EmptyRowCurrent-1][EmptyColCurrent] = EMPTY;
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
        Result->Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent] = S.Eight_Puzzel[EmptyRowCurrent+1][EmptyColCurrent];
        Result->Eight_Puzzel[EmptyRowCurrent+1][EmptyColCurrent] = EMPTY;
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
        Result->Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent] = S.Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent-1];
        Result->Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent-1] = EMPTY;
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
        Result->Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent] = S.Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent+1];
        Result->Eight_Puzzel[EmptyRowCurrent][EmptyColCurrent+1] = EMPTY;
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
            if(S.Eight_Puzzel[Row][Col]!=Goal.Eight_Puzzel[Row][Col]){
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

struct List{
    Node *Element[MAXLENGTH];
    int Size;
};

void Makenulllist(List *List){
    List->Size = 0;
}

int Emptylist(List List){
    return List.Size==0;
}

int Fulllist(List List){
    return List.Size == MAXLENGTH;
}

Node *Element_At(List List, int Position){
    return List.Element[Position-1];
}

void Pushlist(Node *X,int Position, List *List){
    if(!Fulllist(*List)){
        if(Position<1||Position>List->Size+1){
            printf("Position is impossible to add.\n");
        }else{
            for(int Q=List->Size;Q>=Position;Q--){
                List->Element[Q] = List->Element[Q-1];
            }
            List->Element[Position-1] = X;
            List->Size++;
        }
    }else{
        printf("List is full of the elements.\n");
    }
}

void Deletelist(int Position, List *List){
    if(Emptylist(*List)){
        printf("List is empty,cannot delete.\n");
    }else if(Position<1||Position>List->Size){
        printf("Position is impossible to delete.\n");
    }else{
        for(int i=Position-1;i<List->Size;i++){
            List->Element[i] = List->Element[i+1];
        }
        List->Size--;
    }
}

Node *Findstate(State State, List List, int *Position){
    for(int i=1;i<=List.Size;i++){
        if(Comparestates(Element_At(List,i)->State,State)){
            *Position = i;
            return Element_At(List,i);
        }
    }
    return NULL;
}

void Sort(List *List){
    for(int i=0;i<List->Size-1;i++){
        for(int j=i+1;j<List->Size;j++){
            if(List->Element[i]->Heuristic>List->Element[j]->Heuristic){
                Node *Temp = List->Element[i];
                List->Element[i] = List->Element[j];
                List->Element[j] = Temp;
            }
        }
    }
}

Node *BFS(State S, State Goal){
    List Open_BFS,Close_BFS; // Khai báo hai danh sách lưu trữ các nút
    Makenulllist(&Open_BFS);
    Makenulllist(&Close_BFS);
    Node *Root = (Node*)malloc(sizeof(Node)); // Tạo ra một nút gốc để bắt đầu tìm kiếm
    Root->State = S;
    Root->Dad = NULL;
    Root->Order = 0;
    Root->Heuristic = Heuristic_A(Root->State,Goal); // Tính giá trị hàm Heuristic cho nút gốc
    Pushlist(Root,Open_BFS.Size+1,&Open_BFS); // Thêm nút gốc vào danh sách mở
    while(!Emptylist(Open_BFS)){ // Trong khi danh sách mở vẫn còn phần tử
        Node *NODE = Element_At(Open_BFS,1); // Lấy ra nút đầu tiên trong danh sách mở
        Deletelist(1,&Open_BFS); // Xoá nó khỏi danh sách mở
        Pushlist(NODE,Close_BFS.Size+1,&Close_BFS); // Thêm nó vào danh sách đóng
        if(Goalcheck(NODE->State,Goal)){ // Nếu đến được trạng thái đích
            return NODE; // Trả về nút hiện tại
        }
        for(int Option = 1; Option <= MAX_OPERATOR; Option++){ // Lần lượt thực hiện các hành động
            State Newstate;
            Newstate = NODE->State;
            if(Calloperator(NODE->State,&Newstate,Option)){ // Nếu hành động có thể thực hiện được
                Node *Newnode = (Node*)malloc(sizeof(Node)); // Tạo một nút mới
                Newnode->State = Newstate; // Cập nhật trạng thái cho nút mới
                Newnode->Dad = NODE; // Gán nút cha là nút hiện tại
                Newnode->Order = Option; // Gán hành động vừa thực hiện
                Newnode->Heuristic = Heuristic_A(Newstate,Goal); // Tính giá trị hàm Heuristic cho nút mới
                int Pos_Open, Pos_Close;
                // Tìm kiếm nút trong danh sách mở
                Node *Existed_Open = Findstate(Newstate,Open_BFS,&Pos_Open);
                // Tìm kiếm nút trong danh sách đóng
                Node *Existed_Close = Findstate(Newstate,Close_BFS,&Pos_Close);
                // Nếu nút không có trong danh sách mở hoặc danh sách đóng
                if(Existed_Open == NULL && Existed_Close == NULL){
                    Pushlist(Newnode,Open_BFS.Size+1,&Open_BFS); // Thêm nó vào danh sách mở
                // Nếu nút có trong danh sách mở và giá trị hàm heuristic mới nhỏ hơn
                }else if(Existed_Open != NULL && Existed_Open->Heuristic > Newnode->Heuristic){
                    Deletelist(Pos_Open,&Open_BFS); // Xoá nút cũ khỏi danh sách mở
                    Pushlist(Newnode,Pos_Open,&Open_BFS); // Thêm nút mới vào vị trí của nút cũ
                // Nếu nút có trong danh sách đóng và giá trị hàm heuristic mới nhỏ hơn
                }else if(Existed_Close != NULL && Existed_Close->Heuristic > Newnode->Heuristic){
                    Deletelist(Pos_Close,&Close_BFS); // Xoá nút cũ khỏi danh sách đóng
                    Pushlist(Newnode,Open_BFS.Size+1,&Open_BFS); // Thêm nút mới vào danh sách mở
                }
            }
        }
        Sort(&Open_BFS); // Sắp xếp lại danh sách mở theo giá trị hàm heuristic
    }
    return NULL;
}

void Print_WayToGetGoal(Node *Node){
    List List;
    Makenulllist(&List);
    while(Node->Dad!=NULL){
        Pushlist(Node,List.Size+1,&List);
        Node = Node->Dad;
    }
    Pushlist(Node,List.Size+1,&List);
    int No_Action = 0, Pos;
    for(Pos=List.Size;Pos>0;Pos--){
        printf("Action %d: %s.\n",No_Action,Action[Element_At(List,Pos)->Order]);
        Printstate(Element_At(List,Pos)->State);
        No_Action++;
    }
}

int main(int argc, char const *argv[]){
    State S;
    S.Emptyrow = 1;
    S.Emptycol = 1;
    S.Eight_Puzzel[0][0] = 3;
    S.Eight_Puzzel[0][1] = 4;
    S.Eight_Puzzel[0][2] = 5;
    S.Eight_Puzzel[1][0] = 1;
    S.Eight_Puzzel[1][1] = 0;
    S.Eight_Puzzel[1][2] = 2;
    S.Eight_Puzzel[2][0] = 6;
    S.Eight_Puzzel[2][1] = 7;
    S.Eight_Puzzel[2][2] = 8;

    State Goal;
    Goal.Emptyrow = 0;
    Goal.Emptycol = 0;
    Goal.Eight_Puzzel[0][0] = 0;
    Goal.Eight_Puzzel[0][1] = 1;
    Goal.Eight_Puzzel[0][2] = 2;
    Goal.Eight_Puzzel[1][0] = 3;
    Goal.Eight_Puzzel[1][1] = 4;
    Goal.Eight_Puzzel[1][2] = 5;
    Goal.Eight_Puzzel[2][0] = 6;
    Goal.Eight_Puzzel[2][1] = 7;
    Goal.Eight_Puzzel[2][2] = 8;
    Node *P = BFS(S,Goal);
    Print_WayToGetGoal(P);
}