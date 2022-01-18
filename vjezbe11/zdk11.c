#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 1024
#define SIZE_OF_HASH 11

struct _StabloGradova;
typedef struct _StabloGradova* PositionSG;
typedef struct _StabloGradova
{
    char imeG[MAX_LINE];
    int br_stanovnika;
    PositionSG L;
    PositionSG R;
} StabloGradova;

struct _ListaDrzava;
typedef struct _ListaDrzava* PositionLD;
typedef struct _ListaDrzava
{

    char imeD[MAX_LINE];
    PositionLD next;
    PositionSG rootGradova;

} ListaDrzava;

int CitajIzDatoteke(char* dat, PositionLD head, ListaDrzava hashTablica[]);
PositionSG CitajGradoveIzDatoteke(char* dat, PositionSG current);
int UmetniSortiranoUListu(PositionLD novi, PositionLD head);
PositionSG UmetniSortiranoUStablo(PositionSG novi, PositionSG current);
PositionSG StvoriNoviElementStabla(char* imeGrada, int brojStan);
int NovaLista(PositionLD head);
int PrintStabloInorder(PositionSG root);
int UmetniNakon(PositionLD current, PositionLD novi);
//do ovdje kao i 10a)
int HashKljuc(char* imeDrzave);
int NovaHashTablica(ListaDrzava hashTablica[], PositionSG novi);
PositionSG PronadiGrad(PositionSG current, int brojStan);
int PronadiDrzavu(ListaDrzava hashTablica[], PositionLD head, char* imeDrzave, int br_stan);

int main()
{
    ListaDrzava head = {.imeD = {0}, .next = NULL, .rootGradova = NULL};
    PositionLD temp = &head;
    char* imeDrzave[MAX_LINE] = {0};
    int brojS = 0;

    ListaDrzava hashTablica[SIZE_OF_HASH];

    for(int i = 0; i < SIZE_OF_HASH; i++)
    {
        NovaLista(&hashTablica[i]);
    }
    return 0;

    CitajIzDatoteke("drzave.txt", temp, hashTablica);

    printf("Odaberi drzavu: ");
    scanf(" %s", imeDrzave);

    printf("Odaberite broj stanovnika: ");
    scanf("%d", &brojS);

    PronadiDrzavu(hashTablica, temp, imeDrzave, brojS);
}

int CitajIzDatoteke(char* dat, PositionLD head, ListaDrzava hashTablica[])
{
    char buffer[MAX_LINE] = {0};
    int status = 0;
    char DatotekeGradova[MAX_LINE] = {0};

    FILE* fp;
    fp = fopen(dat, "r");

    PositionLD novi = NULL;

    if(!fp)
    {
        printf("Nemoguce otvoriti datoteku!\n");
        return -1;
    }

    while(!feof(fp))
    {
        PositionLD novi = NULL;
        novi = (PositionLD)malloc(sizeof(ListaDrzava));
        if(!novi)
        {
            printf("Nemoguce alocirati memoriju!\n");
            return -2;
        }

        fgets(buffer, MAX_LINE, fp);

        status = sscanf(buffer, " %s %s", novi->imeD, DatotekeGradova);

        if(status == 2)
        {
            novi->rootGradova = NULL;
            novi->rootGradova = CitajGradoveIzDatoteke(DatotekeGradova, novi->rootGradova);
            UmetniSortiranoUListu(novi, head);
            NovaHashTablica(hashTablica, novi);
        }
    }

    fclose(fp);

    return 0;
}
int UmetniSortiranoUListu(PositionLD novi, PositionLD head)
{
    PositionLD temp = head;

    while(temp->next != NULL && strcmp(temp->next->imeD, novi->imeD) < 0)
        temp = temp->next;

    UmetniNakon(temp, novi);

    return 0;
}

int UmetniNakon(PositionLD current, PositionLD novi)
{
    novi->next = current->next;
    current->next = novi;

    return 0;
}

int NovaLista(PositionLD novi)
{
    novi->next = NULL;
    novi->rootGradova = NULL;
    strcpy(novi->imeD, "");


    return 0;

}

