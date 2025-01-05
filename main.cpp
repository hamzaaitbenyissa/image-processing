#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

typedef unsigned char pixelT; //couleur
typedef pixelT* ptrPixelT;
typedef ptrPixelT* imgT;
typedef struct {
int	lg;
int h;
int r;
}dms;
//---------------------------creation---------------------------------------------------
imgT createImage(dms d){
	imgT img;
	long C=d.lg*d.r;//nbre de colonne
	long L=d.h*d.r;//nobre de ligne
	img=(imgT)malloc(L*sizeof(ptrPixelT));
	for(int i=0;i<L;i++)
	img[i]=(ptrPixelT)malloc(C*sizeof(pixelT));
	return img;	
}
//pour saiser les donnees
void saisir(int *ptrL,int *ptrH,int *ptrR){
	printf("Length : ");
	scanf("%d",ptrL);
	printf("Height : ");
	scanf("%d",ptrH);
	printf("Resolution : ");
	scanf("%d",ptrR);
}
//initialization d'image par des pixels

void initImg(imgT im,dms d){
		for(int i=0;i<d.r*d.h;i++)
			for(int j=0;j<d.r*d.lg;j++)
				 im[i][j]=rand()%256;
}
//affichage d'image
void afficher(imgT im,dms d){
	int L=d.h*d.r,C=d.lg*d.r;
	printf("Image{\n\tLength:%d\n\tHeight:%d\n\tPixelList:[\n"
	,d.lg,d.h);
	for(int i=0;i<L;i++){
		printf("\t\t");
		for(int j=0;j<C;j++)
			printf("%4u  ",im[i][j]);
		printf("\n\n");
	}
	printf("]}\n");
}

//faire une copie:
void copier(imgT cop,imgT org,dms d){
int L=d.h*d.r,C=d.lg*d.r;
for(int i=0;i<L;i++)
for(int j=0;j<C;j++)
cop[i][j]=org[i][j];		
}
//--------------------------------------une file dynamique------------------------------

//definir une file dynamique
typedef struct{
	int x,y;
}coordt1;

typedef struct coordT {
	int x,y;//coordonnees d'un pixel
	struct coordT  *suivant;
}coordt;


typedef struct{
	coordt *tete,*queue;	
}filet;

//operations sur la file:

void enfiler(filet *ptrF,coordt1 px){
	coordt *p=(coordt *)malloc(sizeof(coordt));
	p->x=px.x;
	p->y=px.y;
	p->suivant=NULL;
	if(ptrF->queue)
		ptrF->queue->suivant=p;
	else
		ptrF->tete=p;
	ptrF->queue=p;
}

int estVide(filet f){
	return f.queue==NULL;
}	
	
coordt1 defiler(filet *ptrF){
 	coordt1 c={-1,-1};//synonyme d'une file vide
 	coordt *s;
 	if(ptrF->tete){
 		s=ptrF->tete;
 		c.x=s->x;
 		c.y=s->y;
	 	if(ptrF->tete==ptrF->queue)
 			ptrF->tete=ptrF->queue=NULL;
		 else
 			ptrF->tete=s->suivant;
 		free(s);s=NULL;
	}
 	return c;
 }



//-------------------------------------des effets---------------------------------------------

//-----------------------------------transpose d'une matrice----------------------------------
void img_transpose(imgT imgtr/*destination*/,imgT img/*source*/,dms d){
	for(int i=0;i<d.h*d.r;i++)
        for(int j=0;j<d.lg*d.r;j++)
            imgtr[i][j]=img[j][i];
}


//L'effet de miroir Horisontal
void Miroir_H(imgT mir/*destination*/,imgT img/*source*/,dms d){
    int L=d.h*d.r,C=d.lg*d.r;
	int i,ii;
	copier(mir,img,d);
	for(i=0,ii=L-1       ;     i<ii   ;    i++,ii--){
		ptrPixelT temp=img[i];
		mir[i]=mir[ii];
		mir[ii]=temp;
	}
}

//L'effet de miroir vertical

void Miroir_V(imgT mir/*destination*/,imgT img/*source*/,dms d){
	dms dt={d.h,d.lg,d.r};
	imgT img_temp1=createImage(dt);
	imgT img_temp2=createImage(dt);
	img_transpose(img_temp2,img,dt);
	Miroir_H(img_temp1,img_temp2,dt);
	img_transpose(mir,img_temp1,d);
}

