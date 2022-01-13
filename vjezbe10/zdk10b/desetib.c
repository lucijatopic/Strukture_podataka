#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024

//lista
struct _Gradovi;
typedef struct _Gradovi* PositionG;
typedef struct _Gradovi {
	char imeG[MAX_LINE];
	int br_stanovnika;
	PositionG next;
}Gradovi;

//stablo
struct _Drzave;
typedef struct _Drzave* PositionD;
typedef struct _Drzave {
	char imeD[MAX_LINE];
	PositionD L;
	PositionD R;
	PositionG head;
}Drzave;

PositionD CitajDrzaveIzDatoteke(char* dat, PositionD prvi);
int CitajGradoveIzDatoteke(char* dat, PositionG head);
PositionG NovaLista();
PositionG NoviElementListe(char* ImeGrada, int brojStanovnika);
int UnesiGradSortiranoUListu(PositionG head, PositionG novi);
PositionD UnesiDrzaveSortiranoUStablo(PositionD current, PositionD novi);
int UmetniNakon(PositionG current, PositionG novi);
int PrintStabloInorder(PositionD current);
int GradoviSViseStanovnika(char* ImeDrzave, int UneseniBroj, PositionD root);
int PrintListe(PositionG head);
PositionD PronadiDrzavu(char* ImeDrzave, PositionD current);

int main()
{
	PositionD root = NULL;
	char drzava[MAX_LINE] = { 0 };
	int n = 0;

	root = CitajDrzaveIzDatoteke("drzave.txt", root);
	PrintStabloInorder(root);


	printf("\nOdaberite drzavu: ");
	scanf("%s", drzava);

	printf("Unesite broj: ");
	scanf("%d", &n);

	GradoviSViseStanovnika(drzava, n, root);

	return 0;
}

PositionD CitajDrzaveIzDatoteke(char* dat, PositionD prvi)  // poslali smo prvi cvor
{
	char buffer[MAX_LINE] = { 0 };
	int status = 0;
	char DatotekeGradova[MAX_LINE] = { 0 };

	FILE* fp;
	fp = fopen(dat, "r");

	PositionD q = NULL;

	if (!fp)
	{
		printf("Nemoguce otvoriti datoteku!\n");
		return NULL;
	}

	while (!feof(fp))
	{
		q = NULL;
		q = (PositionD)malloc(sizeof(Drzave));
		q->L = NULL;
		q->R = NULL;
		q->head = NovaLista; //svaki cvor pocetak liste

		fgets(buffer, MAX_LINE, fp);
		status = sscanf(fp, " %s %s", q->imeD, DatotekeGradova);

		if (status == 2)
		{
			CitajGradoveIzDatoteke(DatotekeGradova, q->head);
			prvi = UnesiDrzaveSortiranoUStablo(prvi, q);
		}
		/*else
		{
			printf("Nepravilan format datoteke!\n");
			return -2;
		}*/
	}

	fclose(fp);

	return prvi;
}

PositionG NovaLista()
{
	PositionG novi = NULL;
	novi = (PositionG)malloc(sizeof(Gradovi));
	novi->next = NULL;
	novi->br_stanovnika = 0;
	strcpy(novi->imeG, "");

	return novi;
}

int CitajGradoveIzDatoteke(char* dat, PositionG head)
{
	char buffer[MAX_LINE] = { 0 };
	int status = 0;
	int brStan = 0;
	char imeGrada[MAX_LINE] = { 0 };

	PositionG q = NULL;

	FILE* fp;
	fp = fopen(dat, "r");

	if (!fp)
	{
		printf("Nemoguce otvoriti datoteku!\n");
		return -1;
	}

	while (!feof(fp))
	{
		q = NULL;

		fgets(buffer, MAX_LINE, fp);
		status = sscanf(fp, " %s %d", imeGrada, &brStan);

		if (status == 2)
		{
			q = NoviElementListe(imeGrada, brStan);
			UnesiGradSortiranoUListu(head, q);
		}
		/*else
		{
			printf("Nepravilan format datoteke!\n");
			return -2;
		}*/
	}

	fclose(fp);
	//return EXIT_SUCCESS;
}

