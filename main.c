


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define faux 0
#define vrai 1

typedef struct date{
    char  jour[4];
    char  mois[4];
    char  annee[4];
} date;
 /**les_tableau_utilis�**/
char Groupe_Sanguin[8][5]={"O+","A+","B+","O-","A-","AB+","B-","AB-"};
char Region_militaire[7][25]={"1RM-Blida","2RM-Oran","3RM-Bechar","4RM-Ouargla","5RM-Constantine","6RM-Tamanrasset"};
char Force_armee[8][47]={"Armee_de_terre","Armee_de_l_air","Marine_nationale","Defense_aerienne_du_territoire","Gendarmerie_nationale","Garde_republicaine","Departement_du_renseignement_et_de_la_securite","Sante_militaire"};
char Grade[18][30]={"General_de_corps_d_armee","General-Major","General","Colonel","Lieutenant-colonel","Commandant","Capitaine","Lieutenant","Sous-lieutenant","Aspirant","Adjudant-chef","Adjudant","Sergentchef","Sergent","Caporal-chef","Caporal","Djoundi"};
char Wilaya_naissance[48][20]={"Adrar","Chlef","Laghouat","Oum El_Bouaghi","Batna","Bejaia","Biskra","Bechar","Blida","Bouira","Tamenrasset","Tebessa","Telemcen","Tiaret","Tizi_Ouzou","Alger","Djelfa","Jijel","Setif","Saida","Sakikda","Sidi Bel_Abbas","Annaba","Guelma","Constantine","Medea","Mostaganem","M'sila","Mascara","Ouargla","Oran","El_Bayedh","Illizi","Bordj Bou_Arreridj","Boumerd�s","El_Taref","Tindouf","Tissemsilt","El_Oued","Khenchela","Souk_Ahras","Tipaza","Mila","Ain_Defla","Naama","Ain_Temouchent","Ghardaia","Relizane"};
char Alpha[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','K','R','S','T','U','V','W','X','Y','Z'};
/** DECLARATION DES TYPES : */
typedef int boolean ;
/** TYPE TBLOC : STRUCTURE DU BUFFER, OU BLOC DANS LE FICHIER */
typedef struct
{
    char tab[1024];
    int suiv;
}Tbloc;
/** TYPE TENTETE : STRUCTURE DE L'ENTETE DU FICHIER */
typedef struct
{
    int tete;
    int queue;
    int pos_vide;
    int nb_supp;
}Tentete;
/** TYPE LNOVC : STRUCTURE DU FICHIER : LISTE NON ORDONNE DE TAILLE VARIABLE AVEC CHEVAUCHEMENT */
typedef struct
{
    FILE *fichier;
    Tentete entete;
}LNOVC;
/**------------------------------------------------------------------------------------------------**/
/**        Corps des modules d'impl�mentation du mod�le du TP 2 sur les fichiers LNOVC             */
/**------------------------------------ -----------------------------------------------------------*/
/**1*/ void ouvrir(LNOVC *f,char *nomfich,char *mode)///Ouverture d'un fichier en mode nouveau ou ancien
{
    if ((strcmp(mode,"n")==0)||(strcmp(mode,"N")==0))///Fichier inexistant
    {
        f->fichier=fopen(nomfich,"wb+");///Mode d'�criture car nouveau fichier
        ///Initialisation de l'entete du fichier
        f->entete.tete=0;
        f->entete.queue=0;
        f->entete.pos_vide=0;
        f->entete.nb_supp=0;
        fseek(f->fichier,0,SEEK_SET);
        fwrite(&f->entete,sizeof(Tentete),1,f->fichier);///Ecriture de l'entete dans le bloc 0 du
        fseek(f->fichier,0,SEEK_SET);                   ///fichier pour la sauvegarde en m�moire secondaire
    }
    else
    {
        if ((strcmp(mode,"a")==0)||(strcmp(mode,"A")==0))///Fichier existant
        {
            f->fichier=fopen(nomfich,"rb+");///Mode de lecture car ancien fichier
            if ((f->fichier)!=NULL)
            {
                ///R�cuperation de l'ent�te a partir du bloc 0 du fichier (contenant l'ent�te)
                fseek(f->fichier,0,SEEK_SET);
                fread(&f->entete,sizeof(Tentete),1,f->fichier);
                fseek(f->fichier,0,SEEK_SET);
            }
            else
            {
                printf("\nLE FICHIER N'EXISTE PAS.\n");
            }
        }
        else
        {
            printf("\nENTREZ UN MODE D'OUVERTURE VALABLE.\n");
        }
    }
}
/**2*/ void fermer(LNOVC *f)///Fermeture d'un fichier
{
    fclose(f->fichier);
}

/**3*/ void liredir(LNOVC *f,int i,Tbloc *buf)///Recuperation du contenu du bloc i du fichier dans buf
{
    if (f->fichier!=NULL)
    {
        fseek(f->fichier,sizeof(Tentete)+(i-1)*sizeof(Tbloc),SEEK_SET);///Car le fichier contient le bloc 0 (ent�te)
        fread(buf,sizeof(Tbloc),1,f->fichier);///On r�cup�re le bloc de l'adresse correspondante du fichier
        fseek(f->fichier,0,SEEK_SET);
    }
    else
    {
        printf("\nERREUR, FICHIER INEXISTANT.\n");
    }
}

/**4*/ void ecriredir(LNOVC *f,int i,Tbloc buf)///Ecriture du contenu de buf dans le bloc i du fichier
{
    fseek(f->fichier,sizeof(Tentete)+(i-1)*sizeof(Tbloc),SEEK_SET);///Car le fichier contient le bloc 0 (entete)
    fwrite(&buf,sizeof(Tbloc),1,f->fichier);///On �crit le bloc dans le fichier � l'adresse correspondante
    fseek(f->fichier,0,SEEK_SET);
}

/**5*/ int entete(LNOVC *f,int i)///Renvoie l'entete N�i du fichier
{
    switch (i)
    {
        case 1:
            return f->entete.tete;
        break;

        case 2:
            return f->entete.queue;
        break;

        case 3:
            return f->entete.pos_vide;
        break;

        case 4:
            return f->entete.nb_supp;
        break;
    }
    return 0;
}
/**6*/ void aff_entete(LNOVC *f,int i,int val)///Affecte val � l'ent�te N�i du fichier
{
    switch (i)
    {
        case 1:
            f->entete.tete=val;
        break;

        case 2:
            f->entete.queue=val;
        break;

        case 3:
            f->entete.pos_vide=val;
        break;

        case 4:
            f->entete.nb_supp=val;
        break;
    };
    fseek(f->fichier,0,SEEK_SET);///On acc�de au bloc 0 (ent�te)
    fwrite(&f->entete,sizeof(Tentete),1,f->fichier);///Mise � jour de l'entete dans le fichier
    fseek(f->fichier,0,SEEK_SET);
}
/**7*/ int allocbloc(LNOVC *f)///Alloue un bloc dans le fichier, et renvoie son adresse
{
    Tbloc buf;
    ecriredir(f,entete(f,2)+1,buf);///Ajoute un bloc vide au fichier � l'adresse (queue+1)
    aff_entete(f,2,entete(f,2)+1);///Mise � jour de l'entete (queue)
    aff_entete(f,3,0);///Mise � jour de la premi�re position libre
    if (entete(f,1)==0)///Si c'est le 1er bloc � ins�rer (fichier vide)
    {
        aff_entete(f,1,entete(f,1)+1);///On met � jour l'adresse de la nouvelle t�te
    }
    else
    {
        liredir(f,entete(f,2)-1,&buf);///R�cup�ration du dernier bloc (ancienne queue,avant allocation)
        buf.suiv=entete(f,2);///Mise � jour de son suivant (vers le bloc allou�), pour pr�server le cha�nage
        ecriredir(f,entete(f,2)-1,buf);///Ecriture de ce bloc dans le fichier (nouveau suivant)
    }
    return entete(f,2);///On renvoie l'adresse du bloc allou� (nouvelle queue)
}
/**Matricule*************************************************************************************/
char *matr()
{
    int i;
    static char m[6];

  i = (rand() % (999999 - 111111)) + 111111;
    sprintf(m,"%d",i);
   return m;
}
/**Nom/Pr�nom************************************************************************************/
char  *nom_pre()
{
    int i=0,j=0,r=0;

    i =(rand() % (30 - 4)) + 4;// rand()%(30)+4;
   static char n_p[40];
    while(j<i)
    {
        r =(rand() % (25 - 1)) + 1;// rand()%(25-0+1);
        n_p[j]=Alpha[r];
        j++;
    }
     n_p[i+1]='\0';
    return n_p;
}
/**ann�e_qui_n'est_pas_complet*******************************************************************/
int anneeb(int *an)
{

if (*an>1581)
{
if(*an%400==0)
{
return 1;
}
if((*an%4==0) && (*an%100 != 0) )
{
return 1;
}
else
{
return 0;
}
}
else
{
    return 0;
}
}
/**la date_du_naissance*******************************************************************/
void date_naiss(date *d)
{
    int ans,moi,jou;
    char taille1[4]={0},taille2[4]={0},taille3[4]={0};
    ans=(rand() % (1999 - 1940)) + 1940;//rand()%(1999-1940+1)+1940;
     moi=(rand() % (12 - 1)) + 1;//rand()%(12)+1;

     if (anneeb(&ans) == 1 )
        {
         if (moi==2)
         {
             jou = (rand() % (28 - 1)) + 1;//rand()%(28)+1;
         }
    else
            {
             jou = (rand() % (31 - 1)) + 1;//rand()%(31)+1;
            }
         }
     else
        {
         if (moi==2)
         {
             jou = (rand() % (29 - 1)) + 1;//rand()%(29)+1;
         }
         else
         {
        jou= (rand() % (31 - 1)) + 1;//rand()%(31)+1;
         }
         }
     itoa(ans,taille1,10);
     itoa(moi,taille2,10);
     itoa(jou,taille3,10);

     strcpy(d->annee,taille1);
     strcpy(d->mois,taille2);
     strcpy(d->jour,taille3);
}
/**les_wiliya********************************************************************************/
char *wilaya()
{
    int i ;
    i = (rand() % (48 - 1)) + 1;//rand()%(48 +1);
    return Wilaya_naissance[i];
}
/**les_groupe_sanguin************************************************************************/
char *grou_sang()
{
    int i ;
    i = (rand() % (8 - 1)) + 1;//rand()%(8 );
    return Groupe_Sanguin[i];
}
/**grade*************************************************************************************/
char *grade()
{
    int i ;
    i =(rand() % (17 - 1)) + 1;// rand()%(17);
    return Grade[i];
    free(Grade[i]);
}
/**force arm�e*******************************************************************************/
char *force_arm()
{
    int i ;
    i = (rand() % (7 - 1)) + 1;//rand()%(7);
    return Force_armee[i];
    free(Force_armee[i]);
}
/**r�gion_militaire***************************************************************************/
char *reg_mili()
{
    int i ;
    i =(rand() % (6 - 1)) + 1;// rand()%(6);
    return Region_militaire[i];
    free(Region_militaire[i]);
}
///Proc�dure d'affichage du contenu d'un fichier LNOVC
void ecritlnovc (char *nomfich)
{
    LNOVC f;
    Tbloc buf;
    int p,i;
    ouvrir(&f,nomfich,"a");
    printf("\n-------------- Affichage du fichier ----------------");
    if ((f.fichier)!=NULL)
    {
        //On commence par afficher les ent�tes
        printf("\nEntete 1 (Tete)    : %d",entete(&f,1));
        printf("\nEntete 2 (Queue)   : %d",entete(&f,2));
        printf("\nEntete 3 (Pos_vide): %d",entete(&f,3));
        printf("\nEntete 4 (nb_sup)  : %d",entete(&f,4));
        p=entete(&f,1);//On r�cup�re l'adresse de la t�te
        i=0;
        if (p!=-1)
        {
            while (!(((p==entete(&f,2))&&((i==entete(&f,3)))))&&(p!=-1))
                //Tant que (on est pas en m�me temps arriv� � la queue ET � la derni�re position de la queue
                //Ou que l'on a pas parcouru tout le fichier
            {
                liredir(&f,p,&buf);//On r�cup�re le bloc
                printf("\nBloc %d :\n ",p);
                if (p==entete(&f,2))//Si l'on est arriv� � la queue
                {
                    for (i=0;i<entete(&f,3);i++)//On parcourt la queue jusqu'� la premi�re position vide du fichier
                                                //et non tout le bloc
                    {
                        printf("%c",buf.tab[i]);
                    }
                }
                else
                {
                    for (i=0;i<1024;i++)//Nous ne sommes pas � la queue, donc on parcourt tout le bloc
                    {
                    {
                        printf("%c",buf.tab[i]);
                    }
                }
                p=buf.suiv;;//On passe au bloc suivant
            }
        }
        }
        else
        {
            printf("IL N'Y A PAS DE BLOC DE DONNEES");
        }
    }
    printf("\n----------------------------------------------------");
    fermer(&f);
}
///insertion d'un nouvel enregistrement
void insertionlnovc(char *nomfich,char chaine1[1024])
{
    LNOVC f;
    Tbloc buf,buf1,buf2;//buf et buf2 sont d�clar�es ici malgr� leur inutilit� dans le programme car les enlever causerait
date d;
                  //des erreurs au niveau de l'�x�cution
    int i,j,trouv,m,q,p,k,tai;
    char taille[3]={0};
    srand(time(NULL));
    ouvrir(&f,nomfich,"a");
    {
        p=entete(&f,2);//On r�cup�re la queue du fichier
        if (p!=0)//Si le fichier n'est pas vide
        {
            liredir(&f,p,&buf1);//On r�cup�re le bloc de la queue
        }

           j=-1;
    buf.tab[1024]="";

        strcat(chaine1,matr());

//        strcat(chaine1,"|");
        strcat(chaine1,nom_pre());
        strcat(chaine1,"|");
        date_naiss(&d);
        strcat(chaine1,d.annee);
        strcat(chaine1,"|");
        strcat(chaine1,d.mois);
        strcat(chaine1,"|");
        strcat(chaine1,d.jour);
        strcat(chaine1,"|");
        strcat(chaine1,wilaya());
        strcat(chaine1,"|");
        strcat(chaine1,grou_sang());
        strcat(chaine1,"|");
        strcat(chaine1,grade());
        strcat(chaine1,"|");
        strcat(chaine1,force_arm());
        strcat(chaine1,"|");
        strcat(chaine1,reg_mili());
        strcat(chaine1,"\n");
        printf("\nLA DONNEE EST : %s\n",chaine1);
         tai=strlen(chaine1)+4;
        itoa(tai,taille,10);///On convertit la taille en entier
        taille[3]='\0';
        if (tai<100) ///Pour que les trois positions du champ 'taille' soient toutes pleines
        {
            taille[2]=taille[1];
            taille[1]=taille[0];
            taille[0]='0';
        }
        chaine1[1024]='\0';
        k=entete(&f,3);//On r�cup�re la premi�re position vide du fichier, pour y ins�rer la donn�e
        for (m=0;m<=tai;m++)
        {
            if (k>=1024)
            {
                k=0;//On passe � la premi�re position du bloc suivant
                q=p;//On sauvegarde l'adresse du bloc pr�c�dent
                p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y ins�rer la donn�e
                aff_entete(&f,2,p);//On met � jour la nouvelle queue dans l'ent�te
                buf1.suiv=p;//On cha�ne l'ancienne queue avec la nouvelle
                ecriredir(&f,q,buf1);//On met � jour l'ancienne queue dans le fichier
                aff_entete(&f,3,k);//On met � jour l'adresse de la queue dans l'ent�te
            }
            if ((m>=0)&&(m<=2))
            {
                buf1.tab[k]=taille[m];//On remplit le champ 'taille' avec la taille de la donn�e
            }
            if (m==3)
            {
                buf1.tab[k]='0';//On met la champ 'effac�' � '0'
            }
            if ((m>=4))
            {
                buf1.tab[k]=chaine1[m-4];
            }

            k++;//Le remplissage se fait caract�re par caract�re, donc on passe � la position suivante
                //dans buf
        }
        strcpy(chaine1,"");
        buf1.suiv=-1;//On cha�ne la nouvelle queue � NIL
        ecriredir(&f,p,buf1);//On �crit la derni�re queue dans le fichier
        aff_entete(&f,3,k); //On met � jour la premi�re position vide, et on met k � sa place
                            //car le k pointe la d�rni�re position ins�r�e +1 . Si le bloc est plein on aura j+1=b+1
    }
    fermer(&f);
}

///chargement initial::::
void chargement_init(char *nomfich, int n)
{
/**    partie de d�claration*/
    date d;
    char chaine1[1024]={0};
    char  entrer[1024]={0};
    char taille[3]={0};

    int ran;
    Tbloc buf;;
    int i,p,j,tai,m;
    LNOVC f;
   // Tbloc buf;
srand(time(NULL));
    ouvrir(&f,nomfich,"n");
    j=-1;
    buf.tab[1024]="";
    /**    partie de traitement du programme*/
   for(i=0;i<n;i++)
       {

        strcat(chaine1,matr());

       strcat(chaine1,"|");
        strcat(chaine1,nom_pre());
        strcat(chaine1,"|");
        date_naiss(&d);
        strcat(chaine1,d.annee);
        strcat(chaine1,"|");
        strcat(chaine1,d.mois);
        strcat(chaine1,"|");
        strcat(chaine1,d.jour);
        strcat(chaine1,"|");
        strcat(chaine1,wilaya());
        strcat(chaine1,"|");
        strcat(chaine1,grou_sang());
        strcat(chaine1,"|");
        strcat(chaine1,grade());
        strcat(chaine1,"|");
        strcat(chaine1,force_arm());
        strcat(chaine1,"|");
        strcat(chaine1,reg_mili());
        strcat(chaine1,"\n");
        tai=strlen(chaine1)+4;
        itoa(tai,taille,10);///On convertit la taille en entier
        taille[3]='\0';
        if (tai<100) ///Pour que les trois positions du champ 'taille' soient toutes pleines
        {
            taille[2]=taille[1];
            taille[1]=taille[0];
            taille[0]='0';
        }
        chaine1[1024]='\0';
        ran=strlen(chaine1)+4;
           for (m=0;m<=tai;m++)               ///On remplit le buffer caract�re par caract�re
           {
            j++;
            if (j>=1024)
                {
                 j=0;
                 buf.suiv=-1;
                 p=allocbloc(&f);
                 ecriredir(&f,p,buf);
                }
             if ((m>=0)&&(m<=2))  ///On remplit le champ 'taille'
              {
                buf.tab[j]=taille[m];
              }
            if (m==3)  ///On met le champ 'effac�' � 0
              {
                buf.tab[j]='0';
              }
            if (m>=4)
              {
                buf.tab[j]=chaine1[m-4];
              }
           }
           strcpy(chaine1,"");
}
    buf.suiv=-1;                /**On cha�ne le dernier buffer � NIL**/
    p=allocbloc(&f);            /**On alloue un nouveau bloc pour l'insertion du dernier buffer**/
    ecriredir(&f,p,buf);        /**On ins�re le dernier buffer dans le fichier**/
    aff_entete(&f,3,j+1);       /**car le j+1 pointe la d�rni�re position ins�r�e+1 , donc la position libre.**/
                                /**Si le bloc est plein on aura j+1=b+1**/


  fermer(&f);
}
///recherche d'un enregistrement
void reche1(char *nomfich,char matricul[15],int *trouv,int *i,int *j)
{

    int l=1;
        LNOVC f;
    Tbloc buf;
    int k=0 , p=0 , tai=0 , temp=0 , m=0 ;
char *token = NULL;
    char taille[5]={0},chaine[15];
        ouvrir(&f,nomfich,"a");
     printf("\n***************");
            p=entete(&f,1);///On r�cup�re l'adresse de la tete du fichier
               k=0;///k va repr�senter la variable de parcours du bloc
            ///On r�cup�re le bloc de la t�te
           // printf("\nsuivant=%d",buf.suiv);

   while ((p!=-1)&&(*trouv==0)&&(l<entete(&f,3)))
          {
            ///On affecte � temp la position courante
liredir(&f,p,&buf);
            *j=k;
            for (m=1;m<=4;m++)
            {
                if (k>=1024)
                {
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                taille[m-1]=buf.tab[k];///On r�cup�re le champ taille qui se trouve dans le fichier
                k++;
            }
           taille[4]='\0';              ///Pour indiquer la fin du champ 'taille'
           tai=atoi(taille);           ///On convertit la chaine r�cup�r�e en entier
           ///On passe � la position suivante, car le champ 'effac�' ne nous int�resse pas ici
           if (k>=1024)///A chaque fois qu'on incr�mente la position courante, on doit v�rifier si elle
                    ///ne d�passe pas la taille du bloc
            {
                k=0;
                p=buf.suiv;
                liredir(&f,p,&buf);
            }
              for (m=1;m<=tai;m++)
                {
                    if (k>=1024)
                {
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                  chaine[m-1]=buf.tab[k];
                  k++;
                }
               chaine[15]='\0';
               matricul[15]='\0';
    m=strtok(chaine,"|");
    while(m!=NULL)
    {
        if (strcmp(matricul,m)==0)
        {
*trouv=1;
            m=strtok(NULL,"|");
    }
        else
                {
                 k=(k+tai)-9;
                if (k>=1024)
                {   ///On passe au d�but de l'enregistrementsuivante, dans le bloc suivant
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                     *i=p;///On affecte � i l'adresse du bloc courant
                 }
        }
            if ((p==entete(&f,2))&&(k==entete(&f,3))) /// On est arriv�s � la premi�re position vide du fichier
            {
                p=-1; /// Pour sortir de la boucle
            }
          ///On affecte � j temp qui repr�sente la position de l'enregistrementdans le bloc
  l++;
  }
  fermer(&f);
}

///�puration::::::::::::::
void epuration(int array_matricule [3],int array_nom [3][15],int array_date [3][3],int array_wilaya [3],int array_sang [3],int array_grade [3],int array_force [3],int array_region [3],int sup_log[3],int n,int *nb,int *b)
{
    FILE *f = fopen("PERSONNEL-ANP_DZ.bin","wb");

       int i,cpt = 0;
    *b = 0;
    for (i = 0;i < (n - 1);i++)
    {
        if (array_matricule [i] == array_matricule [i + 1])
        {
            sup_log [i] = 0;
            cpt ++;
            *b = 1;
        }
    }
    nb = (n - cpt);
    for(i = 0;i < nb ;i++)
		{
			fprintf(f,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",array_matricule [i],array_nom[i][0],array_nom[i][1],array_nom[i][2],array_nom[i][3],array_nom[i][4],array_nom[i][5],array_nom[i][6],array_nom[i][7],array_nom[i][8],array_nom[i][9],array_nom[i][10],array_nom[i][11],array_nom[i][12],array_nom[i][13],array_nom[i][14],array_date [i][0],array_date [i][1],array_date [i][2],array_wilaya [i],array_sang [i],array_grade [i],array_force [i],array_region [i],sup_log[i]);
		}
    fclose(f);
}


///recharche par matricule::::::::::::

void recherche_matricul(char *nomfich,char matricul[6],int *trouv,int *i,int *j)
{//recherche sur un enregistrement relatif � une le matricule
    int l=1;
        LNOVC f;
    Tbloc buf;
    int k=0 , p=0 , tai=0 , temp=0 , m=0 ;

    char taille[5]={0},chaine[6]={0};
        ouvrir(&f,nomfich,"a");
     printf("\n***************");
            p=entete(&f,1);///On r�cup�re l'adresse de la tete du fichier
               k=0;///k va repr�senter la variable de parcours du bloc
            ///On r�cup�re le bloc de la t�te
           // printf("\nsuivant=%d",buf.suiv);

   while ((p!=-1)&&(*trouv==0)&&(l<entete(&f,3)))
          {
            ///On affecte � temp la position courante
liredir(&f,p,&buf);
            *j=k;
            for (m=1;m<=4;m++)
            {
                if (k>=1024)
                {
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                taille[m-1]=buf.tab[k];///On r�cup�re le champ taille qui se trouve dans le fichier
                k++;
            }
           taille[3]='\0';              ///Pour indiquer la fin du champ 'taille'
           tai=atoi(taille);           ///On convertit la chaine r�cup�r�e en entier
           ///On passe � la position suivante, car le champ 'effac�' ne nous int�resse pas ici
           if (k>=1024)///A chaque fois qu'on incr�mente la position courante, on doit v�rifier si elle
                    ///ne d�passe pas la taille du bloc
            {
                k=0;
                p=buf.suiv;
                liredir(&f,p,&buf);
            }
              for (m=1;m<=6;m++)
                {
                    if (k>=1024)
                {
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                  chaine[m-1]=buf.tab[k];
                  k++;
                }
               chaine[6]='\0';
               matricul[6]='\0';
               if (strcmp(chaine,matricul)==0)
                {
                *trouv=1;
                }
                else
                {
                 k=(k+tai)-9;
                if (k>=1024)
                {   ///On passe au d�but de l'enregistrementsuivante, dans le bloc suivant
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                     *i=p;///On affecte � i l'adresse du bloc courant
                 }
            if ((p==entete(&f,2))&&(k==entete(&f,3))) /// On est arriv�s � la premi�re position vide du fichier
            {
                p=-1; /// Pour sortir de la boucle
            }
          ///On affecte � j temp qui repr�sente la position de l'enregistrementdans le bloc
  l++;
  }
  fermer(&f);
}


/********suppression***********/
void suppressionlnovc(char *nomfich,char matricule[6])
//Effectue la suppression logique d'un enregistrement relativement � une matricule donn�e
{
    LNOVC f;
    Tbloc buf;
    int tai,m,i=1,j,trouv=0;
    char taille[3];
    recherche_matricul(nomfich,matricule,&trouv,&i,&j);//On recherche l'enregistrementdans le fichier

    ouvrir(&f,nomfich,"a");

    if (trouv==1)// Si donn�e trouv�e
    {
        liredir(&f,i,&buf);//On r�cup�re le bloc o� se trouve l'enregistrement� supprimer ( dont l'adresse est le 'i' sortant du module de recherche
        for (m=0;m<=2;m++)//On r�cup�re la taille de l'enregistrementpour la mise � jour de l'ent�te du nombre de caract�res
                                //supprim�s
        {
            if (j>=1024)//Si la position courante d�passe la taille du bloc
            {
                j=0;
                i=buf.suiv;//On passe au bloc suivant
                liredir(&f,i,&buf);
            }
            taille[m-1]=buf.tab[j];//On r�cup�re le champ taille qui se trouve dans le fichier
            j++;
        }
        taille[3]='\0';//Pour indiquer la fin du champ 'taille'
        tai=atoi(taille);//On convertit la chaine r�cup�r�e en entier
        if (j>=1024)//Si le champ effac� de l'enregistrementse trouve dans le bloc suivant
        {
            i=buf.suiv;//On passe au bloc suivant
            liredir(&f,i,&buf);
        }
        j=j%1024;//On r�cup�re le champ 'effac�' de la donn�e
    if (buf.tab[j]=='0')//Donn�e non supprim�e
        {
            buf.tab[j]='1';//On supprime logiquement l'enregistrement(champ 'effac�' � 1 )
            ecriredir(&f,i,buf);//On met � jour le bloc dans le fichier
            aff_entete(&f,4,entete(&f,4)+tai);//On incr�mente le nombre de donn�es supprim�es dans l'ent�te du fichier
        }
        else printf("\Cette enregistrement est d%cja supprim%ce !\n",130,130);
    }
    else
    {
        printf("\nCette enregistrement n'existe pas.\n");
    }
    fermer(&f);
}

  void modif_region(char *nomfich,char matricule[6],char rg[14])
  {
    LNOVC f;
    Tbloc buf ;
    int trouv,o,i,j,k=0,m,tai,l;
    char taille[3]=" ";
    recherche_matricul(nomfich,matricule,&trouv,&i,&j);
    if (trouv==1)
    {
        ouvrir(&f,nomfich,"a");
        liredir(&f,i,&buf);
         for (m=0;m<=2;m++)
         {
             taille[m-1]=buf.tab[k];//On r�cup�re le champ taille qui se trouve dans le fichier
            k++;
         }
           taille[3]='\0';              ///Pour indiquer la fin du champ 'taille'
           tai=atoi(taille);           ///On convertit la chaine r�cup�r�e en entier

         for(l=0;l<12;l++)
         {
             buf.tab[tai-l]=rg[l];
         }

    }
    else printf("la donn� n'existe pas");
    fermer(&f);
  }






int main()
{
    LNOVC f;
    char nomfich[20]="";
    char cle[6]="";char chaine[1024]="";
    int i,j,trouv;
    int n;
    float temp;
    while (1==1)
{
    system("cls");
    int ChoixMenu;
    {
        printf("TP2 SFSD : MANIPULATION DES FICHIER LNOVC.\n\n");
        printf("************************MENU**************************\n");
        printf(" 1-Chargement initial d'un fichier\n");
        printf(" 2-�puration du fichier\n",133);
        printf(" 3-Recherche d'une donnee dans le fichier \n");
        printf(" 4-Insertion d�un nouvel enregistrement au fichier\n");
        printf(" 5-Supprimer un enregistrement donn� par le matricule\n");
        printf(" 6-Supprimer tous les enregistrements relatifs � une force arm�e\n");
        printf(" 7-Modification de la r�gion militaire d�un enregistrement\n");
        printf("\n\nQUEL EST VOTRE CHOIX?");
        scanf("%d",&ChoixMenu);
        switch (ChoixMenu)
        {
        case 1:
            printf("\nChargement initial d'un fichier :\n");
            printf("Non du fichier (ajouter le .bin) : ");
            scanf("%s",nomfich);
            printf("\n");
            printf("donner le nombre d'enregestrement :");
            scanf("%d",&n);
            chargement_init(nomfich,n);
            printf("\nvoici le contenude votre fichier :\n");
            ecritlnovc(nomfich);
            printf("\n");
            system("pause");
            break;

        case 2:

            printf("\n%cpuration du fichier:\n",133);
            printf("Nom du fichier (ajouter le .bin):");
            scanf("%s",nomfich);
            printf("\nLe nom du fichier = %s",nomfich);
            printf("\n");
            printf("voici votre nouvel fichier :\n");
            ecritlnovc(nomfich);
            printf("\n");
            system("pause");
            break;

        case 3:

            printf("\nRecherche d'une donnee dans le fichier :\n");
            printf("Nom du fichier (ajouter le .bin):");
            scanf("%s",nomfich);
            printf("\n");
            printf("Entrer le matricule:");
            scanf("%s",cle);
            printf("\n");
            recherche_matricul(nomfich,cle,&trouv,&i,&j);
            if (trouv) printf("LA DONNE EXISTE AU BLOC %d ET A LA POSITION %d",i,j);
            else printf("LA DONNEE N'EXISTE PAS ");
            printf("\n");
            system("pause");
            break;

        case 4:


            printf("\nInsertion d�un nouvel enregistrement au fichier:\n");
            printf("Nom du fichier (Ajouter le .bin):");
            scanf("%s",nomfich);
            printf("\n");
            printf("\nLa donn%ce %c ins%crer(Al%catoirement):",130,133,130,130,130);
            printf("\n");
            insertionlnovc(nomfich,chaine);
            printf("voici le contenu de fichier:\n\n");
            ecritlnovc(nomfich);
            printf("\n");
            system("pause");
            break;
        case 5:

            printf("\nSupprimer un enregistrement donn� par le matricule :\n\n");
            printf("Nom du fichier ():");
            scanf("%s",nomfich);

            printf("Entrer le matricule:");
            scanf("%s",cle);

            suppressionlnovc(nomfich,cle);
            printf("VOICI LE NOUVEAU CONTENU DU FICHIER : \n\n");
            ecritlnovc(nomfich);
            printf("\n\n\n");
            system("pause");
            break;

        case 6:
            printf("\nSupprimer tous les enregistrements relatifs � une force arm�e:\n\n");
            printf("Nom du fichier:");
            scanf("%s",nomfich);
            printf("Donner une force arm�e:");
            scanf("%s",cle);
            suppressionlnovc(nomfich,cle);
            printf("Voici le contenu de fichier:\n\n");
            ecritlnovc(nomfich);
            printf("\n");
            system("pause");
            break;

        case 7:
            printf("\nModification de la r�gion militaire d�un enregistrement.:\n\n");
            printf("Nom du fichier :");
            scanf("%s",nomfich);
            printf("donner le matricule :");
            scanf("%s",cle);
            printf("donner la nouvelle R�gion_militaire:");
            scanf("%s",chaine);
            modif_region(nomfich,cle,chaine);
            printf("\n");
            system("pause");
          break;


        }
    }
}
}






