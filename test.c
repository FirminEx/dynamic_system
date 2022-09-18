#include <stdio.h>
#include <stdlib.h>
typedef struct Animal Animal;


struct Animal{
	int x;
	int y;
	double energie;
	int dir[2];
	Animal *suivant;
};
Animal *creer_animal(int pX,int pY,double pEnergie){
	Animal *retour= malloc(sizeof(Animal));
	retour->x=pX;		
	retour->y=pY;
	retour->dir[0]=rand()%3-1;
	retour->dir[1]=rand()%3-1;
	retour->energie=pEnergie;
	
}

void ajouter_animal(int x,int y,Animal **liste_animal){
	
	Animal *animal=malloc(sizeof(Animal));
	animal->x=x;
	animal->y=y;
	animal->suivant=*liste_animal;	
	*liste_animal=animal;	
}


int main(int argc,char *argv[]){

	Animal *proie=creer_animal(1,1,1.0);
	Animal **list=&proie;
	Animal *proie2=creer_animal(2,2,2.0);
	Animal *proie3=creer_animal(3,3,3.0);
	Animal *proie4=creer_animal(4,4,4.0);
	
	Animal *ptr=(*list)->suivant;

	return 0;
}

