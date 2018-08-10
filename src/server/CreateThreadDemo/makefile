#源文件，自动找所有.c和.cpp文件，并将目标定义为同名.o文件
SOURCE  	:= $(wildcard *.c) $(wildcard *.cpp)
OBJS    	:= $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE))) 


#目标文件名，输入任意你想要的执行文件名
TARGET  	:= epolldemo

#编译参数
CC      	:= g++ 
LIBS    	:= -lpthread 
LDFLAGS 	:=
DEFINES 	:= 
INCLUDE 	:= -I.  
CFLAGS  	:= -g -Wall $(DEFINES) $(INCLUDE)
CXXFLAGS	:= $(CFLAGS) 

.PHONY : everything objs clean veryclean rebuild install good
everything 	: $(TARGET)
all 		: $(TARGET)
objs 		: $(OBJS)
rebuild: 	veryclean everything 
clean :
	rm -fr *.o
	rm -fr $(TARGET)
veryclean 	: clean 
install:
	cp $(TARGET) ../bin
	#cp ./cfg/* ../bin/cfg
good :		rebuild install
$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