PositionSG CitajGradoveIzDatoteke(char* dat, PositionSG current)
{
    char buffer[MAX_LINE] = {0};
    int br_stanovnika = 0;
    char imeGrada[MAX_LINE] = {0};
    int status = 0;

    FILE* fp;
    fp = fopen(dat, "r");

    PositionSG novi = NULL;

    if(!fp)
    {
        printf("Nemoguce otvoriti datoteku!\n");
        return -1;
    }

    while(!feof(fp))
    {

        fgets(buffer, MAX_LINE, fp);

        status = sscanf(buffer, " %s %d", imeGrada, &br_stanovnika);

        if(status == 2)
        {
            PositionSG novi = StvoriNoviElementStabla(imeGrada, br_stanovnika);
            current = UmetniSortiranoUStablo(novi, current);
        }
    }

    fclose(fp);

    return 0;
}
PositionSG StvoriNoviElementStabla(char* imeGrada, int brojStan)
{
    PositionSG novi = NULL;
    novi = (PositionSG)malloc(sizeof(StabloGradova));

    if(!novi)
    {
        printf("Nemoguce alocirati memoriju!\n");
        return -1;
    }

    strcpy(novi->imeG, imeGrada);
    novi->br_stanovnika = brojStan;
    novi->L = NULL;
    novi->R = NULL;

    return novi;
}
PositionSG UmetniSortiranoUStablo(PositionSG novi, PositionSG current)
{
    if(current == NULL)
        return novi;

    else if(current->br_stanovnika < novi->br_stanovnika)
        current->L = UmetniSortiranoUStablo(novi, current->L);

    else if(current->br_stanovnika > novi->br_stanovnika)
        current->R = UmetniSortiranoUStablo(novi, current->R);

    else // ako su isti
    {
        if(strcmp(current->imeG, novi->imeG) < 0)
            current->R = UmetniSortiranoUStablo(novi, current->R);

        else if(strcmp(current->imeG, novi->imeG) > 0)
            current->L = UmetniSortiranoUStablo(novi, current->L);

        else // dva potpuno ista
            free(novi);
    }

    return current;
}

int PrintStabloInorder(PositionSG root)
{
    if(root == NULL)
        return 0;

    PrintStabloInorder(root->L);
    printf(" %s %d\n", root->imeG, root->br_stanovnika);
    PrintStabloInorder(root->R);

    return 0;
}

int HashKljuc(char* imeDrzave)
{
    int suma = 0;
    suma = imeDrzave[0] + imeDrzave[1] + imeDrzave[2] + imeDrzave[3] + imeDrzave[4];
    return suma%11;
}

int NovaHashTablica(ListaDrzava hashTablica[], PositionSG novi)
{
    int kljuc = HashKljuc(novi->imeG);

    UmetniSortiranoUListu(novi, &hashTablica[kljuc]);

    return 0;
}

int PronadiDrzavu(ListaDrzava hashTablica[], PositionLD head, char* imeDrzave, int br_stan)
{
    PositionLD temp = head->next;

    while(temp != NULL && strcmp(imeDrzave, temp->imeD) != 0)
        temp = temp->next;

    if(temp != NULL)
    {
        printf("Gradovi s vise stanovnika: \n");
        temp->rootGradova = PronadiGrad(temp->rootGradova, br_stan);
    }

    else
        printf("Ta drzava ne postoji!\n");

    return 0;
}

PositionSG PronadiGrad(PositionSG current, int brojStan)
{
    if(current == NULL)
        return NULL;

    else if (current->br_stanovnika >= brojStan)
    {
        current->L = PronadiGrad(current->L, brojStan);
        printf("\nDrzava: %s, broj stanovnika: %d", current->imeG, current->br_stanovnika);
        current->R = PronadiGrad(current->R, brojStan);
    }

    else if(current->br_stanovnika < brojStan)
    {
        current->R = PronadiGrad(current->R, brojStan);
    }

    return current;
}
