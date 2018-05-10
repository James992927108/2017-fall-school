(上傳版本)
編譯環境 ubuntu
執行方法
step 1 在資料夾下面開啟終端機 指令 make 編譯完成後
	會有產生 server.c 和 client.c
step 2 分別在不同的終端機執行 server.c 和 client
先啟動server再啟動client
server啟動方式 ./server 1234(port自行定義)
client啟動方式 ./client 1234(port必須與server設定相同) 127.0.0.1(ip 自行設定)

step 3 啟動 client後必須先輸入name之後再輸入group
client端輸入完後server 接收到 便會 在server端顯示client連線

step 4 client可以對server下指令

功能列表 

1) create homework2.c rwr---
2) read homework2.c
3) write homework2.c o/a
4) modify homework2.c rw---- 
最後一個infomation是顯示權限得變化

詳細作業要求看hw2
