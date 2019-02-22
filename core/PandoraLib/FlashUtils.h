#ifndef __FLASH_UTILS_H__
#define __FLASH_UTILS_H__


void toFileName(char* buf, const char* pathAndName);
void toPath(char* buf, const char* pathAndName);

inline int	iabs(int i) { if (i < 0) return -i; else return i; }
inline int	imax(int a, int b) { if (a < b) return b; else return a; }
inline float	fmax(float a, float b) { if (a < b) return b; else return a; }
inline int	imin(int a, int b) { if (a < b) return a; else return b; }
inline float	fmin(float a, float b) { if (a < b) return a; else return b; }

#endif