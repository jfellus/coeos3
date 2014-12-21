/*
 * tools.cpp
 *
 *  Created on: 19 oct. 2014
 *      Author: jfellus
 */

#include <util/utils.h>
#include <stdarg.h>
#include "promtools.h"


namespace coeos {


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
"RCO",
NULL
};

const char* PROM_LINK_TYPES[] = {
		"one to one modifiable",
		"one to random (modifiable)",
		"one to neighborhood (modifable)",
		"one to one (NON modifiable)",
		"one to all (modifiable)",
		"algorithmic link (virtual)",
		"one to one blocking (vig. control)",
		"one to neighborhood (NON modifiable)",
		"global neuro modulation link (virtual)",
		NULL
};

bool LINK_TYPE_HAS_NAME(int link_type) {
	return link_type==5 || link_type==8;
}


std::string LINK_NO_TO_TYPE(int no) {
	if(no<0) return "?";
	return PROM_LINK_TYPES[no];
}

int LINK_TYPE_TO_NO(const std::string& type) {
	for(uint i=0; PROM_LINK_TYPES[i]!=0; i++) {
		if(type == PROM_LINK_TYPES[i]) return i;
	}
	return -1;
}



}
