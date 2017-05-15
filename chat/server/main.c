#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include "workWithMessages.h"

int PORT = 1337;

typedef struct {
    int sockId;
    int threadInd;
} ThreasdInfo;

typedef struct {
    int isActive;
    int sockId;
    char login[NAME_LEN];
    char password[NAME_LEN];
    size_t userInd;
    int threadInd;
} ClientAddr;

pthread_mutex_t data_mutex;
pthread_t threads[NUM_THREADS];
ClientAddr clientsData[NUM_THREADS];
char userNames[MAX_CONNECTIONS][NAME_LEN];
char history[MAX_MESSAGES_COUNT][STRING_LEN];
//contain all message 'r' + timestamp + login + body, how it has sent
int usersConnectCount = 0;
size_t userLoginsCount = 0;
int threadsCount = 0;
int messagesInHistoryCount = 0;

//this function get userId or error
int addUser(int sock, char* login, char* password, int threadInd, int* isNewActive) {
    *isNewActive = 1;
    int isNewLogin = 1;
    int userFind = -1;
    size_t userFindInd = 0;
    pthread_mutex_lock(&data_mutex);
    for (int i = 0; i < usersConnectCount; ++i) {
        if (!strcmp(login, clientsData[i].login)) {
            isNewLogin = 0;
            userFindInd = clientsData[i].userInd;
            if (strcmp(password, clientsData[i].password)) {
                pthread_mutex_unlock(&data_mutex);
                return -3;
            } else if (!clientsData[i].isActive && userFind == -1) {
                clientsData[i].isActive = 1;
                clientsData[i].sockId = sock;
                clientsData[i].threadInd = threadInd;
                userFind = i;
            } else if (clientsData[i].isActive) {
                *isNewActive = 0;
            }
        }
    }
    if (userFind != -1) {
        pthread_mutex_unlock(&data_mutex);
        return userFind;
    }
    int ind = usersConnectCount++;
    strcpy(clientsData[ind].login, login);
    strcpy(clientsData[ind].password, password);
    clientsData[ind].isActive = 1;
    clientsData[ind].sockId = sock;
    clientsData[ind].threadInd = threadInd;
    if (!isNewLogin) {
        clientsData[ind].userInd = userFindInd;
    } else {
        clientsData[ind].userInd = userLoginsCount;
        strcpy(userNames[userLoginsCount++], login);
    }
    pthread_mutex_unlock(&data_mutex);
    return ind;
}

size_t getCountActiveUsersFromUserInd(size_t ind) {
    size_t count = 0;
    pthread_mutex_lock(&data_mutex);
    for (int i = 0; i < usersConnectCount; ++i) {
        if (clientsData[i].isActive && clientsData[i].userInd == ind) {
            ++count;
        }
    }
    pthread_mutex_unlock(&data_mutex);
    return count;
}

int sendMessageToActiveUsers(char* mess) {
    size_t len = getLenMessage(mess);
    int ok = 0;
    pthread_mutex_lock(&data_mutex);
    for (int i = 0; i < usersConnectCount; ++i) {
        if (clientsData[i].isActive) {
            ssize_t cntSend = send(clientsData[i].sockId, mess, len, 0);
            if (cntSend != len) {
                fprintf(stderr, "Can't send message %c to %s\n", mess[0], clientsData[i].login);
                ok = -1;
            }
        }
    }
    pthread_mutex_unlock(&data_mutex);
    return ok;
}

void addMessageToHistory(char *mess) {
    memcpy(history[messagesInHistoryCount++], mess,
            getLenMessage(mess));
}

