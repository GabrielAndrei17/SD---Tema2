/* ANDREI Gabriel - 313CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
typedef struct 
{
	int minute;

} TEpisod;
typedef struct 
{
	int nr_episoade;

	void* episoade;   //coada de episoade

} TSezon;
typedef struct 
{
	int ID;
	char* nume;
	float rating;
	int nr_sezoane;
	int numar_ordine;

	void* sezoane;   ///coada de sezoane

} TSerial;

#ifndef _FUNCTII_
#define _FUNCTII_
  typedef int (*TF1)(void *, FILE* );
  typedef int (*TF2)(const void *, const void *);
#endif

#ifndef COADA
#define COADA

void* InitQ(size_t d,...); /* creeaza coada vida cu elemente de dimensiune d;
							  anumite implementari pot necesita si alti parametri */
int IntrQ(void* a, void* ae); /* adauga element la sfarsitul cozii */
int ExtrQ(void* a, void* ae); /* extrage primul element din coada la adresa ae */
int PrimQ(void* a, void* ae); /* copiaza primul element din coada la adresa ae */
int VidaQ(void* a);           /* test coada vida */
size_t NrEQ(void* a);         /* numarul de elemente din coada */
size_t DimEQ(void* a);        /* dimensiunea elementelor din coada */
void ResetQ(void* a);         /* transforma coada in coada vida */
void DistrQ(void** aa);       /* elibereaza intregul spatiu ocupat de coada */
size_t AfisQ(void* a, TF1 f, FILE* f2); /* afisaza elementele, folosind functia f;
								   rezultat = numarul de elemente pentru care f != 0 */

int MutaQ(void *ad, void *as);  	/* muta in coada dest. primul element din sursa */
int ConcatQ(void *ad, void *as);	/* concatenare (muta toate elem.din sursa) */
int ActualizareSezoane( void* sezoane, int d);/* Calculeaza durata de timp ramasa din serial */
TSerial* FindQ(void *a, char* nume);/* Cauta in coada elementul cu numele dat */
void EliminaElQ(void** coada, void* el);/* Elimina elementul dat din coada */

#endif
