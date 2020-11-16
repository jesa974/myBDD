#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include "maths.h"


// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 1200
#define HauteurFenetre 600

// Fonction de trace de cercle
void cercle(float centreX, float centreY, float rayon);
/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);



int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("GfxLib", LargeurFenetre, HauteurFenetre);
	
	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	return 0;
}


/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false; // Pour savoir si on est en mode plein ecran ou pas
	static DonneesImageRGB *image = NULL; // L'image a afficher au centre de l'ecran
	static noyau N;
	int garder;
	//Segment courant;
	Segment Mediatrice;
	Segment S[DIM];
	//Point intersection;
	static int position;
	int position_mediatrice ;
	
	switch (evenement)
	{
		case Initialisation:
			
			/* Le message "Initialisation" est envoye une seule fois, au debut du
			programme : il permet de fixer "image" a la valeur qu'il devra conserver
			jusqu'a la fin du programme : soit "image" reste a NULL si l'image n'a
			pas pu etre lue, soit "image" pointera sur une structure contenant
			les caracteristiques de l'image "imageNB.bmp" */
			
			N = chargeNoyau("noyau.txt");
			position = 0;
			
			// Configure le systeme pour generer un message Temporisation
			// toutes les 1/2 secondes
			demandeTemporisation(500);
			break;
		
		case Temporisation:
			
			if(position<N.nbpt)
			{
				position++;
			}
			
			// il faut redessiner la fenetre :
			rafraichisFenetre();
			break;
			
		case Affichage:
			
			// On part d'un fond d'ecran blanc
			effaceFenetre (255, 255, 255);
			
			epaisseurDeTrait(5);
			couleurCourante(0,0,0);
			ligne(800,0,800,600);
			
			Boite(850,50,1150,250);
			
			couleurCourante(255,255,255);
			epaisseurDeTrait(2);
			afficheChaine("Quitter",16,975,150);
			
			Boite(850,350,1150,550);
			
			couleurCourante(255,255,255);
			epaisseurDeTrait(2);
			afficheChaine("Sauvegarde",16,950,450);
			
			position_mediatrice = 0;
			
			for(int i=0;i<position;i++)
			{
				//printf("N° Point=%d et x=%d et y=%d\n",i,N.T[i].x,N.T[i].y);
				
				couleurCourante(255,0,0);
				epaisseurDeTrait(10);
				point(N.T[i].x,N.T[i].y);
				
				for(int j=0;j<i;j++)
				{	
					Mediatrice = calculMediatrice(N.T[i],N.T[j]);
					
					//printf("Mediatrice (%d,%d) - (%d,%d)\n",Mediatrice.M1.x,Mediatrice.M1.y,Mediatrice.M2.x,Mediatrice.M2.y);
					
					if (i==-1)
					{
						// tracé du milieu du segment AB
						couleurCourante(0,0,255);
						epaisseurDeTrait(8);
						point(Mediatrice.milieuDroite.x,Mediatrice.milieuDroite.y);
					}
					
					garder = conditionArret(0,position_mediatrice,N.T[i],N.T[j],S);
					
					if(garder == 1)
					{
						//printf("Mediatrice (%d,%d) - (%d,%d)\n",Mediatrice.M1.x,Mediatrice.M1.y,Mediatrice.M2.x,Mediatrice.M2.y);
						
						//Mediatrice = intersectionIterative(position_mediatrice,Mediatrice,S);
						
						Mediatrice = intersectionRecursive(0,position_mediatrice,Mediatrice,S);
						
						S[position_mediatrice] = Mediatrice;
						traceMediatrice(Mediatrice.M1,Mediatrice.M2);
						position_mediatrice++ ;
					}
				}	
			}
			break;
			
		case Clavier:
			printf("%c : ASCII %d\n", caractereClavier(), caractereClavier());

			switch (caractereClavier())
			{
				case 'Q': /* Pour sortir quelque peu proprement du programme */
				case 'q':
					libereDonneesImageRGB(&image); /* On libere la structure image,
					c'est plus propre, meme si on va sortir du programme juste apres */
					//termineBoucleEvenements();
					break;

				case 'F':
				case 'f':
					pleinEcran = !pleinEcran; // Changement de mode plein ecran
					if (pleinEcran)
						modePleinEcran();
					else
						redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;

				case 'R':
				case 'r':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 2000 secondes (rapide)
					demandeTemporisation(20);
					break;

				case 'L':
				case 'l':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 5 secondes (lent)
					demandeTemporisation(5000);
					break;

				case 'S':
				case 's':
					// Configure le systeme pour ne plus generer de message Temporisation
					demandeTemporisation(-1);
					break;
				
				case 'B':
				case 'b':
					// Configure le systeme pour sauvegarder l'état de la boîte de Pétri dans un fichier binaire nommé "sauvegarde"
					//sauveBoiteBin("sauvegarde.bin",position_mediatrice,N,S);
					//sauveBoiteTxt("sauvegarde.txt",position_mediatrice,N,S);
					break;
			}
			break;
			
		case ClavierSpecial:
			printf("ASCII %d\n", toucheClavier());
			break;

		case BoutonSouris:
			if (etatBoutonSouris() == GaucheAppuye)
			{
				printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
				
			}
			else if (etatBoutonSouris() == GaucheRelache)
			{
				gereClic("sauvegarde.txt",position,N,S);
				printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
			}
			break;
		
		case Souris: // Si la souris est deplacee
			break;
		
		case Inactivite: // Quand aucun message n'est disponible
			break;
		
		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			// Donc le systeme nous en informe
			
			printf("Largeur : %d\t", largeurFenetre());
			printf("Hauteur : %d\n", hauteurFenetre());
			break;
	}
}
