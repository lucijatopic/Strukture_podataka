/*Napisati program koji pomoću vezanih listi (stabala) predstavlja strukturu direktorija.
Omogućiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Točnije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz*/#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 50

/*
   md- create directory
   cd dir- change directory
   cd- change to parent
   dir- print chosen directory
*/

struct _Directory;
typedef struct _Directory* PositionD;
typedef struct _Directory {
	char* name;
	PositionD child;
	PositionD sibling;
}Directory;

struct _Cvor;
typedef struct _Cvor* PositionC;
typedef struct _Cvor {
	PositionD directory;
	PositionC next;
	PositionC prev;
}Cvor;

int MakeDirectory(PositionD);
int DDirectory(PositionD);
int Meni(PositionD, PositionC);
PositionD ChangeDirectory(PositionD, PositionC);
PositionD GoBack(PositionD, PositionC);
PositionD FindByName(char*, PositionD);
PositionD Pop(PositionC);
int Push(PositionC, PositionD);
int Line(PositionC, PositionD, PositionD);
PositionC FindLast(PositionC);


int main()
{
	PositionD root;
	root = (PositionD)malloc(sizeof(Directory));

	root->child = NULL;
	root->sibling = NULL;

	PositionC head;
	head = (PositionC)malloc(sizeof(Cvor));

	head->next = NULL;
	head->prev = NULL;

	root->name = (char*)malloc(MAX_SIZE * sizeof(char));
	root->name = "C:";

	int s = 1;

	while (s)
	{
		s = Meni(root, head);

	}

	return 0;
}

int Line(PositionC head, PositionD root, PositionD current) // VRATI SE NA OVO!!!!!
{
	PositionC a;

	a = FindLast(head);

	if (head->next == NULL)
	{
		printf("%s>", root->name);
		return EXIT_SUCCESS;
	}

	while (a->prev != NULL)
	{
		printf("%s>", a->directory->name);
		a = a->prev;
	}

	printf("%s>", current->name);

	return EXIT_SUCCESS;
}

PositionC FindLast(PositionC head)
{
	PositionC a;
	a = head->next;

	if (a == NULL)
	{
		return NULL; // ako je iduci null to je to nasli smo zadnji
	}

	while (a->next != NULL)
	{
		a = a->next;
	}

	return a;  // vracamo poziciju zadnjeg
}

int Meni(PositionD root, PositionC head)
{
	PositionD current = root;
	char choice[MAX_SIZE] = {0};

	printf("md - add new directory\n");
	printf("cd - open directory\n");
	printf("cd.. - go back to the previous directory\n");
	printf("dir - print all directories in the current directory\n");
	printf("exit - exit the program\n");

	while (1)
	{
		Line(head, root, current);
		scanf("%s", choice);

		if (!strcmp(choice, "md")) //ako se podudaraju znaci da radimo direktorij novi
		{
			MakeDirectory(current);
		}

		else if (!strcmp(choice, "cd"))  // ako se podudaraju mijenjamo direktorij
		{
			current = ChangeDirectory(current, head);
		}

		else if (!strcmp(choice, "cd.."))  // ako se podudaraju vracamo se u prijasnji direktorij
		{
			current = GoBack(current, head);
		}

		else if (!strcmp(choice, "dir"))  // ako se podudaraju brisemo direktorij u kojem smo sada
		{
			DDirectory(current);
		}

		else if (!strcmp(choice, "exit"))  // ako se podudaraju izlazimo iz programa
		{
			system("cls");
			return 0;
		}

		else
			printf("404 error");

	}

	return EXIT_SUCCESS;

}

int MakeDirectory(PositionD current)
{
	PositionD dire = NULL;
	dire = (PositionD)malloc(sizeof(Directory));

	dire->name = (char*)malloc(MAX_SIZE * sizeof(char));

	if (!dire)
		printf("Unable to allocate memory!\n");

	if (current->child != NULL)  // ako ima "dite"
	{
		current = current->child;

		while (current->sibling != NULL) // ako ima "bracu"
		{
			current = current->sibling;
		}

		current->sibling = dire; // idemo od kraja ako to ima smisla
	}

	else
		current->child = dire;

	scanf(" %s", dire->name);

	dire->sibling = NULL;  // U tom direktoriju smo!
	dire->child = NULL;

	return EXIT_SUCCESS;
}

PositionD ChangeDirectory(PositionD current, PositionC head)
{
	PositionD a;
	char* name;

	name = (char*)malloc(MAX_SIZE * sizeof(char));

	scanf(" %s", name);

	if (current->child == NULL) // ako nema "dice"
	{
		printf("There are no directories that you can open!\n");
		return current; // vraca nas na ono na cemu vec jesmo!
	}

	a = FindByName(name, current);

	if (a == 0)
	{
		printf("Directory can not be found!\n");
		return current;
	}

	Push(head, current);

	return a;

}

PositionD FindByName(char* name, PositionD current)
{
	if (current->child == NULL)
	{
		printf("This directory is empty!\n");
		return EXIT_SUCCESS;
	 }

	current = current->child;

	while (strcmp(name, current->name) != 0 && current->sibling != NULL)
	{
		current = current->sibling; // idemo dalje
	}

	if (current->sibling == NULL)
	{
		if (strcmp(name, current->name) != 0)
			return NULL;  // nema tog imena
		else
			return current;
	}

	else
		return current;
}

int Push(PositionC head, PositionD dire)
{
	PositionC temp = (PositionC)malloc(sizeof(Cvor));

	temp->next = head->next;
	temp->prev = head;

	if (head->next != NULL)
	{
		head->next->prev = temp;
	}

	head->next = temp;
	temp->directory = dire;

	return EXIT_SUCCESS;
}

PositionD GoBack(PositionD current, PositionC head)
{
	PositionD a;
	
	a = Pop(head);

	if (a == 0)
	{
		printf("Can't exit!\n");
		a = current;
	}
	
	return a;

}

PositionD Pop(PositionC head)
{
	PositionC temp = (PositionC)malloc(sizeof(Cvor));
	PositionD q;

	if (head->next == NULL)
		return NULL;

	temp = head->next;  // radimo sa vezanin listama
	q = temp->directory; // ali im dodamo i direktorij, amo rec jos jednu dimenziju

	head->next = head->next->next;

	if (head->next != NULL)
		head->next->prev = head;

	free(temp);

	return q;
}

int DDirectory(PositionD current)
{
	if (current->child == NULL)
	{
		printf("This directory is empty!\n");
		return EXIT_SUCCESS;
	}
	current = current->child;

	printf("Directories: \n");
	printf("\t %s \n", current->name);

	while (current->sibling != NULL)
	{
		printf("\t %s \n", current->sibling->name);
		current = current->sibling;
	}

	return EXIT_SUCCESS;
}