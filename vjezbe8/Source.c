/*Napisati program koji omogucava rad s binarnim stablom pretrazivanja. Treba
omoguciti unosenje novog elementa u stablo, ispis elemenata(inorder, preorder, postorder i
level order), brisanje i pronalazenje nekog elementa.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct _stablo;
typedef struct _stablo* Position;
typedef struct _stablo {
	int El;
	Position L;
	Position R;
}stablo;

Position Umetni(Position current, int el);
int Print_Inorder(Position current);
int Print_Postorder(Position current);
int Print_Preorder(Position current);
int Print_Level_Order(Position current, int br_level);
Position Pronadi_Element(Position current, int el);
Position Izbrisi_Element(Position current, int el);
Position Pronadi_Min(Position current);


int main()
{
	Position root;
	root = NULL;

	Position q;
	q = NULL;

	int n, el;

	do
	{

		printf("Odabrite sto zelite napraviti:\n");
		printf("1. Dodavanje novog elementa u stablo\n"); 
		printf("2. Printanje elemenata (inorder)\n");
		printf("3. Printanje elemenata (postoreder)\n");
		printf("4. Printanje elemenata (preorder)\n");
		printf("5. Printanje elemenata (level order)\n");
		printf("6. Pronadi element\n");
		printf("7. Izbrisi element\n");
		printf("8. Izadi iz programa\n");

		scanf("%d", &n);
		system("cls");

		switch (n)
		{
		case 1:
			printf("Koji element zelite dodati u stablo?\n");
			scanf("%d", &el);
			root = Umetni(root, el);
			break;

		case 2:
			Print_Inorder(root);
			break;

		case 3:
			Print_Postorder(root);
			break;

		case 4:
			Print_Preorder(root);
			break;

		case 5:
			Print_Level_Order(root, 4);
			break;

		case 6:
			printf("Koji je vas zeljeni element?\n");
			scanf("%d", &el);
			q = Pronadi_Element(root, el);
			if (q == NULL)
				printf("Element %d postoji u ovome stablu\n", q->El);
			break;

		case 7:
			printf("Koji element zelite izbrisati u stablu?\n");
			scanf("%d", &el);
			root = Izbrisi_Element(root, el);
			break; 

		case 8:
			return 0;

		default:
			printf("Krivi unos! Molimo pokusajte ponovno!\n");

		}


	} while (n != '8');


	return EXIT_SUCCESS;
}

Position Umetni(Position current, int el)
{
	if (current == NULL)
	{
		current = (Position)malloc(sizeof(stablo));

		current->El = el;
		current->L = NULL;
		current->R = NULL;

		return current;
	}

	else if (el > current->El) // veci elementi idu s desne strane
	{
		current->R = Umetni(current->R, el);
	}

	else if (el < current->El) // manji elementi idu s live strane
	{
		current->L = Umetni(current->L, el);
	}

	return current;
}

int Print_Inorder(Position current)
{
	if (current)
	{
		Print_Inorder(current->L);  // idemo prema livo prvo
		printf("%d\n", current->El);
		Print_Inorder(current->R);
	}

	return EXIT_SUCCESS;
}

int Print_Postorder(Position current)
{
	if (current)
	{
		Print_Postorder(current->L);
		Print_Postorder(current->R);
		printf("%d\n", current->El);
	}

	return EXIT_SUCCESS;
}

int Print_Preorder(Position current)
{
	if (current)
	{
		printf("%d\n", current->El);
		Print_Preorder(current->L);
		Print_Preorder(current->R);
	}

	return EXIT_SUCCESS;
}

int Print_Level_Order(Position current, int br_level)
{
	if (current == NULL)
		return EXIT_SUCCESS;

	if (br_level == 1)
		printf("%d\n", current->El); // samo jedan element, onaj prvi, ako je level 1

	else
	{
		Print_Level_Order(current->L, br_level - 1);
		Print_Level_Order(current->R, br_level - 1);
	}

	return EXIT_SUCCESS;
}

Position Pronadi_Element(Position current, int el)
{
	if (!current)
	{
		printf("Element kojeg trazite, ne nalazi se u stablu!\n");
		return NULL;
	}

	else if (el < current->El)
		current = Pronadi_Element(current->L, el);  // manji su lijevo

	else if (el > current->El)
		current = Pronadi_Element(current->R, el); // veci su desno, pa taj element trazimo s desne strane

	return current;
}

Position Izbrisi_Element(Position current, int el)
{
	Position q;
	
	if (!current)
		return current;

	else if (el < current->El)
		current->L = Izbrisi_Element(current->L, el);

	else if (el > current->El)
		current->R = Izbrisi_Element(current->R, el);

	else
	{
		if (current->L && current->R) // dakle ima i lijevu i desnu stranu
		{
			q = Pronadi_Min(current->R);
			current->El = q->El; // idemo od namanjeg prema "gore"
			current->R = Izbrisi_Element(current->R, q->El);
		}

		else
		{
			q = current;

			if (!current->L) // nema tog livog, idemo desno
				current = current->R;

			else if (!current->R) // obrnuto od ovog gore
				current = current->L;

			free(q);
		}
	}

	return q;
}

Position Pronadi_Min(Position current) // onaj koji je na krajnjoj lijevoj strani je minimalan
{
	while (current)
	{
		current = current->L;
	}

	return current;
}