//this function get userId
int login(char *msg, int *userId, int sock, int threadInd) {
    printf("111\n");
    char login[NAME_LEN];
    char password[NAME_LEN];

    if (msg[0] == 'i') {
        printf("222\n");
        if (getCountStringsMessage(msg) == 2) {
            printf("333\n");
            size_t lenLogin = getLenOfStringByIndex(msg, 0);
            printf("444\n");
            size_t lenPassword = getLenOfStringByIndex(msg, 1);
            printf("555\n");
            if (lenLogin < 2 || lenPassword < 2 || lenLogin > 31 || lenPassword > 31) {
                sendStatusMessage(sock, 3);
                return -3;
            }
            printf("777\n");
            strncpy(login, getStringByIndex(msg, 0), lenLogin);
            printf("888\n");
            strncpy(password, getStringByIndex(msg, 1), lenPassword);
            printf("999\n");
            printf("%ld %ld\n", lenLogin, lenPassword);
            login[lenLogin] = 0;
            password[lenPassword] = 0;
            printString(login, lenLogin);
            printf("\n");
            printString(password, lenPassword);
            printf("\n");
            int ind;
            int isNew = 1;
            if ((ind = addUser(sock, login, password, threadInd, &isNew)) >= 0) {
                sendStatusMessage(sock, 0);
                if (isNew) {
                    char buffer[SMALL_BUFF];
                    createNotificationLoginMessage(buffer, (size_t) clientsData[ind].userInd, login);
                    sendMessageToActiveUsers(buffer);
                }
                printf("666\n");
                *userId = ind;
                return ind;
            } else {
                sendStatusMessage(sock, 3);
                return ind;
            }
        } else {
            sendStatusMessage(sock, 3);
            return -3;
        }
    } else {
        fprintf(stderr, "wrong type mess for login from user\n");
        sendStatusMessage(sock, 2);
        return -2;
    }
}

//we garantee that userId exists
int processRegularMessage(char* mess, size_t userId, int sockId) {
    if (userId == USER_ID_UNDEFINED) {
        sendStatusMessage(sockId, 2);
        return -2;
    }
    char *login = clientsData[userId].login;
    if (getCountStringsMessage(mess) != 1) {
        sendStatusMessage(sockId, 6);
        return -6;
    }
    size_t lenBody = getLenOfStringByIndex(mess, 0);
    char* body = getStringByIndex(mess, 0);
    char buffer[BUFF_SIZE];
    createRegularMessage(buffer, login, body, lenBody);
    addMessageToHistory(buffer);
    return sendMessageToActiveUsers(buffer);
}

void processLogoutMessage(size_t userId) {
    char buffer[SMALL_BUFF];
    size_t userLoginInd = clientsData[userId].userInd;
    char* login = clientsData[userId].login;
    if (getCountActiveUsersFromUserInd(userLoginInd) == 1) {
        createNotificationLogoutMessage(buffer, userLoginInd, login);
        sendMessageToActiveUsers(buffer);
    }
    sendStatusMessage(clientsData[userId].sockId, 0);
    clientsData[userId].isActive = 0;
}

void processListMessage(size_t userId) {
    int sockId = clientsData[userId].sockId;
    char *login = clientsData[userId].login;
    char buffer[BUFF_SIZE];
    char names[MAX_CONNECTIONS][NAME_LEN];
    char tmp[NAME_LEN];
    int countDiffUsers = 0;
    setTypeMessage(buffer, 'l');
    createEmptyMessage(buffer);
    pthread_mutex_lock(&data_mutex);
    for (int i = 0; i < usersConnectCount; ++i) {
        int isNew = 1;
        for (int j = 0; j < countDiffUsers; ++j) {
            if (!strcmp(names[j], clientsData[i].login)) {
                isNew = 0;
            }
        }
        if (isNew && clientsData[i].isActive) {
            size_t userInd = clientsData[i].userInd;
            writeIntToStr(tmp, userInd);
            addStringToMessage(buffer, tmp, 4);
            addStringToMessage(buffer, clientsData[i].login,
                    strlen(clientsData[i].login));
        }
    }
    pthread_mutex_unlock(&data_mutex);
    size_t allLen = getLenMessage(buffer);
    ssize_t count = send(sockId, buffer, allLen, 0);
    if (count != allLen) {
        fprintf(stderr, "Error send list message to %s\n", login);
    }
}

