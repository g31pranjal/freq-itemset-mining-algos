cmake_minimum_required(VERSION 3.8)
project(umdblab)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        cpp-frame/udb/testDB.cpp
        cpp-frame/udb/uAlgoFramework.cpp
        cpp-frame/udb/uAlgoFramework.h
        cpp-frame/udb/uBitset.cpp
        cpp-frame/udb/uBitset.h
        cpp-frame/udb/uItem.cpp
        cpp-frame/udb/uItem.h
        cpp-frame/udb/uTidset.cpp
        cpp-frame/udb/uTidset.h
        cpp-frame/udb/uTransactionDatabase.cpp
        cpp-frame/udb/uTransactionDatabase.h
        cpp-frame/algoFramework.cpp
        cpp-frame/algoFramework.h
        cpp-frame/alterDB.cpp
        cpp-frame/transactionDatabase.cpp
        cpp-frame/transactionDatabase.h)

add_executable(umdblab ${SOURCE_FILES})