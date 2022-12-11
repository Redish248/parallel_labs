#include "ipc.h"
#include "pa1.h"

static const char * const log_pipe_read_opened_fmt =
        "Pipe from process %1d to process %1d opened for read with id %1d.\n";

static const char * const log_pipe_write_opened_fmt =
        "Pipe from process %1d to process %1d opened for write with id %1d.\n";

static const char * const log_pipe_read_closed_fmt =
        "Pipe from process %1d to process %1d closed for read.\n";

static const char * const log_pipe_write_closed_fmt =
        "Pipe from process %1d to process %1d closed for write.\n";

/**
 * Print log about receiving all started messages
 * @param id process id
 */
void printReceiveAllStarted(FILE* eventsLog, timestamp_t timestamp, local_id id);

/**
 * Print log about receiving all done messages
 * @param id process id
 */
void printReceiveAllDone(FILE* eventsLog, timestamp_t timestamp, local_id id);

/**
 * Pring log about starting process
 * @param process all process
 */
void printProcessStarted(FILE* eventsLog, timestamp_t timestamp, process_t process, balance_t balance);

/**
 * Print log when process finish it's work
 * @param id process id
 */
void printProcessDone(FILE* eventsLog, timestamp_t timestamp, local_id id, int balance);

/**
 * Print log when pipe for read is opened
 * @param source source
 * @param dest destination
 * @param id pipe's id
 */
void logPipeReadStarted(FILE* pipesLog, int source, int dest, int id);


/**
 * Print log when pipe for write is opened
 * @param source source
 * @param dest destination
 * @param id pipe's id
 */
void logPipeWriteStarted(FILE* pipesLog, int source, int dest, int id);
