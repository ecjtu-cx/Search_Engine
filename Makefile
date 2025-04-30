#当前路径
CUR_DIR = .#$(shell pwd)
#中间文件存放路径
OBJ_DIR = $(CUR_DIR)/obj

#通常项
CC = g++
OUT = bin/SEserver
FLAGS = -O3 -g -std=c++11 -pthread 

#源文件集合
LOG_PATH = $(CUR_DIR)/lib/easylogger

#addsuffix p1 p2 ：把p1接到p2后
C_SRCS = $(wildcard $(addsuffix /*.c,$(LOG_PATH)))
C_SRCS += $(wildcard $(addsuffix /*.c,$(LOG_PATH)/src))
C_SRCS += $(wildcard $(addsuffix /*.c,$(LOG_PATH)/port))
C_SRCS += $(wildcard $(addsuffix /*.c,$(LOG_PATH)/plugins/file))

SERVER_PATH = $(CUR_DIR)/src

CPP_SRCS = $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)))
CPP_SRCS += $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)/TcpServer))
CPP_SRCS += $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)/ThreadPool))
CPP_SRCS += $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)/Cache))
CPP_SRCS += $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)/KeyRecommander))
CPP_SRCS += $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)/WebPageSearcher))
CPP_SRCS += $(wildcard $(addsuffix /*.cc,$(SERVER_PATH)/SearchEngineServer))

# notdir ：除去文件名前的地址信息
# basename ：除去后缀名
# addprefix p1 p2 ： 把p1接到p2前 
OBJS = $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(CPP_SRCS)))))
OBJS += $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(C_SRCS)))))

#头文件集合
INC_DIR  =  ./lib/easylogger/inc ./lib/easylogger/plugins/file
INC_DIR  += ./lib/easylogger/plugins/port
INC_DIR  += ./lib/cppjieba/include/cppjieba
INC_DIR  += ./include/ ./include/Cache 
INC_DIR  += ./include/KeyRecommander  ./include/TcpServer
INC_DIR  += ./include/ThreadPool ./include/WebPageSearcher
INC_DIR  += ./include/SearchEngineServer

INC = $(addprefix -I,$(INC_DIR))

#源文件搜索路径集合
C_DIR = $(LOG_PATH) $(LOG_PATH)/src \
		$(LOG_PATH)/port $(LOG_PATH)/plugins/file

CPP_DIR = $(SERVER_PATH)/KeyRecommander \
		  $(SERVER_PATH)/WebPageSearcher \
		  $(SERVER_PATH)/SearchEngineServer \
		  $(SERVER_PATH)/ThreadPool  \
		  $(SERVER_PATH)/TcpServer \
		  $(SERVER_PATH)/Cache \
		  $(SERVER_PATH) 


#搜索路径
vpath
vpath %.c $(C_DIR)
vpath %.cc $(CPP_DIR)
vpath %.h $(INC_DIR)
vpath %.hpp $(INC_DIR)
vpath %.o $(OBJ_DIR)

ALL:$(OUT)

$(OUT):$(notdir $(OBJS))
	$(CC) $(OBJ_DIR)/*.o $(FLAGS) $(INC) -o $(OUT) 

%.o:%.c
	@cc $< -g -Wall  $(INC) -c -o $(OBJ_DIR)/$@ 

%.o:%.cc
	@$(CC) $< $(FLAGS) $(INC) -c -o $(OBJ_DIR)/$@ 


.PHONY:clean rebuild
clean:
	rm -f $(OUT) $(OBJ_DIR)/*.o
rebuild:clean ALL