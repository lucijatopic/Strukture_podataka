#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 70

//lista
struct _Gradovi;
typedef struct _Gradovi* PositionG;
typedef struct _Gradovi{

    char imeG[MAX_LINE];
    int br_stanovnika;
    PositionG next;
}Gradovi;

//stablo
struct _Drzave;
typedef struct _Drzave* PositionD;
typedef struct _Drzave{

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
    scanf(" %s", drzava);

    printf("Unesite broj: ");
    scanf("%d", &n);

    GradoviSViseStanovnika(drzava, n, root);

    return 0;
}

PositionD CitajDrzaveIzDatoteke(char *dat, PositionD prvi)
{
    char buffer[MAX_LINE] = { 0 };
    int status = 0;
    char DatotekeGradova[MAX_LINE] = { 0 };

    FILE *fp;
    fp=fopen(dat, "r");

    PositionD q = NULL;

    if(!fp)
    {
        printf("Nemoguce otvoriti datoteku!\n");
        return NULL;
    }

    while(!feof(fp))
    {
        q = NULL;
        q = (PositionD)malloc(sizeof(Drzave));
        q->L = NULL;
        q->R =NULL;
        q->head = NovaLista(); //pocetak svake liste

        fgets(buffer, MAX_LINE, fp);
        status = sscanf(buffer, " %s %s",  q->imeD, DatotekeGradova);

        if(status == 2)
        {
            CitajGradoveIzDatoteke(DatotekeGradova, q->head);

            prvi = UnesiDrzaveSortiranoUStablo(prvi, q);
        }
        else if (status!=2)
        {
            printf("Nepravilan format datoteke!\n");
            return -2;
        }
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

    FILE* fp;
    fp = fopen(dat, "r");
    PositionG q = NULL;

    if (!fp)
    {
        printf("Nemoguce otvoriti datoteku!\n");
        return -1;
    }

    while (!feof(fp))
    {
        q = NULL;

        fgets(buffer, MAX_LINE, fp);
        status = sscanf(buffer, " %s %d", imeGrada, &brStan);

        if (status == 2)
        {
            q = NoviElementListe(imeGrada, brStan);
            UnesiGradSortiranoUListu(head, q);
        }
        else if (status!=2)
        {
        	printf("Nepravilan format datoteke!\n");
        	return -2;
        }
    }

    fclose(fp);
    return 0;
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

    if (q->next == NULL || q->next->br_stanovnika != novi->br_stanovnika)
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
        strcpy(current->imeD, novi->imeD);

        current->L = NULL;
        current->R = NULL;
        current->head = novi->head;
    }
    else if (strcmp(novi->imeD, current->imeD) < 0)
    {
        current->L = UnesiDrzaveSortiranoUStablo(current ->L, novi);
    }
    else if (strcmp(novi->imeD, current->imeD) > 0)
    {
        current->R = UnesiDrzaveSortiranoUStablo(current->R, novi);
    }

    return current;
}

int PrintStabloInorder(PositionD current)
{
    if (current == NULL)
        return 0;

    PrintStabloInorder(current->L);
    printf("\n\n%s\n", current->imeD);
    PrintListe(current->head);
    PrintStabloInorder(current->R);

    return 0;
}

int PrintListe(PositionG head)
{
    PositionG q = head->next;
    while (q != NULL)
    {
        printf("\n%s\t\t%d", q->imeG, q->br_stanovnika);
        q = q->next;
    }
    return 0;
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

    PositionG ListaUneseneDrzave = UnesenaDrzava->head; // svi gradovi pod tom drzavom

    while (ListaUneseneDrzave->next != NULL && ListaUneseneDrzave->next->br_stanovnika < UneseniBroj)
        ListaUneseneDrzave = ListaUneseneDrzave->next;

    if (ListaUneseneDrzave->next == NULL)
    {
        printf("Ne postoji grad s vise stanovnika!");
        return -2;
    }

    printf(" %s (gradovi sa vise od %d stanovnika) : \n", ImeDrzave, UneseniBroj);
    PrintListe(ListaUneseneDrzave);

    return 0;

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
