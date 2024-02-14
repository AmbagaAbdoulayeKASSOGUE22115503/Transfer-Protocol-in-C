/*************************************************************
* proto_tdd_v1 -  récepteur                                  *
* TRANSFERT DE DONNEES  v1                                  *
*                                                            *
* Protocole avec contrôle de flux, et reprise sur erreurs  *
**************************************************************/

#include <stdio.h>
#include "application.h"
#include "couche_transport.h"
#include "services_reseau.h"

/* =============================== */
/* Programme principal - récepteur */
/* =============================== */
int main(int argc, char* argv[])
{
    unsigned char message[MAX_INFO]; /* message pour l'application */
    paquet_t paquet,paquetRecu; /* paquet utilisé par le protocole et le paquet reçu du réseau*/
    int fin = 0; /* condition d'arrêt */
    uint8_t somme_controle;  /*variable dans laquelle on va stocker la somme de contrôle*/

    init_reseau(RECEPTION);

    printf("[TRP] Initialisation reseau : OK.\n");
    printf("[TRP] Debut execution protocole transport.\n");

    /* tant que le récepteur reçoit des données */
    while ( !fin ) {

        // attendre(); /* optionnel ici car de_reseau() fct bloquante */
        de_reseau(&paquetRecu);
        /*fait la vérification avant de passer le message à la couche application*/
        somme_controle= (paquetRecu.somme_ctrl)^generer_controle(&paquetRecu);
        if (somme_controle!=0)
        {
            paquet.type=NACK;
            vers_reseau(&paquet);
        }
        else
        {
            /* extraction des donnees du paquet recu */
            for (int i=0; i<paquetRecu.lg_info; i++) {
                message[i] = paquetRecu.info[i];
            }
            
            /* remise des données à la couche application */
            fin = vers_application(message, paquetRecu.lg_info);
            /*envoie l'acquittement à l'emtteur*/
            paquet.type=ACK;
            vers_reseau(&paquet);
        }
    }

    printf("[TRP] Fin execution protocole transport.\n");
    return 0;
}
