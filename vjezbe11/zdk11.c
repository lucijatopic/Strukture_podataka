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
PositionSG CitajGradoveIzDatoteke(char* dat);
int UmetniSortiranoUListu(PositionLD novi, PositionLD head);
PositionSG SortiraniUnosUStablo(char* imeGrada, int brStan, PositionSG current)
int UmetniNakon(PositionLD current, PositionLD novi);
int PrintStabloInorder(PositionSG root);
PositionLD NovaLista();
int PrintListe(PositionLD head);
int GradoviSViseStanovnika(char* imeDrzave, int brStan, ListaDrzava hashTablica[]);
int PronadiMax(PositionSG current);
//do ovdje kao i 10a)
int HashKljuc(char* imeDrzave);
int NovaHashTablica(ListaDrzava hashTablica[], PositionSG novi);
PositionSG PronadiGrad(PositionSG current, int brojStan);
PositionLD PronadiDrzavu(char* imeDrzave, PositionLD head);

int main()
{
    ListaDrzava head = {.imeD = {0}, .next = NULL, .rootGradova = NULL};
    PositionLD temp = &head;

    char* imeDrzave[MAX_LINE] = {0};
    int brojS = 0;

    ListaDrzava hashTablica[SIZE_OF_HASH];

    for(int i = 0; i < SIZE_OF_HASH; i++)
    {
        hashTablica[i] = *NovaLista();
    }


    CitajIzDatoteke("drzave.txt", temp, hashTablica);

    for(int i = 0; i < SIZE_OF_HASH; i++)
    {
        if(hashTablica[i].next != NULL)
        {
            PrintListe(&hashTablica[i]);
        }
    }


    printf("Odaberi drzavu: ");
    scanf(" %s", imeDrzave);

    printf("Odaberite broj stanovnika: ");
    scanf("%d", &brojS);

    GradoviSViseStanovnika(imeDrzave, brojS, hashTablica);
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
        novi->next = NULL;

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
            novi->rootGradova = CitajGradoveIzDatoteke(DatotekeGradova);

            NovaHashTablica(hashTablica, novi);
        }
    }

    fclose(fp);

    return 0;
}
int UmetniSortiranoUListu(PositionLD novi, PositionLD head)
{
    PositionLD temp = head;

    while (temp->next != NULL && strcmp(temp->imeD, novi->imeD) < 0)
    {
        temp=temp->next;
    }

    if(temp->next == NULL || strcmp(temp->next->imeD, novi->imeD) != 0)
    {
        UmetniNakon(temp, novi);
    }

    else if(strcmp(temp->next->imeD, novi->imeD) == 0)
    {
        return -1;
    }

    return EXIT_SUCCESS;

}

int UmetniNakon(PositionLD current, PositionLD novi)
{
    novi->next = current->next;
    current->next = novi;

    return 0;
}

PositionLD NovaLista()
{
    PositionLD novi = NULL;
    novi = (PositionLD)malloc(sizeof(ListaDrzava));

    novi->next = NULL;
    novi->rootGradova = NULL;
    strcpy(novi->imeD, "");

    return novi;

}

int PrintListe(PositionLD head)
{
    PositionLD q = head->next;

    while(q != NULL)
    {
        printf("\n %s \n", q->imeD);
        PrintStabloInorder(q->rootGradova);

        q = q->next;
    }

    return 0;
}

PositionSG CitajGradoveIzDatoteke(char* dat)
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
            novi = SortiraniUnosUStablo(imeGrada, br_stanovnika, novi);
        }
    }

    fclose(fp);

    return novi;
}

PositionSG SortiraniUnosUStablo(char* imeGrada, int brStan, PositionSG current)
{
    if(current == NULL)
    {
        current = (PositionSG)malloc(sizeof(StabloGradova));
        strcpy(current->imeG, imeGrada);
        current->br_stanovnika = brStan;
        current->L = NULL;
        current->R = NULL;
    }

    else if(current->br_stanovnika < brStan)
        current->R = SortiraniUnosUStablo(imeGrada, brStan, current->R);

    else if(current->br_stanovnika > brStan)
        current->L = SortiraniUnosUStablo(imeGrada, brStan, current->L);

    else if (brStan == current->br_stanovnika) // ako su isti
    {
        if(strcmp(current->imeG, imeGrada) < 0)
            current->R = SortiraniUnosUStablo(imeGrada, brStan, current->R);

        else if(strcmp(current->imeG, imeGrada) > 0)
            current->L = SortiraniUnosUStablo(imeGrada, brStan, current->L);

    }

    else // dva potpuno ista
        free(current);

    return current;

}

int PrintStabloInorder(PositionSG root)
{
    if(root == NULL)
        return 0;

    PrintStabloInorder(root->L);
    printf(" %s\t%d\n", root->imeG, root->br_stanovnika);
    PrintStabloInorder(root->R);

    return 0;
}

int GradoviSViseStanovnika(char* imeDrzave, int brStan, ListaDrzava hashTablica[])
{
    int kljuc = HashKljuc(imeDrzave);

    PositionLD UnesenaDrzava = PronadiDrzavu(imeDrzave, &hashTablica[kljuc]);

    int maxim = 0;

    if(UnesenaDrzava == NULL)
    {
        printf("Drzava nije pronadena!\n");
        return -1;
    }

    maxim = PronadiMax(UnesenaDrzava->rootGradova);
    if (brStan > maxim)
    {
        printf("Nisu pronadeni gradovi s vise stanovnika\n");
        return -2;
    }

    printf("\nGradovi s vise stanovnika: \n");
    PronadiGrad(brStan, UnesenaDrzava->rootGradova);

    return 0;
}

int HashKljuc(char* imeDrzave)
{
    int suma = 0;
    suma = imeDrzave[0] + imeDrzave[1] + imeDrzave[2] + imeDrzave[3] + imeDrzave[4];
    return suma%11;
}

int NovaHashTablica(ListaDrzava hashTablica[], PositionLD novi)
{
    int kljuc = HashKljuc(novi->imeD);

    UmetniSortiranoUListu(novi, &hashTablica[kljuc]);

    return 0;
}

int PronadiMax(PositionSG current)
{
    int maxim = 0;

    if (current == NULL)
        return 0;

    if (current->R == NULL)
        return current->br_stanovnika;

    maxim = PronadiMax(current->R);

    return maxim;
}

PositionLD PronadiDrzavu(char* imeDrzave, PositionLD head)
{
    PositionLD temp = head;

    while(temp->next!=NULL && strcmp(temp->next->imeD,imeDrzave) != 0)
        temp=temp->next;

    return temp->next;
}

int PronadiGrad(PositionSG current, int brojStan)
{
    if(current == NULL)
        return 0;

    PronadiGrad(current->L, brojStan);
    if(current->br_stanovnika >= brojStan)
        printf("Grad: %s, broj stanovnika: %d", current->imeG, current->br_stanovnika);

    PronadiGrad(current->R, brojStan);

    return 0;
}
