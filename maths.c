#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GfxLib.h"
#include "maths.h"


void Boite (int x1, int y1, int x2, int y2)
{
	epaisseurDeTrait(3);
	couleurCourante(125,125,125);
	rectangle(x1,y1,x2,y2);
}


void gereClic (chaine nomFichier, int position_mediatrice, noyau N, Segment S[DIM])
{
	if((abscisseSouris() > 850) && (abscisseSouris() < 1150) && (ordonneeSouris() > 50) && (ordonneeSouris() < 250))
	{
		termineBoucleEvenements();
	}
	else if ((abscisseSouris() > 850) && (abscisseSouris() < 1150) && (ordonneeSouris() > 350) && (ordonneeSouris() < 550))
	{
		sauveBoiteTxt("sauvegarde.txt",position_mediatrice,N,S);
	}
}




Point calculIntersection (Point A,Point B,Point C,Point D) // fonction qui calcule l'intersection I entre deux droites AB et CD
{
	Point I;
	
	float Ax = A.x;
	float Ay = A.y;
	
	float Bx = B.x;
	float By = B.y;
	
	float Cx = C.x;
	float Cy = C.y;
	
	float Dx = D.x;
	float Dy = D.y;
	
	float Ix;
	float Iy;
	
	if (((By-Ay)/(Bx-Ax))-((Dy-Cy)/(Dx-Cx)) != 0) // on n'effectue le calcul que si les droites ne sont pas parallèles
	{
		//calcul de l'intersection
		
		Ix = (((((Ay-By)/(Ax-Bx))*Ax)-Ay-(((Cy-Dy)/(Cx-Dx))*Cx)+Cy))/(((Ay-By)/(Ax-Bx))-((Cy-Dy)/(Cx-Dx)));
		Iy = (((Cy-Dy)/(Cx-Dx))*(Ix-Cx))+Cy;
	
		I.x = Ix;
		I.y = Iy;
	}
	else
	{
		I.x=0;
		I.y=0;
	}
	
	return I;
}


noyau chargeNoyau (chaine nomFichier) // fonction qui récupère la position des différents noyaux de cellules dans un fichier texte
{
	FILE*f;
	noyau N = {0,{{0,0},{0,0}}};
	f=fopen(nomFichier,"rt");
	
	if(f==NULL)
	{
		return N;
	
	}
	fscanf(f,"nbpt = %d\n",&(N.nbpt));
	//printf("nbpt lus : %d\n",N.nbpt);
	for(int i=0;i<N.nbpt;i++)
	{
		fscanf(f,"(%d,%d)\n",&(N.T[i].x),&(N.T[i].y));
		//printf("points lus :(%d,%d)\n",N.T[i].x,N.T[i].y);
	}
	
	fclose(f);
	return N;
}


Point calculMilieu (Point A, Point B) // fonction qui calcule le milieu du segment AB
{
	Point M;
	
	M.x = (A.x + B.x)/2;
	M.y = (A.y + B.y)/2;
	
	return M;
}


float calculPente (Point A, Point B) // fonction qui calcul la pente du segment AB
{
	float p;
	
	if(A.x == B.x)
	{
		p = 0.0;
	}
	else
	{
		p = (float)(B.y - A.y)/(float)(B.x - A.x);
	}
	
	return p;
}


float calculInvOppPente (Point A, Point B) // fonction qui calcule l'inverse opposé de la pente du segment AB
{
	float p;
	float IO;
	
	if(A.x == B.x)
	{
		p = 0.0;
		IO = 0.0;
	}
	else
	{
		p = calculPente(A,B);
		IO = -1 / p;
	}
	
	return IO;
}


