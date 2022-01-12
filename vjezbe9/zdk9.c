#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 50

struct _Stablo;
typedef struct _Stablo* PositionS;
typedef struct _Stablo {
	int n;
	PositionS L;
	PositionS R;
}Stablo;

struct _Lista;
typedef struct _Lista* PositionL;
typedef struct _Lista {
	int n;
	PositionL next;
}Lista;

//prve tri funkcije su pod a)
PositionS StvoriNovoStablo(int num, PositionS current);
PositionS Insert(PositionS current, PositionS novi);
int Print_In_Order(PositionS current);
//b)
int Replace(PositionS current); // zamijeni trenutnu
//c)
int RandomBroj(int min, int max);
int UpisiUListu(PositionL head, PositionS current);
int IzbrisiIzListe(PositionL head);
int UpisiUDatoteku(PositionS root, PositionL head, char* datoteka);

int main()
{
	PositionS prvi = NULL;
	PositionL head = NULL;
	int niz[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };

	int n, min, max;
	srand((unsigned)time(NULL));

	char datoteka[MAX_LINE] = { 0 };

	printf("Unesite ime datoteke u koju zelite upisati brojeve: ");
	scanf(" %s", &datoteka);

	for (int i = 0; i < 10; i++)
	{
		prvi = StvoriNovoStablo(niz[i], prvi);
	}

	Print_In_Order(prvi);
	printf("\n");

	UpisiUListu(&head, prvi);
	UpisiUDatoteku(prvi, &head, datoteka);

	Replace(prvi);
	printf("\n");
	Print_In_Order(prvi);
	printf("\n");

	UpisiUListu(&head, prvi);
	UpisiUDatoteku(prvi, &head, datoteka);
	printf("\n");


	printf("\nUnesite koliko brojeva zelite generirati: ");
	scanf(" %d", &n);

	printf("Unesite koji broj zelite kao donju granicu: ");
	scanf(" %d", &min);

	if (min < 10)
	{
		printf("Broj je premali. Molimo unesite broj veci od 10!\n");
		scanf(" %d", &min);
	}

	printf("Unesite koji broj zelite kao gornju granicu: ");
	scanf(" %d", &max);

	if (max > 90)
	{
		printf("Broj je prevelik. Molimo unesite broj manji od 90!\n");
		scanf(" %d", &max);
	}

	prvi = NULL; // bez ovoga bi mi se ispisivali i brojevi nakon replace-a

	for (int i = 0; i < n; i++)
	{
		prvi = StvoriNovoStablo((rand() % (max - min + 1) + min + 1), prvi);
	}

	printf("\n");
	Print_In_Order(prvi);

	return 0;
}

PositionS StvoriNovoStablo(int num, PositionS current)
{
	PositionS q = NULL;
	q = (PositionS)malloc(sizeof(Stablo));
	if (!q)
	{
		printf("Nemoguce alocirati memoriju!\n");
		return NULL;
	}

	q->n = num;
	q->L = NULL;
	q->R = NULL;

	q = Insert(current, q);

	return q;
}

PositionS Insert(PositionS current, PositionS novi)
{
	if (current == NULL)
	{
		return novi;
	}

	else if (novi->n < current->n)
	{
		current->R = Insert(current->R, novi);
	}
	else if (novi->n >= current->n)
	{
		current->L = Insert(current->L, novi);
	}

	return current;
}

// iz 8. zadatka
int Print_In_Order(PositionS current)
{
	if (!current)
		return 0;

	Print_In_Order(current->L);
	printf("%d ", current->n);
	Print_In_Order(current->R);

	return 0;

}
/*****************************************/
int Replace(PositionS current)
{
	int suma = 0;
	int t = 0;

	if (current)
	{
		suma += Replace(current->L);
		suma += Replace(current->R);
		t = current->n;
		current->n = suma;
		suma += t;

	}

	return suma;
}

int RandomBroj(int min, int max)
{
	int broj = 0;
	broj = max - min + 1;

	int n = RAND_MAX - (RAND_MAX % broj);
	int RandomBr = rand();
	if (RandomBr < 99 && RandomBr > 100)
	{
		while (RandomBr > n)
		{
			RandomBr = rand();
		}
	}

	return RandomBr % broj + min;
}
int UpisiUListu(PositionL head, PositionS current)
{
	PositionL q = NULL;
	if (!current)
		return 0;

	UpisiUListu(head, current->R);


	q = (PositionL)malloc(sizeof(Lista));
	if (!q)
	{
		printf("Nemoguce alocirati memoriju!\n");
		return -1;
	}

	q->n = current->n;
	q->next = head->next;
	head->next = q;

	UpisiUListu(head, current->L);

	return 0;
}

int IzbrisiIzListe(PositionL head)
{
	PositionL ZaBrisanje = NULL;

	if (head->next == NULL)
	{
		return 0;
	}

	ZaBrisanje = head->next;
	head->next = ZaBrisanje->next;
	free(ZaBrisanje);

	return 0;
}

int UpisiUDatoteku(PositionS root, PositionL head, char* datoteka)
{
	FILE* fp = NULL;
	fp = fopen(datoteka, "a");

	if (!fp)
	{
		printf("Datoteka se ne moze otvoriti!\n");
		return -1;
	}

	while (head->next != NULL)
	{
		fprintf(fp, "%d ", head->next->n);
		IzbrisiIzListe(head);
	}

	//fprintf(datoteka, "\n");
	fclose(fp);

	return 0;
}
