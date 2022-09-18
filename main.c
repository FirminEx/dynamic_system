#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

	//x axe vertical y horizontal 0 est compté SIZE_X/Y exclus
	
	//faire fonction rafraichir proie
	FILE *evol_pop=NULL;
	evol_pop=fopen("Evol_Pop.txt","w");
	if(!evol_pop){
		printf("echec de l'ouverture du fichier\n");
	}
	
	
	int herbe[SIZE_X][SIZE_Y];
	for(int i=0;i<SIZE_X;i++){
		for(int j=0;j<SIZE_Y;j++){
			herbe[i][j]=0;
		}
	}
			

	srand(time(0));
	
	Animal *tete_liste_proie=creer_animal(0,0,12);
	for(int i=0;i<20;i++){
		
		int x=rand()%SIZE_X;
		int y=rand()%SIZE_Y;
		float energie=(float)(rand()%15);
		
		Animal *animal=creer_animal(x,y,energie);
		ajouter_en_tete_animal(tete_liste_proie,animal);
		tete_liste_proie=animal;
	}
	Animal **liste_proie=&tete_liste_proie;

	Animal*	tete_liste_predateur=creer_animal(SIZE_X-1,SIZE_Y-1,14);
	for(int i=0;i<20;i++){
		
		int x=rand()%SIZE_X;
		int y=rand()%SIZE_Y;
		float energie=(float)(rand()%15);
		
		Animal *animal=creer_animal(x,y,energie);
		ajouter_en_tete_animal(tete_liste_predateur,animal);
		tete_liste_predateur=animal;
	}
	Animal **liste_predateur=&tete_liste_predateur;
		
	
	afficher_ecosys(*liste_proie,*liste_predateur);
	char ligne[100];
	char proies[100]="";
	char predateurs[100]="";
	for(int i=0;i<150;i++){
		ligne[0]='\0';
		sprintf(ligne,"%d",i);
		
		rafraichir_monde(herbe);
		rafraichir_proies(liste_proie,herbe);
		rafraichir_predateur(liste_predateur,liste_proie);
		afficher_liste(*liste_proie);
		afficher_ecosys(*liste_proie,*liste_predateur);		
		
		sprintf(proies,"%d",compter_animaux_recursif(*liste_proie));
		sprintf(predateurs,"%d",compter_animaux_recursif(*liste_predateur));
		
		strcat(ligne," ");
		strcat(ligne,proies);
		strcat(ligne," ");
		strcat(ligne,predateurs);
		strcat(ligne," ");
		strcat(ligne,"\n");
		printf("%s",ligne);
		fputs(ligne,evol_pop);
		
		usleep(100000);
	}
	

	
	
	liberer_liste(*liste_predateur);
	liberer_liste(*liste_proie);
	fclose(evol_pop);
}
