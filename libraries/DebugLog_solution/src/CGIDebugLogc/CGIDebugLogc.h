/* CGIDebugLogc.h
 * Author : Joseph Lin
 * E-mail : joseph.lin@aliyun.com
 * 
 * 
 */

#ifndef __CGIDEBUGLOGC_H__
#define __CGIDEBUGLOGC_H__



int tcdbg_printf(char *message, int msg_len);
int CGIdbg_printf(char *message, int msg_len);

int CGIPrint(const char *fmt, ...);

#endif /* no def __CGIDEBUGLOGC_H__ */
