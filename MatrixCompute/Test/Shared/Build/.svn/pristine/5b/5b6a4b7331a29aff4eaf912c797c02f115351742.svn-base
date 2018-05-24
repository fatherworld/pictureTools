ifeq ($(HOST_OS),windows)
CUST_TEST_CMD                    := dir
endif
ifeq ($(HOST_OS),linux)
CUST_TEST_CMD                    := test -f
endif

AlvaProjectPredefine_Module       :=
AlvaProjectPredefine_INCLUDE      :=

GC_INCLUDE                        := $(Shared_INCLUDE)/GlobalConfig
GC_PATH                           := $(Shared_PATH)/GlobalConfig
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\GlobalConfig
HAVE_TEST_CUST_GC                 := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_GC),)
	AlvaProjectPredefine_Module       +=
	AlvaProjectPredefine_INCLUDE      += $(GC_INCLUDE)
endif


STD_MULTI_PLATFORM_INCLUDE        := $(Shared_INCLUDE)/std-multi-platform
STD_MULTI_PLATFORM_PATH           := $(Shared_PATH)/std-multi-platform
STD_MULTI_PLATFORM_C_FILES        := aligned_malloc.c
STD_MULTI_PLATFORM_SRC_FILES      := $(addprefix $(STD_MULTI_PLATFORM_PATH)/, $(STD_MULTI_PLATFORM_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\std-multi-platform
HAVE_TEST_CUST_STD_MULTI_PLATFORM := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_STD_MULTI_PLATFORM),)
	AlvaProjectPredefine_Module       += $(STD_MULTI_PLATFORM_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(STD_MULTI_PLATFORM_INCLUDE)
endif


DT_INCLUDE                        := $(Shared_INCLUDE)/DebugTools/include
DT_PATH                           := $(Shared_PATH)/DebugTools/src
DT_C_FILES                        := Dump.c \
                                     AlvaTimer.c
DT_SRC_FILES                      := $(addprefix $(DT_PATH)/, $(DT_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\DebugTools\include
HAVE_TEST_CUST_DT                 := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_DT),)
	AlvaProjectPredefine_Module       += $(DT_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(DT_INCLUDE)
endif


DynamicOCL_INCLUDE                := $(Shared_INCLUDE)/DynamicOCL/include
DynamicOCL_PATH                   := $(Shared_PATH)/DynamicOCL/src
DynamicOCL_SRCS                   := Module.c \
                                     SafeCL.c \
                                     SafeCL_GL.c
DynamicOCL_SRC_FILES              := $(addprefix $(DynamicOCL_PATH)/, $(DynamicOCL_SRCS))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\DynamicOCL\include
HAVE_TEST_CUST_DynamicOCL         := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_DynamicOCL),)
	AlvaProjectPredefine_Module       += $(DynamicOCL_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(DynamicOCL_INCLUDE)
endif


OCL_INCLUDE                       := $(Shared_INCLUDE)/OclHelper/include
OCL_PATH                          := $(Shared_PATH)/OclHelper/src
OCL_C_FILES                       := CL_Env.c \
                                     CL_Kernel_Mixup.c \
                                     CL_Mem.c \
                                     DE_cryption.c \
                                     enflCreateProgramWithSource.c
OCL_SRC_FILES                     := $(addprefix $(OCL_PATH)/, $(OCL_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\OclHelper\include
HAVE_TEST_CUST_OCL                := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_OCL),)
	AlvaProjectPredefine_Module       += $(OCL_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(OCL_INCLUDE)
endif


Eigen_INCLUDE                     := $(Shared_INCLUDE)/Eigen
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\Eigen
HAVE_TEST_CUST_Eigen              := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_Eigen),)
	AlvaProjectPredefine_Module       += 
	AlvaProjectPredefine_INCLUDE      += $(Eigen_INCLUDE)
endif


TooN_INCLUDE                      := $(Shared_INCLUDE)/TooN
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\TooN
HAVE_TEST_CUST_TooN               := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_TooN),)
	AlvaProjectPredefine_Module       += 
	AlvaProjectPredefine_INCLUDE      += $(TooN_INCLUDE)
endif


Filter_INCLUDE                    := $(Shared_INCLUDE)/Filter/include
Filter_PATH                       := $(Shared_PATH)/Filter/src
FILTER_C_FILES                    := AlvaFilter.c \
                                     Damp.c \
                                     Filter_Threshold.c \
                                     Filter_Truncation.c \
                                     Filter_ConditionDamp.c
