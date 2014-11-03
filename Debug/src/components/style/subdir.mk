################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/components/style/CSSDefinitions.cpp \
../src/components/style/IStyle.cpp 

OBJS += \
./src/components/style/CSSDefinitions.o \
./src/components/style/IStyle.o 

CPP_DEPS += \
./src/components/style/CSSDefinitions.d \
./src/components/style/IStyle.d 


# Each subdirectory must supply rules for building sources it contributes
src/components/style/%.o: ../src/components/style/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/webkitgtk-3.0 -I/usr/include/cairo -I/usr/include/sigc++-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 webkitgtk-3.0 ` -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