void processHistoryMessage(char* mess, size_t userId) {
    int sockId = clientsData[userId].sockId;
    if (getCountStringsMessage(mess) != 1) {
        sendStatusMessage(sockId, 6);
        return;
    }
    size_t count = readIntFromStr(getStringByIndex(mess, 0));
    printf("h count: %ld\n", count);
    count = min(count, MAX_HISTORY_SEND);
    int firstMess = max(messagesInHistoryCount - (int)count, 0);
    char buffer[BUFF_SIZE];
    setTypeMessage(buffer, 'h');

    pthread_mutex_lock(&data_mutex);
    for (int i = firstMess; i < messagesInHistoryCount; ++i) {
        createEmptyMessage(buffer);
        for (size_t j = 0; j < 3; ++j) {
            char *info = getStringByIndex(history[i], j);
            size_t len = getLenOfStringByIndex(history[i], j);
            addStringToMessage(buffer, info, len);
        }
        size_t allLen = getLenMessage(buffer);
        ssize_t cntSending = send(sockId, buffer, allLen, 0);
        if (cntSending != allLen) {
            fprintf(stderr, "! Error can't send history to %s\n", clientsData[userId].login);
        }
        usleep(1);
    }
    pthread_mutex_unlock(&data_mutex);
}

void processKickMessage(char* mess, size_t userId) {
    int sockId = clientsData[userId].sockId;
    if (getCountStringsMessage(mess) != 2 ||
            getLenOfStringByIndex(mess, 0) != 4) {
        sendStatusMessage(sockId, 6);
        return;
    }
    size_t kickUserLoginId = readIntFromStr(getStringByIndex(mess, 0));
    char *reason = getStringByIndex(mess, 1);
    size_t lenReason = getLenOfStringByIndex(mess, 1);
    char buffer[SMALL_BUFF];
    setTypeMessage(buffer, 'k');
    createMessageFromString(buffer, reason, lenReason);
    pthread_mutex_lock(&data_mutex);
    int countTurnCliens = 0;
    char *login = NULL;
    size_t loginInd = 0;
    for (int i = 0; i < usersConnectCount; ++i) {
        if (clientsData[i].userInd == kickUserLoginId &&
                clientsData[i].isActive) {
            clientsData[i].isActive = 0;
            ++countTurnCliens;
            login = clientsData[i].login;
            loginInd = clientsData[i].userInd;
            ssize_t cntSend = send(clientsData[i].sockId, buffer, getLenMessage(buffer), 0);
            close(clientsData[i].sockId);
            pthread_cancel(threads[clientsData[i].threadInd]);
            if (cntSend != getLenMessage(buffer)) {
                fprintf(stderr, "Can't send kick message\n");
            }
        }
    }
    pthread_mutex_unlock(&data_mutex);
    if (!countTurnCliens) {
        sendStatusMessage(sockId, 2);
    } else {
        createNotificationKickMessage(buffer, loginInd, login);
        sendMessageToActiveUsers(buffer);
    }
}

void processMessage(char *mess, int *userId, int sockId, int threadInd) {
    switch (mess[0]) {
        case 'r':
            processRegularMessage(mess, (size_t)*userId, sockId);
            break;
        case 'o':
            if (*userId == USER_ID_UNDEFINED) {
                sendStatusMessage(sockId, 2);
            } else {
                processLogoutMessage((size_t) *userId);
                *userId = USER_ID_UNDEFINED;
            }
            break;
        case 'i':
            if (*userId != USER_ID_UNDEFINED) {
                sendStatusMessage(sockId, 3);
            } else {
                login(mess, userId, sockId, threadInd);
            }
            break;
        case 'l':
            if (*userId == USER_ID_UNDEFINED) {
                sendStatusMessage(sockId, 2);
            } else {
                processListMessage((size_t)*userId);
            }
            break;
        case 'h':
            if (*userId == USER_ID_UNDEFINED) {
                sendStatusMessage(sockId, 2);
            } else {
                processHistoryMessage(mess, (size_t)*userId);
            }
            break;
        case 'k':
            if (*userId == USER_ID_UNDEFINED) {
                sendStatusMessage(sockId, 2);
            } else if (strcmp(clientsData[*userId].login, "root")) {
                sendStatusMessage(sockId, 5);
            } else {
                processKickMessage(mess, (size_t)*userId);
            }
        default:
            break;
    }
}

