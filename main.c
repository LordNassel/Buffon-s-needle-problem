//Buffon-f�le t� probl�ma megval�s�t�sa


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

/*Strukt�ra meghat�roz�sa*/
typedef struct lista
{
	int sorszam;
	double x;
	double y;
	double fok;
	int vegeredmeny;
	struct lista *kovetkezo; /*Egy ir�nyba l�ncolt lista*/
}lista;

/*Kezd�lista felv�tele*/
lista *kezdolista;

/*Random koordin�ta gener�l� f�ggv�ny*/
void Random_koordinata(int n) 
{
	int i;														//Fut�v�ltoz�
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
	srand((unsigned) time(&t));									//K�v�l kell lennie a for cikluson, nemtudni mi�rt de tapasztalat szerint �gy m�k�dik, �gy nem.
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

		x=((xertek1*10)+(xertek2)+(xertek3/10)+(xertek4/100));	//A marad�kos oszt�sok elv�gz�se ut�n itt �s a k�vetkez� sorban visszalak�tunk. 
		y=((yertek1*10)+(yertek2)+(yertek3/10)+(yertek4/100));	//Val�j�ban az x koordin�ta felesleges  Pi meghat�roz�s�hoz, egyed�l az y a fontos t�nyez�. 

		fprintf(tfp,"x:%lf y:%lf ",x,y);						//F�jlba �r�s
	}
	fclose(tfp);												/*F�jl bez�r�sa*/
}

/*Random d�l�ssz�g gener�l� f�ggv�ny*/
void Random_dolesszog(int n)
{
	int i;
	time_t t;
	double fok[1];

	srand((unsigned) time(&t));										//A srand() mindig k�v�l van a cikluson, �s �gy m�k�dik. Ez nincs le�rva sehol, mert nekik evidens. :)

	FILE *bfp;														//Binary File Pointer
	char *fnev="dolesszog.dat";

	if((bfp = fopen(fnev, "wb")) != NULL) {
			fprintf(stderr,"\nSiker a %s file megnyitasakor!\n",fnev);}
	else{
		fprintf(stderr,"\nHiba a %s file megnyitasakor!\n",fnev);}	/*Else �gba ny�lik hogy ne legyen memory leak.*/

	for(i=0; i<n; i++)
	{
		fok[0] = rand() % 181;
		if(fwrite(fok,sizeof(double),1,bfp)!=1)						/*fok[1] t�mb t�pusban double m�ret� (a majdani sin f�ggv�ny miatt) 1db  elemet �runk bfp pointerrel.*/
			{
				fprintf(stderr,"\nHiba a dolesszogek kiirasakor!\n");
			}
	}
	fclose(bfp);													/*F�jl bez�r�sa*/
}

