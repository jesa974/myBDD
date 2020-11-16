#define DIM 100


typedef char chaine[20];


typedef struct Point {
	int x,y;
}Point;


typedef struct noyau {
	int nbpt;
	Point T[DIM];
}noyau;


typedef struct Segment {
	Point M1;
	Point M2;
	Point milieuDroite;
}Segment;


void gereClic (chaine nomFichier, int position_mediatrice, noyau N, Segment S[DIM]);

void Boite (int x1, int y1, int x2, int y2);

Point calculIntersection (Point A, Point B, Point C, Point D); // fonction qui calcule l'intersection I entre deux droites AB et CD

void sauveBoiteBin (chaine nomFichier, int position_mediatrice, noyau N, Segment S[DIM]); // fonction qui sauvegarde l'état de la boîte dans un fichier binaire

void sauveBoiteTxt(chaine nomFichier, int position_mediatrice, noyau N, Segment S[DIM]); // fonction qui sauvegarde l'état de la boîte dans un fichier texte

noyau chargeNoyau (chaine nomFichier); // fonction qui récupère la position des différents noyaux de cellules dans un fichier texte

Point calculMilieu (Point A, Point B); // fonction qui calcule le milieu du segment AB

float calculPente (Point A, Point B); // fonction qui calcul la pente du segment AB

float calculInvOppPente (Point A, Point B); // fonction qui calcule l'inverse opposé de la pente du segment AB

Segment calculMediatrice1 (Point M, float IO); // calcul du premier point d'intersection entre la médiatrice et la fenêtre

Segment calculMediatrice (Point A, Point B); // fonction qui calcule la médiatrice du segment AB

void traceMediatrice (Point M, Point Med); // fonction qui trace la médiatrice

int calculDistance (Point A, Point B); // fonction qui calcule la distance entre deux points

int Appartenance (Point J, Segment AB); // fonction qui vérifie l'appartenance d'un point J à un segment AB

Segment intersectionRecursive (int k, int position_mediatrice, Segment Mediatrice, Segment S[DIM]); // fonction qui gère les intersections de manière récursive

Segment intersectionIterative (int position_mediatrice, Segment Mediatrice, Segment S[DIM]); // fonction qui gère les intersections de manière itérative

int conditionArret (int k, int position_mediatrice, Point A, Point B, Segment S[DIM]); // fonction qui gère la condition d'arrêt
