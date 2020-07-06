#include<stdio.h>
#include<windows.h>

int main(){
	int n = 1;
	printf("Mau chu");
	for(int i = 0; i< 1000; i++){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		printf("Mau chu %d\n", i);
	}
	
}
