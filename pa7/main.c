#include <string.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

#include "ipc.h"
#include "common.h"
#include "pa1.h"
#include "log.h"
#include "pa2345.h"
#include "banking.h"
#include "priorityQueue.h"

size_t numberOfChildProcesses = 0;
local_id currentId = 0;
process_t processes[16];
bool mutexl = false;

int8_t messagesStartLeft, messagesDoneLeft, csRepliesLeft, csReplies;
int8_t currentPhase = FISRT_PHASE;
Node *requestQueue;
int *forks;
int *poorChilds;

int main(int argc, char *argv[]) {

    if (readParams(argc, argv)) return -1;
    FILE *pipesLog = fopen(pipes_log, "w");
    FILE *eventsLog = fopen(events_log, "w");

    openPipes(pipesLog);
    processes[0].id = PARENT_ID;
    processes[0].pid = getpid();
    processes[0].lamport_time = 0;

    if (forkProcesses()) return 1;
    closeExtraPipes();

    messagesStartLeft = messagesDoneLeft = (currentId == PARENT_ID)
                                           ? numberOfChildProcesses
                                           : numberOfChildProcesses - 1;
    csReplies = numberOfChildProcesses - 1;

    sendStart(eventsLog);
    Message msg;
    receive_any(&processes[currentId], &msg);
    printReceiveAllStarted(eventsLog, get_lamport_time(), currentId);

    currentPhase = SECOND_PHASE;
    if (currentId != PARENT_ID) {
        loopProcessing();
    }

    currentPhase = THIRD_PHASE;
    sendDone(eventsLog);
    receive_any(&processes[currentId], &msg);
    printReceiveAllDone(eventsLog, get_lamport_time(), currentId);


    if (currentId == PARENT_ID) {
        for (size_t i = 1; i <= numberOfChildProcesses; i++) {
            waitpid(processes[i].pid, NULL, 0);
        }
    }

    fclose(eventsLog);
    fclose(pipesLog);
}

int get_forks_number() {
    int sum = 0;
    for (int i = 0; i < numberOfChildProcesses; ++i) {
        sum += forks[i];
    }
    return sum;
}

/**
 * Вход в критическую область
 * 1. понят кто хочет
 * 2. сравнить приоритеты
 * 3. отправить вилку тем, у кого приоритет выше
 * 4. ждать вилку обратно
 * 5. при получении кушать
 */
int request_cs(const void *self) {
    Message msg;
    processes[currentId].lamport_time++;

    sendRequestAll();

    while (get_forks_number() < numberOfChildProcesses - 1)
        for (int i = 1; i <= numberOfChildProcesses; i++) {
            if (i == currentId) continue;
            if (forks[i - 1] == 0) {
                receive(&processes[currentId], (local_id) i, &msg);
            }
        }

    return 0;
}

/*
 * Выход из критической области
 */
int release_cs(const void *self) {
    for (int i = 1; i <= numberOfChildProcesses; i++) {
        if (poorChilds[i - 1] == 1) {
            forks[i - 1] = 0;
            poorChilds[i - 1] = 0;
            sendReply((local_id) i);
        }
    }
    return 0;
}

void fillForksArr(local_id id) {
    int cur_i = id - 1;
    for (int i = 0; i < numberOfChildProcesses; i++) {
        if (i <= cur_i) {
            // i-process has priority
            // cur_i don't have anything (fork)
            forks[i] = 0;
        } else if (i > cur_i) {
            // cur-i has a priority ( clean fork )
            forks[i] = 1;
        }
        poorChilds[i] = 0;
    }
}

void loopProcessing() {
    char log_str[64];

    // fill forks
    forks = malloc(numberOfChildProcesses * sizeof(int));
    poorChilds = malloc(numberOfChildProcesses * sizeof(int));
    fillForksArr(processes[currentId].id);

    int iterationsAmount = 5 * currentId;
    for (int i = 1; i <= iterationsAmount; i++) {
        memset(log_str, 0, sizeof(log_str));
        if (mutexl) {
            request_cs(&processes[currentId]);
//            printf("---- %d in CS\n", currentId);
            sprintf(log_str, log_loop_operation_fmt, currentId, i, iterationsAmount);
            print(log_str);
            release_cs(&processes[currentId]);
        } else {
            sprintf(log_str, log_loop_operation_fmt, currentId, i, iterationsAmount);
            print(log_str);
        }
    }

    sendReleaseAll();
    for (int i = 0; i < numberOfChildProcesses; ++i) {
        forks[i] = 0;
    }
    free(forks);
}

/*
 * Send CS_REQUEST
 */
