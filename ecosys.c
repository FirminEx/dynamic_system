#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ecosys.h"
#include <unistd.h>
float p_ch_dir=0.01;
int gain_energie_proie=3;
int gain_energie_predateur=12;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.2;
float p_manger=0.3;
int temps_repousse_herbe=-15;

Animal *creer_animal(int pX,int pY,double pEnergie){
	Animal *retour= malloc(sizeof(Animal));
	assert(retour);
	retour->x=pX;		
	retour->y=pY;
	retour->dir[0]=rand()%3-1;
	retour->dir[1]=rand()%3-1;
	retour->energie=pEnergie;
	retour->suivant=NULL;
	return retour;
	
}

Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal){
	assert(animal);
	assert(!animal->suivant);
	animal->suivant=liste;
	return animal;
}

int compter_animaux_recursif(Animal *animal){
	if(!animal){ 
		return 0;
	}
	return 1+compter_animaux_recursif(animal->suivant);
}

int compter_animaux_iteration(Animal *animal){
	int retour=0;
	while(animal){
		retour++;
		animal=animal->suivant;
	}
	return retour;
	
}

void ajouter_animal(int x,int y,Animal **liste_animal){
	assert(x>=0 && x<SIZE_X);
	assert(y>=0 && x<SIZE_Y);
	
	Animal *animal=malloc(sizeof(Animal));
	animal->x=x;
	animal->y=y;
	animal->suivant=(*liste_animal)->suivant;	
	*liste_animal=animal;
	
}

void liberer_liste(Animal *liste){
	Animal *animal;
	while(liste){
		animal=liste->suivant;
		free(liste);
		liste=animal;
	}
}

void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
	unsigned int i, j;
	char ecosys[SIZE_X][SIZE_Y];
	int nbpred=0,nbproie=0;
	Animal *pa=NULL;
	/* on initialise le tableau */
	for (i = 0; i < SIZE_X; ++i) {
		for (j = 0; j < SIZE_Y; ++j) {
			ecosys[i][j]=' ';
		}
	}
	/* on ajoute les proies */
	pa = liste_proie;
	while (pa) {
		++nbproie;
		assert (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y);
		ecosys[pa->x][pa->y] = '*';
		pa=pa->suivant;
	}
	/* on ajoute les predateurs */
	pa = liste_predateur;
	while (pa) {
		++nbpred;
		assert(pa->x >= 0 && pa->x < SIZE_X);
		assert(pa->y >= 0 && pa->y < SIZE_Y);
		if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /*
			proies aussi present */
			ecosys[pa->x][pa->y] = '@';
		} else {
			ecosys[pa->x][pa->y] = 'O';
		}
		pa = pa->suivant;
	}
	/* on affiche le tableau */
	printf("+");
	for (j = 0; j < SIZE_Y; ++j) {
		printf("-");
	}
	printf("+\n");
	for (i = 0; i < SIZE_X; ++i) {
		printf("|");
		for (j = 0; j < SIZE_Y; ++j) {
			putchar(ecosys[i][j]);
		}
		printf("|\n");
	}
	printf("+");
	for (j = 0; j<SIZE_Y; ++j) {
		printf("-");
	}
	printf("+\n");
	printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}

int enlever_animal(Animal **liste,Animal *animal){
	Animal *temp=*liste;//on sauvegarde le debut de liste
	if(*liste==animal){//si on souhaite supprimer le permier elt de la liste
		*liste=(*liste)->suivant;
		free(animal);
		return 0;
	}
	while((*liste)->suivant){
		if((*liste)->suivant==animal){
			if(animal->suivant){//si l'animal n'est pas la fin de liste
				(*liste)->suivant=animal->suivant;//on "saute" l'animal
				*liste=temp;//on fait en sorte que liste repointe sur le debut de la liste
				free(animal);//on libere la memoire pour animal
				return 0;//sortie anticipee pour ne pas modifier liste
			}
			else if(!animal->suivant){//meme procede que precedemment sauf que animal est en fin de liste donc l'avant dernier elt devient le dernier
				(*liste)->suivant=NULL;
				*liste=temp;
				free(animal);
				return 0;
			}
		}
		(*liste)=(*liste)->suivant;
	}
	
}



