################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/widget/Browser.cpp \
../src/widget/DocBrowser.cpp \
../src/widget/InfoForm.cpp \
../src/widget/PropertiesForm.cpp \
../src/widget/Window.cpp \
../src/widget/ZoomableDrawingArea.cpp 

OBJS += \
./src/widget/Browser.o \
./src/widget/DocBrowser.o \
./src/widget/InfoForm.o \
./src/widget/PropertiesForm.o \
./src/widget/Window.o \
./src/widget/ZoomableDrawingArea.o 

CPP_DEPS += \
./src/widget/Browser.d \
./src/widget/DocBrowser.d \
./src/widget/InfoForm.d \
./src/widget/PropertiesForm.d \
./src/widget/Window.d \
./src/widget/ZoomableDrawingArea.d 


# Each subdirectory must supply rules for building sources it contributes
src/widget/%.o: ../src/widget/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/webkitgtk-3.0 -I/usr/include/cairo -I/usr/include/sigc++-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 webkitgtk-3.0 ` -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


