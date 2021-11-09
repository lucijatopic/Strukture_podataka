#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define MAX_LINE 1024
#define MAX_SIZE 50

struct _Polynom;
typedef struct _Polynom* Position;
typedef struct _Polynom
{
    int coefficient;
    int exponent;
    Position next;
}Polynom;

int TypeInFileName(char* FileNme);
int ReadFile(Position head1, Position head2, char* FileNme);
int ParseStringToList(Position head, char* buffer);
Position CreateElement(int coefficient, int exponent);
int InsertSorted(Position head, Position NewElement);
int MergeAfter(Position position, Position NewElement);
int InsertAfter(Position position, Position NewElement);
int DeleteAfter(Position previous);
int CreateAndInsertAfter(int coeff, int expo, Position position);
int AddPolynom1(Position resultHead, Position head1, Position head2);
int AddPolynom2(Position resultHead, Position head1, Position head2);
int MultiplyPolynom(Position resultHead, Position head1, Position head2);
int FreeMemory(Position head);
int Print(char* name, Position first);

int main(int argc, char** argv)
{
    Polynom head1={.coefficient=0, .exponent=0, .next=NULL}; //prvi polinom
    Polynom head2={.coefficient=0, .exponent=0, .next=NULL}; //drugi polinom

    Polynom ResultAdd={.coefficient=0, .exponent=0, .next=NULL}; // struct za rezultat zbrajanja
    Polynom ResultMult={.coefficient=0, .exponent=0, .next=NULL}; // struct za rezultat mnozenja

    char filename[MAX_SIZE]={0};

    TypeInFileNmae(filename);

    if(ReadFile(&head1, &head2, filename) == EXIT_SUCCESS)
    {
        Print("p1", head1.next);
        Print("p2", head2.next);

        AddPolynom1(&ResultAdd, &head1, &head2);
        MultiplyPolynom(&ResultMult, &head1, &head2);

        Print("pAdd", ResultAdd.next);
        Print("pMulti", ResultMult.next);

        FreeMemory(&head1);
        FreeMemory(&head2);
        FreeMemory(&ResultAdd);
        FreeMemory(&ResultMult);

    }

    return EXIT_SUCCESS;
}

int TypeInFileName(char* FileNme)
{
    printf("Unesite ime datoteke:\n");
    scanf(" %s", FileNme);

    return EXIT_SUCCESS;
}

