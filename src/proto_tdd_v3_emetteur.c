/*************************************************************
* proto_tdd_v3 -  émetteur                                   *
* TRANSFERT DE DONNEES  v3                                   *
*                                                            *
* Protocole avec le principe du go_back_n                    *
*                                                            *
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message de l'application */
    int taille_msg=-1; /* taille du message*/
    int evenement, born_inf=0, pointeur=0; /*l'événement, la borne inférieur de la fenêtre et le pointeur qui parcour la fenêtre*/
    int verif_controle; /*qui vérifie la somme de contrôle*/
    paquet_t paquet; /* paquet utilisé par le protocole*/
    /*initialisation de la taille de la fenêtre*/
    int taille_fenetre=7;
    if (argc==2)
        taille_fenetre=atoi(argv[1]);
    paquet_t tabPaquet[16];

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(message, &taille_msg);
    /* tant que l'émetteur a des données à envoyer */
    while (taille_msg != 0) {
        /*printf("%d %d \n",pointeur,born_inf);
        printf("%d\n",pointeur!=born_inf);  //à enlever*/

        /*on a un crédit donc la boucle est exécutée lors de la prémière visite.*/
        while( dans_fenetre(born_inf,pointeur,taille_fenetre) && (taille_msg != 0))
        {
            /* construction paquet */
            for (int i=0; i<taille_msg; i++) {
                tabPaquet[pointeur].info[i] = message[i];
            }
            tabPaquet[pointeur].lg_info = taille_msg;
            tabPaquet[pointeur].type = DATA;
            tabPaquet[pointeur].num_seq=pointeur;
            tabPaquet[pointeur].somme_ctrl=generer_controle(&tabPaquet[pointeur]);
            vers_reseau(&tabPaquet[pointeur]);
            if (born_inf==pointeur)
                depart_temporisateur(100);
            pointeur= (pointeur+1)%16;
            de_application(message, &taille_msg);
        }

        do
        {
            /*attente obligatoire*/
            evenement=attendre();
            if (evenement==-1)
            {
                de_reseau(&paquet);
                /*on vérifie la somme de controle et si l'aquittement reçu est dans la séquence*/
                verif_controle= ((paquet.somme_ctrl)^paquet.num_seq^paquet.type)==0;
                printf("%d %d\n",paquet.num_seq,born_inf);
                printf("verif controle %d\n",verif_controle);
                if (verif_controle && dans_fenetre(born_inf,paquet.num_seq,taille_fenetre) )
                {
                    /*on décale la fenêtre*/
                    born_inf=(paquet.num_seq+1)%16;
                    if ( born_inf==pointeur )
                        arret_temporisateur(); /*tous les paquets on été acquité*/
                }
                /*si non le paquet n'est pas dans la fenêtre et on ignore l'acquitement*/
            }
            else /*timeout: réemission de tous jusqu'au pointeur*/
            {
                for(int i=born_inf; i!=pointeur; i=(i+1)%16)
                    vers_reseau(&tabPaquet[i]);
                depart_temporisateur(100);

            }
            printf("%d %d \n",pointeur,born_inf);
        }while(born_inf!=pointeur);
    }

    
    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
