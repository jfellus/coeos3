#################################
# NOTES FOR UBUNTU 12.04 USERS :
# --------------------------------
# EVENTUALLY YOU'LL HAVE TO TYPE : sudo apt-add-repository ppa:teward/libmicrohttpd-quantal
# INSTALL GCC 4.8 and G++ 4.8 by typing : 
#	sudo add-apt-repository ppa:ubuntu-toolchain-r/test
#	sudo apt-get update
#	sudo apt-get install gcc-4.8 g++-4.8
####################################


###### INSTALLATION GUIDE ######
# make external_libs
# make
# ./coeos++
################################


APT_GET_DEPENDENCIES:= libmicrohttpd-dev libmicrohttpd10 libgtk-3-dev libwebkitgtk-dev libxml++2.6-dev


REQUIRED_PACKAGES:= cairo gtk+-3.0 webkitgtk-3.0 libmicrohttpd libxml++-2.6
REQUIRED_LIBS:= -lpthread -lboiboites -lgraphounet

PATH_TO_LIBBOIBOITES:=../libboiboites
PATH_TO_LIBGRAPHOUNET:=../libgraphounet

INCLUDE_PATHS:=$(PATH_TO_LIBBOIBOITES)/src $(PATH_TO_LIBGRAPHOUNET)/src

SRC_DIR:=./src

EXECUTABLE:=coeos++







########################## DON'T EDIT BELOW THIS LINE (unless you are a gnu make's expert ##############

SRC := $(shell find $(SRC_DIR) -name '*.cpp') 
OBJS := $(addprefix bin/,$(SRC:.cpp=.o))

$(EXECUTABLE): $(OBJS)

CXXFLAGS := -fPIC -g -rdynamic -Wall -MMD `pkg-config --cflags $(REQUIRED_PACKAGES)` $(addprefix -I,$(INCLUDE_PATHS))
LDFLAGS := -fPIC -rdynamic `pkg-config --libs $(REQUIRED_PACKAGES)` -L/home/$(USER)/bin_leto_prom/ $(REQUIRED_LIBS) -Wl,-rpath=/home/$(USER)/bin_leto_prom/ 
DEPENDS = $(OBJS:.o=.d)    

$(EXECUTABLE) : $(OBJS)          
	@echo "Build executable $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS) 
	@echo "DONE" 

bin/%.o: %.cpp
	@mkdir -p `dirname $(@:.o=.d)`
	@touch $(@:.o=.d)
	@echo "Compilation : $< "
	@g++ -std=c++11 $(CXXFLAGS) -MMD -c $< -o $@

bin: 
	@mkdir -p bin

clean::
	@rm -f $(EXECUTABLE)
	@rm -rf bin
	
all: $(EXECUTABLE)

external_libs:
	@echo "We need your password for this : "
	@sudo echo "thank you"
	@sudo apt-get install $(APT_GET_DEPENDENCIES)
	@echo "DONE"
	

-include $(DEPENDS) 