void* processClient(void * data) {
    ThreasdInfo threasdInfo = *((ThreasdInfo*)data);
    int sockId = threasdInfo.sockId;
    int threadInd = threasdInfo.threadInd;
    char buffer[BUFF_SIZE];
    int userId = USER_ID_UNDEFINED;

    while (1) {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        ssize_t count = recv(sockId, buffer, BUFF_SIZE, 0);
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        printf("count: %ld\n", count);
        if (count <= 0) {
            if (userId != USER_ID_UNDEFINED) {
                if (getCountActiveUsersFromUserInd(clientsData[userId].userInd) == 1) {
                    char bufferSmall[SMALL_BUFF];
                    createNotificationDisconnectingMessage
                            (bufferSmall, clientsData[userId].userInd,
                             clientsData[userId].login);
                    sendMessageToActiveUsers(bufferSmall);
                }
                clientsData[userId].isActive = 0;
            }
            fprintf(stderr, "recv() failed for userId: %d\n", userId);
            close(sockId);
            return NULL;
        } else {
            printf("***\n");
            printf("c: %c\n", buffer[0]);
            processMessage(buffer, &userId, sockId, threadInd);
            printf("333\n");
        }
    }
    close(sockId);
    return NULL;
}

int main(int argc, char** argv) {
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        printf("%c\n", (char)opt);
        switch ((char)opt) {
            case 'p':
                PORT = atoi(optarg);
                break;
            default:
                break;
        }
    }
    int sockId = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockId == -1) {
        fprintf(stderr, "Socket opening failed\n");
        return 1;
    }

    struct sockaddr_in port;
    port.sin_family = AF_INET;
    port.sin_port = htons((uint16_t)PORT);
    port.sin_addr.s_addr = htonl(INADDR_ANY);
    int status = bind(sockId, (struct sockaddr *) &port, sizeof(port));
    if (status == -1) {
        fprintf(stderr, "Socket binding failed\n");
        return 2;
    }
    printf("Введите пароль для root\n");
    size_t len = 0;
    char* line = NULL;
    ssize_t read = getline(&line, &len, stdin);
    line[read - 1] = 0;
    int isRootNew = 0;
    clientsData[addUser(0, "root", line, 0, &isRootNew)].isActive = 0;

    status = listen(sockId, MAX_CONNECTIONS);
    if (status == -1) {
        fprintf(stderr, "Socket listening failed\n");
        return 3;
    }

    printf("Listening to port %d...\n", ntohs(port.sin_port));

    if (pthread_mutex_init(&data_mutex, NULL) != 0) {
        printf("mutex init failed\n");
        return 1;
    }

    int rc;
    for (;;) {
        struct sockaddr tmp;
        socklen_t tmpLen = sizeof(tmp);
        int clientSock = accept(sockId, &tmp, &tmpLen);
        if (clientSock == -1) {
            fprintf(stderr, "Socket accepting failed\n");
            return 4;
        }
        pthread_mutex_lock(&data_mutex);
        ThreasdInfo threasdInfo;
        threasdInfo.sockId = clientSock;
        threasdInfo.threadInd = threadsCount;
        rc = pthread_create(&threads[threadsCount++], NULL, processClient,
                            (void *)&threasdInfo);
        if (rc) {
            fprintf(stderr, "create thread error; return code from pthread_create() is %d\n", rc);
        }
        pthread_mutex_unlock(&data_mutex);
        printf("someone with sockId:%d and connected\n", status);

    }
    //close(sockId);
}