//Buffon-féle tû probléma megvalósítása


//When I wrote this, only God and I understood what I was doing
//Now, God only knows

#define _CRT_SECURE_NO_WARNINGS
#define Pi 3.14159265359 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<SDL.h>

/*Struktúra meghatározása*/
typedef struct lista
{
	int sorszam;
	double x;
	double y;
	double fok;
	int vegeredmeny;
	struct lista *kovetkezo; /*Egy irányba láncolt lista*/
}lista;

/*Kezdõlista felvétele*/
lista *kezdolista;

/*Random koordináta generáló függvény*/
void Random_koordinata(int n) 
{
	int i;														//Futóváltozó
	double x,y;
	double xertek1,xertek2,xertek3,xertek4;
	double yertek1,yertek2,yertek3,yertek4;
	char *fnev="koordinata.txt";
	FILE *tfp;													//Text File Pointer

	if((tfp = fopen(fnev, "w")) != NULL) {
		fprintf(stderr, "\nSiker a %s file megnyitasakor!\n",fnev);}
	else{
		fprintf(stderr, "\nHiba a %s file megnyitasakor!\n",fnev);}
	
	time_t t; 
	srand((unsigned) time(&t));									//Kívül kell lennie a for cikluson, nemtudni miért de tapasztalat szerint így mûködik, úgy nem.
	for(i=0; i<n; i++)
	{
		xertek1=rand() % 10;
		xertek2=rand() % 10;
		xertek3=rand() % 10;
		xertek4=rand() % 10;

		yertek1=rand() % 10;
		yertek2=rand() % 10;
		yertek3=rand() % 10;
		yertek4=rand() % 10;

		x=((xertek1*10)+(xertek2)+(xertek3/10)+(xertek4/100));	//A maradékos osztások elvégzése után itt és a következõ sorban visszalakítunk. 
		y=((yertek1*10)+(yertek2)+(yertek3/10)+(yertek4/100));	//Valójában az x koordináta felesleges  Pi meghatározásához, egyedül az y a fontos tényezõ. 

		fprintf(tfp,"x:%lf y:%lf ",x,y);						//Fájlba írás
	}
	fclose(tfp);												/*Fájl bezárása*/
}

/*Random dõlésszög generáló függvény*/
void Random_dolesszog(int n)
{
	int i;
	time_t t;
	double fok[1];

	srand((unsigned) time(&t));										//A srand() mindig kívül van a cikluson, és úgy mûködik. Ez nincs leírva sehol, mert nekik evidens. :)

	FILE *bfp;														//Binary File Pointer
	char *fnev="dolesszog.dat";

	if((bfp = fopen(fnev, "wb")) != NULL) {
			fprintf(stderr,"\nSiker a %s file megnyitasakor!\n",fnev);}
	else{
		fprintf(stderr,"\nHiba a %s file megnyitasakor!\n",fnev);}	/*Else ágba nyílik hogy ne legyen memory leak.*/

	for(i=0; i<n; i++)
	{
		fok[0] = rand() % 181;
		if(fwrite(fok,sizeof(double),1,bfp)!=1)						/*fok[1] tömb típusban double méretû (a majdani sin függvény miatt) 1db  elemet írunk bfp pointerrel.*/
			{
				fprintf(stderr,"\nHiba a dolesszogek kiirasakor!\n");
			}
	}
	fclose(bfp);													/*Fájl bezárása*/
}

/*Pi kiírására szolgáló függvény*/
lista *Pikiir(lista *kezdolista, int stick, int line)
{
	double szamlalo=0, nevezo=0;
	double valoszinuseg, pi, kulonbseg;
	lista *mozgo=kezdolista; /*Helyezzük a lista legelejér a mozgo pointert*/
	for(mozgo=kezdolista; mozgo!=NULL; mozgo=mozgo->kovetkezo) //Lista bejárása
		{ 
			if((mozgo->vegeredmeny)==1)
				szamlalo++;
			if((mozgo->vegeredmeny)==0)
				nevezo++;
		}
	nevezo=nevezo+szamlalo;
	if(szamlalo==0)
	{
		printf_s("\nEgyetlen sikeres dobás sincsen. Its not a bug, its a feature!\n");
		return kezdolista;
	}
	valoszinuseg=(szamlalo/nevezo);

	pi=(stick)/(line*valoszinuseg);
	kulonbseg=(Pi-pi);

	printf_s("\nIgazi Pi: 3.14159265359\n");
	printf_s("\nSzamolt Pi: %lf\n",pi);
	printf_s("\nA kulonbseg: %lf\n",kulonbseg);

	return kezdolista;
}

