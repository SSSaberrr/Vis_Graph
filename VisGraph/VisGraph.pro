CONFIG += qt warn_off debug console

HEADERS += BarDisplay.h
HEADERS += BasicFunctions.h
HEADERS += ColorMap.h
HEADERS += CursorShape.h
HEADERS += DataItemEditWidget.h
HEADERS += DataManager.h
HEADERS += DataUser.h
HEADERS += DisplayWidget.h
HEADERS += GraphBase.h
HEADERS += GraphDisplay.h
HEADERS += GraphDisplayWidget.h
HEADERS += InfoTheory.h
HEADERS += MessageCenter.h
HEADERS += qtcolorpicker.h
HEADERS += trackball.h
HEADERS += typeOperation.h
HEADERS += ViewportWidget.h
HEADERS += VisGraph.h
HEADERS += VisGraphRenderWidget.h
HEADERS += VisibilityGraph.h
HEADERS += VisibilityGraphDisplayWidget.h
HEADERS += VisibilityGraphMainWindow.h
HEADERS += VolumeData.h

SOURCES += ColorMap.cpp
SOURCES += CursorShape.cpp
SOURCES += DataItemEditWidget.cpp
SOURCES += DataManager.cpp
SOURCES += DisplayWidget.cpp
SOURCES += GraphDisplayWidget.cpp
SOURCES += InfoTheory.cpp
SOURCES += main.cpp
SOURCES += MessageCenter.cpp
SOURCES += qtcolorpicker.cpp
SOURCES += trackball.cpp
SOURCES += ViewportWidget.cpp
SOURCES += VisGraph.cpp
SOURCES += VisGraphRenderWidget.cpp
SOURCES += VisibilityGraphDisplayWidget.cpp
SOURCES += VisibilityGraphMainWindow.cpp
SOURCES += VolumeData.cpp

#QT_OPENGL_INC=F:\glew-2.2.0\include
#QT_OPENGL_LIB=F:\glew-2.2.0\lib\Release\x64
#QT_WIN_SDK=C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\Lib\um\x86
#QT_MSVC=F:\Qt\Qt5.13.2\5.13.2\msvc2017_64\mkspecs\win32-msvc
#QT_EIGEN=F:\Eigen\eigen3
#QT_OPENCV=F:\opencv\build

#CUDA_PATH_V10_2=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v10.2
#VULKAN_SDK=F:\VulkanSDK\1.2.148.1

INCLUDEPATH += $$(QT_MSVC)
INCLUDEPATH += $$(CUDA_PATH_V10_2)/include
INCLUDEPATH += $$(QT_EIGEN)
INCLUDEPATH += $$(QT_OPENGL_INC)
INCLUDEPATH += $$(VULKAN_SDK)/include
INCLUDEPATH += $$(QT_OPENCV)/include
INCLUDEPATH += $$(QT_OPENCV)/include/opencv
INCLUDEPATH += $$(QT_OPENCV)/include/opencv2
INCLUDEPATH += ..
INCLUDEPATH += "F:\glew-2.2.0\include"

DEPENDPATH += $$(CUDA_PATH_V10_2)/lib/x64
DEPENDPATH += $$(QT_OPENGL_LIB)
DEPENDPATH += $$quote(QT_WIN_SDK)
DEPENDPATH += $$(QT_OPENCV)/x64/vc15/lib


LIBS += $$(CUDA_PATH_V10_2)/lib/x64/cudart.lib
LIBS += $$(CUDA_PATH_V10_2)/lib/x64/cuda.lib
LIBS += $$(CUDA_PATH_V10_2)/lib/x64/curand.lib
LIBS += $$(QT_OPENGL_LIB)/glew32.lib
LIBS += $$(QT_OPENCV)/x64/vc15/lib/opencv_world3410d.lib
LIBS += OpenGL32.lib
LIBS += Glu32.lib




CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
CUDA_LIBS = $$join(LIBS, ' -l','-l',' ')
Debug: OBJECTS_DIR = ./Win32/Debug/
Release: OBJECTS_DIR = ./Win32/Release/

Debug: NVCCFLAGS = -gencode=arch=compute_75,code=\\\"sm_75,compute_75\\\" --machine 64 -ccbin $$quote(QT_WIN_SDK) -Xcompiler \"/EHsc /W3 /nologo /Od /Zi /MDd\"

Release: NVCCFLAGS = -gencode=arch=compute_75,code=\\\"sm_75,compute_75\\\" --machine 64 -ccbin $$quote(QT_WIN_SDK) -Xcompiler \"/EHsc /W3 /nologo /Od /Zi /MD\"

cuda.input = CUDA_SOURCES
cuda.output = $$OBJECTS_DIR${QMAKE_FILE_BASE}_cuda.obj
cuda.commands = \"$$(CUDA_PATH)/bin/nvcc\" $$NVCCFLAGS $$CUDA_INC $$CUDA_LIBS ${QMAKE_FILE_NAME} -maxrregcount=0  --compile -o ${QMAKE_FILE_OUT}
cuda.dependency_type = TYPE_C
cuda.depend_command = \"$$(CUDA_PATH)/bin/nvcc\" -g -G -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}

QMAKE_EXTRA_COMPILERS += cuda

FORMS += VisGraph.ui
QT += opengl widgets
