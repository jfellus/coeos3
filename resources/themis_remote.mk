-include perso.mk


############### REMOTE ####################

.PHONY:mkdir_promnet


mkdir_promnet:
	rsh -C $(login)@$(computer) mkdir -p promnet/$(logical_name)

mkdir_bin_leto_prom:
	rsh -C $(login)@$(computer) mkdir -p bin_leto_prom

if (!script->overwrite_res){
$(path_res)_upload_promnet: $(path_res) mkdir_promnet
	rsync --ignore-existing $< $(login)@$(computer):promnet/$(logical_name)/$<

$(path_gcd)_upload_promnet: $(path_gcd) mkdir_promnet
	rsync --ignore-existing $< $(login)@$(computer):promnet/$(logical_name)/$<

%_upload_promnet: % mkdir_promnet
	rsync -a $< $(login)@$(computer):promnet/$(logical_name)/$<

%_upload_file: % mkdir_promnet
	rsync -a $< $(login)@$(computer):promnet/$(logical_name)/$<

%_upload_directory:% mkdir_promnet
	rsync -a $< $(login)@$(computer):promnet/$(logical_name)/$(<D)

all_upload_bin_leto_prom:~/bin_leto_prom/ mkdir_bin_leto_prom
	rsync -a  $< $(login)@$(computer):bin_leto_prom/

all_upload: all_upload_bin_leto_prom $(foreach file, $(synchronize_files), $(file)_upload_file) $(foreach directory, $(synchronize_directories),$(directory)_upload_directory) $(path_script) $(path_config) $(path_res) $(path_dev) $(path_gcd) $(path_prt)
			


deamon_run_cmd:=$(login)@$(computer) 'mkdir -p /tmp/$(login)/logs; cd promnet/$(logical_name);nohup ~/bin_leto_prom/$(promethe_binary) -n$(logical_name) -i$(themis_id) $(promethe_args) --distant-terminal $(path_script) $(path_config) $(path_res) $(path_dev) $(path_gcd) $(path_prt) > /tmp/$(login)/logs/$(logical_name).log' &

deamon_run:
	rsh -Ct $(deamon_run_cmd)

daemon_run_graphic:
	rsh -XCt $(deamon_run_cmd)

		
run_debug:
	rsh -XCt $(login)@$(computer) 'cd promnet/$(logical_name);nemiver ~/bin_leto_prom/$(promethe_binary)_debug -n$(logical_name) -i$(themis_id) $(promethe_args)  $(path_script) $(path_config) $(path_res) $(path_dev) $(path_gcd) $(path_prt)'|| echo -e "\a"

run_valgrind:
	rsh -XCt $(login)@$(computer) 'cd promnet/$(logical_name);valgrind ~/bin_leto_prom/$(promethe_binary)_debug -n$(logical_name)  -i$(themis_id) $(promethe_args) $(path_script) $(path_config) $(path_res) $(path_dev) $(path_gcd) $(path_prt)' || echo -e "\a"

run_graphic:
	rsh -Ct $(login)@$(computer) 'cd promnet/$(logical_name);~/bin_leto_prom/$(promethe_binary) -n$(logical_name)  -i$(themis_id) $(promethe_args) $(path_script) $(path_config) $(path_res) $(path_dev) $(path_gcd) $(path_prt) ' || echo -e "\a"


run:
	rsh -XCt $(login)@$(computer) 'cd promnet/$(logical_name);~/bin_leto_prom/$(promethe_binary) -n$(logical_name) -i$(themis_id) $(promethe_args) $(path_script) $(path_config) $(path_res) $(path_dev) $(path_gcd) $(path_prt) ' || echo -e "\a"


show_log:
	rsh -C $(login)@$(computer) 'cat /tmp/$(login)/logs/$(logical_name).log'