int ReadFile(Position head1, Position head2, char* FileNme)
{
    FILE* fp=NULL;
    char buffer[MAX_LINE]={0};
    int status=EXIT_SUCCESS;

    fp=fopen(FileNme, "r");

    if(fp==NULL)
    {
        perror("Nemoguce otvoriti datoteku!\n");
        return -1;
    }

    fgets(buffer, MAX_LINE, fp);
    status=ParseStringToList(head1, buffer);

    if(status!=EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    fgets(buffer, MAX_LINE, fp); // 2 puta, dva reda
    status=ParseStringToList(head2, buffer);

    if(status!=EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

int ParseStringToList(Position head, char* buffer)
{
    char* currentBuffer= buffer;
    int coefficient=0;
    int exponent=0;
    int nBytes=0;
    int status=0;
    Position NewElement=NULL;

    while(strlen(currentBuffer)>0)
    {
        status = sscanf(currentBuffer, " %d %d %n", &coefficient, &exponent, &nBytes); // u datoteci, koef i ekspo
      
        if(status != 2)                                                                // su poslozeni u parovima
        {
            printf("Datoteka nije dobra!\n");
            return EXIT_FAILURE;
        }

        NewElement = CreateElement(coefficient, exponent);

        if(NewElement == NULL)
        {
            return EXIT_FAILURE;
        }

        InsertSorted(head, NewElement);

        currentBuffer+= nBytes;  // ZAR NEBI TREBALO BIT -= AKO IDE WHILE STRLEN>0 ???????????????????????!!!!!!!!!!!!!!!!
    }

    return EXIT_SUCCESS;

}

Position CreateElement(int coefficient, int exponent)
{
    Position Element = NULL;

    Element = (Position)malloc(sizeof(Polynom));

    if(Element == NULL)
    {
        perror("Nemoguce alocirati memoriju!\n");
        return Element; // vraca NULL onda
    }

    Element->coefficient=coefficient;
    Element->exponent=exponent;
    Element->next=NULL;

    return Element;
}

int InsertSorted(Position head, Position NewElement)  // NE KUZIN BAS!!!!!!!!!!!!!!1!!!!
{
    Position temp = head;

    while(temp->next != NULL && temp->next->exponent < NewElement->exponent)
    {
        temp = temp->next;
    }

    MergeAfter(temp, NewElement);

    return EXIT_SUCCESS;
}

int MergeAfter(Position position, Position NewElement) //sjediniti (npr. ako imaju isit eksponent ili tipa + i -)
{
    if(position->next == NULL || position->next->exponent != NewElement->exponent)  // ako novi element nema isti eksponent kao i postojeci elemenet
    {
        InsertAfter(position, NewElement);
    }
    else  // imaju novi element i postojeci element isti koeficijent, pretvaramo ih u jedan element
    {
        int resultCoeff = position->next->coefficient + NewElement->coefficient;
       
        if(resultCoeff==0) // zbroj je nula pa element nestaje
        {
            DeleteAfter(position);
        }
        else
        {
            position->next->coefficient = resultCoeff;
        }

        free(NewElement);
    }

    return EXIT_SUCCESS;
}

int InsertAfter(Position position, Position NewElement)
{
    NewElement->next=position->next;
    position->next=NewElement;

    return EXIT_SUCCESS;
}

int DeleteAfter(Position previous)
{
    Position Del=NULL;

    Del=previous->next;
    previous->next=Del->next;

    free(Del);

    return EXIT_SUCCESS;
}

int AddPolynom1(Position resultHead, Position head1, Position head2)
{
    Position a= head1->next;  // zasto ide od next a ne od head? (uvik tako), ipak logicno nvm!
    Position b= head2->next;
    Position c= resultHead;  // ova opcija stvara novu listu
    Position temp= NULL;

    while(a != NULL && b != NULL)
    {
        if(a->exponent == b->exponent)
        {
            CreateAndInsertAfter(a->coefficient + b->coefficient, a->coefficient, c);
            a = a->next;
            b = b->next;
        }

        else if(a->exponent < b->exponent)
        {
            CreateAndInsertAfter(a->coefficient, a->exponent, c);
            a = a->next;
        }
        
        else
        {
            CreateAndInsertAfter(b->coefficient, b->exponent, c);
            b = b->next;
        }
    }

    if(a == NULL)
    {
        temp=b;
    }

    else
    {
        temp=a;
    }

   while(temp!= NULL)
   {
       CreateAndInsertAfter(temp->coefficient, temp->exponent, c);
       temp=temp->next;
   }
    
    return EXIT_SUCCESS;

}

int AddPolynom2(Position resultHead, Position head1, Position head2)
{
    Position i = NULL;
    Position j = NULL;

    for(i = head1->next; i != NULL; i = i->next)
    {
        Position newEl = CreateElement(i->coefficient, i->exponent);
        if(!newEl)
        {
            return EXIT_FAILURE;
        }

        InsertSorted(resultHead, newEl);
    }

    for(j = head2->next; j != NULL; j = j->next)
    {
        Position newEl = CreateElement(j->coefficient, j->exponent);
        if(!newEl)
        {
            return EXIT_FAILURE;
        }

        InsertSorted(resultHead, newEl);
    }

    return EXIT_SUCCESS;
}

int CreateAndInsertAfter(int coeff, int expo, Position position)
{
    Position NewElement = CreateElement(coeff, expo);

    if(NewElement==NULL)
    {
        return EXIT_FAILURE;
    }

    InsertAfter(position, NewElement);
    position=position->next;

    return EXIT_SUCCESS;
}

int MultiplyPolynom(Position resultHead, Position head1, Position head2)
{
    Position i = NULL;
    Position j = NULL;

    for(i = head1->next; i != NULL; i = i->next) // ide po jednom i po drugom
    {
        for(j = head2->next; j != NULL; j = j->next)
        {
            Position newEl= CreateElement(i->coefficient * j->coefficient, i->exponent + j->exponent);
            if(!newEl)
            {
                return EXIT_FAILURE;
            }

           InsertSorted(resultHead, newEl); 
        }
    }

    return EXIT_SUCCESS;
}

int Print(char* name, Position first)
{
    printf(" %s = ", name);
    if(first)
    {
        if(first->exponent < 0)
        {
            if(first->coefficient == 1)
            {
                printf("x^(%d)", first->exponent);
            }
            else
            {
                printf("%dx^(%d)", first->coefficient, first->exponent);
            }
        }
        else
        {
            if(first->coefficient == 1)
            {
                printf("x^%d", first->exponent);
        
            }
            else
            {
                printf("%dx^%d", first->coefficient, first->exponent);
            }

        }

        first = first->next;
    }

    for(; first!=NULL; first = first->next)
    {
        if(first->coefficient <0)
        {
            if(first->exponent < 0)
            {
                printf("- %dx^(%d)", abs(first->coefficient), first->exponent);
            }
            else
            {
                printf("- %dx^%d", abs(first->coefficient), first->exponent);
            }
        }
        else
        {
            if(first->exponent <0)
            {
                    if(first->coefficient == 1)
                    {
                        printf("+ x^(%d)", first->exponent);
                    }
                    else
                    {
                        printf("+ %dx^(%d)", first->coefficient, first->exponent);

                    }
            }
            else
            {
                    if(first->coefficient == 1)
                    {
                        printf("+ x^%d", first->exponent);          
                    }
                    else
                    {
                        printf("+ %dx^%d", first->coefficient, first->exponent);
                    }
            
            }

        }
    }

    print("\n");
    return EXIT_SUCCESS;

}

int FreeMemory(Position head)
{
    Position temp = head;
    while(temp->next)
    {
        DeleteAfter(temp);
    }

    return EXIT_SUCCESS;
}