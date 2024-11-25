#pragma once

void* SNDAPP_StreamOpenFile(const char* fileName);
void SNDAPP_StreamCloseFile(void* file);
int SNDAPP_StreamGetSize(void* file);
void SNDAPP_StreamRequestData(void* file, int offset, int size, void (*callback)(void*, int, void*), void* context);
void SNDAPP_StreamCancel(void* file);