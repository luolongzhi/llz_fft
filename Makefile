#  llzlab - luolongzhi algorithm lab 
#  Copyright (C) 2012 luolongzhi (Chengdu, China)
#
#  This program is part of llzlab, all copyrights are reserved by luolongzhi. 
#
#  filename: Makefile
#  time    : 2012/07/07 18:42 
#  author  : luolongzhi ( luolongzhi@gmail.com )
#
#


include Makefile.include

INCLUDEDIR = ./include

ifeq ($(ARCH),ARCH_ARM)
LDFLAGS += -lm -lpthread  
else
LDFLAGS += -lm -lpthread  
endif

TARGET    =  llzfft 
CSRCFILES =  $(shell ls *.c)
COBJFILES =  $(patsubst %.c,%.o,$(CSRCFILES))

SRCFILES  =	 $(CSRCFILES) 
OBJFILES  =	 $(COBJFILES) 

CFLAGS    += -I.  -I$(INCLUDEDIR)

all: $(OBJFILES) 
	rm -f $(TARGET) 
	$(CC) -o $(TARGET) $(OBJFILES) $(LDFLAGS)	 

clean : 
	@rm -f *.o 
	@rm -f *.out	
	@rm -f $(TARGET)	

install :

