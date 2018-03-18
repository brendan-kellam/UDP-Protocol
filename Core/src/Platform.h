#pragma once

#define UDP_TRAP(expr) \
if (expr) { } \
else \
{ \
do { \
_CrtDbgReport( \
_CRT_ASSERT, __FILE__, __LINE__, #expr, NULL); \
_CrtDbgBreak(); \
} while (1); \
}