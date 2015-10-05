#include "os.h"

static list_t task_head;
static list_t timer_head;
static list_t event_head;
static list_t hook_head;
static boolean os_start = false;

void list_init(list_t *node)
{
    node->prev = node->next = node;
}
void list_insert(list_t *head, list_t *node)
{    
    head->next->prev = node;
    node->next = head->next;
    head->next = node;
    node->prev = head;
}
void list_delete(list_t *list)
{
    list->next->prev = list->prev;
    list->prev->next = list->next;
    list->next = list->prev = list;
}

void task_create(task_t *ptask) 
{
    assert_detect(null != ptask->task.pfunc);
    assert_detect(null != ptask);
    list_insert(&task_head, &ptask->list);
    if(null != ptask->pfunc_init)
    {
        ptask->pfunc_init();
    }
}

void task_delete(task_t *ptask)
{
    assert_detect(null != ptask);
    list_delete(&ptask->list);
}


void timer_create(timer_t *ptimer) 
{
    assert_detect(null != ptimer->timer.pfunc);
    assert_detect(null != ptimer);
    list_insert(&timer_head, &ptimer->list);
}

void timer_delete(timer_t *ptimer)
{
    assert_detect(null != ptimer);
    assert_detect(ptimer->list.next != ptimer->list.prev);
    list_delete(&ptimer->list);
}

void timer_start(timer_t *ptimer)
{
    assert_detect(null != ptimer);
    ptimer->on = true;
}
void timer_stop(timer_t *ptimer)
{
    assert_detect(null != ptimer);
    ptimer->on = false;
}

void task_schedule(void)
{
    list_t *node = &task_head;
    task_t *ptask = null;
    
    for(; node->next != &task_head; node = node->next)
    {
        ptask = get_entry(node->next, task_t, list);
        assert_detect(ptask != null);
        if(ptask->task.hit)
        {
            ptask->task.pfunc();
            ptask->task.hit = false;
        }
    }
}

void timer_schedule(void)
{
    list_t *node = &timer_head;
    timer_t *ptimer = null;
    
    for(; node->next != &timer_head; node = node->next)
    {
        ptimer = get_entry(node->next, timer_t, list);
        assert_detect(ptimer != null);
        if(ptimer->timer.hit)
        {
            ptimer->timer.pfunc();
            ptimer->timer.hit = false;
        }
    }
}

void timer_tick_handle(void)
{
    list_t *node = &timer_head;
    timer_t *ptimer = null;

    if(!os_start)
    {
        return;
    }
    for(; node->next != &timer_head; node = node->next)
    {
        ptimer = get_entry(node->next, timer_t, list);
        assert_detect(ptimer != null);
        if(false == ptimer->on)
        {
            continue;
        }
        if(ptimer->timer.delay > 0)
        {
            ptimer->timer.delay--;
            ptimer->timer.hit = true;
        }
        else
        {
            if(ptimer->timer.period > 0)
            {
                ptimer->timer.delay = ptimer->timer.period;
            }
            ptimer->timer.hit = false;
        }
    }
}

void task_tick_handle(void)
{
    list_t *node = &task_head;
    task_t *ptask = null;

    if(!os_start)
    {
        return;
    }
    
    for(; node->next != &task_head; node = node->next)
    {
        ptask = get_entry(node->next, task_t, list);
        assert_detect(ptask != null);
        if(ptask->task.delay > 0)
        {
            ptask->task.delay--; 
        }
        else
        {
            ptask->task.hit = true;
            if(ptask->task.period > 0)
            {
                ptask->task.delay = ptask->task.period;
            }           
        }
    }
}

void event_post(event_t *pevent, uint32 event)
{
    assert_detect(null != pevent);
    assert_detect(null != pevent->event.pfunc);
    pevent->event.event = event;
    pevent->event.hit = true;
    list_insert(&event_head, &pevent->list);    
}

void event_create(event_t *pevent)
{
    assert_detect(null != pevent);
    list_init(&pevent->list);
}

void event_delete(event_t *pevent)
{
    assert_detect(null != pevent);
    assert_detect(pevent->list.next != pevent->list.prev);
    list_delete(&pevent->list);
}

void event_schedule(void)
{
    list_t *node = &event_head;
    event_t *pevent = null;
    
    for(; node->next != &event_head; node = node->next)
    {
        pevent = get_entry(node->next, event_t, list);
        assert_detect(pevent != null);
        if(pevent->event.hit)
        {
            pevent->event.pfunc(pevent->event.event);
            pevent->event.hit = false;
            event_delete(pevent);
        }
    }
}

void hook_create(hook_t *phook)
{
    assert_detect(null != phook);
    list_insert(&hook_head, &phook->list);
}
void hook_delete(hook_t *phook)
{
    assert_detect(null != phook);
    assert_detect(phook->list.next != phook->list.prev);
    list_delete(&phook->list);
}

void hook_schedule(void)
{
    list_t *node = &hook_head;
    hook_t *phook = null;
    
    for(; node->next != &hook_head; node = node->next)
    {
        phook = get_entry(node->next, hook_t, list);
        assert_detect(phook != null);
        phook->pfunc();
    }
}

void os_init(void)
{
    list_init(&task_head);
    list_init(&timer_head);
    list_init(&event_head);
    list_init(&hook_head);
    os_start = true;
}

void system_running(void)
{
    task_schedule();
    timer_schedule();
    event_schedule();
    hook_schedule();
}

