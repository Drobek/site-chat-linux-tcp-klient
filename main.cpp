#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <netinet/ether.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFSIZE 1000

using namespace std;

int main(int argc, char *argv[]) {
    string text("Ahoj\n"); // Odesílaný a přijímaný text
    hostent *host; // Vzdálený počítač;
    sockaddr_in serverSock; // Vzdálený "konec potrubí"
    int mySocket; // Soket
    int port; // Číslo portu
    char buf[BUFSIZE]; // Přijímací buffer
    int size; // Počet přijatých a odeslaných bytů
    if (argc != 3) {
        cerr << "Syntaxe:\n\t" << argv[0]
                << " " << "adresa port" << endl;
        return -1;
    }
    port = atoi(argv[2]);
    // Zjistíme info o vzdáleném počítači
    if ((host = gethostbyname(argv[1])) == NULL) {
        cerr << "Špatná adresa" << endl;
        return -1;
    }
    // Vytvoříme soket
    if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        cerr << "Nelze vytvořit soket" << endl;
        return -1;
    }

    // Zaplníme strukturu sockaddr_in
    // 1) Rodina protokolů
    serverSock.sin_family = AF_INET;
    // 2) Číslo portu, ke kterému se připojíme
    serverSock.sin_port = htons(port);
    // 3) Nastavení IP adresy, ke které se připojíme
    memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
    // Připojení soketu

    //    if (connect(mySocket, (sockaddr *) & serverSock, sizeof (serverSock)) == -1) {
    //        cerr << "Nelze navázat spojení" << endl;
    //        return -1;
    //    }

    //    while (true) {
    string input = "";
    // Odeslání dat
    cout << "zadejte zpravu: ";
    getline(cin, input);

    if (connect(mySocket, (sockaddr *) & serverSock, sizeof (serverSock)) == -1) {
        cerr << "Nelze navázat spojení" << endl;
        return -1;
    }
    if ((size = send(mySocket, input.c_str(), input.size() + 1, 0)) == -1) {
        cerr << "Problém s odesláním dat" << endl;
        return -1;
    }
    //    cout << "Odesláno " << size << endl;
    // Příjem dat
    text = "";
    int can_end = 0;
    while (((size = recv(mySocket, buf, BUFSIZE, 0)) != -1) && (can_end == 0)) {
        //            cout << "Přijato " << size << endl;
        text += buf;
        //            cout << text << endl;
        if (size == 0 || text.length() < BUFSIZE) can_end = 1;
        if (can_end == 1) break;
    }
    cout << text << endl;
    close(mySocket);

    //    }
    //    cout << endl << text << endl;
    return 0;
}