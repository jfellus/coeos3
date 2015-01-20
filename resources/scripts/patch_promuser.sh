#!/bin/bash

echo "Patch prom_user"
cp -f "`dirname $0`/Create_dynamic_lib_Makefile.sh" ~/bin_leto_prom/simulator/prom_user/
cp -f "`dirname $0`/make_groups_infos.sh" ~/bin_leto_prom/simulator/scripts/

