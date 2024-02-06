CC := gcc
CFLAGS := -Wall -Wextra

# 获取所有的.c文件
SOURCES := $(wildcard *.c)
# 生成对应的目标文件列表
OBJECTS := $(SOURCES:.c=.o)

# 主要的目标
all: PrintMCUCan

# PrintMCUCan是你要生成的可执行文件
PrintMCUCan: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# 为每个.c文件生成对应的.o文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f PrintMCUCan $(OBJECTS)