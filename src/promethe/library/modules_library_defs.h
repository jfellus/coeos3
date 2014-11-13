/*
 * modules_library_defs.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef MODULES_LIBRARY_DEFS_H_
#define MODULES_LIBRARY_DEFS_H_


#include <util/utils.h>

typedef struct type_help
{
  char *description;
} type_help;

typedef struct group_function_pointers {
  const char *name;
  void (*f_appel_algo) (int gpe);
  void (*f_new) (int gpe);      /* fonctions appelee a la fin de create_and_manage() */
  void (*f_destroy) (int gpe);
  void (*f_save) (int gpe); /* fonction utilisee lors de la sauvegarde du reseau */
  type_help *help;
  int no; /* au cas ou l'on aurait besoin de se balader dans le tableau, on peut numeroter automatiquement. utilisateur met -1 */
  int nb_links_min; /* pour un jour tester si l'on a assez de liens sur certains groupes ? -1 ou 0 par defaut*/
} type_group_function_pointers;


type_group_function_pointers NN_Core_function_pointers[] =
{

{ "debut", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_debut", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_stop_if_non_rt", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_min_period", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_ca3", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_ca3_2", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_qlearning", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_assocond", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_spike_prediction", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_vigilance2neurone", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_neurone2vigilance", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_flip_flop", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_transition_detect", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_even_first_transition_detect", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_STM", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_grabimages_cleanbuffer", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_amplifi", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_clean_next", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_clean_next_conditionel", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_cleans_conditionel", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_clean", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_cleans", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_raise_fault", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_DG_1", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_DG_t2", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_DG_v3", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_WTA_sorin", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_TimeGenerator", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_InitialiseHippo", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_EC", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_base_de_temps", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_observable_moyen", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_recupere_micro_neurone", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_seuil_et_memoire", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_ctrnn", NULL, NULL, NULL, NULL, NULL, -1, -1 },
{ "f_lissage_temp", NULL, NULL, NULL, NULL, NULL, -1, -1 },

/* pour indiquer la fin du tableau*/
{ NULL, NULL, NULL, NULL, NULL, NULL, -1, -1 } };




#endif /* MODULES_LIBRARY_DEFS_H_ */
