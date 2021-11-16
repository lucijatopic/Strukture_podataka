#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_SIZE 50

struct _StackElement;
typedef struct _StackElement* Position;
typedef struct _StackElement
{
    double number;
    Position next;
} StackElement;

Position CreateElement(double number);
int InsertAfter(Position position, Position newElement);
int Push(Position Head, double number);
int DeleteAfter(Position position);
int Pop(double* ResultDestination, Position Head);
int PerformOperation(Position Head, char operation);
int CalculatePostfixFromFile(double* Result, char* fileName);
int DeleteAll(Position Head);

int main(int argc, char** argv)
{
    char file[MAX_SIZE] = {0};
    double result;

    printf("Ime datoteke: ");
    scanf(" %s", file);

    if(CalculatePostfixFromFile(&result, file)!= EXIT_SUCCESS)
        return EXIT_FAILURE;

    printf("Result is: %lf", result);

    return EXIT_SUCCESS;
}

Position CreateElement(double number)
{
    Position newElement = NULL;
    newElement = (Position)malloc(sizeof(StackElement));

    if(!newElement)
    {
        perror("Can't allocate memory!");
        return NULL; // zasto NULL???
    }

    newElement->number = number;
    newElement->next = NULL;

    return newElement;
}

int InsertAfter(Position position, Position newElement)
{
    newElement->next = position->next;
    position->next = newElement;

    return EXIT_SUCCESS;
}

int Push(Position Head, double number) // slicno kao append u prvom zdk-u
{
    Position newE = NULL;
    newE = CreateElement(number);

    if(!newE)
    {
        return -1;
    }

    InsertAfter(Head, newE);

    return EXIT_SUCCESS;
}

int DeleteAfter(Position position)
{
    Position pom = position->next;

    if(!pom)
    {
        return EXIT_SUCCESS; // jer dobili smo odmah ono sto smo htjeli, ne trebamo cijelu funkciju
    }

    position->next = pom->next;
    free(pom);

    return EXIT_SUCCESS;
}

int Pop(double* ResultDestination, Position Head)
{
    Position first = Head -> next;

    if(!first)
    {
        perror("Postfix not valid! Please check your file!\n");
        return -1;
    }

    *ResultDestination = first->number; // ovdje treba zamisliti one knjige sta se izvlace
    DeleteAfter(Head);

    return EXIT_SUCCESS;
}

int PerformOperation(Position Head, char operation)
{
    double o1 = 0;
    double o2 = 0;

    double rezultat = 0;

    int status1 = EXIT_SUCCESS;
    int status2 = EXIT_SUCCESS;


    status2 = Pop(&o2, Head);  // kad skuzin pop bit ce mi jasno!!!!!!!!!!!
    if(status2 != EXIT_SUCCESS)
    {
        return -1;
    }

    status1 = Pop(&o1, Head);
    if(status1 != EXIT_SUCCESS)
    {
        return -2;
    }

    switch(operation)
    {
    case '+':
    {
        rezultat = o1 + o2;
        //printf("[+] -> %lf", rezultat);
        break;
    }
    case '-':
    {
        rezultat = o1 - o2;
        //printf("[-] -> %lf", rezultat);
        break;
    }
    case '*':
    {
        rezultat = o1 * o2;
        //printf("[*] -> %lf", rezultat);

        break;
    }
    case '/':
    {

        if(o2 == 0)
        {
            perror("Can't divide by zero!\n");
            return -3;
        }
        rezultat = o1 / o2;
        //printf("[/] -> %lf", rezultat);

        break;
    }
    default:
    {
        printf("This operation isn't supported!\n");
        DeleteAll(Head);
        return -4;
    }
    }

    Push(Head, rezultat);
    return EXIT_SUCCESS;

}

int CalculatePostfixFromFile(double* Result, char* fileName)
{
    FILE* fp = NULL;
    int fileLength = 0;
    char* buffer = NULL;
    char* currentBuffer = NULL;
    int status = 0;
    int number = 0;
    int brBytes = 0;
    char operation = 0;

    StackElement head = {.next=NULL, .number=0};

    fp = fopen(fileName, "rb");  // read binary!!!!!!
    if(!fp)
    {
        perror("Can't open file!\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);  // nadi kraj datoteke
    fileLength = ftell(fp);  // govori koliko smo prosli, kolika je duljina svega toga

    buffer = (char*)calloc(fileLength + 1, sizeof(char)); // ovaj + 1 zbog stringa i \0
    if(!buffer)
    {
        perror("Can't allocate memory!\n");
        return -2;
    }

    rewind(fp);
    fread(buffer, sizeof(char), fileLength, fp);  // procitaj mi u buffer, ovoliko (fileLength), ove velicine (sizeof(char)) iz file
    printf("|%s|\n", buffer);
    fclose(fp);

    currentBuffer = buffer;
    while(strlen(currentBuffer)>0) // ka i prosli zdk
    {
        status = sscanf(currentBuffer, "%lf %n", &number, &brBytes);
        if(status == 1)
        {
            Push(&head, number);
            currentBuffer += brBytes;
        }
        else
        {
            status = sscanf(currentBuffer, " %c %n", &operation, &brBytes);
            status = PerformOperation(&head, operation);
            if(status != EXIT_SUCCESS)
            {
                free(buffer);
                DeleteAll(&head);
                return -4;
            }
            currentBuffer += brBytes;
        }
    }

    free(buffer);

    status = Pop(Result, &head);
    if(status != EXIT_SUCCESS)
    {
        DeleteAll(&head);
        return -5;
    }

    if(head.next)
    {
        printf("Postfix not valid! Please check your file!\n");
        DeleteAll(&head);
        return -6;
    }

    return EXIT_SUCCESS;
}
int DeleteAll(Position Head)
{
    while(Head->next)
    {
        DeleteAfter(Head);
    }

    return EXIT_SUCCESS;
}
