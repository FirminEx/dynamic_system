typedef struct Animal Animal;
struct Animal{
	int x;
	int y;
	double energie;
	int dir[2];
	Animal *suivant;
};
#define SIZE_X 20
#define SIZE_Y 20

Animal *creer_animal(int pX,int pY,double pEnergie);
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);
int compter_animaux_recursif(Animal *animal);
int compter_animaux_iteration(Animal *animal);
void ajouter_animal(int x,int y,Animal **liste_animal);
void liberer_liste(Animal *liste);
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur);
int enlever_animal(Animal **liste,Animal *animal);
void afficher_liste(Animal *liste);
void bouger_animaux(Animal *liste);
void reproduire(Animal **liste,float p_reproduce);
void rafraichir_monde(int herbe[SIZE_X][SIZE_Y]);
void rafraichir_proies(Animal **liste,int herbe[SIZE_X][SIZE_Y]);
Animal *animal_en_xy(Animal *liste,int x,int y);
void rafraichir_predateur(Animal **liste,Animal **liste_proie);

extern float p_ch_dir;
extern int gain_energie_proie;
extern int gain_energie_predateur;
extern float p_reproduce_proie;	
extern float p_reproduce_predateur;
extern int temps_repousse_herbe;
