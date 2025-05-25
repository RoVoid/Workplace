#include <stdio.h>
#include <stdarg.h>

#pragma once

#define LOG_FOLDER "./logs/"

#define lclose(log) { _lclose(log); log = NULL; }

typedef FILE* Log;

Log lastLog;

/**
 * Открывает лог-файл для записи.
 *
 * Пример использования:
 *
 * ```c
 * Log log = lopen("test");
 * if (!log) return 1; // Ошибка при открытии
 * ```
 *
 * @param name Имя лог-файла (без расширения).
 * @return Указатель на файл (Log), или NULL в случае ошибки.
 */
inline Log lopen(const char* name) {
    char path[512];
    snprintf(path, sizeof(path), "%s%s%s", LOG_FOLDER, name, ".log");

    Log log = fopen(path, "w");
    if (log) lastLog = log;

    return log;
}

/**
 * Записывает форматированное сообщение в лог-файл.
 *
 * @param log Указатель на лог-файл.
 * @param format Форматная строка (как в printf).
 */
inline void lprint(Log log, const char* format, ...) {
    if (!log) return;

    va_list args;
    va_start(args, format);
    vfprintf(log, format, args);
    va_end(args);
}

/**
 * Записывает форматированное сообщение в последний открытый лог-файл.
 *
 * ```c
 * if (!lopen("test")) return 1;
 * lprintl("%s", "Hello World");
 * ```
 *
 * @param format Форматная строка (как в printf).
 */
inline void lprintl(const char* format, ...) {
    if (!lastLog) return;

    va_list args;
    va_start(args, format);
    vfprintf(lastLog, format, args);
    va_end(args);
}

/**
 * Закрывает указанный лог-файл.
 *
 * @param log Указатель на лог-файл.
 */
inline void _lclose(Log log) {
    if (log) {
        fclose(log);
        log = NULL;
    }
}

/**
 * Закрывает последний открытый лог-файл.
 */
inline void lclosel() {
    if (lastLog) {
        fclose(lastLog);
        lastLog = NULL;
    }
}
