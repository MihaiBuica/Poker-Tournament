/*BUICA Mihai-Victoras - 312CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

Masa CitireMasa(FILE*);
TLista CitireJucatori(int n, FILE* f); //n este numarul curent de jucatori
void Afisare(TLista a, FILE* f);//,  void(*f)(void*));
TLista AlocCelula(void *x) //functie de alocare a unei celule
{
	TLista aux;
	aux = (TLista)malloc(sizeof(Celula));
	if(!aux) return NULL;
	aux->info = x;
	aux->urm = NULL;
	return aux;
}
int InsLG(ALista aL, void* ae) //inserare lista generica
{
  TLista aux;
  TLista p, u;
  aux = AlocCelula(ae);
  p = *aL; u = NULL;
  for(; p != NULL; u = p, p = p->urm);
  if ( u == NULL)
  	*aL = aux;
  else
  	u->urm = aux;
  return 1;
}
int InsLCS(TLista s, void* ae) //inserare la sfarsit lista circulara cu santinela
{
	TLista aux;
	TLista p, u = NULL;
	aux = AlocCelula(ae);
	if (!aux) return 0; //cod eroare
	aux->urm = s;
	p = s;
	for(p = p->urm; p != s; u = p, p = p->urm);
  	if ( u == NULL)
  		s->urm = aux;
 	 else
  		u->urm = aux;
 	 return 1; //cod succes
}
TLista InitializareListaSantinela() //initializare lista santinela
{
	TLista aux;
	aux = (TLista)malloc(sizeof(Celula));
	if(!aux) return NULL;
	aux->info = NULL;
	aux->urm = aux;
	return aux;
}
Sala CitireSala(FILE*f) //functie citire sala
{
	int rez = 0, nrMese = 0;
	Sala sala;
	sala =(Sala) malloc(sizeof(struct sala));
	if( !sala ) return NULL;

	fscanf(f, "%d", &nrMese);
	sala->nrMese = nrMese;
	sala->nrLocCrt = 0;
	sala->nrLocMax = 0;
	int i;
	TLista mese = NULL;
	Masa masaCrt; //masa curenta
	sala->masa =(TLista) malloc(sizeof(Celula)); //lista de mese
	if(!sala->masa) 
		{ 
			free(sala); 
			return NULL;
		}
	for(i = 0; i < nrMese; i++)
	{
		masaCrt = malloc(sizeof(Masa));
		if(!masaCrt) 
			{
				free(sala->masa); 
				free(sala); 
				return NULL;
			}
		masaCrt->numeMasa = malloc(15*sizeof(char)); //numele mesei:maxim 15 caractere
		if(!masaCrt->numeMasa)
			{
				free(masaCrt); 
				free(sala->masa); 
				free(sala); 
				return NULL;
			} 
		fscanf(f, "%s %d %d", masaCrt->numeMasa, &(masaCrt->nrCrtJucatori), &(masaCrt->nrMaxJucatori));
		//citirea datelor despre masa
		sala->nrLocCrt += masaCrt->nrCrtJucatori; sala->nrLocMax += masaCrt->nrMaxJucatori;
		//actualizarea datelor din sala: nr Maxim si Curent de jucatori
		masaCrt->jucatori = malloc(sizeof(Celula)); //lista de jucatori
		if(!masaCrt->jucatori)
			{
				free(masaCrt->numeMasa); 
				free(masaCrt); free(sala->masa); 
				free(sala); 
				return NULL;
			}

		masaCrt->jucatori = CitireJucatori(masaCrt->nrCrtJucatori, f);
		rez = InsLG(&mese, masaCrt); //inserare in lista generica
		if(rez == 0) return NULL;
	}
	sala->masa = mese;
	return sala;
}
TLista CitireJucatori(int n, FILE*f) //n = nr de jucatori
{
	int i, rez;
	TLista j;
	Jucator jucatorCrt; //jucatorul curent
	j = InitializareListaSantinela(); //j devine o lista cu santinela 
	if(!j) return NULL;
	for(i = 0; i< n; i++)
	{
		jucatorCrt = malloc(sizeof(Jucator)); 
		if (!jucatorCrt){free(j); return NULL;}
		jucatorCrt->nume = malloc(50*sizeof(char)); //numele uni jucator-maxim 50 caractere
		if(!jucatorCrt->nume){free(jucatorCrt); free(j); return NULL;}
		fscanf(f,"%s %d", jucatorCrt->nume, &(jucatorCrt->nrMaini));
		rez = InsLCS(j,jucatorCrt); //informatiile sunt inserate in lista circ cu santinela
		if (rez == 0) return NULL; 
	}
	return j;
}
void AfiJucator(void* ae, FILE*f) //functie de afisare jucator
{
	Jucator p = (Jucator)ae;
	fprintf(f, "%s %d\n", p->nume, p->nrMaini);
}
void AfiMasa(void* ae, FILE*f) //functie de afisare masa si lista jucatori
{	
	
	Masa p = (Masa)ae;
	TLista j,s;
	j = p->jucatori;
	s = j;
	j = j->urm;
	fprintf(f,"%s: ", p->numeMasa);
	for(; j != s; j = j->urm)
	{
		if(j->urm != s) 
		{
			Jucator juc = (Jucator)j->info;
			fprintf(f, "%s - %d; ", juc->nume, juc->nrMaini);
		}
		else //daca este ultimul, numele este urmat de .
		{
			Jucator juc = (Jucator)j->info;
			fprintf(f, "%s - %d.", juc->nume, juc->nrMaini);
		}

	}
	fprintf(f, "\n");
}
void Afisare(TLista a, FILE*f)//functie de afisare masa/jucator
{
	if(!a) { fprintf(f, "Sala este inchisa!\n"); return;}
	
	for(; a != NULL; a = a->urm) //conditia pentru jucator este a->info != NULL
		AfiMasa(a->info,f);	
}
/*Functii eliberare memorie: masa, jucator, sala*/
void free_elem(void* e)
{
	Masa a = (Masa)e;
	free(a->numeMasa); //eliberare nume alocat
}
void free_jucator(void* e)
{
	Jucator a = (Jucator)e;
	free(a->nume);
}
void Distrugere(ALista aL, void(*f)(void*))
{
	while(*aL != NULL && (*aL)->info != NULL)
	{
		TLista aux = *aL;
		if (!aux) return;
		f(aux->info);
		*aL = aux->urm;
		free(aux);
	}
}
void noroc(Sala s, char* numeMasa, char* numeJucator, int grad, FILE*f)
{
	TLista mese, jucatori, santinela;
	Masa masaCrt;
	Jucator jucatorCrt;
	mese = s->masa;
	for(;mese != NULL; mese = mese->urm)
	{
		masaCrt = malloc(sizeof(Masa));
		if(!masaCrt) return;
		masaCrt = (Masa) mese->info;
		if(strcmp(masaCrt->numeMasa,numeMasa) == 0) //cautare masa
		{
			jucatori = masaCrt->jucatori;
			santinela = masaCrt->jucatori;
			for(jucatori = jucatori->urm; jucatori != santinela; jucatori = jucatori->urm)
			{
				jucatorCrt = malloc(sizeof(Jucator));
				if(!jucatorCrt) {free(masaCrt); return;}
				jucatorCrt = (Jucator) jucatori->info;
				if(strcmp(jucatorCrt->nume, numeJucator) == 0) //cautare jucator
				{
					jucatorCrt->nrMaini += grad; //incrementare noroc
					return;
				}
			}
			fprintf(f,"Jucatorul %s nu exista la masa %s!\n",numeJucator, numeMasa);
			return;
			//daca se ajunge pe acest caz, inseamna ca jucatorul nu a fost gasit
		}
		
	}
	fprintf(f,"Masa %s nu exista!\n",numeMasa);
	//daca se ajunge pe acest caz, inseamna ca masa nu a fost gasita
}

