#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

struct Domino{
	int state;
	int A;
	int B;
	struct Domino *next;
};

struct Domino *first;
int play=0, DominoGracza1=0, DominoGracza2=0, DominoWolne=0;

int PrzydzielDomina(){
	int x=0;
	for(int i=0; i<=6; i++){
		for(int j=x; j<=6; j++){
			struct Domino *tmp = (struct Domino *)malloc (sizeof(struct Domino));
			if(DominoGracza1<7 && DominoGracza2<7 && DominoWolne<14)
				tmp->state = rand() % 3;		 //(rand() % 100 > 33)? ((rand() % 100 > 66)? 2 : 1) : 0;	//((rand() % 3 == 0) && DominoWolne<14)? 0 : (((rand() % 3 == 1) && DominoGracza1<7)? 1 : (((rand() % 3 == 2) && DominoGracza2<7)? 2 : (rand() % 3))); 		
			else{
				if(DominoWolne==14)
					tmp->state = (1 + rand() % 2);
				if(tmp->state==1 && DominoGracza1==7 && DominoGracza2!=7)
					tmp->state = 2;
				if(tmp->state==2 && DominoGracza2==7  && DominoGracza1!=7)
					tmp->state = 1;
			}
			(tmp->state==1)? (DominoGracza1++) : ((tmp->state==2)? (DominoGracza2++) : (DominoWolne++));
			
			tmp->A=i;
			tmp->B=j;
			tmp->next = NULL;
			if(first==NULL)				
				first=tmp;
			else{
				tmp->next=first;
				first=tmp;	}
		}
		x++;
	}
	return 0;
}

int main(){
	srand(time(NULL));
	first=NULL;
	char ODP;
	std::cout << "\nChcesz grac w Domino Zaglady? [T/N]" << std::endl;
	while(play!=1){
		std::cin >> ODP;
		if(ODP == 84){PrzydzielDomina(); play=1;}
		else if(ODP == 78) return 0;
		else std::cout << "Zla komenda. Chcesz grac? [T/N]" << std::endl;
	}

	struct Domino *tmp;
	tmp=first;
	while(tmp){
		if(tmp->state==1)
			std::cout << "["<< tmp->A <<"|"<<tmp->B<<"]" << "\t";
		tmp=tmp->next;
	}
	std::cout << "\nGracz1: " << DominoGracza1 << " , Gracz2: " << DominoGracza2 << " , Wolne: "<< DominoWolne << std::endl;
	return 0;
}




/*
	int a,b;
	std::cin >> a; std::cin >> b;
	printf("Z CINÓW: %d %d\n", a, b);
*/