#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX_LEN 128

typedef struct _student {
	char ime[50];
	char prezime[50];
	double bodovi;
	double RelBod;
}student;

int UnosPodataka(student s[MAX_LEN], int brS)
{
	FILE* dat = NULL;
	int i;

	dat = fopen("studenti.txt", "r");
	if (dat == NULL)
	{
		printf("Neuspjesno otvaranje datoteke!");
		return -1;
	}
	for (i = 0; i < brS; i++)
		fscanf(dat, "%s %s %lf", s[i].ime, s[i].prezime, &s[i].bodovi);
}

int main()
{
	FILE* dat = NULL;
	int brS = 0;
	char rec[MAX_LEN];
	student* s;
	double maxBod;
	int i;

	dat = fopen("studenti.txt", "r");

	if (dat == NULL)
	{
		printf("Pogreska pri otvaranju datoteke!");
		return -1;
	}

	while (!feof(dat))
	{
		fgets(rec, MAX_LEN, dat);
		brS++;
	}

	fclose(dat);

	s = (student*)malloc(brS * sizeof(student));

	if (s == NULL)
	{
		printf("Nije se alocirala memorija!");
		return -1;
	}

	UnosPodataka(s, brS);

	maxBod = s[0].bodovi;

	for (i = 1; i < brS; i++)
	{
		if (maxBod < s[i].bodovi)
			maxBod = s[i].bodovi;
	}

	printf("IME\t\tPREZIME\t\tREL. BODOVI\n");

	for (i = 0; i < brS; i++)
	{
		printf("%s\t\t%s\t\t%.4lf\t%.4lf\n", s[i].ime, s[i].prezime, s[i].bodovi, (s[i].bodovi / maxBod) * 100);
	}

	free(s);

	return 0;
}