Sala ghinion(Sala s, char* numeMasa, char* numeJucator, int grad, FILE*f)
{
	int ok = 0;
	TLista  mese, jucatori, santinela, ant, antm; //ant = jucatorul anterior; antm= masa anterioara
	Masa masaCrt;//, aux2;
	Jucator jucatorCrt;//, aux;
	mese = s->masa; //lista de mese
	//m = s->masa;
	for(antm = NULL; mese != NULL;antm = mese, mese = mese->urm)
	{
		masaCrt = malloc(sizeof(Masa));
		if(!masaCrt) return s;
		masaCrt = (Masa) mese->info;
		if(strcmp(masaCrt->numeMasa,numeMasa) == 0)
		{
			jucatori = masaCrt->jucatori;
			santinela = masaCrt->jucatori;
			for(ant = NULL, jucatori = jucatori->urm; jucatori != santinela;ant = jucatori, jucatori = jucatori->urm)
			{
				jucatorCrt = malloc(sizeof(Jucator));
				if(!jucatorCrt) {free(masaCrt); return s;}
				jucatorCrt = (Jucator) jucatori->info;
				if(strcmp(jucatorCrt->nume, numeJucator) == 0)
				{
					jucatorCrt->nrMaini -= grad;
					ok = 1; //jucatorul a fost gasit
					if (jucatorCrt->nrMaini <= 0) //jucatorul trebuie eliminat cand nrMaini <= 0
					{
						//aux = jucatorCrt;
						if(ant == NULL)
						{
							santinela->urm = jucatori->urm;
							jucatori = santinela;
						}//refacere legaturi daca jucatorul era primul din lista
						else
						{
							ant->urm = jucatori->urm;
							jucatori = ant;
						}//refacere legaturi daca juc este pe pozitie>=2
						//free_jucator((void*)aux);
						masaCrt->nrCrtJucatori -= 1; //dupa eliminare, scade nr de jucatori
						s->nrLocCrt -= 1; //scade si nrLoc curente din sala
						if(masaCrt->nrCrtJucatori <= 0)//daca nrCrtJucatori<=0 masa trebuie eliminata
						{
							//aux2 = masaCrt;
							if(antm == NULL)
							{
								s->masa = mese->urm;
								mese = mese->urm;
							} //refacere legaturi daca masa este prima
							else
							{
								antm->urm = mese->urm;
								mese = mese->urm;
								
							} //refacere legaturi masa>=2
							//free_elem((void*)aux2);

							s->nrLocMax -= masaCrt->nrMaxJucatori; //nr de locuri maxim din sala cade cu 
																	//nr maxim de locuri al mesei eliminate
						}

						s->nrMese -= 1;
						if(s->nrMese <= 0) //daca nu mai exista mese in sala
						{
							return NULL; //inchidere sala
						}
						else
							return s;
					}
				}
			}
			if( ok == 0)
			{
			fprintf(f,"Jucatorul %s nu exista la masa %s!\n",numeJucator, numeMasa);
			}
			return s;
		}
		
	}
	fprintf(f,"Masa %s nu exista!\n",numeMasa);
	return s;

}