Segment calculMediatrice1 (Point M,float IO) // calcul du premier point d'intersection entre la médiatrice et la fenêtre
{
	Segment resultat = {{0,0},{0,0},{0,0}};
	Point Med1 = {0,0};
	Point Med2 = {0,0};
	int cas = 0;
	
	float b = (float)(M.y - IO*M.x); // calcul de la constante de l'équation de la médiatrice
	
	Med1.y = IO*Med1.x + b;
	
	if((Med1.y >= 0) && (Med1.y <= 600))
	{
		cas = 1;
	}
	
	if(cas == 0)
	{
		Med1.y = 0;
		Med1.x = (Med1.y - b)/IO;
	
		if((Med1.x >= 0) && (Med1.x <= 800))
		{
			cas = 2;
		}
	}
	
	if(cas == 0)
	{
		Med1.y = 600;
		Med1.x = (Med1.y - b)/IO;
		
		if((Med1.x >= 0) && (Med1.x <= 800))
		{
			cas = 3;
		}
	}
	
	Med2.x = 800;
	Med2.y = IO*Med2.x + b;
	
	if((Med2.y >= 0) && (Med2.y <= 600))
	{
		resultat.M1 = Med1;
		resultat.M2 = Med2;
		
		return resultat;
	}
	
	if(cas != 3)
	{
		Med2.y = 600;
		Med2.x = (Med2.y - b)/IO;
		
		if((Med2.x >= 0) && (Med2.x <= 800))
		{
			resultat.M1 = Med1;
			resultat.M2 = Med2;
			
			return resultat;
		}
	}
	
	if(cas != 2)
	{
		Med2.y = 0;
		Med2.x = (Med2.y - b)/IO;
		
		if((Med2.x >= 0) && (Med2.x <= 800))
		{
			resultat.M1 = Med1;
			resultat.M2 = Med2;
			
			return resultat;
		}
	}
	
	return resultat;
}


Segment calculMediatrice (Point A, Point B) // fonction qui calcule la médiatrice du segment AB
{
	Point milieu;
	float IOpente;
	Segment Mediatrice;
	
	milieu = calculMilieu(A,B);
	//printf("milieu : x=%d et y=%d\n",milieu.x,milieu.y);

	IOpente = calculInvOppPente(A,B);
	//printf("Inverse Opposé pente = %f\n",IOpente);
	
	Mediatrice = calculMediatrice1(milieu,IOpente); // premier point d'intersection entre la médiatrice et la fenêtre
	
	Mediatrice.milieuDroite = milieu;
	
	return Mediatrice;
}


void traceMediatrice (Point Med1,Point Med2) // fonction qui trace la médiatrice
{
	// tracé de la médiatrice
	couleurCourante(0,255,0);
	epaisseurDeTrait(3);
	ligne(Med1.x,Med1.y,Med2.x,Med2.y);
}


int calculDistance (Point A, Point B) // fonction qui calcule la distance entre deux points
{
	int distance;
	
	distance = sqrt((B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y)); // calcul de la distance entre le point A et le point B
	
	if(A.x > B.x)
	{
		return -1*distance; // distance négatice si le point A se trouve à droite du point B
	}
	else
	{
		return distance; // distance positive si le point A se trouve à gauche du point B
	}
}