PositionG NoviElementListe(char* ImeGrada, int brojStanovnika)
{
	PositionG novi = NULL;
	novi = (PositionG)malloc(sizeof(Gradovi));

	strcpy(novi->imeG, ImeGrada);
	novi->br_stanovnika = brojStanovnika;
	novi->next = NULL;

	return novi;
}
int UnesiGradSortiranoUListu(PositionG head, PositionG novi)
{
	PositionG q = head;

	while (q->next != NULL && q->next->br_stanovnika < novi->br_stanovnika) // po broju stanovnika
	{
		q = q->next;
	}

	if (q->next == NULL || q->next->br_stanovnika > novi->br_stanovnika)
	{
		UmetniNakon(q, novi);
	}

	else if (q->next->br_stanovnika == novi->br_stanovnika)
	{
		if (strcmp(q->next->imeG, novi->imeG) > 0) // po imenu
			UmetniNakon(q, novi);
		else if (strcmp(q->next->imeG, novi->imeG) < 0)
			UmetniNakon(q->next, novi);
		else
		{
			printf("Ne mogu biti dva grada sa istim nazivom!\n");
			return 0;
		}
	}

	else
		free(q);

	return EXIT_SUCCESS;
}

int UmetniNakon(PositionG current, PositionG novi)
{
	novi->next = current->next;
	current->next = novi;

	return EXIT_SUCCESS;
}
PositionD UnesiDrzaveSortiranoUStablo(PositionD current, PositionD novi)
{
	if (current == NULL)
	{
		current = (PositionD)malloc(sizeof(Drzave));
		current->L = NULL;
		current->R = NULL;
		strcpy(current->imeD, novi->imeD);
		current->head = novi->head;
	}
	else if (strcmp(current->imeD, novi->imeD) < 0)
	{
		current->L = UnesiDrzaveSortiranoUStablo(current ->L, novi);
	}
	else if (strcmp(current->imeD, novi->imeD) > 0)
	{
		current->R = UnesiDrzaveSortiranoUStablo(current->R, novi);
	}

	return current;
}

int PrintStabloInorder(PositionD current)
{
	if (current == NULL)
		return NULL;

	PrintStabloInorder(current->L);
	printf("\n&s\n", current->imeD);
	PrintStabloInorder(current->R);

	return EXIT_SUCCESS;
}

int GradoviSViseStanovnika(char* ImeDrzave, int UneseniBroj, PositionD root)
{
	PositionD UnesenaDrzava = NULL;
	UnesenaDrzava = PronadiDrzavu(ImeDrzave, root);

	if (UnesenaDrzava == NULL)
	{
		printf("Ta drzava ne postoji!\n");
		return -1;
	}

	PositionG ListaUnesenihDrzava = UnesenaDrzava->head; // svi gradovi pod tom drzavom

	while (ListaUnesenihDrzava->next != NULL && ListaUnesenihDrzava->next->br_stanovnika < UneseniBroj)
		ListaUnesenihDrzava = ListaUnesenihDrzava->next;

	if (ListaUnesenihDrzava->next == NULL)
	{
		printf("Ne postoji grad s vise stanovnika!");
		return -1;
	}

	printf(" %s (gradovi sa vise od %d stanovnika) : \n", ImeDrzave, UneseniBroj);
	PrintListe(ListaUnesenihDrzava);

	return EXIT_SUCCESS;

}

int PrintListe(PositionG head)
{
	PositionG q = head->next;
	while (q != NULL)
	{
		printf(" %s\t\t%n", q->imeG, q->br_stanovnika);
		q = q->next;
	}
	return EXIT_SUCCESS;
}

PositionD PronadiDrzavu(char* ImeDrzave, PositionD current)
{
	PositionD q = NULL;
	PositionD p = NULL; // provjeravamo i lijevu i desnu stranu

	if (current == NULL)
		return NULL;

	else if (strcmp(current->imeD, ImeDrzave) == 0)
		return current;

	q = PronadiDrzavu(ImeDrzave, q->L);
	p = PronadiDrzavu(ImeDrzave, p->R);

	if (q != NULL)
		current = q;

	else if (p != NULL)
		current = p;

	else
		current = NULL;

	return current;
}
