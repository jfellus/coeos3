-include perso.mk

################## LOCAL #######################

empty := 
sp := $(empty) $(empty)

dep_res:=$(subst $(sp),\$(sp),$(path_res))
dep_script:=$(subst $(sp),\$(sp),$(path_script))
dep_symb:=$(subst $(sp),\$(sp),$(path_symb))
deps:= $(dep_script) $(dep_res) $(subst $(sp),\$(sp),$(path_dev)) $(subst $(sp),\$(sp),$(path_gcd)) $(subst $(sp),\$(sp),$(path_prt)) $(subst $(sp),\$(sp),$(path_config))


RANDOM:=$(shell awk 'BEGIN{srand();printf("%d", 65536*rand())}')

ARGS= "$(path_script)" "$(path_config)" "$(path_res)" "$(path_dev)" "$(path_gcd)" "$(path_prt)" -n"$(logical_name)" -b$(themis_ip) -i$(themis_id) $(promethe_args)

/tmp/%:	
	mkdir -p $@	

daemon_run_debug: | /tmp/$(USER)/logs	
	nohup nemiver $(promethe_binary)_debug $(ARGS) --distant-terminal> /tmp/$(USER)/logs/$(logical_name).log& 

daemon_run: | /tmp/$(USER)/logs
	nohup $(promethe_binary) $(ARGS) --distant-terminal > /tmp/$(USER)/logs/$(logical_name).log &
	echo "$!" >> ~/bin_leto_prom/.running_promethes.txt


run_debug:
	nemiver $(promethe_binary)_debug $(ARGS) || echo -e "\a"

run_valgrind:
	valgrind $(promethe_binary)_debug $(ARGS) || echo -e "\a"

run_blind: $(deps)
	~/bin_leto_prom/bpromethe $(ARGS)

run_gui: $(deps)
	~/bin_leto_prom/promethe $(ARGS) 

$(dep_res): $(dep_script)
	@echo "Compile $(path_script) to $(path_res)"
	@echo $(RANDOM) | /home/$(USER)/bin_leto_prom/cc_leto "$(path_script)" "$(path_res)"

$(dep_script): $(dep_symb)
	@echo "Compile $(path_symb) to $(path_script)"
	touch "$(path_var)"
	~/bin_leto_prom/lpreprocess.sh "$(path_symb)" "$(path_var)" "$(path_script)"


