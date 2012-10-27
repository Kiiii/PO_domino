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
int play=0, CzyjRuch=1, DominoGracza1=0, DominoGracza2=0, DominoWolne=0, X=8, Y=8;

int PrzydzielDomina(){
	int x=0;
	for(int i=0; i<=6; i++){
		for(int j=x; j<=6; j++){
			Domino *tmp = new Domino;
			if(DominoGracza1<7 && DominoGracza2<7 && DominoWolne<14)	tmp->state = rand() % 3;		
			else{
				if(DominoWolne==14)	tmp->state = (1 + rand() % 2);
				if(tmp->state==1 && DominoGracza1==7 && DominoGracza2!=7)	tmp->state = 2;
				if(tmp->state==2 && DominoGracza2==7  && DominoGracza1!=7)	tmp->state = 1;
			}
			(tmp->state==1)? (DominoGracza1++) : ((tmp->state==2)? (DominoGracza2++) : (DominoWolne++));			
			tmp->A=i;	tmp->B=j;
			tmp->next = NULL;
			(first==NULL)? (first=tmp) : ({tmp->next=first; first=tmp;});
		}
		x++;
	}
	return 0;
}

int WypiszDomina(int x){
	struct Domino *tmp;
	tmp=first;
	std::cout << "\nDomina Gracza " << x << ":\t";
	while(tmp){
		if(tmp->state==x)
			std::cout << "["<< tmp->A <<"|"<<tmp->B<<"]" << "  ";
		tmp=tmp->next;
	}
	std::cout << "\nCzyn swoj ruch, Graczu " << x << "..." << std::endl;
	return 0;
}


int SprawdzRuch(char tab[], int n){		//ascii z tab siê dostaje: 48==0  54==6
	struct Domino *tmp;
	tmp=first;
	while(tmp){		
		if((tmp->state==n) && ((tmp->A + 48 == tab[1] && tmp->B  + 48 == tab[3]) || (tmp->B + 48 == tab[1] && tmp->A + 48 == tab[3]))){
			tmp->state=3;
			if(n==1) {DominoGracza1--; CzyjRuch=2;}
			else 	  {DominoGracza2--; CzyjRuch=1;}
			if(X==8 && Y==8) {X=tab[1]-48; Y=tab[3]-48;}		 //pierwszy ruch w grze
			else if(X==tab[1]-48) X=tab[3]-48;
			else if(Y==tab[1]-48) Y=tab[3]-48;

			std::cout << "\n\t\t["<< X << "|...]...[...|" << Y << "]" << std::endl; //schemat aktualnego weza (same konce)

			return 0;
		}
		tmp=tmp->next;
	}
	std::cout << "\nBrak ruchu.\n"<< std::endl;
	if(n==1) CzyjRuch=2;
	else 	  CzyjRuch=1;
	return 0;
}	

int main(){
	srand(time(NULL));
	first=NULL;
	char ODP, RUCH1[6], RUCH2[6];
	std::cout << "X: "<< X << " Y: " << Y << std::endl;
	std::cout << "\nZechcesz zagrac w Domino Zaglady? [T/N]" << std::endl;
	while(play!=1){
		fgets(&ODP,50,stdin); 	//gets(&ODP);->wczytuje niograniczenie, stad niebezpieczna
		if(ODP == 84){PrzydzielDomina(); play=1;} //'T'
		else if(ODP == 78) return 0; //'N'
		else std::cout << "Zla komenda. Chcesz grac? [T/N]" << std::endl;
	}
	while(play==1 && (DominoGracza1>0 || DominoGracza2>0)){
		if(CzyjRuch==1){
			WypiszDomina(CzyjRuch);
			fgets(RUCH1,50,stdin);			
			SprawdzRuch(RUCH1, CzyjRuch);
		}
		if(CzyjRuch==2){
			WypiszDomina(CzyjRuch);
			fgets(RUCH2,50,stdin);			
			SprawdzRuch(RUCH2, CzyjRuch);
		}
	}	
	return 0;
}


/*
	Schemat wensza:  X -->  [i|j][q|w]...[z|w]  <-- Y
*/


//(rand() % 2)? 0 : 2//
//std::cout << "\nGracz1: " << DominoGracza1 << " , Gracz2: " << DominoGracza2 << " , Wolne: "<< DominoWolne << std::endl;
//std::cout << "\ntab[1]: "<< tab[1]<<"  tab[3]: "<< tab[3] << std::endl;  printf("TE TABY: %d %d\n",tab[1],tab[3]);
//std::cout << "\nRUCH2: " << RUCH2[0]<< RUCH2[1]<< RUCH2[2]<< RUCH2[3]<< RUCH2[4] << std::endl;
//std::cout << "\nRUCH1: " << RUCH1[0]<< RUCH1[1]<< RUCH1[2]<< RUCH1[3]<< RUCH1[4] << std::endl;
