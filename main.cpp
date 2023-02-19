#include "flux_io.h"
#include "cvm 21.h"
#include "Livres.h"
#include "Clients.h"
#include "Location.h"
#include<conio.h>

#ifdef _CORRECTION__
#include "test.h"

std::string globalFileName;

// Ne touchez pas à cette fonction.
// Si vous la modifiez, vous aurez automatiquement la note de 0
int main(int argc, char** argv){
	setcp(1252);

	if (argc > 1) {
		globalFileName = argv[1];
	}
	else
		globalFileName = "--null";

	RunTest();

	return 0;
}
#else

int main() {
	char choix;
	int idClient, idLivre;
	Date_s date;
	Client_s client;
	float total;

	do
	{
		choix = '0';
		idClient = idLivre = 0;
		gotoxy(0, 0); clreoscr();

		gotoxy(5, 5); ecrire << "1. Ajouter un nouveau livre";
		gotoxy(5, 6); ecrire << "2. Ajouter un nouveau client";
		gotoxy(5, 7); ecrire << "3. Afficher le dossier d'un client";
		gotoxy(5, 8); ecrire << "4. Louer un livre";
		gotoxy(5, 9); ecrire << "5. Retourner un livre";
		gotoxy(5, 10); ecrire << "6. Liste des livres pretes";
		gotoxy(5, 11); ecrire << "7. Liste des clients en retard";
		gotoxy(5, 12); ecrire << "8. Quitter";

		gotoxy(5, 14); ecrire << "Entrez votre choix : "; choix = _getche();

		switch (choix)
		{
		case '1':
			NouveauLivre();
			break;
		case '2':
			NouveauClient();
			break;
		case '3':
			gotoxy(5, 16); ecrire << "Entrez l'identifiant du client : "; lire >> idClient;
			AfficherDossierClient(idClient);

			ecrire << "Pressez sur une touche pour revenir au menu";

			_getch();
			break;
		case '4':
			date = {};
			gotoxy(5, 16); ecrire << "Entrez l'identifiant du client : "; lire >> idClient;
			gotoxy(5, 17); ecrire << "Entrez l'identifiant du livre a louer : "; lire >> idLivre;

			if (DateEqual(date, Location(idClient, idLivre)))
				ecrire << "\n\nLOCATION IMPOSSIBLE";
			else {
				date = Location(idClient, idLivre);
				ecrire << "\n\nLocation effectuee, retour prevu le : " << date.jour << "/" << date.mois << "/" << date.annee;
			}

			ecrire << "\n\nPressez sur une touche pour revenir au menu";

			_getch();
			break;

		case '5':
			date = Aujourdhui();
			gotoxy(5, 16); ecrire << "Entrez l'identifiant du client : "; lire >> idClient;
			gotoxy(5, 17); ecrire << "Entrez l'identifiant du livre a retourner : "; lire >> idLivre;

			total = Retour(idClient, idLivre, date) ;

			gotoxy(5, 19);
			if (total > 0) 
				ecrire << "Vous etes en retard et devez payer : " << total << "$ de frais.";
			else
				ecrire << "Livre retourner.";

			gotoxy(5, 21); ecrire << "Pressez sur une touche pour revenir au menu";

			_getch();
			break;

		case '6':
			ListeDesLivresPretes();

			ecrire << setw(5) << ""; ecrire << "Pressez sur une touche pour revenir au menu";
			_getch();
		case '7':
			gotoxy(0, 0); clreoscr();
			gotoxy(0, 5); ecrire << "Liste des clients en retard \n\n";
			ListeDesClientsEnRetard();
			ecrire << setw(5) << ""; ecrire << "Pressez sur une touche pour revenir au menu";
			_getch();
			break;
		case '8':
			exit(EXIT_SUCCESS);
		default:
			break;
		}
	} while (true);
	
}
#endif