//remplissage de l'image
void remplir(imgT im /*destination*/,imgT img/*source*/,dms d,coordt1 px,pixelT cn){
	copier(im,img,d);
	filet f={NULL,NULL};//file vide;
	coordt1 px2,px1;
	enfiler(&f,px);
	pixelT c0=im[px.x][px.y];
	while(!estVide(f)){
		px1=defiler(&f);
		im[px1.x][px1.y]=cn;
		if(px1.x>0 && im[px1.x-1][px1.y]==c0){
			px2.x=px1.x-1;px2.y=px1.y;
			enfiler(&f,px2);	
		}
		if(px1.y>0 && im[px1.x][px1.y-1]==c0){
			px2.x=px1.x;px2.y=px1.y-1;
			enfiler(&f,px2);	
		}
		if(px1.x<d.h*d.r-1 && im[px1.x+1][px1.y]==c0){
			px2.x=px1.x+1;px2.y=px1.y;
			enfiler(&f,px2);	
		}
		if(px1.y<d.lg*d.r-1 && im[px1.x][px1.y+1]==c0){
			px2.x=px1.x;px2.y=px1.y+1;
			enfiler(&f,px2);	
		}
	}
}



//-----------------------------------trier une image----------------------------------
//chercher le min d'un ligne
int ligne_min(imgT M,dms d,int i0,int j0){
int L=d.h*d.r,C=d.lg*d.r;
pixelT min=M[i0][j0];int i,j,indice=i0;
for(i=i0;i<L;i++){
if(i==i0) {for(j=j0;j<C;j++) if(min>M[i][j]) {indice=i;min=M[i][j];} }
else{
for(j=0;j<C;j++)
if(min>M[i][j]) { indice=i;min=M[i][j];}}}
return indice;
}


//chercher le min d'un colonne
int colonne_min(imgT M,dms d,int i0,int j0){
int L=d.h*d.r,C=d.lg*d.r;
pixelT min=M[i0][j0];int i,j,indice=j0;
for(i=i0;i<L;i++){
if(i==i0) { for(j=j0;j<C;j++) if(min>M[i][j]) {indice=j;min=M[i][j];}}
else{
for(j=0;j<C;j++)
if(min>M[i][j]) {indice=j;min=M[i][j];}	}}
return indice;
}

//Echanger deux pixelles
void echanger(imgT M,int i0 , int j0,int i1,int j1){
pixelT aux;
aux=M[i0][j0];	
M[i0][j0]=M[i1][j1];	
M[i1][j1]=aux;	
}

//algorithme de tri
void trier(imgT M,dms d){
int L=d.h*d.r,C=d.lg*d.r;
int i,j;	
for(i=0;i<L;i++)	
for(j=0;j<C;j++)	
echanger(M,i,j,ligne_min(M,d,i,j),colonne_min(M,d,i,j));
}


//------------------------------------coleur dominante-------------------------------
// savoir nombre des differents coleurs d'une image
int distinct(imgT M,dms d){
imgT MC=createImage(d);
copier(MC,M,d);
int L=d.h*d.r,C=d.lg*d.r;
trier(MC,d);int s=1,i,j;
for(i=0;i<L;i++){
for(j=0;j<C-1;j++)
if(MC[i][j]!=MC[i][j+1]) s++;
if(i!=L-1 && MC[i][C-1]!=MC[i+1][0]) s++;
}
return s;
}

//les coleurs d'une image
ptrPixelT coleurs(imgT img,dms d){
int L=d.h*d.r,C=d.lg*d.r;
ptrPixelT clr=(ptrPixelT)malloc(sizeof(pixelT)*distinct(img,d));
imgT MC=createImage(d);
copier(MC,img,d);
trier(MC,d);int s=1,i,j;
clr[0]=MC[0][0];
for(i=0;i<L;i++){
for(j=0;j<C-1;j++)
if(MC[i][j]!=MC[i][j+1])                {clr[s]=MC[i][j+1];s++;}
if(i!=L-1 && MC[i][C-1]!=MC[i+1][0])    {clr[s]=MC[i+1][0];s++;}	
}
return clr;
}

//frequance d'une couleur dans une image
float frequance(imgT img,dms d,pixelT c){
int s=0;
float t;
for(int i=0;i<d.r*d.h;i++){		
		for(int j=0;j<d.r*d.lg;j++)
			if(img[i][j]==c) s++;
}
t=float(s*100)/float(d.lg*d.h*d.r*d.r);
return t;
}


