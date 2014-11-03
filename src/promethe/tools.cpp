/*
 * tools.cpp
 *
 *  Created on: 19 oct. 2014
 *      Author: jfellus
 */

#include "tools.h"
#include "../util/utils.h"
#include <stdarg.h>


#define LINE_MAX 10000



const char* PROM_GROUP_TYPES[] = {
"Hebb",
"Winner",
"Kohonen",
"Widrow",
"Special",
"Ou",
"Et",
"Entree",
"Sortie",
"PTM",
"Hebb_Seuil",
"Winner_Special",
"Hopfield",
"Winner_Colonne",
"Fonction_Algo",
"But",
/*-------------------------*/
"Pyramidal",
"Inter_neurone",
/*-------------------------*/
"Fonction_Algo_mvt",
/*-------------------------*/
"Granular",
"Micro_Pyramid",
/*-------------------------*/
"SV_cor",
"Dyn",
/*-------------------------*/
"SAW",
"Sigma_PI",
"Winner_Selectif",
"Macro_Colonne",
"BREAK",
"LMS",
"RTTOKEN",
"KO_Discret",
"KO_Continu",
"Hebb_Seuil_binaire",
"Pyramidal_plan",
"LMS_2",
"BCM",
"PLG",
"Mean_Mvt",
"Asso_Cond",
"Sub_Network",
"LMS_delayed",
"Winner_Macro",
"NLMS",
"Sutton_Barto",
"PCR",
"selective_winner_modulated",
"Selverston",
"RCO"
};

bool LINK_TYPE_HAS_NAME(int link_type) {
	return link_type==5 || link_type==8;
}



