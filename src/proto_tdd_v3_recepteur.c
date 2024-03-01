/*************************************************************
* proto_tdd_v3 -  émetteur                                   *
* TRANSFERT DE DONNEES  v3                                   *
*                                                            *
* Protocole avec le principe du go_back_n                    *
*                                                            *
* E. Lavinal - Univ. de Toulouse III - Paul Sabatier         *
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message pour l'application */
    paquet_t paquet, paquetRecu; /* paquet utilisé par le protocole et le paquet reçu du réseau*/
    int fin = 0; /* condition d'arrêt */
    int numPaquetAttendu=0, dernierPaquetRecu=255 ; /*le numéro du prochain paquet attendu et celui du dernier paquet reçu initialisé à une valeur qui n'est pas dans la fenêtre qui l'emetteur*/
    int verif_controle;  /*qui vérifie la somme de contrôle*/
    

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /*on prépare le champ type du paquet qui est toujours positif*/
    paquet.type=ACK;

    /* tant que le récepteur reçoit des données */
    while ( !fin ) {

        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquetRecu);
        /*on vérifie la somme de contrôle*/
        verif_controle= ((paquetRecu.somme_ctrl)^generer_controle(&paquetRecu))==0;
        if (verif_controle && (paquetRecu.num_seq==numPaquetAttendu))
        { 
            //printf("%d\n",paquetRecu.num_seq);
            /* extraction des donnees du paquet recu */
            for (int i=0; i<paquetRecu.lg_info; i++) {
                message[i] = paquetRecu.info[i];
            }
            /* remise des données à la couche application */
            fin = vers_application(message, paquetRecu.lg_info);
            //on envoit un acquittement du paquet à l'emetteur
            paquet.num_seq=paquetRecu.num_seq;
            paquet.somme_ctrl=paquet.num_seq^paquet.type;
            vers_reseau(&paquet);
            /*mémorise le numéro du dernier paquet reçu et passe au numéro du prochain paquet attendu*/
            dernierPaquetRecu= numPaquetAttendu;
            numPaquetAttendu=(numPaquetAttendu+1)%16;

        }
        else
        {
            paquet.num_seq=dernierPaquetRecu;
            paquet.somme_ctrl=paquet.num_seq^paquet.type;
            vers_reseau(&paquet);
            printf("%d %d\n",numPaquetAttendu,dernierPaquetRecu);
        }

    }

    /*le recpteur attend un peu avant de se terminer tout en envoyant l'acquitement du dernier paquet qui pourrait être perdu lors de son dernier envoi*/
    for(int i=0;i<100;i++) vers_reseau(&paquet);

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
//on envois l'acquitement du paquet 14 et on attend encore le paquet 14. Suppossont qur il u avait une erreur lorsqu'on a reçu le 14
