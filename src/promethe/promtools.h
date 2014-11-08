/*
 * tools.h
 *
 *  Created on: 19 oct. 2014
 *      Author: jfellus
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <util/utils.h>


#define No_Hebb              0
#define No_Winner            1
#define No_Kohonen           2
#define No_Widrow            3
#define No_Special           4
#define No_Ou                5
#define No_Et                6
#define No_Entree            7
#define No_Sortie            8
#define No_PTM               9
#define No_Hebb_Seuil       10
#define No_Winner_Special   11
#define No_Hopfield         12
#define No_Winner_Colonne   13
#define No_Fonction_Algo    14
#define No_But              15

/*-------------------------*/

#define No_Pyramidal        16  /* can be a good model of granular cell     */
#define No_Inter_neurone    17  /* no STM, usefull for inhibition mechanism */
/*-------------------------*/
#define No_Fonction_Algo_mvt 18
/*-------------------------*/
#define No_Granular         19
#define No_Micro_Pyramid    20
/*-------------------------*/
#define No_SV_cor 21            /* to correlate (difference) 2 forms */
#define No_Dyn 22
/*-------------------------*/
#define No_SAW              23
#define No_Sigma_PI	     24
#define No_Winner_Selectif	25
#define No_Macro_Colonne 26     /* defined to allow self organization beween neurons of a given macro column */
#define No_BREAK	27
#define No_LMS		  28        /* Group added by O.Ledoux */
#define No_RTTOKEN	  29        /* Group added by A.Patard: Groupe d'execution temps reel. */
#define No_KO_Discret     30    /* ajout M. Quoy 1/03/2005 */
#define No_KO_Continu     31    /* ajout M. Quoy 1/03/2005 */
#define No_Hebb_Seuil_binaire       32
#define No_Pyramidal_plan 33
#define No_LMS_2	34          /* ajoute par B. Siri le 07/07/05 */
#define No_BCM		35          /* ajoute par B. Siri le 20/05/05 */
#define No_PLG 36               /*ajoute par M.M. mais le groupe est pas encore vraiment fonctionnel... */
#define No_Mean_Mvt 37
#define No_Asso_Cond 38
#define No_Sub_Network 39       /* pour gerer l'inclusion s'un script */
#define No_LMS_delayed 40
#define No_Winner_Macro 41
#define No_NLMS 42
#define No_Sutton_Barto 43
#define No_PCR 44
/*#define No_selective_winner_modulated 45
 #define No_PTM_2_old 46*/
#define No_Selverston 45
#define No_RCO 46


#define No_l_1_1_modif     0
#define No_l_1_a           1
#define No_l_1_v           2
#define No_l_1_1_non_modif 3
#define No_l_1_t           4
#define No_l_algorithmique 5
#define No_l_1_1_non_modif_bloqueur 6
#define No_l_1_v_non_modif 7
#define No_l_neuro_mod     8




extern const char* PROM_GROUP_TYPES[];



bool LINK_TYPE_HAS_NAME(int link_type);




#endif /* TOOLS_H_ */
