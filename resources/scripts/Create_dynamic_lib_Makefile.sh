#!/bin/bash
################################################################################
# Copyright  ETIS — ENSEA, Université de Cergy-Pontoise, CNRS (1991-2014)
#promethe@ensea.fr
#
# Authors: P. Andry, J.C. Baccon, D. Bailly, A. Blanchard, S. Boucena, A. Chatty, N. Cuperlier, P. Delarboulas, P. Gaussier, 
# C. Giovannangeli, C. Grand, L. Hafemeister, C. Hasson, S.K. Hasnain, S. Hanoune, J. Hirel, A. Jauffret, C. Joulain, A. Karaouzène,  
# M. Lagarde, S. Leprêtre, M. Maillard, B. Miramond, S. Moga, G. Mostafaoui, A. Pitti, K. Prepin, M. Quoy, A. de Rengervé, A. Revel ...
#
# See more details and updates in the file AUTHORS 
#
# This software is a computer program whose purpose is to simulate neural networks and control robots or simulations.
# This software is governed by the CeCILL v2.1 license under French law and abiding by the rules of distribution of free software. 
# You can use, modify and/ or redistribute the software under the terms of the CeCILL v2.1 license as circulated by CEA, CNRS and INRIA at the following URL "http://www.cecill.info". 
# As a counterpart to the access to the source code and  rights to copy, modify and redistribute granted by the license, 
# users are provided only with a limited warranty and the software's author, the holder of the economic rights,  and the successive licensors have only limited liability. 
# In this respect, the user's attention is drawn to the risks associated with loading, using, modifying and/or developing or reproducing the software by the user in light of its specific status of free software, 
# that may mean  that it is complicated to manipulate, and that also therefore means that it is reserved for developers and experienced professionals having in-depth computer knowledge. 
# Users are therefore encouraged to load and test the software's suitability as regards their requirements in conditions enabling the security of their systems and/or data to be ensured 
# and, more generally, to use and operate it in the same conditions as regards security. 
# The fact that you are presently reading this means that you have had knowledge of the CeCILL v2.1 license and that you accept its terms.
################################################################################
####################################################
#script permettant de generer un Makefile pour compiler les librairies dynamiques 
#Yannick PERDREAU 26/10/2010
###################################################

####################################################
#definition de $CFLAGS $FLAGS_OPTIM $FLAGS_DEBUG et autres variables globales a tout promethe...
####################################################

####################################################
#Definition des chemins d'acces, options de compile etc...
####################################################

# Nom du programme

ALL_PROJECTS=(SigProc IHM IO_Robot Parallel_Comm NN_IO Sensors) 
ALL_CONFIGURATIONS=(debug release)
ALL_MODES=(gui blind)

for MODE in ${ALL_MODES[@]} #car declare comme tableau
do
	for CONFIGURATION in ${ALL_CONFIGURATIONS[@]} #car declare comme tableau
	do
		
		source ../scripts/COMPILE_FLAG
		
		# Initialisation des libs, includes et flags
		INCLUDES="$GTKINCLUDES -I$SIMULATOR_PATH/shared/include -I$PWD/include -I. -I$INCLUDE2 -I$SIMULATOR_PATH" 

		echo "$*" | grep -q "-enable-firewire" 
		if [ $? -eq 0 ]
		then echo "compile $PROG_NAME with firewire"
		    CFLAGS="$CFLAGS -DFIREWIRE"
		    LIBS="$LIBS $DC1394LIB $RAW1394LIB "		
		fi
		
		echo "$*" | grep -q "-enable-player" 
		if [ $? -eq 0 ]
		then echo "compile $PROG_NAME with player"
		    CFLAGS="$CFLAGS -DPLAYER"
		    INCLUDES="$INCLUDES -I/usr/local/include/player-2.1/"="$LIBS -L/usr/local/lib/ $PLAYERLIB"
		fi	
		
		echo "$*" | grep -q "-enable-sim2d" 
		if [ $? -eq 0 ]
		then echo "compile $PROG_NAME with sim2d"
		    CFLAGS="$CFLAGS -DSIM2D"
		fi	
		
		echo "$*" | grep -q "-enable-threads"
		if [ $? -eq 0 ]
		then echo "compile enable-threads..."
		    KERNELLIB="${KERNELLIB}_threads"
		    CFLAGS="$CFLAGS -DUSE_THREADS -DUSE_THREADS_2"
		else echo "compile disable-threads..."    
		fi
		
		echo "$*" | grep -q "-enable-enet"
		if [ $? -eq 0 ]
		then echo "compile enable-threads..."
		    CFLAGS="$CFLAGS -DFLAG_ENET"
		    LIBS="$LIBS -lenet"
		else echo "compile disable-enet..."    
		fi

		if [ "$MODE" == "blind" ] 
		then
				HARDWARELIB="${HARDWARELIB}_${MODE}"
				KERNELLIB="${KERNELLIB}_${MODE}"
				COMMLIB="${COMMLIB}_${MODE}"
				CFLAGS="$CFLAGS -DAVEUGLE"
		fi  #En graphique on ne change pas les noms

	   if [ "$CONFIGURATION" == "debug" ]
	   then
				HARDWARELIB="${HARDWARELIB}_${CONFIGURATION}"
				KERNELLIB="${KERNELLIB}_${CONFIGURATION}"
				COMMLIB="${COMMLIB}_${CONFIGURATION}"
				CFLAGS="$CFLAGS $FLAGS_DEBUG "
		else #En release on ne change pas les noms
				CFLAGS="$CFLAGS $FLAGS_OPTIM" 
		fi
		
		#Version finale des libs, includes et flags
		FINALINCLUDES="$INCLUDES"
		#FINALLIBS=" -L$KERNELLIBPATH $KERNELLIB -L$GRAPHICLIBPATH $GRAPHICLIB -L$SCRIPTLIBPATH $SCRIPTLIB -L$HARDWARELIBPATH $HARDWARELIB -L$IVYLIBPATH $IVYLIB -lpcre $LIBS -L$COMMLIBPATH $COMMLIB"
		FINALCFLAGS="$CFLAGS -DUSE_FLOAT_PRECISION_FFT"
		
		echo "$SYSTEM" | grep -q "Darwin"
		if [ $? -eq 0 ]
		then
			FINALLIBS="$FINALLIBS $KERNELLIB $GTKLIB -framework CoreServices"	
		fi
				