void tura(Sala s, char* numeMasa, FILE*f)
{
	int ok = 0;
	TLista mese, jucatori, santinela, ant, aux, antm;
	Masa masaCrt; //,auxm;
	Jucator jucatorCrt; //, auxj;
	mese = s->masa;
	for(antm = NULL;mese != NULL;antm = mese, mese = mese->urm)
	{
		masaCrt = malloc(sizeof(Masa));
		if(!masaCrt) return;
		masaCrt = (Masa) mese->info;
		if(strcmp(masaCrt->numeMasa,numeMasa) == 0)
		{
			ok = 1; //masa gasita

			jucatori = masaCrt->jucatori; //lista de jucatori
			jucatori = jucatori->urm; //ignorare santinela
			santinela = masaCrt->jucatori; //memorare santinela
			aux = jucatori; //retine primul element din lista
			jucatori = jucatori->urm; 
			aux->urm = santinela; //leaga primul element la santinela => devine ultimul
			santinela->urm = jucatori; //santinela legata la al doilea jucator
			//al doilea jucator devine primul
			jucatorCrt = malloc(sizeof(Jucator));
			if(!jucatorCrt) {free(masaCrt); return;}
			jucatorCrt = (Jucator) aux->info;

			for(; jucatori->urm != santinela; jucatori = jucatori->urm); //se parcurg jucatorii
			jucatori->urm = aux;											//se opreste pe ultimul
			//jucatorul din aux devine ultimul
			jucatori = santinela->urm; //lista noua cu jucatorii rotiti
			/*eliminare jucator, masa, sala in cazul in care nr de jucatori scade dupa o tura*/
			for(ant = NULL; jucatori != santinela; ant = jucatori, jucatori = jucatori->urm)
			{
				jucatorCrt = malloc(sizeof(Jucator));
				if(!jucatorCrt) {free(masaCrt); return;}
				jucatorCrt = (Jucator) jucatori->info;
				jucatorCrt->nrMaini -= 1;
				if (jucatorCrt->nrMaini == 0)
				{
					//auxj = jucatorCrt;
					if(ant == NULL)
					{
						santinela->urm = jucatori->urm;
						jucatori = santinela; //pentru a se putea continua parcurgerea
					}//refacere legaturi
					else
					{
						ant->urm = jucatori->urm;
						jucatori = ant;//pentru a se putea continua parcurgerea
					}//refacere legaturi
					//free_jucator((void*)auxj);
					masaCrt->nrCrtJucatori -= 1;
					s->nrLocCrt -= 1;
					if(masaCrt->nrCrtJucatori == 0)
					{
						//auxm = masaCrt;
						if(antm == NULL)
						{
							s->masa = mese->urm;
							mese = mese->urm;
						}
						else
						{
							antm->urm = mese->urm;
							mese = mese->urm;
						}
						//free_elem((void*)auxm);
						s->nrMese -= 1;
						if(s->nrMese == 0)
							s = NULL;
					}
				}
			}
		}
		if(ok == 1) //daca masa a fost gasita, functia se paraseste
			return;
	}
	if(ok == 0)
		fprintf(f,"Masa %s nu exista!\n",numeMasa);
}
	