void sendRequestAll() {
    MessageHeader messageHeader;
    messageHeader.s_magic = MESSAGE_MAGIC;
    messageHeader.s_type = CS_REQUEST;
    messageHeader.s_local_time = get_lamport_time();
    messageHeader.s_payload_len = 0;
    Message message;
    message.s_header = messageHeader;

    send_multicast(&processes[currentId], &message);
}

/*
 * Send CS_RELEASE
 */
void sendReleaseAll() {
    MessageHeader messageHeader;
    messageHeader.s_magic = MESSAGE_MAGIC;
    messageHeader.s_type = CS_RELEASE;
    processes[currentId].lamport_time++;
    messageHeader.s_local_time = get_lamport_time();
    messageHeader.s_payload_len = 0;
    Message message;
    message.s_header = messageHeader;

    send_multicast(&processes[currentId], &message);

}

/*
 * Send CS_REPLY
 */
void sendReply(local_id dst) {
    MessageHeader messageHeader;
    messageHeader.s_magic = MESSAGE_MAGIC;
    messageHeader.s_type = CS_REPLY;
    processes[currentId].lamport_time++;
    messageHeader.s_local_time = get_lamport_time();
    messageHeader.s_payload_len = 0;
    Message message;
    message.s_header = messageHeader;

    send(&processes[currentId], dst, &message);

}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

timestamp_t get_lamport_time() {
    return processes[currentId].lamport_time;
}

void chooseLamportTime(process_t *process, timestamp_t msg_time) {
    if (msg_time > process->lamport_time) {
        process->lamport_time = msg_time;
    }
    process->lamport_time++;
}


//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

int send(void *self, local_id dst, const Message *msg) {
    int flags = fcntl(((process_t *) self)->writePipes[dst], F_GETFL, 0);
    fcntl(((process_t *) self)->writePipes[dst], F_SETFL, flags | O_NONBLOCK);

    if (msg->s_header.s_magic != MESSAGE_MAGIC) {
        return -2;
    }

    write(((process_t *) self)->writePipes[dst], msg, sizeof(MessageHeader) + msg->s_header.s_payload_len);

//    printf("--- send msg from %d to %d\n", currentId, dst);
    return 0;
}


int send_multicast(void *self, const Message *msg) {
    for (local_id i = 0; i <= numberOfChildProcesses; i++) {
        if (((process_t *) self)->id != i) {
            send(self, i, msg);
        }
    }
    return 0;
}


int receive(void *self, local_id from, Message *msg) {
    void *buffer = &msg->s_header;
    int readBytes = (int) read(processes[from].readPipes[((process_t *) self)->id], buffer, sizeof(MessageHeader));
    if (readBytes <= 0) {
        return -1;
    }

    if (msg->s_header.s_magic != MESSAGE_MAGIC) {
        return -2;
    }

    buffer = &msg->s_payload;

    readBytes = (int) read(processes[from].readPipes[((process_t *) self)->id], buffer, msg->s_header.s_payload_len);
    if (readBytes <= 0 && msg->s_header.s_payload_len != 0) {
        return -1;
    }

    chooseLamportTime((process_t *) self, msg->s_header.s_local_time);

    switch (msg->s_header.s_type) {
        case STARTED:
            messagesStartLeft--;
            break;
        case DONE:
            messagesDoneLeft--;
            break;
        case CS_REQUEST:
            if (((process_t *) self)->id != PARENT_ID) {
//                printf("--- process %d receive REQUEST msg from %d\n", currentId, from);
                int id = from - 1;
                if (forks[id] == 0) sendReply(from);
                else poorChilds[id] = 1;
            }
            break;
        case CS_REPLY: // process allow to use fork
            if (((process_t *) self)->id != PARENT_ID) {
//                printf("--- process %d receive REPLY msg from %d\n", currentId, from);
                int id = from - 1;
                forks[id] = 1;
            }
            break;
        case CS_RELEASE: // process give a fork after eat
            if (((process_t *) self)->id != PARENT_ID) {
//                printf("--- process %d receive RELEASE msg from %d\n", currentId, from);
                int id = from - 1;
                forks[id] = 1;
            }
            break;
    }

    return 0;
}