void afficher_liste(Animal *liste){
	printf("\n");
	while(liste){
		printf("e: %f, %d %d\n",liste->energie,liste->x,liste->y);
		liste=liste->suivant;
	}
	printf("\n");
}

void bouger_animaux(Animal *liste){
	while(liste){//on itere dans la liste
		printf("coordonne debut %d %d\n",liste->x,liste->y);
		printf("deplacement vers %d %d\n",liste->dir[0],liste->dir[1]);
		double deplacement=(double)rand()/RAND_MAX;//on tire un numero aleatoire entre 0 et 1
		
		if(deplacement<p_ch_dir){//si on doit operer un changement de direction
			printf("changement de dir\n");
			liste->dir[0]=rand()%3-1;
			liste->dir[1]=rand()%3-1;
		}
		liste->energie--;
		liste->x+=liste->dir[0];//on applique le deplacement
		liste->y+=liste->dir[1];
		if(liste->x<0){//si x devient trop petit il prend la valeur max
			liste->x=SIZE_X-1;
		}
		if(liste->y<0){//idem pour y
			liste->y=SIZE_Y-1;
		}
		if(liste->x==SIZE_X){//si x devient trop grand il prend la valeur minimum
			liste->x=0;
		}
		if(liste->y==SIZE_Y){//idem pour y
			liste->y=0;
		}
		printf("coordonne fin %d %d\n",liste->x,liste->y);
		liste=liste->suivant;
	}
}


void reproduire(Animal **liste,float p_reproduce){
	Animal *it=*liste;//on recupere la liste qui vont se reprod
	Animal *preced=*liste;
	Animal *new;
	while(it){
		double reprod=(double)rand()/RAND_MAX;
		if(reprod<p_reproduce){
				printf("nouvel animal de coordonnes %d %d\n",it->x,it->y);
			 new=creer_animal(it->x,it->y,it->energie);
			 *liste=ajouter_en_tete_animal(*liste,new);		
		}
		it=it->suivant;
		
	}	
}

void rafraichir_monde(int herbe[SIZE_X][SIZE_Y]){
	for(int i=0;i<SIZE_X;i++){
		for(int j=0;j<SIZE_Y;j++){
			herbe[i][j]++;
		}
	}
}

	

Animal *animal_en_xy(Animal *liste,int x,int y){
	while(liste){
		if(liste->x==x && liste->y==y){
			return liste;
		}
		liste=liste->suivant;
	}
	return NULL;

}

void rafraichir_proies(Animal **liste, int herbe[SIZE_X][SIZE_Y]){
	Animal *liste_proie=*liste;
	bouger_animaux(liste_proie);
	while(liste_proie){
		printf("proie %d %d\n",liste_proie->x,liste_proie->y);
		liste_proie->energie--;
		if(herbe[liste_proie->x][liste_proie->y]>0){
			printf("mange herbe\n");
			liste_proie->energie+=gain_energie_proie;
			herbe[liste_proie->x][liste_proie->y]=temps_repousse_herbe;
		}
		if(liste_proie->energie<0){
			printf("supression de %d %d\n",liste_proie->x,liste_proie->y);
			enlever_animal(liste,liste_proie);
		}
		liste_proie=liste_proie->suivant;
		
	}
	reproduire(liste,p_reproduce_proie);
}




void rafraichir_predateur(Animal **liste,Animal **liste_proie){
	
	Animal *liste_predateur=*liste;
	bouger_animaux(liste_predateur);
	while(liste_predateur){
		printf("predateur %d %d\n",liste_predateur->x,liste_predateur->y);
		liste_predateur->energie--;
		double manger=(double)rand()/RAND_MAX;
		if(animal_en_xy(*liste_proie,liste_predateur->x,liste_predateur->y) && manger<p_manger){
			printf("predateur mange en %d %d\n",liste_predateur->x,liste_predateur->y);
			liste_predateur->energie+=gain_energie_predateur;
			enlever_animal(liste_proie,animal_en_xy(*liste_proie,liste_predateur->x,liste_predateur->y));
			
		}
		if(liste_predateur->energie<0){
			printf("supression du predateur %d %d\n",liste_predateur->x,liste_predateur->y);
			enlever_animal(liste,liste_predateur);
		}
		liste_predateur=liste_predateur->suivant;
	}
	reproduire(liste,p_reproduce_predateur);
}