/*A listát felépítõ függvény. Az adatokat a Központi függvénybõl hívja meg.*/
lista *Kezdolista_beszur(lista*eleje, double x, double y,double fi,int stick, int line, int i,int n)
{
	double egyseghossz=line, eredmeny=0;
	double h, h1, c;
	do{
		c=((stick*sin(fi))/2);
		h=egyseghossz-y;
		h1=line-h;
		if(((h>=0)&&(h<=c))||((h1>=0)&&(h1<=c))) /*Ezt még Buffon határozta meg így. ANNO 1777*/
			{
				eredmeny=1;
			}
		else
			{	
				eredmeny=0;
				egyseghossz=egyseghossz+line;
			}
	}while((egyseghossz<=100) && (eredmeny==0)); /*Lehetne módosítani (100+(stick/2)) -re is akár!*/

	lista *lp=(lista*)malloc(sizeof(lista)); 
	lp->x=x;
	lp->y=y;
	lp->fok=fi;
	lp->vegeredmeny=eredmeny;
	lp->sorszam=n-i;		//ELLENÕRZÕ PONT //printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",lp->sorszam,lp->x,lp->y,lp->fok,lp->vegeredmeny);
	lp->kovetkezo=eleje;
	return lp;
}

/*Itt hajtódik végre a fájlból olvasás és az adatok továbbísa a láncolt listát megalkotó beszúró függvényhez*/
lista *Kozponti_fv(int n, int set, int line, int stick) 
{
	FILE *tfp;
	FILE *bfp;
	int i=0; //Sorszámot ad meg, nullától kezdõdik.
	double x,y;
	double fok[1], fi;

	tfp=fopen("koordinata.txt","r");
	if(tfp!=NULL){ 
		printf_s("\nSiker a koordinata.txt fajl olvasasakor!\n");}
	else{ 
		printf_s("\nHiba a koordinata.txt fajl olvasasakor!\n");}

	bfp=fopen("dolesszog.dat","rb");
	if(bfp!=NULL){
		printf_s("\nSiker a dolesszog.dat olvasasakor!\n");}
	else{
		printf_s("\nHiba a dolesszog.dat olvasasakor!\n");}

	//LISTA LÉTREHOZÁS//

	lista *kezdolista = NULL;
	while((fscanf(tfp,"x:%lf y:%lf ",&x,&y)==2) && (fread(fok,sizeof(double),1,bfp)==1))
		{
			fi=fok[0];
			kezdolista=Kezdolista_beszur(kezdolista,x,y,fi,stick,line,i,n);
			i++;
		}
	fclose(tfp);
	fclose(bfp);
	kezdolista=Pikiir(kezdolista,stick,line);
	return kezdolista;
}