int Appartenance (Point J, Segment AB) // fonction qui vérifie l'appartenance d'un point J à un segment AB
{
	int d;
	int debug = 0;
	
	d = calculDistance(AB.M1,AB.M2); // vérification de l'emplacement de A par rapport à B
	
	if (debug==1)
	{
		printf("distance : %d\n",d);
	}
		
	if(d < 0) // si A se situe à droite de B
	{
		if(AB.M2.y < AB.M1.y) // si B se situe au dessus de A
		{
			if((J.x >= AB.M2.x) && (J.x <= AB.M1.x) && (J.y >= AB.M2.y) && (J.y <= AB.M1.y)) // si J appartient au segment BA
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else // si B se situe en dessous de A
		{
			if((J.x >= AB.M2.x) && (J.x <= AB.M1.x) && (J.y >= AB.M1.y) && (J.y <= AB.M2.y)) // si J appartient au segment BA
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	else // si A se situe à gauche de B
	{
		if(AB.M2.y < AB.M1.y) // si B se situe au dessus de A
		{
			if((J.x >= AB.M1.x) && (J.x <= AB.M2.x) && (J.y >= AB.M2.y) && (J.y <= AB.M1.y)) // si J appartient au segment AB
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else // si B se situe en dessous de A
		{
			if((J.x >= AB.M1.x) && (J.x <= AB.M2.x) && (J.y >= AB.M1.y) && (J.y <= AB.M2.y)) // si J appartient au segment AB
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}


Segment intersectionRecursive (int k, int position_mediatrice, Segment Mediatrice, Segment S[DIM])
{
	Point intersection;
	
	if(k < position_mediatrice)
	{
		intersection = calculIntersection(Mediatrice.M1,Mediatrice.M2,S[k].M1,S[k].M2);
		
		//printf("Mediatrice (%d,%d) - (%d,%d)\n",Mediatrice.M1.x,Mediatrice.M1.y,Mediatrice.M2.x,Mediatrice.M2.y);
		
		if((Appartenance(intersection,Mediatrice) == 1) && (Appartenance(intersection,S[k]) == 1))
		{
			if(calculDistance(intersection,Mediatrice.milieuDroite) > 0)
			{
				if(calculDistance(intersection,Mediatrice.M1) > 0)
				{
					Mediatrice.M2 = intersection;
				}
				else
				{
					Mediatrice.M1 = intersection;
				}
			}
			else
			{
				if(calculDistance(intersection,Mediatrice.M2) > 0)
				{
					Mediatrice.M2 = intersection;
				}
				else
				{
					Mediatrice.M1 = intersection;
				}
			}
		}
		
		Mediatrice = intersectionRecursive(k+1,position_mediatrice,Mediatrice,S);
	}
	
	return Mediatrice;
}


Segment intersectionIterative (int position_mediatrice, Segment Mediatrice, Segment S[DIM])
{
	Point intersection;
	
	for(int k=0;k<position_mediatrice;k++)
	{
		intersection = calculIntersection(Mediatrice.M1,Mediatrice.M2,S[k].M1,S[k].M2);
		
		//printf("Mediatrice (%d,%d) - (%d,%d)\n",Mediatrice.M1.x,Mediatrice.M1.y,Mediatrice.M2.x,Mediatrice.M2.y);
		
		if((Appartenance(intersection,Mediatrice) == 1) && (Appartenance(intersection,S[k]) == 1))
		{
			if(calculDistance(intersection,Mediatrice.milieuDroite) > 0)
			{
				if(calculDistance(intersection,Mediatrice.M1) > 0)
				{
					Mediatrice.M2 = intersection;
				}
				else
				{
					Mediatrice.M1 = intersection;
				}
			}
			else
			{
				if(calculDistance(intersection,Mediatrice.M2) > 0)
				{
					Mediatrice.M2 = intersection;
				}
				else
				{
					Mediatrice.M1 = intersection;
				}
			}
		}
	}
	
	return Mediatrice;
}


int conditionArret (int k, int position_mediatrice, Point A, Point B, Segment S[DIM]) // fonction qui gère la condition d'arrêt
{
	Segment courant;
	Point intersection;
	
	if(k < position_mediatrice)
	{
		intersection = calculIntersection(A,B,S[k].M1,S[k].M2);
		
		//printf("intersection : x=%d et y=%d\n",intersection.x,intersection.y);
		//printf("I / J : xa=%d et xb=%d\n",A.x,B.x);
		
		courant.M1 = A;
		courant.M2 = B;
		
		if(Appartenance(intersection,courant) == 1 && Appartenance(intersection,S[k]) == 1)
		{
			return 0;
		}
		
		return conditionArret(k+1,position_mediatrice,A,B,S);
	}
	else
	{
		return 1;
	}
}


void sauveBoiteBin (chaine nomFichier, int position_mediatrice, noyau N, Segment S[DIM]) // fonction qui sauvegarde l'état de la boîte dans un fichier binaire
{
	FILE*f;
	f=fopen(nomFichier,"wb");
	
	if(f==NULL)
	{
		return;
	}
	
	for(int j=0;j<=position_mediatrice;j++)
	{
		fwrite(&(N.T[j]),sizeof(noyau),1,f);
	}
	
	for(int i=0;i<position_mediatrice;i++)
	{
		fwrite(&(S[i]),sizeof(Segment),1,f);
	}
	
	fclose(f);
}


void sauveBoiteTxt(chaine nomFichier, int position_mediatrice, noyau N, Segment S[DIM]) // fonction qui sauvegarde l'état de la boîte dans un fichier texte
{
	printf("position_mediatrice : %d\n",position_mediatrice);
	FILE*f;
	f=fopen(nomFichier,"wt");
	
	if(f==NULL)
	{
		return;
	}
	
	fprintf(f,"nbpt = %d\n",N.nbpt);
	
	for(int i=0;i<N.nbpt;i++)
	{
		fprintf(f,"Points dessinés (%d,%d)\n",N.T[i].x,N.T[i].y);
	}
	
	for(int j=0;j<position_mediatrice;j++)
	{
		fprintf(f,"Mediatrices dessinées (%d,%d)\n",S[j].M1.x,S[j].M2.y);
		fprintf(f,"Milieu droite (%d,%d)\n",S[j].milieuDroite.x,S[j].milieuDroite.y);
	}
	
	fclose(f);
}
