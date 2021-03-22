#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define TAILLE 20





char antonin[]="antotchoin";
char adrien[]="adripute";
char alban[]="albite";
char elies[]="eliesus";
int personnage=0;
int couleur=2;
int vitesse=20;
int dispertion=100;

int option(char* mot){
	if(*mot=='b') personnage=3;
	else if(*mot=='e') personnage=2;
	else if(*mot=='p') personnage=1;
	else if(*mot=='t') personnage=0;
	if(*mot=='C'){
		if(*(mot+1)=='n') couleur=0;
		else if(*(mot+1)=='r') couleur=1;
		else if(*(mot+1)=='g') couleur=2;
		else if(*(mot+1)=='y') couleur=3;
		else if(*(mot+1)=='b') couleur=4;	
		else if(*(mot+1)=='m') couleur=5;
		else if(*(mot+1)=='c') couleur=6;
		else if(*(mot+1)=='w') couleur=7;
		else{
		       	printf("Rentrez une des valeurs suivantes:\nn pour noir (ça ne montre rien)\nr pour rouge\ng pour vert (par défault)\ny pour jaune\n");
			printf("b pour bleu\nm pour mauve\nc pour cyan\nw pour blanc\n");
			printf("Exemple: antotchoin -Cw\n");
			return 1;
		}
	}
	if(*mot=='V'){
		if(0<atoi(mot+1) && atoi(mot+1)<100) vitesse=atoi(mot+1);
		else {
			printf("Rentrez une valeur entre 1 et 99\nExemple: antotchoin -V20\n");
			return 1;
		}
	}
	if(*mot=='D'){
		if(0<atoi(mot+1) && atoi(mot+1)<10000) dispertion=atoi(mot+1);
		else {
			printf("Rentrez une valeur entre 1 et 9999\nExemple: antotchoin -D100\n");
			return 1;
		}
	}
	if(*mot=='h'){
		printf("Usage: antochoin -[aept] -[C couleur] -[V vitesse] -[D densité]\n");
		printf("-t rend hommage au grand Antonin (par défault)\n");
		printf("-p rend toute sa gloire à Adrien\n");
		printf("-e rend l'honneur au grand Elies\n");
		printf("-b juste le créateur\n");
		printf("-V ajusté la vitesse de 1 à 99\n");
		printf("-C changer la couleur nrgybmcw (vert par défault)\n");
		printf("-D ajuster la densité de lettrede 1 à 9999\n");
		return 1;
	}
	return 0;
}

void afficher_ligne(char** tab, int nb_col,int nb_ligne, int affichage){
	for(int x=0; x<nb_col; x++){
		char temp=tab[x][nb_ligne];
		move(affichage, x);
		if(temp>96 && rand()%20==0){
			addch((temp-32));
		}
		else if(tab[x][nb_ligne]=='e' && personnage==2 && tab[x][nb_ligne-1]=='l'){
			if(rand()%7==0) addch('E' | COLOR_PAIR(2));
			//else if(rand()%5==0) addch('&' | COLOR_PAIR(2));
			else addch('e' | COLOR_PAIR(2)); 
		}
		else if(tab[x][nb_ligne]=='a'){
			if(rand()%7==0) addch('A' | COLOR_PAIR(2));
			//else if(rand()%5==0) addch('@' | COLOR_PAIR(2));
			else addch('a' | COLOR_PAIR(2));
		}
		else addch(tab[x][nb_ligne]);
	}
}


int main(int argc, char** argv){
	int a;
	for(int i=1; i<argc; i++) if(*argv[i]=='-') a=option((argv[i]+1));
	if(a==1) return 0;
	int col, ligne;
	initscr();
	if(has_colors() == FALSE)
	{	endwin();
		printf("Votre Terminal ne support pas les couleurs\n");
		exit(1);
	}
	signal(SIGINT, SIG_IGN);
	noecho();
	nodelay(stdscr, TRUE);
	scrollok(stdscr, FALSE);

	start_color();
	init_pair(1, couleur, 0);
	init_pair(2, 7, 0);
	attron(COLOR_PAIR(1));
	
	//creation du tableau 
	getmaxyx(stdscr, ligne, col);
	char** tableau=malloc(sizeof(char*)*col);
	for(int y=0; y<col;y++) tableau[y]=malloc(sizeof(char)*TAILLE*ligne);
	for(int a=0; a<col; a++){
		int test=rand()%dispertion;
		int space=0;	
		int suite=0;
		if(test==0){
		       	if(personnage==0)suite=strlen(antonin);
			if(personnage==1)suite=strlen(adrien);
			if(personnage==2)suite=strlen(elies);
			if(personnage==3)suite=strlen(alban);
		}
		else space=test;	
		for(int i=0; i<(ligne*TAILLE); i++){
			if(suite>0){
				if(personnage==0) tableau[a][i]=antonin[suite-1];
				if(personnage==1) tableau[a][i]=adrien[suite-1];
				if(personnage==2) tableau[a][i]=elies[suite-1];
				if(personnage==3) tableau[a][i]=alban[suite-1];
				suite--;
				if(suite==0) suite=-1;
			}
			if(space>0){
				tableau[a][i]=' ';
				space--;
				if(space==0) suite=0;
			}
			if(suite==-1) space=(rand()%dispertion)+1;
			if(suite==0 && personnage==0) suite=strlen(antonin);
			if(suite==0 && personnage==1) suite=strlen(adrien);
			if(suite==0 && personnage==2) suite=strlen(elies);		
			if(suite==0 && personnage==3) suite=strlen(alban);


		}
	}
	
	//boucle affichage
	int temps=1000000/vitesse;
	int i=0;
	while(i<=(ligne*TAILLE)){
		if((a=getch()) =='q') break;
		if (i>=ligne*TAILLE) i=ligne;
		for(int s=0; s<=i; s++){
		       	afficher_ligne(tableau, col, s, i-s);//afficher la ligne s du tableau à la place i-s sur le tableau	
		}
		refresh();
		i++;
		usleep(temps);//en microsecondes	
	}



	attroff(COLOR_PAIR(1));
	endwin();
	
	
	/*for(int i=0; i<(2*ligne);i++){
	       	for(int a=0; a<col; a++) printf("%c", tableau[a][i]);
		printf("\n");
	}*/
 
	for(int z=0;z<col;z++) free(tableau[z]);
	free(tableau);
	return 0;
}
	



	
