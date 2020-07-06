#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<cstdlib>
#include<math.h>
#include<time.h>
#include<windows.h>
#define m 20
#define n 25
#define goal 3
#define start 2
#define maxlength 500

typedef struct{
	int Maze[m][n];
	int start_position_row, start_position_col;
	int end_position_row, end_position_col;
	int position_row, position_col;
}State;


void cp_State(State state, State *result){
	int row, col;
	for(row=0; row<m; row++){
		for(col=0; col<n; col++)
			result->Maze[row][col] = state.Maze[row][col];		
	}
}


void print_state(State state){
	int row, col;
	for(row=0; row<n; row++) printf(" ---");
	printf("\n");
	for(row=0; row<m; row++){
		for(col=0; col<n; col++)
			printf("| %d ", state.Maze[row][col]);
		printf("|\n");
	}
	for(row=0; row<n; row++) printf(" ---");
	printf("\n");
}

void print_state_to_txt(State state, FILE *f){
	int row, col;
	for(row=0; row<m; row++){
		for(col=0; col<n; col++)
			fprintf(f,"%d ", state.Maze[row][col]);
		fprintf(f," \n");
	}
	fprintf(f,"\n");
}

void color_text(State state, int k, int t ){
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	switch(state.Maze[k][t]){
		case 0: {
			SetConsoleTextAttribute(h,255);
			printf("    ");
			SetConsoleTextAttribute(h,255);
			break;
		};
		case 1: {
			SetConsoleTextAttribute(h,102);
			printf("    ");
			SetConsoleTextAttribute(h,102);
			break;
		};
		case 2: {
			SetConsoleTextAttribute(h,34);
			printf("    ");
			SetConsoleTextAttribute(h,34);
			break;
		};
		case 3: {
			SetConsoleTextAttribute(h,31);
			printf("    ");
			SetConsoleTextAttribute(h,31);
			break;
		};
		case 4: {
			SetConsoleTextAttribute(h,68);
			printf("    ");
			SetConsoleTextAttribute(h,68);
			break;
		}
	}
}

void print_state_with_color(State state){	
	int row, col;
	
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	
	printf("\n");
	for(row=0; row<m; row++){
		for(col=0; col<n; col++)
			color_text(state,row,col);
			
		printf("\n");
	}
	SetConsoleTextAttribute(h,7);
	printf("\n");
}

float Heuristic(State state){
	float h;
	int dx = (state.position_row - state.end_position_row)*(state.position_row - state.end_position_row);
	int dy = (state.position_col - state.end_position_col)*(state.position_col - state.end_position_col);
	h = sqrt(dx + dy);
	return h;
}

int checkgoal(State state){
	return Heuristic(state) == 0;
}

int comparestate(State state1, State state2){
	if((state1.position_row != state2.position_row) ||(state1.position_col != state2.position_col)) return 0;
	return 1;
}

int Up(State state, State *result){
	*result = state;
	if(state.Maze[state.position_row - 1][state.position_col] != 1){
		result->Maze[state.position_row-1][state.position_col] = 4;
		if(state.Maze[state.position_row][state.position_col] != 2)
			result->Maze[state.position_row][state.position_col] = 0;
		result->position_row = state.position_row - 1;
		result->position_col = state.position_col;
		return 1;
	}
	return 0;
}

int Down(State state, State *result){
	*result = state;
	if(state.Maze[state.position_row + 1][state.position_col] != 1){
		result->Maze[state.position_row+1][state.position_col] = 4;
		if(state.Maze[state.position_row][state.position_col] != 2)
			result->Maze[state.position_row][state.position_col] = 0;
		result->position_row = state.position_row + 1;
		result->position_col = state.position_col;
		return 1;
	}
	return 0;
}

int Left(State state, State *result){
	*result = state;
	if(state.Maze[state.position_row][state.position_col-1] != 1){
		result->Maze[state.position_row][state.position_col-1] = 4;
		if(state.Maze[state.position_row][state.position_col] != 2)
			result->Maze[state.position_row][state.position_col] = 0;
		result->position_row = state.position_row;
		result->position_col = state.position_col - 1;
		return 1;
	}
	return 0;
}

int Right(State state, State *result){
	*result = state;
	if(state.Maze[state.position_row][state.position_col+1] != 1){
		result->Maze[state.position_row][state.position_col+1] = 4;
		if(state.Maze[state.position_row][state.position_col] != 2)
			result->Maze[state.position_row][state.position_col] = 0;
		result->position_row = state.position_row;
		result->position_col = state.position_col + 1;
		return 1;
	}
	return 0;
}

int Call_State(State state, State *result, int action){
	switch(action){
		case 1: return Up(state, result);
		case 2: return Down(state, result);
		case 3: return Left(state, result);
		case 4: return Right(state, result);
		return 1;
	}
	return 0;
}

typedef struct Node{
	State state;
	struct Node* parent;
	int no_function;
	float f;
	int g;
	float h;
}Node;

typedef struct{
	Node* Elements[maxlength];
	int size;
}List;

void MakeNullList(List *L){
	L->size = 0;
}
int EmptyList(List L){
	return L.size == 0;
}