int receive_any(void *self, Message *msg) {
    while (currentPhase == SECOND_PHASE ||
           (currentPhase == FISRT_PHASE && messagesStartLeft) ||
           (currentPhase == THIRD_PHASE && messagesDoneLeft)) {
        for (size_t i = 0; i <= numberOfChildProcesses; i++) {
            if (i != ((process_t *) self)->id) {
                receive(self, (local_id) i, msg);
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------


/*
 * При запуске программы с параметром командой строки «--mutexl» перед каждым
 * вызовом print() процесс должен входить в критическую область, а после вызова выходит
 * из нее, т.е. запрещается выполнять несколько вызовов print() в пределах одной
 * критической области. При отсутствии параметра «--mutexl» программа должна
 * выполняться без использования критической области,
 */
int readParams(int argc, char *argv[]) {
    if (argc == 3 && !strncmp(argv[1], "-p", 2)) {
        numberOfChildProcesses = (size_t) strtol(argv[2], NULL, 10);
    } else {
        if (argc == 4) {
            if (!strncmp(argv[1], "-p", 2) && !strncmp(argv[3], "--mutexl", 8)) {
                numberOfChildProcesses = (size_t) strtol(argv[2], NULL, 10);
                mutexl = true;
            } else {
                if (!strncmp(argv[1], "--mutexl", 8) && !strncmp(argv[2], "-p", 2)) {
                    numberOfChildProcesses = (size_t) strtol(argv[3], NULL, 10);
                    mutexl = true;
                } else {
                    printf("Incorrect input format!");
                    return -1;
                }
            }
        }
    }

    if (numberOfChildProcesses > MAX_PROCESS_ID) {
        printf("Too many processes requested!");
        return -1;
    }

    return 0;
}

void openPipes(FILE *pipesLog) {
    int flags;
    for (size_t src = 0; src <= numberOfChildProcesses; src++) {
        for (size_t dst = 0; dst <= numberOfChildProcesses; dst++) {
            if (src != dst) {
                int p[2];
                pipe(p);
                processes[src].readPipes[dst] = p[0];
                processes[src].writePipes[dst] = p[1];
                flags = fcntl(processes[src].readPipes[dst], F_GETFL, 0);
                fcntl(processes[src].readPipes[dst], F_SETFL, flags | O_NONBLOCK);
                flags = fcntl(processes[src].writePipes[dst], F_GETFL, 0);
                fcntl(processes[src].writePipes[dst], F_SETFL, flags | O_NONBLOCK);

                logPipeReadStarted(pipesLog, (int) src, (int) dst, p[0]);
                logPipeWriteStarted(pipesLog, (int) src, (int) dst, p[1]);
                //log pipe opened
            }
        }
    }
}

int forkProcesses() {
    for (local_id i = 1; i <= numberOfChildProcesses; i++) {
        int child = fork();
        if (child > 0) {
            //parent
            currentId = PARENT_ID;
            processes[i].id = i;
            processes[i].pid = child;
            processes[i].parentPid = processes[0].pid;
            processes[i].lamport_time = 0;
        } else {
            if (child == 0) {
                //in child
                currentId = (local_id) i;
                processes[i].id = i;
                processes[i].pid = getpid();
                processes[i].parentPid = processes[0].pid;
                processes[i].lamport_time = 0;
                break;
            } else {
                perror("Error during fork processes!");
                return 1;
            }
        }
    }
    return 0;
}

void closeExtraPipes() {
    for (size_t src = 0; src <= numberOfChildProcesses; src++) {
        for (size_t dst = 0; dst <= numberOfChildProcesses; dst++) {
            if (src != currentId && dst != currentId && src != dst) {
                close(processes[src].readPipes[dst]);
                close(processes[src].writePipes[dst]);
            }
            if (src == currentId && dst != currentId) {
                close(processes[src].readPipes[dst]);
            }
            if (dst == currentId && src != currentId) {
                close(processes[src].writePipes[dst]);
            }

        }
    }
}

void sendStart(FILE *eventsLog) {
    if (currentId != PARENT_ID) {
        MessageHeader messageHeader;
        messageHeader.s_magic = MESSAGE_MAGIC;
        messageHeader.s_type = STARTED;
        processes[currentId].lamport_time++;
        messageHeader.s_local_time = get_lamport_time();
        Message message;
        message.s_header = messageHeader;

        sprintf(message.s_payload, log_started_fmt, get_lamport_time(), currentId, getpid(), getppid(), 0);
        message.s_header.s_payload_len = (uint16_t) strlen(message.s_payload);
        send_multicast(&processes[currentId], &message);

        printProcessStarted(eventsLog, get_lamport_time(), processes[currentId], 0);
        //log started
    }
}

void sendDone(FILE *eventsLog) {
    if (currentId != PARENT_ID) {
        MessageHeader messageHeader;
        messageHeader.s_magic = MESSAGE_MAGIC;
        messageHeader.s_type = DONE;
        processes[currentId].lamport_time++;
        messageHeader.s_local_time = get_lamport_time();
        Message message;
        message.s_header = messageHeader;

        sprintf(message.s_payload, log_done_fmt, get_lamport_time(), currentId, 0);
        message.s_header.s_payload_len = (uint16_t) strlen(message.s_payload);
        send_multicast(&processes[currentId], &message);

        printProcessDone(eventsLog, get_lamport_time(), currentId, 0);
        //log done
    }
}
