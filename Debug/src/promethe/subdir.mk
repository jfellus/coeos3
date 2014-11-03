################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/promethe/PromGroup.cpp \
../src/promethe/PromLink.cpp \
../src/promethe/PromProject.cpp \
../src/promethe/PromScript.cpp \
../src/promethe/PromWorkbench.cpp \
../src/promethe/promethe_modules.cpp \
../src/promethe/tools.cpp 

OBJS += \
./src/promethe/PromGroup.o \
./src/promethe/PromLink.o \
./src/promethe/PromProject.o \
./src/promethe/PromScript.o \
./src/promethe/PromWorkbench.o \
./src/promethe/promethe_modules.o \
./src/promethe/tools.o 

CPP_DEPS += \
./src/promethe/PromGroup.d \
./src/promethe/PromLink.d \
./src/promethe/PromProject.d \
./src/promethe/PromScript.d \
./src/promethe/PromWorkbench.d \
./src/promethe/promethe_modules.d \
./src/promethe/tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/promethe/%.o: ../src/promethe/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/webkitgtk-3.0 -I/usr/include/cairo -I/usr/include/sigc++-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 webkitgtk-3.0 ` -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