FILTER_SRC_FILES                  := $(addprefix $(Filter_PATH)/, $(FILTER_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\Filter\include
HAVE_TEST_CUST_Filter             := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_Filter),)
	AlvaProjectPredefine_Module       += $(FILTER_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(Filter_INCLUDE)
endif


Prediction_INCLUDE                := $(Shared_INCLUDE)/Prediction/include
Prediction_PATH                   := $(Shared_PATH)/Prediction/src
PREDICTION_C_FILES                := AlvaPrediction.c \
                                     ConstSpeed.c \
                                     ConstAcceleration.c \
                                     ConstSpeedH.c
PREDICTION_SRC_FILES              := $(addprefix $(Prediction_PATH)/, $(PREDICTION_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\Prediction\include
HAVE_TEST_CUST_Prediction         := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_Filter),)
	AlvaProjectPredefine_Module       += $(PREDICTION_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(Prediction_INCLUDE)
endif


Project_INCLUDE                   := $(Shared_INCLUDE)/Project/include
Project_PATH                      := $(Shared_PATH)/Project/src
Project_C_FILES                   := ComputeH_Direct.c
Project_SRC_FILES                 := $(addprefix $(Project_PATH)/, $(Project_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\Project\include
HAVE_TEST_CUST_Project            := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_Project),)
	AlvaProjectPredefine_Module       += $(Project_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(Project_INCLUDE)
endif


Consistency_INCLUDE               := $(Shared_INCLUDE)/Consistency/include
Consistency_PATH                  := $(Shared_PATH)/Consistency/src
Consistency_C_FILES               := Consistency_Rand.c
Consistency_SRC_FILES             := $(addprefix $(Consistency_PATH)/, $(Consistency_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\Consistency\include
HAVE_TEST_CUST_Consistency        := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_Consistency),)
	AlvaProjectPredefine_Module       += $(Consistency_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(Consistency_INCLUDE)
endif


AngleHelper_INCLUDE               := $(Shared_INCLUDE)/AngleHelper/include
AngleHelper_PATH                  := $(Shared_PATH)/AngleHelper/src
AngleHelper_C_FILES               := EulerRotationMatrix.c \
                                     QuarternionRotationMatrix.c
AngleHelper_SRC_FILES             := $(addprefix $(AngleHelper_PATH)/, $(AngleHelper_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\AngleHelper\include
HAVE_TEST_CUST_AngleHelper        := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_AngleHelper),)
	AlvaProjectPredefine_Module       += $(AngleHelper_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(AngleHelper_INCLUDE)
endif


PoseHelper_INCLUDE                := $(Shared_INCLUDE)/PoseHelper/Core/include
PoseHelper_INTERFACE              := $(Shared_INCLUDE)/PoseHelper/interface
PoseHelper_PATH                   := $(Shared_PATH)/PoseHelper/Core/src
POSE_HELPER_C_FILES               := ComputePose.cpp \
                                     ComputeH.cpp
POSE_HELPER_SRC_FILES             := $(addprefix $(PoseHelper_PATH)/, $(POSE_HELPER_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\PoseHelper\Core\include
HAVE_TEST_CUST_PoseHelper         := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_PoseHelper),)
	AlvaProjectPredefine_INCLUDE      += $(PoseHelper_INCLUDE)
	AlvaProjectPredefine_INCLUDE      += $(PoseHelper_INTERFACE)
endif
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\PoseHelper\Core\src\ComputePose.cpp
HAVE_TEST_CUST_PoseHelper_Src     := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)")
ifneq ($(HAVE_TEST_CUST_PoseHelper),)
	AlvaProjectPredefine_Module       += $(POSE_HELPER_SRC_FILES)
endif


ImageHelper_INCLUDE               := $(Shared_INCLUDE)/ImageHelper/include
ImageHelper_PATH                  := $(Shared_PATH)/ImageHelper/src
IMAGE_HELPER_C_FILES              := BmpHelper.c \
                                     ImageConverter.c \
                                     ImageDistortionCorrection.c \
                                     ImageFilter.c \
                                     ImageResize.c \
                                     ImageRotate.c 
IMAGE_HELPER_SRC_FILES            := $(addprefix $(ImageHelper_PATH)/, $(IMAGE_HELPER_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\ImageHelper\include
HAVE_TEST_CUST_ImageHelper        := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_ImageHelper),)
	AlvaProjectPredefine_Module       += $(IMAGE_HELPER_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(ImageHelper_INCLUDE)
endif


JNI_HELPER_INCLUDE                := $(Shared_INCLUDE)/JniHelper/include
JNI_HELPER_PATH                   := $(Shared_PATH)/JniHelper/src
JNI_HELPER_C_FILES                := JniHolder.c \
                                     AndroidHelper.c \
                                     JniLoad.cpp
JNI_HELPER_SRC_FILES              := $(addprefix $(JNI_HELPER_PATH)/, $(JNI_HELPER_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\JniHelper\include
HAVE_TEST_CUST_JNI_HELPER         := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_JNI_HELPER),)
	AlvaProjectPredefine_Module       += $(JNI_HELPER_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(JNI_HELPER_INCLUDE)
endif


AuthChecker_INCLUDE               := $(Shared_INCLUDE)/AuthChecker
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\AuthChecker
HAVE_TEST_CUST_AuthChecker        := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_AuthChecker),)
	AlvaProjectPredefine_Module       += 
	AlvaProjectPredefine_INCLUDE      += $(AuthChecker_INCLUDE)
endif

XML_HELPER_INCLUDE                := $(Shared_INCLUDE)/XmlHelper
XML_HELPER_PATH                   := $(Shared_PATH)/XmlHelper
XML_HELPER_C_FILES                := XML.c
XML_HELPER_SRC_FILES              := $(addprefix $(XML_HELPER_PATH)/, $(XML_HELPER_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\XmlHelper
HAVE_TEST_CUST_XML_HELPER         := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_XML_HELPER),)
	AlvaProjectPredefine_Module       += $(XML_HELPER_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(XML_HELPER_INCLUDE)
endif


SharedModule_INCLUDE              := $(Shared_INCLUDE)/SharedModule/Implements/include
SharedModule_INTERFACE            := $(Shared_INCLUDE)/SharedModule/Implements/interface
SharedModule_PATH                 := $(Shared_PATH)/SharedModule/Implements/src
SharedModule_C_FILES              := SharedMachineCode.c
SharedModule_SRC_FILES            := $(addprefix $(SharedModule_PATH)/, $(SharedModule_C_FILES))

SharedModule_JI_PATH              := $(Shared_PATH)/SharedModule/JniInterface
SharedModule_JI_FILES             := SharedMachineCode_Jni.cpp
SharedModule_JI_SRC_FILES         := $(addprefix $(SharedModule_JI_PATH)/, $(SharedModule_JI_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\SharedModule\Implements\include
HAVE_TEST_CUST_SharedModule       := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_SharedModule),)
	AlvaProjectPredefine_Module       += $(SharedModule_SRC_FILES) \
                                         $(SharedModule_JI_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(SharedModule_INCLUDE) \
                                         $(SharedModule_INTERFACE)
endif


DataQueue_INCLUDE                 := $(Shared_INCLUDE)/DataQueue/Implements/include
DataQueue_INTERFACE               := $(Shared_INCLUDE)/DataQueue/Implements/interface
DataQueue_PATH                    := $(Shared_PATH)/DataQueue/Implements/src
DataQueue_C_FILES                 := DataQueue.c
DataQueue_SRC_FILES               := $(addprefix $(DataQueue_PATH)/, $(DataQueue_C_FILES))

DataQueue_JI_PATH                 := $(Shared_PATH)/DataQueue/JniInterface
DataQueue_JI_FILES                := DataQueue_Jni.cpp
DataQueue_JI_SRC_FILES            := $(addprefix $(DataQueue_JI_PATH)/, $(DataQueue_JI_FILES))
M_CHECK_DataQueue_INCLUDE         := $(Shared_Check_INCLUDE)\DataQueue\Implements\include
HAVE_TEST_CUST_DataQueue          := $(shell $(CUST_TEST_CMD) "$(M_CHECK_DataQueue_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_DataQueue),)
	AlvaProjectPredefine_Module       += $(DataQueue_SRC_FILES) \
                                             $(DataQueue_JI_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(DataQueue_INCLUDE) \
                                             $(DataQueue_INTERFACE)
endif


MultiThread_INCLUDE               := $(Shared_INCLUDE)/MultiThread/include
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\MultiThread\include
HAVE_TEST_CUST_MultiThread        := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_MultiThread),)
	AlvaProjectPredefine_Module       += 
	AlvaProjectPredefine_INCLUDE      += $(MultiThread_INCLUDE)
endif


MatrixCompute_INCLUDE             := $(Shared_INCLUDE)/MatrixCompute/include
MatrixCompute_PATH                := $(Shared_PATH)/MatrixCompute/src
MatrixCompute_C_FILES             := AlvaMatrix.cpp 
MatrixCompute_SRC_FILES           := $(addprefix $(MatrixCompute_PATH)/, $(MatrixCompute_C_FILES))
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\MatrixCompute\include
HAVE_TEST_CUST_MatrixCompute      := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_MatrixCompute),)
	AlvaProjectPredefine_Module       += $(MatrixCompute_SRC_FILES)
	AlvaProjectPredefine_INCLUDE      += $(MatrixCompute_INCLUDE)
endif


FFTW_INCLUDE                      := $(Shared_INCLUDE)/FFTW/include
M_CHECK_Shared_INCLUDE            := $(Shared_Check_INCLUDE)\FFTW\include
HAVE_TEST_CUST_FFTW               := $(shell $(CUST_TEST_CMD) "$(M_CHECK_Shared_INCLUDE)\")
ifneq ($(HAVE_TEST_CUST_FFTW),)
    AlvaProjectPredefine_Module       += $(Shared_PATH)/FFTW/Android/libfftw3.a
    AlvaProjectPredefine_INCLUDE      += $(FFTW_INCLUDE)
endif