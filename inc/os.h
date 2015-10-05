#ifndef __OS_H
#define __OS_H

#include "data_type.h"
#include "os_cpu.h"
#include "os_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_node
{
    struct list_node *next;
    struct list_node *prev;
}list_t;    

typedef void (*pfunc_t)(void);
typedef struct
{
    uint32 delay;
    uint32 period;
    pfunc_t pfunc;
    boolean hit;
}callback_t;

typedef struct
{
    const int8 *name;
    callback_t task;
    pfunc_t pfunc_init;
    list_t list;
}task_t;


typedef struct
{
    const int8 *name;
    callback_t timer;
    boolean    on;
    list_t list;
}timer_t;


#define get_entry(node, type, member) ((type *)((int8 *)(node) - (uint32)(&((type *)0)->member)))

#define DEFINE_TASK(NAME, DELAY, PERIOD, TASK_FUCN, TASK_INIT_FUNC)  \
    task_t task_##NAME = \
        {#NAME, {DELAY, PERIOD, TASK_FUCN, false}, TASK_INIT_FUNC}
#define REF_TASK(NAME)     (task_t *)&task_##NAME
#define EXT_TASK(NAME)      extern task_t task_##NAME


#define DEFINE_TIMER(NAME, DELAY, PERIOD, TIMER_FUNC)  \
    timer_t timer_##NAME = \
        {#NAME, {DELAY, PERIOD, TIMER_FUNC, false}, false}
#define REF_TIMER(NAME)     (timer_t *)&timer_##NAME
#define EXT_TIMER(NAME)     extern timer_t timer_##NAME

typedef void (*pevent_t)(uint32 event);
typedef struct
{
    uint32 event;
    pevent_t pfunc;
    boolean hit;
}event_hit_t;

typedef struct
{
    const int8 *name;
    event_hit_t event;
    list_t list;
}event_t;

#define DEFINE_EVENT(NAME, EVENT_INIT, EVENT_FUNC) \
    event_t event_##NAME = \
    {#NAME, {EVENT_INIT, EVENT_FUNC, false}, false}
#define REF_EVENT(NAME) (event_t *)&event_##NAME
#define EXT_EVENT(NAME) extern event_t event_##NAME


typedef void (*phook_t)(void);
typedef struct
{
    const int8 *name;
    phook_t pfunc;
    list_t  list;
}hook_t;

#define DEFINE_HOOK(NAME, HOOK) \
    hook_t hook_##NAME = \
    {#NAME, HOOK};
#define REF_HOOK(NAME) (hook_t *)&hook_##NAME
#define EXT_HOOK(NAME) extern hook_t hook_##NAME

void task_create(task_t * ptask);
void task_delete(task_t * ptask);
void task_schedule(void);
void task_tick_handle(void);

void timer_create(timer_t * ptimer);
void timer_delete(timer_t * ptimer);
void timer_start(timer_t * ptimer);
void timer_stop(timer_t * ptimer);
void timer_schedule(void);
void timer_tick_handle(void);

void event_create(event_t * pevent);
void event_delete(event_t * pevent);
void event_post(event_t * pevent, uint32 event);
void event_schedule(void);

void hook_create(hook_t *phook);
void hook_delete(hook_t *phook);
void hook_schedule(void);

void os_init(void);
void system_running(void);


#ifdef __cplusplus
}
#endif

#endif
