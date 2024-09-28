#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <thread>
#include <vector>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
    // 启动Windows socket2 环境
    // 初始化套节字
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);

    // 1 创建一个socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == sock)
    {
        cout << "错误，建立socket失败" << endl;
        closesocket(sock);
        WSACleanup();
    }
    else
    {
        cout << "建立socket成功.." << endl;
    }
    // 2 连接服务器
    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(4567);
    sin.sin_addr.S_un.S_addr = inet_addr("192.168.1.4");
    if (SOCKET_ERROR == connect(sock, (sockaddr *)&sin, sizeof(sockaddr_in)))
    {
        cout << "建立连接失败..." << endl;
    }
    else
    {
        cout << "建立连接成功..." << endl;
    }
    // 3 接收服务器信息
    char recvBuf[256] = {};
    int nlen = recv(sock, recvBuf, 256, 0);
    if (nlen > 0)
    {
        cout << "接受到数据：" << recvBuf << endl;
    }
    // 4 关闭套节字closesocket
    closesocket(sock);
    getchar();
    // 清楚Windows socket环境
    WSACleanup();
}