/*Rendezõ függvény, mely a kívánt set érték szerint sorba rendez.*/
lista *Csere_fv(lista*kezdolista,int n, int set)
{
	int i;
	lista *bp=(lista*)malloc(sizeof(lista));
	if(bp==NULL){
		return 0;
	}
	lista *lemarado=kezdolista;
	lista *mozgo=kezdolista->kovetkezo;

	/*Minden egyes függvény a set_fv ben meghatározott módon rendezi sorba az elemeket. A rendezés n*n-szer fut végig, mert ha az utolsó elemmel akarunk valamit,
	például elõre hozni, akkor azt n-szer biztosan elõrébb kell hozzni. A lépészám kijön n^2-bõl. 
	Lehet hogy lehet kevesebbõl is, de házit írok nem a NASA-t töröm fel! :)*/

	if(set==0){
				printf_s("\nSorszam szerinti rednezes!\n");
				for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++)
					{
						mozgo->sorszam=i;
						printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
					}
				return kezdolista;
			  }
	if(set==1){
				printf_s("\nX szerint novekvo rednezes!\n");
				for(i=0;i<n*n;i++)
					{
						lemarado=kezdolista;
						mozgo=kezdolista->kovetkezo;
				do{
						if((lemarado->x)>(mozgo->x))
							{
								(bp->x)=(lemarado->x);
								(lemarado->x)=(mozgo->x);
								(mozgo->x)=(bp->x);
								lemarado=lemarado->kovetkezo;
								mozgo=mozgo->kovetkezo;
							}
						else
							{	
								lemarado=lemarado->kovetkezo;
								mozgo=mozgo->kovetkezo;
							}
					}while(mozgo!=NULL);
					}
				for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++)
					{
						mozgo->sorszam=i;
						printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
					}
				return kezdolista;
				}
	if(set==2){		printf_s("\nY szerint novekvo rednezes!\n");
					for(i=0;i<n*n;i++)
						{
							lemarado=kezdolista;
							mozgo=kezdolista->kovetkezo;
							do{
								if((lemarado->y)>(mozgo->y))
									{
										(bp->y)=(lemarado->y);
										(lemarado->y)=(mozgo->y);
										(mozgo->y)=(bp->y);
										lemarado=lemarado->kovetkezo;
										mozgo=mozgo->kovetkezo;
									}
								else
									{	
										lemarado=lemarado->kovetkezo;
										mozgo=mozgo->kovetkezo;
									}
								}while(mozgo!=NULL);
						}
					for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++)
						{
							mozgo->sorszam=i;
							printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
						}
				return kezdolista;

				}
	if(set==3){		printf_s("\nFok szerint novekvo rednezes!\n");
					for(i=0;i<n*n;i++)
					{
								lemarado=kezdolista;
								mozgo=kezdolista->kovetkezo;
							do{
								if((lemarado->fok)>(mozgo->fok))
									{
										(bp->fok)=(lemarado->fok);
										(lemarado->fok)=(mozgo->fok);
										(mozgo->fok)=(bp->fok);
										lemarado=lemarado->kovetkezo;
										mozgo=mozgo->kovetkezo;
									}
								else
									{	
										lemarado=lemarado->kovetkezo;
										mozgo=mozgo->kovetkezo;
									}
								}while(mozgo!=NULL);
					}
					for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++)
					{
						mozgo->sorszam=i;
						printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
					}
					return kezdolista;
				
				}
	if(set==-1){		printf_s("\nX szerint csokkeno rednezes!\n");
					for(i=0;i<n*n;i++)
						{
							lemarado=kezdolista;
							mozgo=kezdolista->kovetkezo;
							do{
								if((lemarado->x)<(mozgo->x)){
									(bp->x)=(mozgo->x);
									(mozgo->x)=(lemarado->x);
									(lemarado->x)=(bp->x);
									lemarado=lemarado->kovetkezo;
									mozgo=mozgo->kovetkezo;}
								else{	
									lemarado=lemarado->kovetkezo;
									mozgo=mozgo->kovetkezo;}
								}while(mozgo!=NULL);}
							for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++){
								mozgo->sorszam=i;
								printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
							}
							return kezdolista;
				}
	if(set==-2){		printf_s("\nY szerint csokkeno rednezes!\n");
					for(i=0;i<n*n;i++){
							lemarado=kezdolista;
							mozgo=kezdolista->kovetkezo;
						do{
							if((lemarado->y)<(mozgo->y)){
								(bp->y)=(mozgo->y);
								(mozgo->y)=(lemarado->y);
								(lemarado->y)=(bp->y);
								lemarado=lemarado->kovetkezo;
								mozgo=mozgo->kovetkezo;}
							else{	
								lemarado=lemarado->kovetkezo;
								mozgo=mozgo->kovetkezo;}
							}while(mozgo!=NULL);}
						for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++)
							{
								mozgo->sorszam=i;
								printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
							}
						return kezdolista;
						}
	if(set==-3){		printf_s("\nFok szerint csokkeno rednezes!\n");
					for(i=0;i<n*n;i++)
						{
							lemarado=kezdolista;
							mozgo=kezdolista->kovetkezo;
						do{
							if((lemarado->fok)<(mozgo->fok))
								{
									(bp->fok)=(mozgo->fok);
									(mozgo->fok)=(lemarado->fok);
									(lemarado->fok)=(bp->fok);
									lemarado=lemarado->kovetkezo;
									mozgo=mozgo->kovetkezo;
								}
							else
								{	
									lemarado=lemarado->kovetkezo;
									mozgo=mozgo->kovetkezo;
								}
							}while(mozgo!=NULL);
						}
					for(mozgo=kezdolista,i=0; mozgo!=NULL,i<n; mozgo=mozgo->kovetkezo,i++)
							{
								mozgo->sorszam=i;
								printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mozgo->sorszam,mozgo->x,mozgo->y,mozgo->fok,mozgo->vegeredmeny);
							}
					return kezdolista;
					}
	/*Ez az else ág kell, mert hisztizett a fordító*/
	else
		{
			return kezdolista;
		}
}

