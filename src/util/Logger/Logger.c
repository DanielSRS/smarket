#include "Logger.h"
#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include "../Cstrings/Cstrings.h" // duplicateString
#include <time.h> // time_t, tm

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct _loggerOpt {
  alocatedCString namespace;
  void (*destroy)(struct _loggerOpt** self);
} loggerOptions;

void destroyLoggerOptions(loggerOptions** self) {
  if(!*self) return;
  alocatedCString namespace = (*self)->namespace;

  if (namespace) free(namespace);                 // libera os membros da estrutura
  free(*self);                                    // libera a própria estrutura
  *self = NULL;
}

loggerOptions* newLoggerOptions() {
  loggerOptions* _newLoggerOptions = calloc(1, sizeof(loggerOptions));
  _newLoggerOptions->destroy = destroyLoggerOptions;
  return _newLoggerOptions;
}

alocatedCString formatTime(){
  time_t rawtime;
  struct tm * timeinfo;
  
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  
  return formatedCString("%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

/** Log log */
void logIndicator() {
  // printf("\x1b[1m");            // set text bold
  printf("\x1b[30m");           // set backgroud color black
  printf("\x1b[47m");           // set foreground color white
  printf(" LOG ");
  printf("\x1b[0m");            // Reset colors
  printf(" ");                  // Espaço em branco to conteúdo
}

/** Envia para a saída padrão a mensagem de log */
void debug(Logger* self, const char* logMessage) {
  logIndicator();
  alocatedCString currentTime = formatTime();
  printf("%s | %s | DEBUG : %s\n", currentTime, self->_loggerOptions->namespace, logMessage);
  freeAlocatedCString(currentTime);
}

/** Envia para a saída padrão a mensagem de log */
void info(Logger* self, const char* logMessage) {
  logIndicator();
  alocatedCString currentTime = formatTime();
  printf(ANSI_COLOR_BLUE "%s | %s | INFO : %s\n" ANSI_COLOR_RESET, currentTime, self->_loggerOptions->namespace, logMessage);
  freeAlocatedCString(currentTime);
}

/** Envia para a saída padrão a mensagem de log */
void warn(Logger* self, const char* logMessage) {
  logIndicator();
  alocatedCString currentTime = formatTime();
  printf(ANSI_COLOR_YELLOW "%s | %s | WARN : %s\n" ANSI_COLOR_RESET, currentTime, self->_loggerOptions->namespace, logMessage);
  freeAlocatedCString(currentTime);
}

/** Envia para a saída de erro a mensagem de log */
void error(Logger* self, const char* logMessage) {
  logIndicator();
  alocatedCString currentTime = formatTime();
  printf(ANSI_COLOR_RED "%s | %s | ERROR : %s\n" ANSI_COLOR_RESET, currentTime, self->_loggerOptions->namespace, logMessage);
  freeAlocatedCString(currentTime);
}

/** Extende o logger para o namespace informado */
Logger* extend(Logger* self, const char* namespace) {
  self->_loggerOptions->namespace = duplicateString(namespace);
  return self;
}

/** Destroi o logger */
void destroy(Logger** self) {
  if (!*self) return;
  if (!(*self)->_loggerOptions) return;

  (*self)->_loggerOptions->destroy(&((*self)->_loggerOptions));

  free(*self);
  *self = NULL;
}

Logger* createLogger() {
  Logger *newLogger = malloc(sizeof(Logger));
  newLogger->debug = debug;
  newLogger->destroy = destroy;
  newLogger->error = error;
  newLogger->extend = extend;
  newLogger->info = info;
  newLogger->warn = warn;
  newLogger->_loggerOptions = newLoggerOptions();

  return newLogger;
}