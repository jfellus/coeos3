

ifeq ($(path_prt),)
       include ~/bin_leto_prom/themis_local.mk
else
       include ~/bin_leto_prom/themis_remote.mk
endif
