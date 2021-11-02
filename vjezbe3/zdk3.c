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
int PrintList(Position head);
Position CreatePerson(char* name, char* surname, int birthYear);
int InsertAfter(Position position, Position newPerson);
Position FindLast(Position head);
int FindBySurname(Position head, char* surname);
Position FindBefore(Position first);
Position FindPrevious(Position head, Position newPerson);
int Delete(Position head, char* surname);
int Delete2(Position head, Position del);
int SortListBySurname(Position head);
int Write(Position head);
int Read(Position head);


int main(int argc, char** argv)
{

    Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
    Position p = &Head;

    int odabir=1;

    char name[MAX_SIZE]= {' '};
    char surname[MAX_SIZE]= {' '};
    char trazenoprezime[MAX_SIZE]= {' '};

    int birthYear=0;

    Position pom=NULL;
    pom=(Position)malloc(sizeof(Person));

    while(odabir!=0) //kad je nula izlazimo iz programa
    {

        PrintList(p);

        printf("Odaberite sto zelite napraviti:\n");
        printf("1. Dodavanje osobe na pocetak \n");
        printf("2. Dodavanje osobe na kraj\n");
        printf("3. Pronaci osobu po prezimenu\n");
        printf("4. Izbrisati osobu\n");
        printf("5. Dodati novu osobu iza odredene osobe\n");
        printf("6. Dodati novu osobu ispred odredene osobe\n");
        printf("7. Sortiranje liste po prezienima osoba\n");
        printf("8. Upisati listu u datoteku\n");
        printf("9. Procitati listu iz datoteke\n");

        printf("\nVas odabir je: ");
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
            printf("\nTrazena osoba je: %s %s, &d", pom->name, pom->surname, pom->birthYear);
            break;

        case 4:
            printf("Unesite prezime osobe koju zelite izbrisati iz liste: ");
            scanf(" %s", trazenoprezime);
            pom=FindBySurname(p, trazenoprezime);
            Delete2(p, pom);
            break;

        case 5:
            printf("Unesite prezime osobe iza koje zelite nadodati novu osobe: ");
            scanf(" %s", trazenoprezime);
            printf("\nIme i prezime nove osobe koj nadodajete: ");
            scanf(" %s %s", name, surname);
            printf("\nGodina rodenja osobe koju nadodajete: ");
            scanf("%d", &birthYear);
            pom=FindBySurname(p, trazenoprezime);
            InsertAfter(pom, CreatePerson(name, surname, birthYear));
            break;

        case 6:
            printf("Unesite prezime osobe ispred koje zelite nadodati novu osobe: ");
            scanf(" %s", trazenoprezime);
            pom=FindBySurname(p, trazenoprezime);
            printf("\nIme i prezime nove osobe koj nadodajete: ");
            scanf(" %s %s", name, surname);
            printf("\nGodina rodenja osobe koju nadodajete: ");
            scanf(" %d", &birthYear);

            pom=FindPrevious(p, pom);
            InsertAfter(pom, CreatePerson(name, surname, birthYear));
            break;

        case 7:
            SortListBySurname(p);
            break;

        case 8:
            Write(p);
            break;

        case 9:
            Read(p);
            break;
        }
    }

    if(odabir<1 || odabir>9)
        printf("Ne postojeca vrijednost odabira!");

    return EXIT_SUCCESS;
}

int PrependList(Position head, char* name, char* surname, int birthYear)
{

    Position newPerson = NULL;

    newPerson = CreatePerson(name, surname, birthYear);
    if (!newPerson)
    {
        perror("Nemoguce alocirati memoriju!\n");
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
        perror("Nemoguce alocirati memoriju!\n");
        return -1;
    }

    last = FindLast(head); // 103. linija
    InsertAfter(last, newPerson);

    return EXIT_SUCCESS;
}

int PrintList(Position head)
{
    Position temp = head->next;

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
        perror("Nemoguce alocirati memoriju!\n");
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

    while (temp->next)     //sve dok nije kraj vezane liste ajde dalje i taj zadnji vrati
    {
        temp=temp->next;
    }

    return temp;
}

int FindBySurname(Position first, char* surname)
{
    Position temp = first->next;
   /* while (temp)   // dok nije kraj
    {
        if (strcmp(temp->surname, surname) == 0)
        {
            printf("Osoba koju smo trazili: %s %s %d", temp->name, temp->surname, temp->birthYear);
            return temp;
        }
        temp=temp->next;

        return NULL; // ako nema tog prezimena vraca NULL
    }*/

    while(temp!=NULL && strcmp(temp->surname, surname))
        temp=temp->next;

    return temp;

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
/*.........*/
Position FindPrevious(Position head, Position newPerson)
{
    Position pom=head;

    while(pom!=NULL && pom->next != newPerson)
        pom=pom->next;

    return pom;

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
    //zasto moram dva puta napisat prezime?????
    return 0;
}
/*.......*/
int Delete2(Position head, Position del)
{
    Position temp=NULL;
    Position pom=NULL;

    temp=(Position)malloc(sizeof(Person));
    pom=(Position)malloc(sizeof(Person));

    pom=del;
    temp=FindPrevious(head, del); //pronademo elem prije one koje zelimo izbrisat(del), i pvezemo to sa temp te oslobodimo pom
    temp->next=pom->next;

    free(pom);

    return EXIT_SUCCESS;
}
int SortListBySurname(Position head)
{
    Position temp = head;
    Position x = NULL;
    Position y = NULL;
    Position last = NULL;  // zadnja

    while(temp->next != last)  // za prvo je da je razlicito od NULL
    {
        y = temp;
        x = y->next;

        while(x->next != last)
        {
            if(strcmp(x->surname, x->next->surname)>0)  // idemo uzlazno sortiranje
            {
                y->next = x->next;
                x->next = x->next->next;  // cila zamjena! nacrtano u teci
                y->next->next = x;
                x = y->next;
            }

            y = x;
            x = x->next;
        }
        last = x;

    }
    return EXIT_SUCCESS;
}
int Write(Position head)  // upisujemo listu u datoteku
{
    Position temp = NULL;
    temp = (Position)malloc(sizeof(Person));
    temp = head->next; // prva stvarna osoba

    FILE* fp = NULL;
    fp = fopen("osobe.txt", "w");

    if(fp==NULL)
    {
        printf("Datoteka je prazna!\n");
        return -1;
    }

    while(temp!=NULL)// ide do kraja liste
    {
        fprintf(fp, "%s\t%s\t%d\n", temp->name, temp->surname, temp->birthYear);
        temp=temp->next;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
int Read(Position head)
{
    char buf[1024]= {0}; // buffer je kao temporary storage!
    int br=0;
    int i;
    Position temp=NULL;
    Position pom=FindLast(head);

    FILE* fp= NULL;
    fp = fopen("osobe.txt", "r");

    if(fp==NULL)
    {
        printf("Datoteka je prazna!\n");
        return -1;
    }

    while(fgets(buf, 1024, fp))
        {br++;}

    rewind(fp); //sets the file position to the beginning of the file

    for(i=0; i<br; i++)
    {
        temp=(Position)malloc(sizeof(Person)); //sad svaku osobu citamo iz datoteke i stvaramo vezanu listu

        temp->next=NULL;
        pom->next=temp;
        pom=pom->next;

        fscanf(fp, " %s %s %d", temp->name, temp->surname, &temp->birthYear);

    }

    for(i=0; i<br; i++)
    {
        printf("%s %s %d", temp->name, temp->surname, temp->birthYear);
    }

    puts("");
    fclose(fp);

    return EXIT_SUCCESS;

}

