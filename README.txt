开发工具：Qt Creator 5.3.0
开发环境：Linux
文件结构&功能说明:
        |-cipher（保存密文文件）
                |-flag.txt（密文文件标志）
                |-0.txt（密文文件）
                ...
        |-debug（保存可执行文件和目标文件）
                |-dsse（可执行文件）
                |-dsse.o（目标文件）
                ...
        |-libtomcrypt（开源加密算法库）
                |-
                ...
        |-makedata（生成原始文件）
                |-makedata.c
                |-make
        |-original（原始文件）
                |-keyword.txt（关键字）
                |-keyword_id.txt（关键字和ID的对应关系）
                |-original1.txt（原始文件1）
                |-original2.txt（原始文件2）
        |-plain（保存解密后的明文文件）
                |-0.txt（明文文件）
                ...
        |-src（源代码）
                |-avltree.h
                |-avltree.cpp
                ...
