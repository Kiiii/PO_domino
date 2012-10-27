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
/*========================== ZMIENNE GLOBALNE ==========================*/
struct Domino *first;
int play=0, CzyjRuch=1, DominoGracza1=0, DominoGracza2=0, DominoWolne=0, X=8, Y=8; //X to lewa koñcówka 'wê¿a', Y prawa. Zaczynaj¹ z wartoœci¹ 8, by by³o wiadomo, ¿e to pierwszy ruch w grze [zmienia X *oraz* Y].
char RUCH1[6], RUCH2[6]; //tablice, do których wczytuje podawane przez graczy wartoœci wybieranych domin podczas danego ruchu.

/*========================== PRZYDZIEL DOMINA ==========================*/
//Tworzenie listy jednokierunkowej z przydzia³em domin do graczy. U¿ywana w funkcji main tylko raz przy zaczêciu nowej gry.
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
/*========================== WYPISZ DOMINA ==========================*/
//Wypisuje domina aktualnie dostêpne dla danego gracza. U¿ywana za ka¿dym przebiegiem pêtli 'while(play==0)'.
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
/*========================== ZRÓB RUCH ==========================*/
//Sprawdza, czy wybrane przez gracza domino jest w puli jego klocków oraz czy mo¿na je przy³o¿yæ do koñcówki X lub Y. U¿ywana za ka¿dym przebiegiem pêtli 'while(play==0)'.
// !!! Wartoœci z tab s¹ w ASCII wiêc 0==48, 1==49, 2==50,..., 6==54, st¹d do X i Y dodawa³am 48, by mo¿na by³o u¿yæ porównania w if'ach [takie lamerskie rzutowanie 'char' na 'int', bo coœ mi nie wychodzi³o inaczej albo po prostu Ÿle robi³am...]
int ZrobRuch(char tab[], int n){
	struct Domino *tmp;
	tmp=first;
	while(tmp){		
		if((tmp->state==n) && (tab[1]==X + 48 || tab[1]==Y + 48 || (X==8 && Y==8)) && ((tmp->A + 48 == tab[1] && tmp->B  + 48 == tab[3]) || (tmp->B + 48 == tab[1] && tmp->A + 48 == tab[3]))){
			tmp->state=3;
			if(n==1) {DominoGracza1--; CzyjRuch=2;}
			else 	  {DominoGracza2--; CzyjRuch=1;}
			if(X==8 && Y==8) {X=tab[1]-48; Y=tab[3]-48;}		 //pierwszy ruch w grze
			else if(X==tab[1]-48) X=tab[3]-48;
			else if(Y==tab[1]-48) Y=tab[3]-48;
			std::cout << "\n\t\t["<< X << "|_]...[_|" << Y << "]" << std::endl;	 //schemat aktualnego weza (same konce)
			return 0;
		}
		tmp=tmp->next;
	}
	std::cout << "\nNiewlasciwie dobrane domino. Wybierz inne z Tobie dostêpnych..." << std::endl;
	(n)? ({fgets(RUCH1,50,stdin);  ZrobRuch(RUCH1,n);}) : ({fgets(RUCH2,50,stdin); ZrobRuch(RUCH2, n);});
	return 0;
}	
/*========================== DODAWANIE DOMINA ==========================*/
//Dodawanie wolnych domin [tych ze state==0] do puli gracza, który nie móg³ wykonaæ ruchu. U¿ywana w funkcji SprawdzRuch(char tab[], int n).
// !!! Trzeba jeszcze uwzglêdniæ REMIS, bo teraz chyba bêdzie przeskakiwaæ od ruchu Gracza 1 do ruchu Gracza 2 w nieskoñczonej pêtli, jeœli WolneDomina==0 i nikt nie mo¿e dorzuciæ klocka.
int DodawanieDomin(int n){
	struct Domino *tmp;
	tmp=first;
	while(DominoWolne>0){ 
		if(tmp->state==0){
			if(rand() % 100 > 70){
				tmp->state=n;
				DominoWolne--;
				(n==1)? (DominoGracza1++) : (DominoGracza2++);
				if(tmp->A==X || tmp->B==X || tmp->A==Y || tmp->B==Y) return 0;
			}
		}	
		tmp=tmp->next;
		if(!tmp) tmp=first;
	}
	std::cout << "\n\t\tOZESZ, SKONCZYLY SIE WOLNE DOMINA. JESTESMY ZGUBIENI. :O\n" << std::endl;
	if(n==1) CzyjRuch=2;
	else CzyjRuch=1;
	return 0;
}
/*========================== SPRAWD RUCH ==========================*/
//Sprawdza, czy w puli domin danego gracza jest domino, które mo¿na przy³o¿yæ do koñcówki X lub Y. Jeœli nie, to wywo³uje funkcjê dodawania wolnych domin. U¿ywana za ka¿dym przebiegiem pêtli 'while(play==0)' przed wykonaniem ruchu.
int SprawdzRuch(char tab[], int n){
	if(X==8 || Y==8) return 1;
	else{
		struct Domino *tmp;
		tmp=first;
		while(tmp){
			if((tmp->state==n) && (tmp->A==X || tmp->B==X || tmp->A==Y || tmp->B==Y)) return 1;
			tmp=tmp->next;	
		}
		if(DominoWolne>0) {
			std::cout << "\nGracz " << n <<" nie mo¿e zrobic ruchu. Dostaje dodatkowe domina z puli niewykorzystanych...";
			DodawanieDomin(n);
			return 1;
		}
	}
	return 0;
}
/*========================== PODLICZ PUNKTY ==========================*/
//W przypadku remisu zwyciêzca wybierany jest na podstawie mniejszej sumy oczek wszystkich jego pozosta³ych domin. U¿ywana w funkcji main tylko raz przy zakoñczeniu gry.
int PodliczPunkty(){
	struct Domino *tmp;
	tmp=first;
	int suma1=0, suma2=0;
	while(tmp){	
		if(tmp->state==1) suma1++;
		else if(tmp->state==2) suma2++;
	}
	if(suma1 < suma2) std::cout << "\n\n\t\t Chwalebne zwyciestwo Gracza 1!\n" << std::endl;
	else if(suma1 > suma2) std::cout << "\n\n\t\t Chwalebne zwyciestwo Gracza 2!\n\n" << std::endl;
	else std::cout << "\n\n\t\t Remis!\n\n" << std::endl;
	return 0;
}
/*========================== G£ÓWNA FUNKCJA ==========================*/
int main(){
	srand(time(NULL));
	first=NULL;
	char ODP;
	std::cout << "\n\n\t\tZechcesz zagrac w Domino Zaglady? [T/N]" << std::endl;
	while(play!=1){
		fgets(&ODP,50,stdin); 					//gets(&ODP);->wczytuje niograniczenie, stad niebezpieczna
		if(ODP == 84){PrzydzielDomina(); play=1;}
		else if(ODP == 78) return 0;
		else std::cout << "\t\tZla komenda. Chcesz grac? [T/N]" << std::endl;
	}
	while(play==1){
		if(CzyjRuch==1)
			(SprawdzRuch(RUCH1, CzyjRuch)==1)? ({WypiszDomina(CzyjRuch); fgets(RUCH1,50,stdin); ZrobRuch(RUCH1, CzyjRuch);}) : CzyjRuch=2;
		else if(CzyjRuch==2)
			(SprawdzRuch(RUCH2, CzyjRuch)==1)? ({WypiszDomina(CzyjRuch); fgets(RUCH2,50,stdin); ZrobRuch(RUCH2, CzyjRuch);}) : CzyjRuch=1;
		if(DominoGracza1==0 || DominoGracza2==0) play=0;
	}
	if(DominoGracza1==0) std::cout << "\n\n\t\t Chwalebne zwyciestwo Gracza 1!\n" << std::endl;
	else if(DominoGracza2==0) std::cout << "\n\n\t\t Chwalebne zwyciestwo Gracza 2!\n\n" << std::endl;
	else if(DominoGracza1>0 && DominoGracza2>0) PodliczPunkty();
	return 0;
}