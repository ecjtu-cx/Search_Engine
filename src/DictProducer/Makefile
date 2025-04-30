#当前路径
CUR_DIR = .#$(shell pwd)
#中间文件存放路径
OBJ_DIR = $(CUR_DIR)/obj

#通常项
CC = g++
OUT = ../../bin/Cutter
FLAGS = -O3 -g -std=c++11 -lpthread

SRCS = $(wildcard $(addsuffix /*.cc,$(CUR_DIR)))

#中间文件集合
# notdir ：除去文件名前的地址信息
# basename ：除去后缀名
# addprefix p1 p2 ： 把p1接到p2前 
OBJS = $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(SRCS)))))
OBJS += Configuration.o

INC_DIR  = .../../lib/cppjieba/include/cppjieba
INC_DIR  += ../../include/DictProducer
INC_DIR  += ../../include/SearchEngineServer
INC_DIR  += ../../include

INC  =  $(addprefix -I,$(INC_DIR))

ALL : $(OUT)

$(OUT) : $(notdir $(OBJS))
	$(CC) $(OBJ_DIR)/*.o $(FLAGS) -o $(OUT)
%.o:%.cc
	@$(CC) $< $(FLAGS) $(INC) -c -o $(OBJ_DIR)/$@
Configuration.o:../SearchEngineServer/Configuration.cc
	@$(CC) $< $(FLAGS) $(INC) -c -o $(OBJ_DIR)/$@

.PHONY:clean rebuild
clean:
	rm -f $(OUT) $(OBJ_DIR)/*.o
rebuild:clean ALL