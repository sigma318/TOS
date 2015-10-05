#ifndef __OS_CPU_H
#define _OS_CPU_H




#ifdef __cplusplus
extern "C" {
#endif





void os_goto_sleep(void);
void os_sch_start(void);

void os_tick_init(void);

void os_tick_interrupt(void);/*定时器中断函数*/








#ifdef __cplusplus
}
#endif

#endif