/*Az adott elemeket törlõ függvény*/
lista *Torol_fv(lista*kezdolista, int del,int stick, int line)
{ 
	/*Három esetre bontjuk szét*/

	lista *lemarado=NULL, *mozgo=kezdolista;
	while(mozgo!=NULL && mozgo->sorszam!=del) /*Itt keressük meg az elemet, tehát itt megy végbe a keresés!*/
		{
			lemarado=mozgo; mozgo=mozgo->kovetkezo;
		}
	if(mozgo==NULL)
		{
			printf_s("\nNincs ilyen elem!\n"); /*Itt állapítja meg a függvény, hogy van-e egyáltalán megfelelõ eleme a sorozatnak.*/
			free(lemarado); //Bár a lemarado úgyis a NULL-ra mutat.
			return kezdolista;
		}
	if(lemarado==NULL)
	{ //Az elsõ cucc törlésésre. //Ha az elsõ cuccot kell törölni.
		lista *ujkezdolista=mozgo->kovetkezo;
		free(mozgo);

		lista *mocorgo; //Újra kiírás, mert ez is a program feladata.
		printf_s("\nUjra kiiras!\n");
		for(mocorgo=ujkezdolista;mocorgo!=NULL;mocorgo=mocorgo->kovetkezo)
			{
				printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mocorgo->sorszam,mocorgo->x,mocorgo->y,mocorgo->fok,mocorgo->vegeredmeny);
			}
		ujkezdolista=Pikiir(ujkezdolista,stick,line);
		return ujkezdolista;
	}
	lemarado->kovetkezo=mozgo->kovetkezo; //Közepe vagy vége
	free(mozgo);
	lista *mocorgo; /*Azért kellett a mocorgo pointer, mert nem lehetett tovább dolgozni a mozgo pointerrel, hiszen azt már felszabadítottuk.*/
		printf_s("\nUjra kiiras!\n");
		for(mocorgo=kezdolista;mocorgo!=NULL;mocorgo=mocorgo->kovetkezo)
			{
				printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mocorgo->sorszam,mocorgo->x,mocorgo->y,mocorgo->fok,mocorgo->vegeredmeny);
			}
		kezdolista=Pikiir(kezdolista,stick,line);
	return kezdolista;
}

/*Adatbekérõ függvény [n]*/
int dobas_szam()
{
	int n;
	double limit=2147483648; //Ennyit bír az int. Többet nagyon úgyse érdemes, mert itt ülünk még jövõ ugyanekkor is.
	do{
		printf_s("\n\nHany feldobast szeretne megvalositani? Csak egesz szamot adjon meg!\n");
		scanf_s("%d",&n);
		if(n>0 && n<=limit)
			{
				return n;
			}
		else
			{
				printf_s("\nHiba!\n");
			}
	}while(n<=0 || n>limit);
	return n; /*Ezt is csak azért kellett, mert visított a fordító...*/
}

/*Adatbekérõ függvény [set]*/
int set_fv()
{
	int set;
	do{
		printf_s("\nMilyen sorrendben szeretne legeneralni a majdani legeneralt ertekeket?\n[(Csokkeno: - vagy Novekvo: + ) \nHelybenhagyas:0; X szerint: 1; Y szerint: 2; Fok szerint: 3]\nCsak egesz szamot adjon meg!\n");
		scanf_s("%d",&set);
		if(set>3 || set<-3){
			printf_s("\nHiba!\n");
		}
	}while(set>3 || set<-3);
	return set;
}

/*Adatbekérõ függvény [line]*/
int line_fv()
{
	int line;
	do{
		printf_s("\nKerem adja meg a parhuzamos vonalak tavolsagat, mely minimum +2 es maximum +50 lehet!\nCsak egesz szamot adjon meg!\n");
		scanf_s("%d",&line);
		if(line<=1 || line>50)
		{
			printf_s("\nHiba!\n");
		}
	}while(line<=1 || line>50);
	return line;
}