############################################################################################
		for PROJECT in ${ALL_PROJECTS[@]}
		do
		
			case $PROJECT in
				"SigProc")
					LIBS="$LIBS";;
				"IHM")
					LIBS="$LIBS";;
				"IO_Robot")		
					LIBS="$LIBS";;
				"Parallel_Comm")
					LIBS="$LIBS";;
				"NN_IO")
					LIBS="$LIBS";;
				"Sensors")
					LIBS="$LIBS -lasound  -lfftw3f $JPEGLIB";;
			esac
					
					 		
			LIB_NAME=lib${PROJECT}
			VERSION=${MODE}_${CONFIGURATION}
			LIB_VERSION=${LIB_NAME}_${VERSION}
		
			#Les repertoires de destination des fichiers compiles
			OBJDIR="$OBJPATH/$LIB_NAME"
					
			#Gestion des fichiers a compiler
			SOURCES=`find src/$PROJECT -depth -type f -name '*.c'`
			SOURCES="$SOURCES src/${LIB_NAME}.c"
			
			####################################################
			#Creation du Makefile
			####################################################
			MAKEFILE="src/${PROJECT}/Makefile.$LIB_VERSION"
			
			
			#Parametres lies au programme mais pas a l'installation (recalcul a chaque make) 			
echo "# Pour preciser que ce ne sont pas des noms de fichiers meme s'ils existent
.PHONY: all  clean reset default" > $MAKEFILE
echo "all: $DIR_DYN_LIB_PROM_USER/$LIB_VERSION.$DYNAMIC_LIBRARY_SUFFIXE $DIR_DYN_LIB_PROM_USER/$LIB_VERSION.info " >> $MAKEFILE
echo "" >> $MAKEFILE	
			
			# pour recreer les repertoires dans tmp si l'on travalle avec tmp		
			echo "$BINDIR:" >> $MAKEFILE
			echo -e "\tmkdir -p \$@\n" >> $MAKEFILE
			
			echo "$OBJDIR:" >> $MAKEFILE
			echo -e "\tmkdir -p \$@\n" >> $MAKEFILE
			
			echo "$DIR_DYN_LIB_PROM_USER:" >> $MAKEFILE
			echo -e "\tmkdir -p \$@\n" >> $MAKEFILE

			
			
			OBJECTS=""
			for i in $SOURCES
			do
			    FICHIER=$(basename "$i" .c)
			    echo "$OBJDIR/${FICHIER}_${VERSION}.o: $i | $OBJDIR" >> $MAKEFILE
			    echo -e "\t$CC -c $FINALCFLAGS $CFLAGS_DYNAMIC_OBJECT $FINALINCLUDES $i -o $OBJDIR/${FICHIER}_${VERSION}.o">> $MAKEFILE
			    echo "" >> $MAKEFILE
			    OBJECTS="$OBJECTS $OBJDIR/${FICHIER}_${VERSION}.o"
			done #objects 
			
			#Creation librairies dynamiques
			echo "$DIR_DYN_LIB_PROM_USER/$LIB_VERSION.$DYNAMIC_LIBRARY_SUFFIXE: $OBJECTS | $BINDIR $DIR_DYN_LIB_PROM_USER" >> $MAKEFILE
			echo -e "\t$CC $CFLAGS_DYNAMIC_LIBRARY $OBJECTS -o $BINDIR/$LIB_VERSION.$DYNAMIC_LIBRARY_SUFFIXE $LIBS" >> $MAKEFILE
			echo -e	"\tcp -f $BINDIR/$LIB_VERSION.$DYNAMIC_LIBRARY_SUFFIXE $DIR_DYN_LIB_PROM_USER/$LIB_VERSION.$DYNAMIC_LIBRARY_SUFFIXE" >> $MAKEFILE
			echo "" >> $MAKEFILE

			# Creation du fichier d'info sur les noms et parametres des groupes (J.Fellus 2014)
			echo -e "$DIR_DYN_LIB_PROM_USER/$LIB_VERSION.info:" >> $MAKEFILE
			echo -e "\t./../scripts/make_groups_infos.sh src/lib$PROJECT.c" >> $MAKEFILE
			echo "" >> $MAKEFILE

			
			echo "clean:" >> $MAKEFILE
			echo -e "\trm -f  $OBJDIR/*.o $BINDIR/*.$DYNAMIC_LIBRARY_SUFFIXE" >> $MAKEFILE
			echo "" >> $MAKEFILE
			
			echo "reset: clean" >> $MAKEFILE 
			echo -e "\trm -f $DIR_DYN_LIB_PROM_USER/$LIB_VERSION.$DYNAMIC_LIBRARY_SUFFIXE" >> $MAKEFILE
			echo "" >> $MAKEFILE

		done #project
	done #configuration
done #mode
