#include <WinSock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

int main(){
    // check Version
    WORD verSion = MAKEWORD(2, 2);
    WSADATA dat;

    int nRes = WSAStartup(verSion, &dat);

    if (2 != HIBYTE(dat.wVersion) || 2 != LOBYTE(dat.wVersion))
    {
        std::cout << "WSA has a wrong version!" << std::endl;
        WSACleanup();
        return 0;
    }

    // create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sin = {};
    sin.sin_family = AF_INET; // 地址类型
    sin.sin_port = htons(4567); // 端口号
    sin.sin_addr.S_un.S_addr = inet_addr("192.168.1.4"); // ip地址

    // bind ip addr
    if (SOCKET_ERROR == bind(sock, (sockaddr*)&sin, sizeof(sin)))
    {
        std::cout << "绑定网络端口失败" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }
    std::cout << "绑定网络端口成功" << std::endl;

    // listen socket
    if (SOCKET_ERROR == listen(sock, SOMAXCONN))
    {
        std::cout << "监听网络端口失败" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }
    std::cout << "监听网络端口成功" << std::endl;

    // wait for connection
    sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET cSock = INVALID_SOCKET;
	cSock = accept(sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == cSock)
	{
		std::cout << "错误，接受到无效客户端SOCKET..." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 0;
	}
    std::cout << "新客户端加入：" << inet_ntoa(clientAddr.sin_addr) << std::endl;

    // receive message from connection
    std::string msg = "Hello, I'm Server";
    char recvBuf[1024] = {};
    while (true){
        int nlen = recv(cSock, recvBuf, sizeof(recvBuf), 0);
        if (nlen == 0)
        {
            std::cout << "客户端退出..." << std::endl;
            break;
        }
        
        send(cSock, msg.c_str(), msg.length(), 0);
    }

    // close socket after use
    closesocket(sock);
    // close WSA library after use
    WSACleanup();
    return 0;
}

