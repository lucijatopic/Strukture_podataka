#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024
// vezana lista (po nazivu drzava), svaki cvor gradovi (po br. stanovnika, a onda i nazivu)

//stablo
struct _Gradovi;
typedef struct _Gradovi* PositionG;
typedef struct _Gradovi {
	char imeG[MAX_LINE];
	int br_stanovnika;
	PositionG L;
	PositionG R;

}Gradovi;

//lista
struct _Drzave;
typedef struct _Drzave* PositionD;
typedef struct _Drzave {
	char ime[MAX_LINE];
	PositionD next;
	PositionG root;
}Drzave;

int CitajIzDatoteke(char *dat, PositionD head);
PositionG CitajGradoveIzDat(char* dat);
int UnesiDrzaveSortiranoUListu(PositionD head, PositionD novi);
PositionG UnesiGradSortiranoUStablo(char* ImeGrada, int brojStanovnika, PositionG current);
int UmetniNakon(PositionD current, PositionD novi);
int PrintListe(PositionD head);
int PrintStabloInorder(PositionG current);
int GradoviSViseStanovnika(char* ImeDrzave, int UneseniBroj, PositionD head);
PositionD PronadiDrzavu(char* ImeDrzave, PositionD head);
int PrintPronadenihGradova(int UneseniBroj, PositionG current);
int MaxBroj(PositionG current);

int main()
{
	Drzave head = { .ime = 0, .next = NULL, .root = NULL };
	char drzava[MAX_LINE] = { 0 };
	int n = 0;

	CitajIzDatoteke("drzave.txt", &head);

	PrintListe(&head);

	printf("\nOdaberite drzavu: ");
	scanf("%s", drzava);

	printf("Unesite broj: ");
	scanf("%d", &n);
	GradoviSViseStanovnika(drzava, n, &head);

	return 0;
}

int CitajIzDatoteke(char *dat, PositionD head)
{
    char DatotekeGradova[MAX_LINE] = { 0 };
    char buffer[MAX_LINE] = { 0 };
    int status = 0;

    FILE *fp;
    fp=fopen(dat, "r");

    PositionD q = NULL;

    if(!fp)
    {
        printf("Nemoguce otvoriti datoteku!\n");
		return -1;
    }

    while(!feof(fp))
    {

        PositionD q = NULL;
        q = (Drzave*)malloc(sizeof(Drzave));
        q->next = NULL;

        fgets(buffer, MAX_LINE, fp);
        status = sscanf(buffer, " %s %s",  q->ime, DatotekeGradova);

        if(status == 2)
        {
            q->root = NULL;
            q->root = CitajGradoveIzDat(DatotekeGradova);

            UnesiDrzaveSortiranoUListu(head, q);
        }
        else
			printf("Nepravilan format datoteke!\n");
    }

    fclose(fp);

    return 0;
}

PositionG CitajGradoveIzDat(char* dat)
{

	int br_stanovnika = 0;
	char ImeGrada[MAX_LINE] = { 0 };
	int status = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* fp;
	fp = fopen(dat, "r");

	PositionG prvi = NULL;

	if(!fp)
	{
		printf("Nemoguce otvoriti datoteku!\n");
		return NULL;
	}

	while(!feof(fp))
	{
		fgets(buffer, MAX_LINE, fp);
		//status = sscanf(buffer, " %s %d", ImeGrada, &br_stanovnika);

		if (sscanf(buffer, " %s %d", ImeGrada, &br_stanovnika) == 2)
		{
			prvi = UnesiGradSortiranoUStablo(ImeGrada, br_stanovnika, prvi);
		}

		else
			printf("Nepravilan format datoteke!\n");

	}

	fclose(fp);

	return prvi;
}

int UnesiDrzaveSortiranoUListu(PositionD head, PositionD novi)
{
	PositionD q = head;

	while (q->next != NULL && strcmp(q->next->ime, novi->ime) < 0) //ide po abecedi
	{
		q = q->next;
	}

	if (q->next == NULL || strcmp(q->next->ime, novi->ime) != 0)
	{
		UmetniNakon(q, novi);
	}

	else
		return -1;

	return EXIT_SUCCESS;
}

int UmetniNakon(PositionD current, PositionD novi)
{
	novi->next = current->next;
	current->next = novi;

	return EXIT_SUCCESS;
}

PositionG UnesiGradSortiranoUStablo(char* ImeGrada, int brojStanovnika, PositionG current)
{
	if (current == NULL)
	{
		current = (PositionG)malloc(sizeof(Gradovi));
		current->L = NULL;
		current->R = NULL;
		current->br_stanovnika = brojStanovnika;
		strcpy(current->imeG, ImeGrada);
	}

	else if (brojStanovnika < current->br_stanovnika)
	{
		current->L = UnesiGradSortiranoUStablo(ImeGrada, brojStanovnika, current->L);
	}
	else if (brojStanovnika > current->br_stanovnika)
	{
		current->R = UnesiGradSortiranoUStablo(ImeGrada, brojStanovnika, current->R);
	}

	else if (brojStanovnika == current->br_stanovnika)
	{
		if (strcmp(ImeGrada, current->imeG) < 0)
			current->L = UnesiGradSortiranoUStablo(ImeGrada, brojStanovnika, current->L);

		else if (strcmp(ImeGrada, current->imeG) > 0)
			current->R = UnesiGradSortiranoUStablo(ImeGrada, brojStanovnika, current->R);

		else
			return current;
	}

	else
        free(current);

	return current;
}

int PrintStabloInorder(PositionG current)
{
	if (current == NULL)
		return 0;

	PrintStabloInorder(current->L);
	printf(" %s\t\t%d\n", current->imeG, current->br_stanovnika);
	PrintStabloInorder(current->R);

	return 0;
}

int PrintListe(PositionD head)
{
	PositionD q = head->next;
	while (q != NULL)
	{
		printf("%s\n", q->ime);
		PrintStabloInorder(q->root);
		q = q->next;
	}

	return 0;
}

int GradoviSViseStanovnika(char* ImeDrzave, int UneseniBroj, PositionD head)
{
	PositionD UnesenaDrzava = PronadiDrzavu(ImeDrzave, head);

	int max = 0;

	if (UnesenaDrzava == NULL)
	{
		printf("Ta drzava ne postoji!\n");
		return -1;
	}

	max = MaxBroj(UnesenaDrzava->root);

	if (UneseniBroj > max)
	{
		printf("Ne postoje gradovi sa vise stanovnika!\n");
		return -2;
	}

	printf(" %s (gradovi sa vise od %d stanovnika) : \n", ImeDrzave, UneseniBroj);
	PrintPronadenihGradova(UneseniBroj, UnesenaDrzava->root);

	return 0;

}

PositionD PronadiDrzavu(char* ImeDrzave, PositionD head)
{
	PositionD q = head;

	while (q->next != NULL && strcmp(q->ime, ImeDrzave) != 0)
		q = q->next;

	return q;
}

int MaxBroj(PositionG current)
{
	int max = 0;

	if (current == NULL)
		return 0;

	if (current->R == NULL)
		return current->br_stanovnika;

	max = MaxBroj(current->R);

	return max;
}

int PrintPronadenihGradova(int UneseniBroj, PositionG current)
{
	if (current == NULL)
		return 0;

	PrintPronadenihGradova(UneseniBroj, current->L);

	if (current->br_stanovnika >= UneseniBroj)
		printf(" %s\t\%d\n", current->imeG, current->br_stanovnika);

	PrintPronadenihGradova(UneseniBroj, current->R);

	return 0;
}