void tura_completa(Sala s, FILE*f) //se apeleaza functia de tura pentru fiecare masa
{
	TLista mese;
	Masa masaCrt;
	mese = s->masa;
	for(; mese != NULL; mese = mese->urm)
	{
		masaCrt = malloc(sizeof(Masa));
		if(!masaCrt) return;
		masaCrt = (Masa) mese->info;
		tura(s, masaCrt->numeMasa, f);
	}
} 
int compPunctaj(void*a, void*b)
{
	Jucator j1, j2;
	j1 = (Jucator) a;
	j2 = (Jucator) b;
	if (j1->nrMaini == j2->nrMaini)
		return strcmp(j1->nume,j2->nume); //comparare ASCII
	else
		return (j1->nrMaini-j2->nrMaini);	//comparare nrMaini
}
int InsOrd(TLista a, void* info) //inserare ordine descrescatoare
{
	TLista aux = malloc(sizeof(Celula));
	if(!aux) return 0; //cod eroare
	aux->info = info;
	aux->urm = a;
	if(a->urm == a)//cazul listei in care exista doar santinela
	{
		a->urm = aux;
		aux->urm = a;
		return 1;
	} //elementul este introdus imediat dupa santinela
	TLista p, ant;
	ant = NULL;
	for(p = a->urm; p != a; ant = p, p = p->urm)
	{
		if(compPunctaj(aux->info, p->info) > 0) //inserarea el inaintea unui element mai mic
		{
			if(ant == NULL)  //fara anterior: inaintea primul element
			{
				aux->urm = p;
				a->urm = aux;
				return 1; //cod succes
			}
			/*cu anterior*/
			aux->urm = p; 
			ant->urm = aux;
			return 1;
		}
	}
		if(p == a) //daca lista a fost parcursa pana la capt, elementul se aseaza la sfarsit
			ant->urm = aux;
		return 1;
	
	return 1;
}
TLista clasament(Sala s, char* numeMasa, FILE*f)
{
	int ok = 0, rez1;
	TLista mese, jucatori, santinela, jucatori_nou = NULL;// jucatori_nou = lista sortata
	Masa masaCrt; //,auxm;
	mese = malloc(sizeof(Celula));
	if(!mese) return NULL;
	mese = s->masa;
	for(; mese != NULL; mese = mese->urm)
	{
		masaCrt = malloc(sizeof(Masa));
		if(!masaCrt) return NULL;
		masaCrt = (Masa) mese->info;
		if(strcmp(masaCrt->numeMasa,numeMasa) == 0)
		{
			ok = 1;
			jucatori = masaCrt->jucatori;
			jucatori = jucatori->urm;
			santinela = masaCrt->jucatori;
			jucatori_nou = InitializareListaSantinela();
			for(; jucatori != santinela; jucatori = jucatori->urm)
			{
				rez1 = InsOrd(jucatori_nou,jucatori->info);
				if(rez1 == 0) return NULL;
			}//fiecare element este inserat in noua lista

		}
		if (ok == 1) break;
	}
	return jucatori_nou;
}

