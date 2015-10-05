#ifndef __OS_CFG_H
#define __OS_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#define     OS_TICK_PERIOD      1000


#ifdef    GLOBAL_CFG
#define EXT    extern
#else
#define    EXT    
#endif

#define USE_ASSERT  0

#if USE_ASSERT    
#define assert_detect(EX)                                                         \
if(!(EX))                                                                    \
{                                                                             \
    debug_printf("\r\n>>BOX:\t(%s) assert failed at %s:%d \n", #EX, __FUNCTION__, __LINE__);\
    while (1)
    {;}                                                       \
}    
#else
#define assert_detect(EX)
#endif




#ifdef __cplusplus
}
#endif


#endif
