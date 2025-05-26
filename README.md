## 项目简介

这是一个基于C++实现的搜索引擎系统，支持中文分词、网页预处理、关键词推荐和全文检索等功能。项目采用模块化设计，包含多个独立组件，如网页预处理器、搜索服务器、客户端等。

## 功能特性

- 支持中文分词（基于cppjieba）
- 网页预处理和索引
- 关键词推荐功能
- 多线程并发处理
- 高效的缓存机制
- TCP通信协议
- 客户端-服务器架构

## 系统架构

项目包含以下主要组件：

- **SearchEngineServer**: 核心搜索引擎服务器
- **PagePreprocessor**: 网页预处理模块
- **WebPageSearcher**: 网页搜索模块
- **KeyRecommander**: 关键词推荐模块
- **DictProducer**: 字典生成工具
- **TcpServer**: TCP服务器实现
- **ThreadPool**: 线程池
- **Cache**: 缓存系统

## 安装与构建

### 依赖项

- C++编译器（推荐G++ 4.1以上或Clang++）
- Make构建工具
- CMake（用于构建cppjieba，推荐2.6以上版本）

### 构建步骤

```bash
# 克隆仓库（如果是首次获取代码）
git clone <仓库URL>
cd Search_Engine

# 编译项目
make

# 构建cppjieba库（如果需要）
cd lib/cppjieba
mkdir build && cd build
cmake ..
make
```

## 使用方法

### 启动服务器

```bash
cd bin
./SEserver
```

### 运行客户端

```bash
cd bin
./Client
```

### 网页预处理

```bash
cd bin
./Pageprocessor
```

## 数据目录

- **data/cache/**: 缓存数据
- **data/dict/**: 字典文件
- **data/page/**: 网页数据
- **data/stopWord/**: 停用词
- **data/xml/**: XML格式的原始文档

## 配置文件

系统配置在server.conf文件中，可根据需要进行修改。

## 中文分词

本项目使用[cppjieba](https://github.com/yanyiwu/cppjieba)作为中文分词库，这是"结巴"中文分词的C++版本，特点是:

- 支持UTF-8编码
- 项目自带完善的单元测试
- 支持载入自定义用户词典