int main(int argc, char* argv[])
{
	Sala a;
	/*fisierele de citire-scriere*/
	FILE*conf = fopen(argv[1],"rt");
	if(conf == NULL){ printf("Eroare deschidere fisier conf\n"); return -1;}
	FILE*even = fopen(argv[2],"rt");
	if(even == NULL){ printf("Eroare deschidere fisier event\n"); return -1;}
	FILE*out = fopen(argv[3],"wt");
	if(out == NULL){ printf("Eroare deschidere fisier out\n"); return -1;}
	
	//a =(Sala) malloc(sizeof(Sala));
	a = CitireSala(conf);
	char comanda[250],*numeComanda,*prm1, *prm2,*prm3;
	char* separator = " \n"; //separatorii pentru impartirea comenzii
	while (fgets(comanda, 250, even) != NULL) //citire comanda
	{
		/*delimitare si recunoastere comanda*/
		numeComanda = strtok(comanda, separator); 
		if (numeComanda != NULL)
		{
			if (strcmp(numeComanda, "print") == 0)
				{
					Afisare(a->masa,out);
				}
			else if (strcmp(numeComanda, "noroc") == 0)
			{
				/*citire parametrii*/
				prm1 = strtok(NULL, separator);
				prm2 = strtok(NULL, separator);
				prm3 = strtok(NULL, separator);
				if(prm1 == NULL || prm2 == NULL) //parametrii invalizi
					return -1;
				if(prm3 == NULL)
					noroc(a,prm1,prm2,0,out);
				else //parametrii gresiti
					noroc(a,prm1,prm2,atoi(prm3),out);
			}
			else if (strcmp(numeComanda, "ghinion") == 0)
			{
				prm1 = strtok(NULL, separator);
				prm2 = strtok(NULL, separator);
				prm3 = strtok(NULL, separator);
				if(prm1 == NULL || prm2 == NULL) //parametrii invalizi
					return -1;
				if(prm3 == NULL)
				{	
					a = ghinion(a,prm1,prm2,0,out);
				}
				else
				{	
					a = ghinion(a,prm1,prm2,atoi(prm3),out);
				}
			}
			else if (strcmp(numeComanda, "tura") == 0)
			{
				prm1 = strtok(NULL, separator);
				tura(a,prm1,out);
			}				
			else if(strcmp(numeComanda, "tura_completa") == 0)
			{
				tura_completa(a, out);
			}
			else if(strcmp(numeComanda, "clasament") == 0)
			{
				TLista lista_sort, s = NULL;
				prm1 = strtok(NULL, separator);
				lista_sort = clasament(a, prm1, out);
				s = lista_sort;
				fprintf(out, "Clasament %s:\n",prm1);
				if (s != NULL) 
					for(s = s->urm; s != lista_sort; s = s->urm)
					{
						AfiJucator(s->info, out);
					}
				else return 1;
				Distrugere(&s,&free_jucator);

			}
		}
	}
	/* DISTRUGEREA SALII */
	/*lista_mese = a->masa;
	for (; lista_mese != NULL; lista_mese = lista_mese->urm)
	{
		Masa mc;
		mc = malloc(sizeof(Masa));
		mc = (Masa)lista_mese->info;
		m = mc->jucatori;
		Distrugere(&m, &free_jucator);
	}
	Distrugere(&a->masa, &free_elem);
	                */
	fclose(conf);
	fclose(even);
	fclose(out);
	return 0;	
}