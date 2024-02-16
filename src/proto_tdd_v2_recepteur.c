/*************************************************************
* proto_tdd_v2 -  émetteur                                   *
* TRANSFERT DE DONNEES  v2                                   *
*                                                            *
* Protocole avec retransmission après timeout                *
*                                                            *
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
    int numPaquetAttendu=0 ; /*le numéro du paquet attendu*/
    int verif_controle;  /*variable dans laquelle on va stocker le résultat du test de contrôl*/

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /*on prépare le champ type du paquet qui est toujours positif*/
    paquet.type=ACK;
    /* tant que le récepteur reçoit des données */
    while ( !fin ) {

        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquetRecu);
       /*on vérifie la somme de contrôle et ensuite le numéro de sequence*/
        verif_controle= ((paquetRecu.somme_ctrl)^generer_controle(&paquetRecu))==0;
        if (verif_controle)
        {
            if (paquetRecu.num_seq==numPaquetAttendu)
            {
                //printf("%d\n",paquetRecu.num_seq);
                /* extraction des donnees du paquet recu */
                for (int i=0; i<paquetRecu.lg_info; i++) {
                    message[i] = paquetRecu.info[i];
                }
                /* remise des données à la couche application */
                fin = vers_application(message, paquetRecu.lg_info);
                numPaquetAttendu=(numPaquetAttendu+1)%2;
            }
            //on envoit un acquittement du paquet à l'emetteur
            paquet.num_seq=paquetRecu.num_seq;
            vers_reseau(&paquet);
        }
    }

    /*le recpteur attend un peu avant de se terminer tout en envoyant l'acquitement du dernier paquet qui pourrait être perdu lors de son dernier envoi*/
    for(int i=0;i<100;i++) vers_reseau(&paquet);

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
