/*************************************************************
* proto_tdd_v2 -  émetteur                                   *
* TRANSFERT DE DONNEES  v2                                   *
*                                                            *
* Protocole avec retransmission après timeout                *
*                                                            *
**************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - émetteur  */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message de l'application */
    int taille_msg, numProchainPaquet=0; /* taille du message et une variable qui indique le numéro du paquet à envoyer*/
    int evenement; /*variable qui va contenir un événement*/
    paquet_t paquet, paquetDeReseau; /* paquet utilisé par le protocole et le paquet reçu du réseau*/

    init_reseau(EMISSION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* lecture de donnees provenant de la couche application */
    de_application(message, &taille_msg);

    /* tant que l'émetteur a des données à envoyer */
    while ( taille_msg != 0 ) {

        /* construction paquet */
        for (int i=0; i<taille_msg; i++) {
            paquet.info[i] = message[i];
        }
        paquet.lg_info = taille_msg;
        paquet.type = DATA;
        paquet.num_seq=numProchainPaquet;
        paquet.somme_ctrl=generer_controle(&paquet);

        do
        {
            vers_reseau(&paquet);
            depart_temporisateur(100);
            evenement=attendre();
            if (evenement==-1)
            {
                de_reseau(&paquetDeReseau);
                /*on vérifie que c'est le bon paquet. Si non; on met une valeur différente de -1 dans evenement pour retransmettre de nouveau le paquet*/
                if (paquetDeReseau.num_seq!=paquet.num_seq)
                    evenement=-2; 
            }
            
        }while (evenement!=-1);

        /*on arrête le temporisateur*/
        arret_temporisateur();
        /*passe au numéro du prochain paquet à envoyer*/
        numProchainPaquet=(numProchainPaquet+1)%2;
        /* lecture des donnees suivantes de la couche application */
        de_application(message, &taille_msg);
    }

    printf("[TRP] Fin execution protocole transfert de donnees (TDD).\n");
    return 0;
}
