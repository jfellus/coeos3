################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/module/Document.cpp \
../src/module/Group.cpp \
../src/module/GroupComponent.cpp \
../src/module/Link.cpp \
../src/module/Module.cpp \
../src/module/Property.cpp 

OBJS += \
./src/module/Document.o \
./src/module/Group.o \
./src/module/GroupComponent.o \
./src/module/Link.o \
./src/module/Module.o \
./src/module/Property.o 

CPP_DEPS += \
./src/module/Document.d \
./src/module/Group.d \
./src/module/GroupComponent.d \
./src/module/Link.d \
./src/module/Module.d \
./src/module/Property.d 


# Each subdirectory must supply rules for building sources it contributes
src/module/%.o: ../src/module/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/webkitgtk-3.0 -I/usr/include/cairo -I/usr/include/sigc++-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 webkitgtk-3.0 ` -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


