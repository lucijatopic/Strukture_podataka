#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 50

struct _Person;
typedef struct _Person* Position;
typedef struct _Person
{
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    int birthYear;
    Position next;
} Person;

int PrependList(Position head, char* name, char* surname, int birthYear); //napunit listu
int AppendList(Position head, char* name, char* surname, int birthYear);  //dodat u
int PrintList(Position first);
Position CreatePerson(char* name, char* surname, int birthYear);
int InsertAfter(Position position, Position newPerson);
Position FindLast(Position head);
int FindBySurname(Position head, char* surname);
Position FindBefore(Position first);
int Delete(Position head, char* surname);

int main(int argc, char** argv)
{

    Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
    Position p = &Head;

    Position pom=NULL;
    pom=(Position)malloc(sizeof(Person));

    int odabir=1;

    char name[MAX_SIZE]= {' '};
    char surname[MAX_SIZE]= {' '};

    char trazenoprezime[MAX_SIZE]={' '};

    int birthYear=0;

    while(odabir!=0)
    {

        PrintList(p);

        printf("Odaberite sto zelite napraviti:\n");
        printf("1. Dodavanje osobe na pocetak \n");
        printf("2. Dodavanje osobe na kraj\n");
        printf("3. Pronaci osobu po prezimenu\n");
        printf("4. Izbrisati osobu\n");
        printf("\nVas odabir: ");
        scanf(" %d", &odabir);


        switch(odabir)
        {

        case 1:
            printf("Unesite ime i prezime osobe koju zelite nadodati na pocetak liste: ");
            scanf(" %s %s", name, surname);
            printf("Godina rodenja osobe koju unosite: ");
            scanf("%d", &birthYear);
            PrependList(p, name, surname, birthYear);
            break;

        case 2:
            printf("Unesite ime i prezime osobe koju zelite nadodati na kraj liste: ");
            scanf(" %s %s", name, surname);
            printf("Godina rodenja osobe koju unosite: ");
            scanf("%d", &birthYear);
            AppendList(p, name, surname, birthYear);
            break;

        case 3:
            printf("Unesite prezime osobe koju zelite pronaci u listi: ");
            scanf(" %s", trazenoprezime);
            pom=FindBySurname(p, trazenoprezime);
            printf("\nTrazena osoba je: %s %s, %d", pom->name, pom->surname, pom->birthYear);
            break;

        case 4:
            printf("Unesite prezime osobe koju zelite izbrisati iz liste: ");
            scanf(" %s", trazenoprezime);
            Delete(p, trazenoprezime);
            break;

        }
    }

    if(odabir<1 || odabir>4)
        printf("Ne postojeca vrijednost odabira!");

    return EXIT_SUCCESS;
}

int PrependList(Position head, char* name, char* surname, int birthYear)
{

    Position newPerson = NULL;

    newPerson = CreatePerson(name, surname, birthYear);
    if (!newPerson)
    {
        perror("Can't allocate memory!\n");
        return -1;
    }

    InsertAfter(head, newPerson); // umetni nakon head-a novu osobu, puni listu, 95. linija

    return EXIT_SUCCESS;
}

int AppendList(Position head, char* name, char* surname, int birthYear) // na kraj dodajemo novu osobu, UnosK na predavanjima
{
    Position newPerson = NULL;
    Position last = NULL;

    newPerson = CreatePerson(name, surname, birthYear);
    if (!newPerson)
    {
        perror("Can't allocate memory!\n");
        return -1;
    }

    last = FindLast(head); // 103. linija
    InsertAfter(last, newPerson);

    return EXIT_SUCCESS;
}

int PrintList(Position first)
{
    Position temp = first;

    while (temp)   // dok nije kraj liste, odnosno dok nije NULL, ispisuj ovo
    {
        printf("Name: %s, surname: %s, birthyear: %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;  // ovde se mice
    }

    return EXIT_SUCCESS;
}

Position CreatePerson(char* name, char* surname, int birthYear)  // Position je jer vraca adresu nove osobe
{
    Position newPerson = NULL;  // newPerson je kao q na predavanjima + postavljen na NULL

    newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson)
    {
        perror("Can't allocate memory!\n");
        return -1;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;

    return newPerson;
}

int InsertAfter(Position position, Position newPerson)
{
    newPerson->next = position->next;
    position->next = newPerson;

    return EXIT_SUCCESS;
}

Position FindLast(Position head)
{
    Position temp = head;

    while (temp->next!=NULL)     //sve dok nije kraj vezane liste ajde dalje i taj zadnji vrati
    {
        temp=temp->next;
    }

    return temp;
}

int FindBySurname(Position first, char* surname)
{
    Position temp = first->next;
    while (temp)   // dok nije kraj
    {
        if (strcmp(temp->surname, surname) == 0)
        {
            printf("Osoba koju smo trazili: %s %s %d", temp->name, temp->surname, temp->birthYear);
            return temp;
        }
        temp=temp->next;

        return NULL; // ako nema tog prezimena vraca NULL

        //Funkcija mi ne ispisuje osobu koju smo trazili......
    }

}

Position FindBefore(Position first) //pronadi prije odredene lokacije
{
    Position temp= first->next;

    while(temp)
    {
        temp=first->next;
        if(temp==NULL)
            return NULL;
        else
            return first;
    }

    return temp;

}
int Delete(Position head, char* surname) //izbrisati odredeni element
{
    Position temp=head->next;
    head=FindBefore(temp);

    if(head==NULL)
        printf("Greska!");

    else
    {
        temp=head->next;
        head->next=temp->next;
        free(temp);
    }
    return 0;
}
