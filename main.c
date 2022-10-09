/* ANDREI Gabriel - 313CB */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"

#define MAX_SEZOANE 11
#define MAX_SERIALE 2048
#define MAX_EPISOADE 11

typedef struct celulag
{
  void* info;
  struct celulag *urm;

} TCelulaG, *TLG;   //lista de seriale

float SerialCmp(TSerial * a, TSerial * b)
{
	float rating = a->rating - b->rating;
	if(rating != 0)
		return rating;

	return strcmp(b->nume, a->nume);
}
int InitSerial(TSerial * serial, int info, char* name, float r)
{
	//info reprezinta pentru serialele care nu sunt in top id-ul
	//pentru cele din top numarul de ordine 
	serial->ID = info;

	serial->nume = (char*)calloc( strlen(name) + 1, 1 );
	strcpy(serial->nume, name);

	serial->rating = r;
	serial->numar_ordine = info;

	serial->sezoane = InitQ(sizeof(TSezon), MAX_SEZOANE);
	if(serial->sezoane == NULL)
		printf("Eroare la initializarea cozii");
	return 0;
}
TLG AddLista(TLG *inceput, int id, char* name, float r, int* poz)
{
	//functia insereaza o celula la lista initiala si
	//returneaza pozitia celulei inserate
	//se creaza celula
	TLG aux = (TLG)calloc(1, sizeof(TCelulaG));
	if(aux == NULL)
		return NULL;

	aux->info = (TSerial*)calloc(1, sizeof(TSerial));
	InitSerial(aux->info, id, name, r);
	aux->urm = NULL;

	//se insereaza celula
	if(*inceput == NULL)//caz particular pentru lista vida
	{
		*inceput = aux;
		*poz = 1;
		return *inceput;
	}
	if(SerialCmp(aux->info, (*inceput)->info) > 0)//caz particular pentru inserare la inceputul listei
	{
		aux->urm = *inceput;
		*inceput = aux;
		*poz = 1;
		return *inceput;
	}
	TLG curent = *inceput;
	*poz = 2;
	while(curent->urm != NULL)
	{
		if( SerialCmp(aux->info, curent->urm->info) > 0 )
			break;
		curent = curent->urm;
		(*poz) ++;
	}
	//inserare dupa curent
	aux->urm = curent->urm;
	curent->urm = aux;

	return aux;
}
TLG AddLista_Top10(TLG *inceput, int ord, char* name, float r)
{
	//se creaza celula
	TLG aux = (TLG)calloc(1, sizeof(TCelulaG));
	if(aux == NULL)
		return NULL;

	aux->info = (TSerial*)calloc(1, sizeof(TSerial));
	InitSerial(aux->info, ord, name, r);
	aux->urm = NULL;

	//se insereaza celula
	if(*inceput == NULL)//caz particular pentru lista vida
	{
		*inceput = aux;
		return *inceput;
	}

	TLG curent = *inceput;
	if(ord == 1)//caz particular pentru inserare la inceputul listei
	{
		aux->urm = *inceput;
		*inceput = aux;

		curent = (*inceput)->urm;
	}
	else
	{
		while( ((TSerial*)(curent->info))->numar_ordine != ord - 1)
			curent = curent->urm;

		aux->urm = curent->urm;
		curent->urm = aux;

		curent = curent->urm->urm;

	}
	
	//actualizez pozitiile ramase in top10
	while(curent != NULL)
	{
		((TSerial*)(curent->info))->numar_ordine ++;
		if( ((TSerial*)(curent->info))->numar_ordine == 9 )
		{
			TLG aux2 = curent->urm;
			curent->urm = NULL;
			if(aux2 != NULL)
				free(aux2);
			break;
		}
		curent = curent->urm;
	}
	return aux;

}
int AddSezon(TSerial * serial, void *episoade, int nr)
{
	//se creaza noul sezon:
	serial->nr_sezoane ++;
	TSezon *sezon = (TSezon*)calloc(1, sizeof(TSezon));

	sezon->nr_episoade = nr;
	
	sezon->episoade = episoade;

	IntrQ(serial->sezoane, sezon);

	return 0;

}
int AfiSerial(void *inf, FILE *f2)
{
	TSerial *a =(TSerial *)inf; 
	fprintf(f2, "(%s, %.1f)", a->nume, a->rating);
	return 0;
}
void AfiLista(TLG L, FILE* f2)
{
	fprintf(f2, " [");
	AfiSerial(L->info, f2);
	L = L->urm;

	while(L != NULL)
	{
		fprintf(f2, ", ");
		AfiSerial(L->info, f2);
		L = L->urm;
	}
	fprintf(f2, "].\n");
}
void show(TLG Seriale[], char *aux, void* watch_later, void* history, void* watching, FILE *f2)
{
	if(aux[0] == '1')//categoria 1
	{
		if(Seriale[1] != NULL)
		{
			fprintf(f2, "Categoria 1:");
			AfiLista(Seriale[1], f2);
		}
		else
			fprintf(f2, "Categoria 1: [].\n");
		return;
	}
	if(aux[0] == '2')//categoria 2
	{
		if(Seriale[2] != NULL)
		{
			fprintf(f2, "Categoria 2:");
			AfiLista(Seriale[2], f2);
		}
		else
			fprintf(f2, "Categoria 2: [].\n");
		return;
	}
	if(aux[0] == '3')//categoria 3
	{
		if(Seriale[3] != NULL)
		{
			fprintf(f2, "Categoria 3:");
			AfiLista(Seriale[3], f2);
		}
		else
			fprintf(f2, "Categoria 3: [].\n");
		return;
	}

	if(strcmp(aux, "later") == 0)//categoria watch_later
	{
		if(VidaQ(watch_later) == 0)
		{
			fprintf(f2, "Categoria later:");
			AfisQ(watch_later, AfiSerial, f2);
		}
		else
			fprintf(f2, "Categoria later: [].\n");
		return;
	}
	if(strcmp(aux, "top10") == 0)//categoria 3
	{
		if(Seriale[0] != NULL)
		{
			fprintf(f2, "Categoria top10:");
			AfiLista(Seriale[0], f2);
		}
		else
			fprintf(f2, "Categoria top10: [].\n");
		return;
	}
	if(strcmp(aux, "watching") == 0)
	{
		if(VidaS(watching) == 0)
		{
			fprintf(f2, "Categoria watching:");
			AfiS_V_B(watching, AfiSerial, f2);
		}
		else
			fprintf(f2, "Categoria watching: [].\n");
		return;
	}
	if(strcmp(aux, "history") == 0)
	{
		if(VidaS(history) == 0)
		{
			fprintf(f2, "Categoria history:");
			AfiS_V_B(history, AfiSerial, f2);
		}
		else
			fprintf(f2, "Categoria history: [].\n");
		return;
	}

}
TLG FindByName(TLG L, char* name)
{
	if(L == NULL)
		return NULL;
	
	while(L != NULL)
	{
		TSerial* serial_aux = (TSerial*)L->info;
		if( strcmp(serial_aux->nume, name) == 0)
			return L;
		L = L->urm;
	}
	return NULL;
}
void StergeDinLista(TLG *L, TLG el, int categorie)
{
	if(*L == el)
	{
		*L = (*L)->urm;
	}
	else
	{
		while((*L)->urm != el)
			(*L) = (*L)->urm;

		(*L)->urm = el->urm;
		el->urm = NULL;
	}
	if(categorie == 0)//lista top10
	{
		//actualizez pozitiile ramase in top10
		TLG curent = *L;
		while(curent != NULL)
		{
			((TSerial*)(curent->info))->numar_ordine --;
			curent = curent->urm;
		}
	}

}
int NumeSerial(void* a, char* n)
{
	TSerial* S = (TSerial*)a;
	if(strcmp(S->nume, n) == 0)
		return 1;
	else
		return 0;
}
int NumeSerial2(void* a, char* n)
{
	TSerial* S1 = (TSerial*)a;
	TSerial* S2 = (TSerial*)n;
	if(strcmp(S1->nume, S2->nume) == 0)
		return 1;
	else
		return 0;
}
int main(int argc, char *argv[])
{
	TLG Seriale[4];
	void * watch_later = InitQ(sizeof(TSerial), MAX_SERIALE);
	void * history = InitS(sizeof(TSerial), MAX_SERIALE);
	void * watching = InitS(sizeof(TSerial), MAX_SERIALE);

	for(int i = 0; i < 4; i++)
		Seriale[i] = NULL;


	FILE *f1, *f2;
	f1 = fopen(argv[1], "rt");
	f2 = fopen(argv[2], "wt");
	char * line = NULL;
	size_t len = 0;


	while (getline(&line, &len, f1) != -1) 
	{
		if (line[strlen(line) - 1] == '\n')
		{
			line[strlen(line) - 1] = '\0';
		}

		char * comand = strtok(line, " ");


		if(strcmp(comand, "add") == 0)          //FUNCTIA ADD
		{
			char * aux = strtok(NULL, " ");//ID
			int ID = (int)atoi(aux);

			char * name = strtok(NULL, " ");//Nume

			aux = strtok(NULL, " ");
			float rating = (float)atof(aux);//Rating

			int poz = 0;                    //pozitia pe care va fi inserat elementul
			TLG curent = AddLista(&Seriale[ID], ID, name, rating, &poz);

			aux = strtok(NULL, " ");
			int nr_sezoane = (int)atoi(aux);//Sezoane
			for(int i = 1; i <= nr_sezoane; i++)
			{
				aux = strtok(NULL, " ");
				int nr_episoade = (int)atoi(aux);

				void * episoade = InitQ(sizeof(TEpisod), MAX_EPISOADE);

				for(int j = 0; j < nr_episoade; j++)
				{
					aux = strtok(NULL, " ");//Episoade
					TEpisod * aux_episod = (TEpisod*)calloc(1, sizeof(TEpisod));
					aux_episod->minute = atoi(aux);

					IntrQ(episoade, aux_episod);
				}
				AddSezon(curent->info, episoade, nr_episoade);
			}
			fprintf(f2, "Serialul %s a fost adaugat la pozitia %d.\n", name, poz);

			continue;
		}
		if(strcmp(comand, "show") == 0)          //FUNCTIA SHOW
		{
			char * aux = strtok(NULL, " ");//ID
			show(Seriale, aux, watch_later, history, watching, f2);

			continue;
		}
		if(strcmp(comand, "add_sez") == 0)     //FUNCTIA ADDSEZON
		{
			char * name = strtok(NULL, " ");//Nume
			char * aux = strtok(NULL, " ");
			int nr_episoade = (int)atoi(aux);

			void * episoade = InitQ(sizeof(TEpisod), MAX_EPISOADE);

			for(int j = 0; j < nr_episoade; j++)
			{
				aux = strtok(NULL, " ");//Episoade
				TEpisod * aux_episod = (TEpisod*)calloc(1, sizeof(TEpisod));
				aux_episod->minute = atoi(aux);

				IntrQ(episoade, aux_episod);
			}
			
			TLG curent = NULL;
			int i;
			for(i = 0; i <= 3; i++)
			{
				curent = FindByName(Seriale[i], name);
				if(curent != NULL)
					break;
			}
			if(curent == NULL)//serialul este in coada watch-later sau in stiva watching
			{
				TSerial* aux = FindQ(watch_later, name);
				if(aux == NULL)
					aux = FindS_B_V(watching, NumeSerial, name);
				if(aux == NULL)
					continue;
				AddSezon(aux, episoade, nr_episoade);

			}
			else
				AddSezon(curent->info, episoade, nr_episoade);

			fprintf(f2, "Serialul %s are un sezon nou.\n", name);

			continue;
		}
		if(strcmp(comand, "later") == 0)          //FUNCTIA WATCH LATER
		{
			char * name = strtok(NULL, " ");//Nume
			int i;
			TLG curent = NULL;
			for(i = 0; i <= 3; i++)
			{
				curent = FindByName(Seriale[i], name);
				if(curent != NULL)
					break;
			}
			if(curent == NULL)
				continue;

			StergeDinLista(&Seriale[i], curent, i);

			IntrQ(watch_later, curent->info);
			fprintf(f2, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", name, (int)NrEQ( watch_later ) );
			continue;
		}
		if(strcmp(comand, "add_top") == 0)          //FUNCTIA ADD TOP10
		{
			char * aux = strtok(NULL, " ");//pozitia in top 10
			int poz = (int)atoi(aux);

			char * name = strtok(NULL, " ");//Nume

			aux = strtok(NULL, " ");
			float rating = (float)atof(aux);//Rating

			TLG curent = AddLista_Top10(&Seriale[0], poz, name, rating);

			aux = strtok(NULL, " ");
			int nr_sezoane = (int)atoi(aux);//Sezoane
			for(int i = 1; i <= nr_sezoane; i++)
			{
				aux = strtok(NULL, " ");
				int nr_episoade = (int)atoi(aux);

				void * episoade = InitQ(sizeof(TEpisod), MAX_EPISOADE);

				for(int j = 0; j < nr_episoade; j++)
				{
					aux = strtok(NULL, " ");//Episoade
					TEpisod * aux_episod = (TEpisod*)calloc(1, sizeof(TEpisod));
					aux_episod->minute = atoi(aux);

					IntrQ(episoade, aux_episod);
				}
				AddSezon(curent->info, episoade, nr_episoade);
			}
			show(Seriale, "top10", watch_later, history, watching, f2);

			continue;
		}
		if(strcmp(comand, "watch") == 0)         //FUNCTIA WATCH
		{
			char * name = strtok(NULL, " ");
			char * aux = strtok(NULL, " ");
			int durata = (int)atoi(aux);

			TLG curent = NULL;
			int i;
			for(i = 0; i <= 3; i++)
			{
				curent = FindByName(Seriale[i], name);
				if(curent != NULL)
					break;
			}
			if(curent == NULL)///serialul se afla intr-o coada sau intr-o stive
			{
				TSerial* aux = FindQ(watch_later, name);
				if(aux == NULL)
					aux = FindS_B_V(watching, NumeSerial, name);
				if(aux == NULL)
					continue;

				int ok = ActualizareSezoane( aux->sezoane, durata);
				EliminaElQ(&watch_later, aux);
				EliminaElS(&watching, aux, NumeSerial2);
				if(ok == 1)
				{
					fprintf(f2, "Serialul %s a fost vizionat integral.\n", name);
					Push(history, aux);
				}
				else
					Push(watching, aux);
			}
			else//serialul se afla intr-o lista
			{
				int ok = ActualizareSezoane( ((TSerial*)(curent->info))->sezoane, durata);
				StergeDinLista(&Seriale[i], curent, i);
				if(ok == 1)
				{
					fprintf(f2, "Serialul %s a fost vizionat integral.\n", name);
					Push(history, curent->info);
				}
				else
					Push(watching, curent->info);
			}
			continue;
		}
		printf("comanda %s nu exista\n", comand);
	}
	
	fclose(f1);
	fclose(f2);
	return 0;
}