/*Adatbekérõ függvény [stick]*/
int stick_fv(int line)
{
	int stick;
	do{
		printf_s("\nKerem adja meg a tu hosszat! Legyen nem negativ egesz es kisebbegyenlo mint a parhuzamos vonalak tavolsaga!\n");
		scanf_s("%d",&stick);
		if(stick<=0 || stick>line)
			{
				printf_s("\nHiba!\n");
			}

	}while(stick<=0 || stick>line);
	return stick;
}

/*A "Szeretne-e törölni?" függvény*/
int delc_beker(){
	int delnumber;
	do{
		printf_s("\nSzeretne elemet torolni? Igen=1 || Nem=0\nCsak egesz szamot adjon meg!\n");
		scanf_s("%d",&delnumber);
		if(delnumber==1)
			{
				return 1;
			}
		if(delnumber==0)
			{
				return 0;
			}
		else{printf_s("\nHiba!\n");}
	}while((delnumber!=0) || (delnumber !=1));
	return 0;
}

/*A "Melyik elemet szeretné törlölni?" függvény [deln]*/
int del_beker(int deln)
{
	/*Nem feladata megállapítani hogy már egy elemet töröltünk-e korábban a listában*/
	int del;
	do{
		printf_s("\nMelyik elemet szeretne torolni?\nCsak egesz szamot adjon meg!\n");
		scanf_s("%d",&del);
		if((0<=del) && (del<deln)){}
		if(del<0 || del>=deln)
			{
				printf_s("\nHiba! Nincs ilyen elem\n");
			}
	}while(del<0 || del>=deln);
	return del;
}

/*A fõ függvény.*/
int main(){
	printf_s("\t\tWelcome User!\n");

	//ELSÕ FÁZIS - ADATOK BEKÉRÉSE//

	//0//

	int n;
	n=dobas_szam();	

	//1//

	int set;
	set=set_fv();

	//2//

	int line;
	line=line_fv();

	//3//

	int stick;
	stick=stick_fv(line);

	//MÁSODIK FÁZIS - KOORDINÁTA ÉRTÉKEK LEGENERÁLÁSA//

	Random_koordinata(n); //Fájlba is ír, minden zsír ezzel a gyöngyszemmel!

	//HARMADIK FÁZIS -  SZÖG ÉRTÉKEK LEGENERÁLÁSA//

	Random_dolesszog(n); //Jó, ha int értékel nem megy, akkor csináljuk struktúrával! //S lám, azzal megy :D

	//NEGYEDIK FÁZIS - KOORDINÁTÁK ÉS SZÖGEK BEOLVASÁSA FÁJLBÓL LÁNCOLT LISTÁBA - A FÕ PROGRAMRÉSZ //

	kezdolista=Kozponti_fv(n,set,line,stick);

	//ÖTÖDIK FÁZIS - ELEMEK RENDEZÉSE//

	kezdolista=Csere_fv(kezdolista,n,set);
	if(kezdolista==NULL)
	{
		printf_s("\nHibas memoria foglalas, a program kilep!\n");
		return 0;
	}


	//HATODIK FÁZIS - ELEMEK TÖRLÉSE - PI újbóli számolt értékének kiírása//
	
	int delc;
	int del;
	do{
		delc=delc_beker();
		if(delc==1)
		{
			del=del_beker(n);
			kezdolista=Torol_fv(kezdolista,del,stick,line);
		}
		if(delc==0)
		{
			printf_s("\nTobb elem torlesere nincs lehetoseg!\n");
		}
	}while(delc==1);

	//HETEDIK FÁZIS - Könnyes búcsú// 

	lista *torol;
	torol=kezdolista;
	while(torol!=NULL)						/*A teljes listát fel kell szabadítani.*/
	{
		lista *mostani=torol->kovetkezo;		/*Készítünk egy változót amit végigpörgetünk a listán.*/
		free(torol);						/*Felszabadítjuk az elõzõleg dinamikusan lefoglalt területet.*/
		torol=mostani;						/*A következõ elemre állítjuk be a pointert.*/
	}
	kezdolista=NULL;						/*Végül a NULL-ba állítjuk a kezdolista pointert. :"(*/

	printf_s("\n\t\tGoodby User!\t\t\n");
	getchar();
	return 0;
}

/*Már csak az SDL-es megírás maradt, de ezt a laborvezetõ, Nagy Balázs megengedte, hogy december 4-éig bemutassam, tehát kitolta a határidõt!*/