/*Pi ki�r�s�ra szolg�l� f�ggv�ny*/
lista *Pikiir(lista *kezdolista, int stick, int line)
{
	double szamlalo=0, nevezo=0;
	double valoszinuseg, pi, kulonbseg;
	lista *mozgo=kezdolista; /*Helyezz�k a lista legelej�r a mozgo pointert*/
	for(mozgo=kezdolista; mozgo!=NULL; mozgo=mozgo->kovetkezo) //Lista bej�r�sa
		{ 
			if((mozgo->vegeredmeny)==1)
				szamlalo++;
			if((mozgo->vegeredmeny)==0)
				nevezo++;
		}
	nevezo=nevezo+szamlalo;
	if(szamlalo==0)
	{
		printf_s("\nEgyetlen sikeres dob�s sincsen. Its not a bug, its a feature!\n");
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

/*A list�t fel�p�t� f�ggv�ny. Az adatokat a K�zponti f�ggv�nyb�l h�vja meg.*/
lista *Kezdolista_beszur(lista*eleje, double x, double y,double fi,int stick, int line, int i,int n)
{
	double egyseghossz=line, eredmeny=0;
	double h, h1, c;
	do{
		c=((stick*sin(fi))/2);
		h=egyseghossz-y;
		h1=line-h;
		if(((h>=0)&&(h<=c))||((h1>=0)&&(h1<=c))) /*Ezt m�g Buffon hat�rozta meg �gy. ANNO 1777*/
			{
				eredmeny=1;
			}
		else
			{	
				eredmeny=0;
				egyseghossz=egyseghossz+line;
			}
	}while((egyseghossz<=100) && (eredmeny==0)); /*Lehetne m�dos�tani (100+(stick/2)) -re is ak�r!*/

	lista *lp=(lista*)malloc(sizeof(lista)); 
	lp->x=x;
	lp->y=y;
	lp->fok=fi;
	lp->vegeredmeny=eredmeny;
	lp->sorszam=n-i;		//ELLEN�RZ� PONT //printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",lp->sorszam,lp->x,lp->y,lp->fok,lp->vegeredmeny);
	lp->kovetkezo=eleje;
	return lp;
}

/*Itt hajt�dik v�gre a f�jlb�l olvas�s �s az adatok tov�bb�sa a l�ncolt list�t megalkot� besz�r� f�ggv�nyhez*/
lista *Kozponti_fv(int n, int set, int line, int stick) 
{
	FILE *tfp;
	FILE *bfp;
	int i=0; //Sorsz�mot ad meg, null�t�l kezd�dik.
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

	//LISTA L�TREHOZ�S//

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

/*Rendez� f�ggv�ny, mely a k�v�nt set �rt�k szerint sorba rendez.*/
lista *Csere_fv(lista*kezdolista,int n, int set)
{
	int i;
	lista *bp=(lista*)malloc(sizeof(lista));
	if(bp==NULL){
		return 0;
	}
	lista *lemarado=kezdolista;
	lista *mozgo=kezdolista->kovetkezo;

	/*Minden egyes f�ggv�ny a set_fv ben meghat�rozott m�don rendezi sorba az elemeket. A rendez�s n*n-szer fut v�gig, mert ha az utols� elemmel akarunk valamit,
	p�ld�ul el�re hozni, akkor azt n-szer biztosan el�r�bb kell hozzni. A l�p�sz�m kij�n n^2-b�l. 
	Lehet hogy lehet kevesebb�l is, de h�zit �rok nem a NASA-t t�r�m fel! :)*/

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
	/*Ez az else �g kell, mert hisztizett a ford�t�*/
	else
		{
			return kezdolista;
		}
}

/*Az adott elemeket t�rl� f�ggv�ny*/
lista *Torol_fv(lista*kezdolista, int del,int stick, int line)
{ 
	/*H�rom esetre bontjuk sz�t*/

	lista *lemarado=NULL, *mozgo=kezdolista;
	while(mozgo!=NULL && mozgo->sorszam!=del) /*Itt keress�k meg az elemet, teh�t itt megy v�gbe a keres�s!*/
		{
			lemarado=mozgo; mozgo=mozgo->kovetkezo;
		}
	if(mozgo==NULL)
		{
			printf_s("\nNincs ilyen elem!\n"); /*Itt �llap�tja meg a f�ggv�ny, hogy van-e egy�ltal�n megfelel� eleme a sorozatnak.*/
			free(lemarado); //B�r a lemarado �gyis a NULL-ra mutat.
			return kezdolista;
		}
	if(lemarado==NULL)
	{ //Az els� cucc t�rl�s�sre. //Ha az els� cuccot kell t�r�lni.
		lista *ujkezdolista=mozgo->kovetkezo;
		free(mozgo);

		lista *mocorgo; //�jra ki�r�s, mert ez is a program feladata.
		printf_s("\nUjra kiiras!\n");
		for(mocorgo=ujkezdolista;mocorgo!=NULL;mocorgo=mocorgo->kovetkezo)
			{
				printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mocorgo->sorszam,mocorgo->x,mocorgo->y,mocorgo->fok,mocorgo->vegeredmeny);
			}
		ujkezdolista=Pikiir(ujkezdolista,stick,line);
		return ujkezdolista;
	}
	lemarado->kovetkezo=mozgo->kovetkezo; //K�zepe vagy v�ge
	free(mozgo);
	lista *mocorgo; /*Az�rt kellett a mocorgo pointer, mert nem lehetett tov�bb dolgozni a mozgo pointerrel, hiszen azt m�r felszabad�tottuk.*/
		printf_s("\nUjra kiiras!\n");
		for(mocorgo=kezdolista;mocorgo!=NULL;mocorgo=mocorgo->kovetkezo)
			{
				printf_s("\n%d. \t %lf \t %lf \t %lf \t %d\n",mocorgo->sorszam,mocorgo->x,mocorgo->y,mocorgo->fok,mocorgo->vegeredmeny);
			}
		kezdolista=Pikiir(kezdolista,stick,line);
	return kezdolista;
}

/*Adatbek�r� f�ggv�ny [n]*/
int dobas_szam()
{
	int n;
	double limit=2147483648; //Ennyit b�r az int. T�bbet nagyon �gyse �rdemes, mert itt �l�nk m�g j�v� ugyanekkor is.
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
	return n; /*Ezt is csak az�rt kellett, mert vis�tott a ford�t�...*/
}

/*Adatbek�r� f�ggv�ny [set]*/
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

/*Adatbek�r� f�ggv�ny [line]*/
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

/*Adatbek�r� f�ggv�ny [stick]*/
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

/*A "Szeretne-e t�r�lni?" f�ggv�ny*/
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

/*A "Melyik elemet szeretn� t�rl�lni?" f�ggv�ny [deln]*/
int del_beker(int deln)
{
	/*Nem feladata meg�llap�tani hogy m�r egy elemet t�r�lt�nk-e kor�bban a list�ban*/
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

/*A f� f�ggv�ny.*/
int main(){
	printf_s("\t\tWelcome User!\n");

	//ELS� F�ZIS - ADATOK BEK�R�SE//

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

	//M�SODIK F�ZIS - KOORDIN�TA �RT�KEK LEGENER�L�SA//

	Random_koordinata(n); //F�jlba is �r, minden zs�r ezzel a gy�ngyszemmel!

	//HARMADIK F�ZIS -  SZ�G �RT�KEK LEGENER�L�SA//

	Random_dolesszog(n); //J�, ha int �rt�kel nem megy, akkor csin�ljuk strukt�r�val! //S l�m, azzal megy :D

	//NEGYEDIK F�ZIS - KOORDIN�T�K �S SZ�GEK BEOLVAS�SA F�JLB�L L�NCOLT LIST�BA - A F� PROGRAMR�SZ //

	kezdolista=Kozponti_fv(n,set,line,stick);

	//�T�DIK F�ZIS - ELEMEK RENDEZ�SE//

	kezdolista=Csere_fv(kezdolista,n,set);
	if(kezdolista==NULL)
	{
		printf_s("\nHibas memoria foglalas, a program kilep!\n");
		return 0;
	}


	//HATODIK F�ZIS - ELEMEK T�RL�SE - PI �jb�li sz�molt �rt�k�nek ki�r�sa//
	
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

	//HETEDIK F�ZIS - K�nnyes b�cs�// 

	lista *torol;
	torol=kezdolista;
	while(torol!=NULL)						/*A teljes list�t fel kell szabad�tani.*/
	{
		lista *mostani=torol->kovetkezo;		/*K�sz�t�nk egy v�ltoz�t amit v�gigp�rget�nk a list�n.*/
		free(torol);						/*Felszabad�tjuk az el�z�leg dinamikusan lefoglalt ter�letet.*/
		torol=mostani;						/*A k�vetkez� elemre �ll�tjuk be a pointert.*/
	}
	kezdolista=NULL;						/*V�g�l a NULL-ba �ll�tjuk a kezdolista pointert. :"(*/

	printf_s("\n\t\tGoodby User!\t\t\n");
	getchar();
	return 0;
}

/*M�r csak az SDL-es meg�r�s maradt, de ezt a laborvezet�, Nagy Bal�zs megengedte, hogy december 4-�ig bemutassam, teh�t kitolta a hat�rid�t!*/