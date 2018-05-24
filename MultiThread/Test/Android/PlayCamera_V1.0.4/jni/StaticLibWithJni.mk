

Palm_Module       :=
Palm_INCLUDE      :=

Current :=.
Current1:=../../../../Core
PalmCore_INCLUDE                        := $(Current)/PalmCore/include
PalmCore_PATH                           := $(Current)/PalmCore/src
PalmCore_C_FILES                        := 
Palm_SRC_FILES                      := $(addprefix $(PalmCore_PATH)/, $(PalmCore_C_FILES))


Palm_Module       += $(Palm_SRC_FILES)
Palm_INCLUDE      += $(PalmCore_INCLUDE)

Core_INCLUDE := $(Current1)/include
Palm_INCLUDE      += $(Core_INCLUDE)


Interface_INCLUDE                        := $(Current)/Interface/include
Interface_PATH                           := $(Current)/Interface/src
Interface_C_FILES                        := #PalmInterface.c \
											
										   
Interface_SRC_FILES                      := $(addprefix $(Interface_PATH)/, $(Interface_C_FILES))


Palm_Module       += $(Interface_SRC_FILES)
Palm_INCLUDE      += $(Interface_INCLUDE)

JniInterface_INCLUDE                        := $(Current)/JniInterface
JniInterface_PATH                           := $(Current)/JniInterface
JniInterface_C_FILES                        := PalmInterface_jni.cpp \
												
										   
JniInterface_SRC_FILES                      := $(addprefix $(JniInterface_PATH)/, $(JniInterface_C_FILES))


Palm_Module       += $(JniInterface_SRC_FILES)
Palm_INCLUDE      += $(JniInterface_INCLUDE)