//afficher des couleurs d'une image et leurs frequances
void affiher_tab_clr(imgT img,dms d){
ptrPixelT m=coleurs(img,d);
float max=frequance(img,d,*(m));int indice_max=0;
for(int i=0;i<distinct(img,d);i++){
printf("color%d : %u----------->%.2f %%\n\n",i,*(m+i),frequance(img,d,*(m+i)));
if(max<frequance(img,d,*(m+i))){ max=frequance(img,d,*(m+i));indice_max=i;}    
}
printf("\n\nThe dominant color is :%4u",*(m+indice_max));
}
void Negatif(imgT neg/*destination*/,imgT img/*source*/,dms d){
			for(int i=0;i<d.r*d.h;i++)
			for(int j=0;j<d.r*d.lg;j++){
				 if(img[i][j]<128) neg[i][j]=0;
	        else neg[i][j]=1;
}
}


//--------------------------------------------MENU------------------------------------
void Menu(){
printf("*******************AIT BEN YISSA HAMZA************************\n\n");
printf("* 1---->    Create image                                 *\n");
printf("* 2---->    Show image                                   *\n");
printf("* 3---->    Frequencies                                  *\n");
printf("* 4---->    Negative                                     *\n");
printf("* 5---->    Fill                                         *\n");
printf("* 6---->    Horizontal Mirror                            *\n");
printf("* 7---->    Vertical Mirror                              *\n");
printf("* 8---->    Clear screen                                 *\n");
printf("* 9---->    Menu                                         *\n");
printf("*10---->    Quit                                         *\n");
printf("********************ENSET GLSID S1****************************\n\n");
}

// main menu:
int menu(){
int c;
do{
printf("Enter your choice :  "); scanf("%d",&c);
printf("\n\n");
if(c<1 || c>10) {printf("\tInvalid choice !\n\n");sleep(1);};
}while(c<1 || c>10);
return c;
}

// -------------------------------------------------application-----------------------------------
int main(){
	int lg,h,r,a,k=0,choice;dms d;
	coordt1 crd;
    int cn;
	imgT img,img_remplissage,img_miroir_V,img_miroir_H,img_negatif;
    Menu();
    do{
        choice=menu();

        switch (choice){
            case 1:
                    saisir(&lg,&h,&r);
	                d.lg=lg;d.h=h;d.r=r;
	                img=createImage(d);
	                initImg(img,d);
	                k++;
	            printf("\n\n\t Operation completed successfully.  \n\n");
					sleep(1);
					   break;
            case 2:
            	if(k==0) {printf("\nPlease create an image first!\n\n"); break;}
	            	afficher(img,d);
                    printf("\n\n\t Operation completed successfully.  \n\n");
					sleep(1);
					break;
			case 3:
				if(k==0) {printf("\nPlease create an image first!\n\n"); break;}
				    affiher_tab_clr(img,d);
				    printf("\n\n");
                    sleep(1);
					break;
		    case 4:
                   if(k==0) {printf("\nPlease create an image first!\n\n"); break;}
				  img_negatif=createImage(d);
                  Negatif(img_negatif,img,d);
			      afficher(img_negatif,d);
                  printf("\n\n\t Operation completed successfully.  \n\n");
                  sleep(1);
                  break;
				    break;
			case 5:
			      if(k==0) {printf("\nPlease create an image first!\n\n"); break;}
			      printf("\nPlease enter the coordinates  :  \n\n");
			      printf("\tX :  ");  scanf("%d",&crd.x);
			      printf("\tY :  ");  scanf("%d",&crd.y);
                  printf("New pixel  :  "); scanf("%u",&cn);printf("\n\n");
                  img_remplissage=createImage(d);
				  remplir(img_remplissage,img,d,crd,cn);
				 // remplir(img_remplissage,img,d,crd,0);
                  afficher(img_remplissage,d);
                  printf("\n\n\t Operation completed successfully.  \n\n");
                  sleep(1);
					break;
			case 6:
			if(k==0) {printf("\nPlease create an image first!\n\n"); break;}
                img_miroir_H=createImage(d);
                Miroir_H(img_miroir_H,img,d);
				afficher(img_miroir_H,d);
                  printf("\n\n\t Operation completed successfully.  \n\n");
                  sleep(1);
                  break;
			case 7:
				if(k==0) {printf("\nPlease create an image first!\n\n"); break;}
				img_miroir_V=createImage(d);
                Miroir_V(img_miroir_V,img,d);
				afficher(img_miroir_V,d);
                  printf("\n\n\t Operation completed successfully.  \n\n");
                  sleep(1);
			 break;
			case 8:system("cls");Menu(); break;
			case 9:Menu(); break;

			case 10: printf("\n\tThank you for using our service.\n\n");
			default:
			    break;
			}
				}while (choice!=10);
    return 0;
}