int FullList(List L){
	return L.size == maxlength;
}

Node* element_at(int p, List L){
	return L.Elements[p-1];
}

void InsertList(Node* X, int Position, List *L){
	if(!FullList(*L)){
		int q;
		for(q=L->size; q>=Position; q--)
			L->Elements[q] = L->Elements[q-1];
		L->Elements[Position -1] = X;
		L->size++;
	}else{
		printf("List Full");
	}
}

void Delete(int Position, List *L){
	if(EmptyList(*L)){
		printf("List empty");
	}
	else if(Position < 1 || Position >L->size){
		printf("Vi tri khong hop le!");
	}else{
		int i;
		for(i=Position -1; i<L->size; i++)
			L->Elements[i] = L->Elements[i+1];
		
		L->size--;
	}
}
Node* FindState(State state, List L, int *Position){
	int i;
	for(i=1; i<=L.size; i++)
		if(comparestate(element_at(i, L)->state, state)){
			*Position = i;
			return element_at(i,L);
		}
	return NULL;
}

void sortList(List *L){
	int i, j;
	for(i=0; i< L->size; i++)
		for(j=i+1; j< L->size; j++){
			if(L->Elements[i]->f > L->Elements[i]->f){
				Node* node = L->Elements[i];
				L->Elements[i] = L->Elements[j];
				L->Elements[j] = node;
			}
		}
}


Node* A_Star(State state){
	List open, close;
	MakeNullList(&open);
	MakeNullList(&close);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->no_function = 0;
	root->parent = NULL;
	root->g = 0;
	root->h = Heuristic(root->state);
	root->f = root->g + root->h;
	InsertList(root, open.size+1, &open);
	while(!EmptyList(open)){
		Node* node = element_at(1,open);
		Delete(1,&open);
		InsertList(node, close.size+1, &close);
		if(checkgoal(node->state)) return node;
		int opt;
		
		for(opt=1; opt<=4; opt++){
			State newState;
			newState = node->state;
			if(Call_State(node->state, &newState, opt)){

				Node* newnode = (Node*)malloc(sizeof(Node));
				
				newnode->parent = node;
				newnode->state = newState;
				newnode->no_function = opt;
				newnode->g= node->g+1;
				newnode->h = Heuristic(newState);
				newnode->f = newnode->g + newnode->h;
				
				int posOpen, posClose;
				
				Node* foundOpen = FindState(newState, open, &posOpen);
				Node* foundClose = FindState(newState, close, &posClose);
				
				if(foundOpen == NULL && foundClose==NULL) {
				InsertList(newnode, open.size + 1, &open);
				}
				else if(foundOpen !=NULL && foundOpen->g > newnode->g){
					Delete(posOpen, &open);
					InsertList(newnode, posOpen, &open);
				}
				else if(foundClose != NULL && foundClose->g > newnode->g){
					Delete(posClose, &close);
					InsertList(newnode, open.size + 1, &open);
				}
				
			}
		}
		sortList(&open);
	}
	
	return NULL;
	
}

void Sleeep(int t){
	int tm = 1000*t;
	clock_t time = clock();
	while(clock()<time + tm){
		
	}
}

void print_waystogetgoal(Node* node){
	List p;
	MakeNullList(&p);
	while(node->parent != NULL){
		InsertList(node, p.size  + 1, &p);
		node = node->parent;
	}
	InsertList(node, p.size + 1, &p);
	int i;
	FILE *f = fopen("ketqua.txt","w");
	for(i= p.size-1; i>=0; i--){
		/*print_state_with_color(element_at(i,p)->state);
		Sleeep(0.5);
		if(i!=1){
			system("cls");	
		}*/
		fprintf(f,"%d\n",p.Elements[i]->state.position_row);
		fprintf(f,"%d\n",p.Elements[i]->state.position_col);
	}
	fclose(f);
}

void print_waystogetgoal_to_txt(Node* node, FILE *f){
	List p;
	MakeNullList(&p);
	while(node->parent != NULL){
		InsertList(node, p.size  + 1, &p);
		node = node->parent;
	}
	InsertList(node, p.size + 1, &p);
	int i;
	for(i= p.size; i>0; i--){
		print_state_to_txt(element_at(i,p)->state, f);
	}
	printf("Da luu file!");
}

int main(){
	State file_state, state, result;
	freopen("maze.txt","r", stdin);
	int i,j;
	for(i=0;i<m; i++){
		for(j=0; j<n; j++){
			int x;
			scanf("%d", &x);
			file_state.Maze[i][j] = x;
		}
	}
	cp_State(file_state,&state);	
	state.position_row = state.start_position_row = 2;
	state.position_col = state.start_position_col = 2;
	state.end_position_row =18;
	state.end_position_col = 23;
	//FILE *f = fopen("ketqua.txt","w");
	//print_state_with_color(state);
	Node* node =  A_Star(state);
	print_waystogetgoal(node);
	system("start http://localhost/TTNT/index.php");
	//print_waystogetgoal_to_txt(node,f);
	
	
	return 0;
	
}

