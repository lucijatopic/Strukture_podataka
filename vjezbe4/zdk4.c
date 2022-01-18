#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_LINE 1024

struct _Polinomi;
typedef struct _Polinomi* Position;
typedef struct _Polinomi
{
    int koeficijent;
    int eksponent;
    Position next;

} Polinomi;

int CitajPolinomIzDatoteke(char* dat, Position head1, Position head2);
int ParseStringToList(char* buffer, Position head);
Position StvoriNoviElement(int koef, int ekspo);
int UnesiSortiranoUListu(Position novi, Position head);
int UmetniNakon(Position current, Position novi);
int IzbrisiNakon(Position prethodni);
int ZbrajanjePolinoma(Position head1, Position head2, Position rezultatZbrajanja);
int MnozenjePolinoma(Position head1, Position head2, Position rezultatMnozenja);
void PrintPolinoma(Position head);

int main()
{
    Polinomi head1 = {.koeficijent = 0, .eksponent = 0, .next = NULL};
    Polinomi head2 = {.koeficijent = 0, .eksponent = 0, .next = NULL};
    Polinomi rezZbrajanja = {.koeficijent = 0, .eksponent = 0, .next = NULL};
    Polinomi rezMnozenja = {.koeficijent = 0, .eksponent = 0, .next = NULL};

    Position pos1 = &head1;
    Position pos2 = &head2;
    Position rezZbr = &rezZbrajanja;
    Position rezMno = &rezMnozenja;

    if(CitajPolinomIzDatoteke("Polinomi.txt", pos1, pos2) == EXIT_SUCCESS)
    {

        printf("Prvi polinom: ");
        PrintPolinoma(pos1);

        printf("\nDrugi polinom: ");
        PrintPolinoma(pos2);

        if(ZbrajanjePolinoma(pos1, pos2, rezZbr) == EXIT_SUCCESS)

        {
            printf("\nRezultat zbrajanja: ");
            PrintPolinoma(rezZbr);

        }

        if(MnozenjePolinoma(pos1, pos2, rezMno) == EXIT_SUCCESS)
        {
            printf("\nRezultat mnozenja: ");
            PrintPolinoma(rezMno);
        }


    }
    return EXIT_SUCCESS;
}

int CitajPolinomIzDatoteke(char* dat, Position head1, Position head2)
{
    char buffer[MAX_LINE] = {0};
    int status = 0;

    FILE* fp;
    fp = fopen(dat, "r");

    if(!fp)
    {
        printf("Nemoguce otvoriti datoteku!\n");
        return -1;
    }

    //za prvi polinom

    fgets(buffer, MAX_LINE, fp);

    status = ParseStringToList(buffer, head1);

    if(status != EXIT_SUCCESS)
        return EXIT_FAILURE;

    //za drugi polinom

    fgets(buffer, MAX_LINE, fp);

    status = ParseStringToList(buffer, head2);

    if(status != EXIT_SUCCESS)
        return EXIT_FAILURE;

    fclose(fp);
    return EXIT_SUCCESS;
}

//!!!
int ParseStringToList(char* buffer, Position head)
{
    char* currentBuffer = buffer;
    int status = 0;
    int koef = 0;
    int eksp = 0;
    int brB = 0;

    Position novi = NULL;

    while(strlen(currentBuffer)>0)
    {
        status = sscanf(currentBuffer, "%d %d %n", &koef, &eksp, &brB);

        if(status != 2)
        {
            printf("Nepravilan format datoteke!\n");
            return EXIT_FAILURE;
        }

        novi = StvoriNoviElement(koef, eksp);

        if(!novi)
        {
            return EXIT_FAILURE;
        }

        UnesiSortiranoUListu(novi, head);

        currentBuffer += brB; //za toliko mijesta se micemo
    }

    return EXIT_SUCCESS;
}

Position StvoriNoviElement(int koef, int ekspo)
{
    Position noviEl = NULL;
    noviEl = (Position)malloc(sizeof(Polinomi));

    if(!noviEl)
    {
        printf("Nemoguce alocirati memoriju!\n");
        return noviEl;
    }

    noviEl->eksponent = ekspo;
    noviEl->koeficijent = koef;
    noviEl->next = NULL;

    return noviEl;

}

int UnesiSortiranoUListu(Position novi, Position head)
{
    Position temp = head;
    if(novi->koeficijent != 0)
    {
        if(temp->next == NULL) // nema jos nicega u listi
            return UmetniNakon(temp, novi);

        while(temp != NULL)
        {
            if(temp->next->eksponent == novi->eksponent)
            {
                if(temp->next->koeficijent == -(novi->koeficijent)) // zbroj im je 0, pa jednog brisemo
                    return IzbrisiNakon(temp);

                temp->next->koeficijent += novi->koeficijent;

                return EXIT_SUCCESS;

            }

            else if(temp->next->eksponent < novi->eksponent)
                return UmetniNakon(temp, novi);

            else if((temp->next->next==NULL) && (temp->next->eksponent > novi->eksponent))
                return UmetniNakon(temp->next, novi);

            temp = temp->next;
        }
    }

    return EXIT_SUCCESS;
}

int UmetniNakon(Position current, Position novi)
{
    novi->next = current->next;
    current->next = novi;

    return EXIT_SUCCESS;

}

int IzbrisiNakon(Position prethodni)
{
    Position temp = NULL;

    temp = prethodni->next;
    prethodni->next = prethodni->next->next;

    free(temp);

    return EXIT_SUCCESS;
}

int ZbrajanjePolinoma(Position head1, Position head2, Position rezultatZbrajanja)
{
    Position novi = NULL;

    while(head1->next)
    {
        novi = StvoriNoviElement(head1->koeficijent, head1->eksponent);
        if(!novi)
        {
            return EXIT_FAILURE;
        }

        UnesiSortiranoUListu(rezultatZbrajanja, novi); // ona zapravo obavlja zbrajanje

        head1 = head1->next;
    }

    while(head2->next)
    {
        novi = StvoriNoviElement(head2->koeficijent, head2->eksponent);
        if(!novi)
        {
            return EXIT_FAILURE;
        }

        UnesiSortiranoUListu(rezultatZbrajanja, novi);

        head2 = head2->next;
    }

    return EXIT_SUCCESS;
}

int MnozenjePolinoma(Position head1, Position head2, Position rezultatMnozenja)
{
    Position i = NULL;
    Position j = NULL;

    Position novi = NULL;

    if(head1->next == NULL || head2->next == NULL)
        return EXIT_SUCCESS;

    for(i = head1->next; i != NULL; i = i->next)
        for(j = head2->next; j != NULL; j = j->next)
        {
            novi = StvoriNoviElement((i->koeficijent)*(j->koeficijent), (i->eksponent)+(j->eksponent));
            if(!novi)
            {
                return EXIT_FAILURE;
            }

            UnesiSortiranoUListu(rezultatMnozenja, novi);
        }

    return EXIT_SUCCESS;
}

void PrintPolinoma(Position head)
{
    Position i = NULL;

    if(head->next == NULL)
        printf(" 0 ");

    for(i = head->next; i != NULL; i = i->next)
    {
        printf("(%d)x^(%d)", i->koeficijent, i->eksponent);
        if(i->next != NULL)
            printf(" + ");
    }
    printf("\n");
}
