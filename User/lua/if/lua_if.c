#include "lua_if.h"
#include "bsp.h"
#include "param.h"
#include "file_lib.h"
#include "main.h"
#include "prog_if.h"

/* 

lua.h 定义错误代码
#define LUA_ERRRUN    2
#define LUA_ERRSYNTAX    3
#define LUA_ERRMEM    4
#define LUA_ERRGCMM    5
#define LUA_ERRERR    6
*/

/*
    luaconf.h 文件对浮点和整数的处理。 缺省64位整数，双精度浮点
        default configuration for 64-bit Lua ('long long' and 'double')
*/

lua_State *g_Lua = 0;

char s_lua_prog_buf[LUA_PROG_LEN_MAX + 1];
uint32_t s_lua_prog_len;
uint32_t s_lua_func_init_idx;
uint32_t s_lua_func_write_idx;
uint32_t s_lua_func_read_idx;

uint8_t s_lua_read_buf[LUA_READ_LEN_MAX];
uint32_t s_lua_read_len;

static uint8_t s_lua_quit = 0;


static int get_runtime(lua_State* L);
static int check_runtime(lua_State* L);

static void lua_RegisterFunc(void);

static void LuaYeildHook(lua_State *_L, lua_Debug *ar);

///* lua源码调用该函数，会告警 */
//void exit(int status)
//{
//    return;
//}

int system(const char *cmd)
{
    return 0;
}

/* time_t : date/time in unix secs past 1-Jan-70 */
time_t time(time_t *_t)
{
    /* 以下代码来自于： https://blog.csdn.net/qq_29350001/article/details/87637350 */
    #define xMINUTE (60)          /* 1分的秒数 */
    #define xHOUR   (60*xMINUTE)  /* 1小时的秒数 */
    #define xDAY    (24*xHOUR   ) /* 1天的秒数 */
    #define xYEAR   (365*xDAY   ) /* 1年的秒数 */

    /* 将localtime（UTC+8北京时间）转为UNIX TIME，以1970年1月1日为起点 */
    static unsigned int  month[12] =
    {
        /*01月*/xDAY*(0),
        /*02月*/xDAY*(31),
        /*03月*/xDAY*(31+28),
        /*04月*/xDAY*(31+28+31),
        /*05月*/xDAY*(31+28+31+30),
        /*06月*/xDAY*(31+28+31+30+31),
        /*07月*/xDAY*(31+28+31+30+31+30),
        /*08月*/xDAY*(31+28+31+30+31+30+31),
        /*09月*/xDAY*(31+28+31+30+31+30+31+31),
        /*10月*/xDAY*(31+28+31+30+31+30+31+31+30),
        /*11月*/xDAY*(31+28+31+30+31+30+31+31+30+31),
        /*12月*/xDAY*(31+28+31+30+31+30+31+31+30+31+30)
    };
    unsigned int  seconds = 0;
    unsigned int  year = 0;
  
    RTC_ReadClock();
    year = g_tRTC.Year - 1970;                        /* 不考虑2100年千年虫问题 */
    seconds = xYEAR * year + xDAY * ((year + 1) /4);  /* 前几年过去的秒数 */
    seconds += month[g_tRTC.Mon - 1];                  /* 加上今年本月过去的秒数 */
    if ((g_tRTC.Mon > 2) && (((year + 2) % 4) == 0))        /* 2008年为闰年 */
    seconds += xDAY;                                /* 闰年加1天秒数 */
    seconds += xDAY * (g_tRTC.Day-1);     /* 加上本天过去的秒数 */
    seconds += xHOUR * g_tRTC.Hour;        /* 加上本小时过去的秒数 */
    seconds += xMINUTE * g_tRTC.Min;        /* 加上本分钟过去的秒数 */
    seconds += g_tRTC.Sec;                /* 加上当前秒数<br>　seconds -= 8 * xHOUR; */
    *_t = seconds;
    return *_t;
}

/*
*********************************************************************************************************
*    函 数 名: lua_Init
*    功能说明: 初始化lua虚拟机
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void lua_Init(void)
{    
    g_Lua = luaL_newstate();         /* 建立Lua运行环境 */
    luaL_openlibs(g_Lua);
    luaopen_base(g_Lua);
    
    lua_RegisterFunc();        /* 注册c函数，供lua调用 */

	//错误处理函数
	lua_sethook(g_Lua, LuaYeildHook, LUA_MASKLINE, 0);    
}

/* 关闭释放Lua */
void lua_DeInit(void)
{    
    if (g_Lua > 0)
    {
        lua_close(g_Lua);                /* 释放内存 */
        g_Lua = 0;
    }
}

// 每行执行的钩子函数，用于终止lua程序
extern MEMO_T g_LuaMemo;
extern void PG_PrintText(char *_str);
void LuaYeildHook(lua_State *_L, lua_Debug *ar)
{
	if (s_lua_quit == 1)
	{
		s_lua_quit = 0;

		lua_yield(_L, 0);
	}
    
    if (g_MainStatus == MS_LUA_EXEC_FILE)
    {
        uint8_t ucKeyCode;
        
        /* 显示Lua程序print的字符串. 内容在bsp_uart_fif文件 fputc 函数填充的 */
        if (g_LuaMemo.Refresh == 1)     
        {
            LCD_SetEncode(ENCODE_GBK);
            LCD_DrawMemo(&g_LuaMemo);            
            LCD_SetEncode(ENCODE_UTF8);
        }

        ucKeyCode = bsp_GetKey2(); /* 读取键值, 无键按下时返回 KEY_NONE = 0 */
        if (ucKeyCode != KEY_NONE)
        {
            /* 有键按下 */
            switch (ucKeyCode)
            {            
                case KEY_LONG_DOWN_C:   /* C键长按 - 终止Lua */
                    lua_yield(_L, 0);   
                    break;

                default:
                    break;
            }
        }        
    }
        
    if (g_MainStatus == MS_PROG_WORK)
    {
        uint8_t ucKeyCode;

        ucKeyCode = bsp_GetKey2(); /* 读取键值, 无键按下时返回 KEY_NONE = 0 */
        if (ucKeyCode != KEY_NONE)
        {
            /* 有键按下 */
            switch (ucKeyCode)
            {            
                case KEY_LONG_DOWN_C:   /* C键长按 - 终止Lua */
                    PG_PrintText("用户终止运行");
                    g_tProg.AutoStart = 0;
                    lua_yield(_L, 0); 
                    break;

                case KEY_UP_S:
                case KEY_UP_C:
                    if (g_tProg.AutoStart == 1)
                    {
                        ;
                    }
                    break;
                
                default:
                    break;
            }
        }        
    }
}

// 终止lua
void lua_abort(void)
{
	s_lua_quit = 1;
}

// 装载文件并初始化lua全局对象
void lua_DownLoadFile(char *_path)
{
    lua_DeInit();   // 先释放
    
    lua_Init();
    
    // 读文件到内存
    s_lua_prog_len = ReadFileToMem(_path, 0, s_lua_prog_buf, LUA_PROG_LEN_MAX);
    
    if (s_lua_prog_len > 0)
    {
        s_lua_prog_buf[s_lua_prog_len] = 0; /* 文件末尾补0 */    
    }
    
    lua_do(s_lua_prog_buf);
}

// 重新封装下执行函数，支持打印调试信息
void lua_do(char *buf)
{
    int re;
	static int s_run = 0;   /* 避免重入 */
    static const char *str;    

	if (s_run > 0)
	{
		return;
	}

	s_run = 1;

    
    re = luaL_dostring(g_Lua, buf);
	if (re != LUA_OK)
	{        
        str = lua_tostring(g_Lua, -1);
        if (strstr(str, "attempt to yield from outside a coroutine"))   /* 用户终止了程序 */
        {
            /* 程序被用户终止\r\n 这是UTF8编码,需要GBK编码 */
            printf("\xB3\xCC\xD0\xF2\xB1\xBB\xD3\xC3\xBB\xA7\xD6\xD5\xD6\xB9\r\n");    
        }
        else    /* 程序语法或执行错误 */
        {
            printf(str);
            printf("\r\n");
        }  
		lua_pop(g_Lua, 1); //将错误信息出栈        
	}

    /* 显示Lua程序print的字符串. 内容在bsp_uart_fif文件 fputc 函数填充的 */
    if (g_LuaMemo.Refresh == 1)     
    {
        LCD_SetEncode(ENCODE_GBK);
        LCD_DrawMemo(&g_LuaMemo);            
        LCD_SetEncode(ENCODE_UTF8);
    } 
        
    s_run = 0;
}

// 通信程序用的函数，下载文件到lua程序缓冲区，不执行。
void lua_DownLoad(uint32_t _addr, uint8_t *_buf, uint32_t _len, uint32_t _total_len)
{
    uint32_t i;
    
    for (i = 0; i < _len; i++)
    {
        if (_addr < LUA_PROG_LEN_MAX)
        {
            s_lua_prog_buf[_addr + i] = _buf[i];
        }
    }
    
    s_lua_prog_len = _total_len;
    s_lua_prog_buf[s_lua_prog_len] = 0;
    
    if (g_Lua > 0)
    {
        lua_DeInit();
    }
    lua_Init();

}

void lua_Poll(void)
{
    if (g_tVar.LuaRunOnce == 1)
    {
        g_tVar.LuaRunOnce = 0;
        luaL_dostring(g_Lua, s_lua_prog_buf);
    }
}

void lua_DoInit(void)
{
    luaL_dostring(g_Lua, "init()");
}

void lua_StackDump(lua_State *L) 
{
    int i;
    int top = lua_gettop(L);

    printf("\r\nlua stack top = %d\r\n", top);   
    for (i = 1; i <= top; i++) 
    {
        int t = lua_type(L, i);
        switch (t) 
        {
            case LUA_TSTRING:
                printf("%d, %s\r\n", i, lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:
                printf("%d, %s\r\n", i, lua_toboolean(L, i) ? "true":"false");
                break;

            case LUA_TNUMBER:
                printf("%d, %f\r\n", i, lua_tonumber(L, i));
                break;

            default:
                printf("%d, %s\r\n", i, lua_typename(L, t));
                break;

        }
    }
}

// 通信写文件
uint8_t lua_66H_Write(uint32_t _addr, uint8_t *_buf, uint32_t _len)
{
    uint8_t re;
    
    lua_getglobal(g_Lua, "write");    // 函数入栈 获取lua函数write
    lua_pushinteger(g_Lua, _addr);     
    lua_pushlstring(g_Lua, (char *)_buf, _len); 
    lua_pushinteger(g_Lua, _len); 
    
    lua_pcall(g_Lua, 3, 1, 0);
    /*
        lua_pcall(lua_State *L,int nargs,int nresults,int errfunc)
        1
        nargs 参数个数
        nresults 返回值个数
        errFunc 错误处理函数，0表示无，表示错误处理函数在栈中的索引
    */
    re = lua_tonumber(g_Lua, -1);
    lua_pop(g_Lua, 1);
    return re;
}

// 通信读文件
uint8_t lua_67H_Read(uint32_t _addr, uint8_t *_buf, uint32_t _len)
{
    uint8_t re = 0;
    uint32_t i;
    
    lua_getglobal(g_Lua, "read");    // 函数入栈 获取lua函数write
    lua_pushinteger(g_Lua, _addr);     
    lua_pushinteger(g_Lua, _len); 
    
    lua_pcall(g_Lua, 2, 1, 0);
    /*
        lua_pcall(lua_State *L,int nargs,int nresults,int errfunc)
        1
        nargs 参数个数
        nresults 返回值个数
        errFunc 错误处理函数，0表示无，表示错误处理函数在栈中的索引
    */
    
    for (i = 0; i < _len; i++)
    {
        _buf[i] = s_lua_read_buf[i];
    }

    return re;
}

/*
*********************************************************************************************************
*    函 数 名: beep
*    功能说明: 蜂鸣函数 lua调用
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int beep(lua_State* L)
{
    uint16_t usBeepTime;
    uint16_t usStopTime;
    uint16_t usCycle;
    
    if (lua_type(L, 1) == LUA_TNUMBER)  /* 判断第1个参数. */
    {        
        usBeepTime = luaL_checknumber(L, 1);
        
        if (lua_type(L, 1) == LUA_TNUMBER)  /* 判断第2个参数. */
        {
            usStopTime = luaL_checknumber(L, 2);
        }
        else
        {
            return 0;
        }

        if (lua_type(L, 1) == LUA_TNUMBER)  /* 判断第3个参数. */
        {
            usCycle = luaL_checknumber(L, 3);
        } 
        else
        {
            return 0;
        }        
            
        BEEP_Start(usBeepTime, usStopTime, usCycle); /* 鸣叫50ms，停10ms， 1次 */
    }
    else
    {
        BEEP_Start(5, 1, 1); /* 鸣叫50ms，停10ms， 1次 */
    }
    
    //返回值用于提示该C函数的返回值数量，即压入栈中的返回值数量。
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: delayus
*    功能说明: 微秒延迟 lua函数
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int delayus(lua_State* L)
{
    uint32_t n = luaL_checknumber(L,1);

    bsp_DelayUS(n);
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: delayms
*    功能说明: 毫秒延迟 lua函数
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int delayms(lua_State* L)
{
    uint32_t n = luaL_checknumber(L,1);

    bsp_DelayMS(n);
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: print_hex
*    功能说明: 打印二进制数据    print_hex("123", 16)  print_hex("123", 16, 0x08000000)
*    形    参: 第1个是二进制数据，第2个是换行位置(缺省16)
*    返 回 值: 无
*********************************************************************************************************
*/
extern uint8_t USBCom_SendBuf(int _Port, uint8_t *_Buf, uint16_t _Len);
extern void lua_udp_SendBuf(uint8_t *_buf, uint16_t _len, uint16_t _port);
static int print_hex(lua_State* L)
{
    const char *data;
    size_t len;
    size_t line_bytes;
    int i;
    int j;
    char buf[128];
    uint16_t pos = 0;
    uint8_t ch;
    size_t intvalue;
    uint8_t DispAddr = 0;
    uint32_t addr = 0;
    
    if (lua_type(L, 1) == LUA_TSTRING)          /* 判断第1个参数 */
    {        
        data = luaL_checklstring(L, 1, &len);   /* 1是参数的位置， len是string的长度 */     
    }
    else
    {
        buf[0] = 0;
        for (i = 1; i <= 8; i++)
        {
            if (lua_type(L, i) == LUA_TNUMBER)
            {
                intvalue = luaL_checknumber(L, i);
                
                sprintf(&buf[strlen(buf)], "%08X ", intvalue);
            }
            else
            {
                break;
            }            
        }
        
        if (i > 1)
        {
            strcpy(&buf[strlen(buf)], "\r\n");
            #if PRINT_TO_UDP == 1
                lua_udp_SendBuf((uint8_t *)buf, strlen(buf), LUA_UDP_PORT);
            #else    
                USBCom_SendBuf(1, (uint8_t *)buf, strlen(buf));
            #endif             
        }
        return 0;
    }

    if (lua_type(L, 2) == LUA_TNUMBER)          /* 判断第2个参数 */
    {
        line_bytes = luaL_checknumber(L, 2);    /* 每行几个字节 */
    }
    else
    {
        line_bytes = 16;
    }    
    if (line_bytes == 0)
    {
        line_bytes = 16;
    }
    else if (line_bytes > 32)
    {
        line_bytes = 32;
    }
    
    if (lua_type(L, 3) == LUA_TNUMBER)      /* 判断第3个参数 */
    {
        addr = luaL_checknumber(L, 3);      /* 地址 */
        DispAddr = 1;     
    }
    else
    {
        DispAddr = 0;
    }

    /* 整行打印 */
    for (i = 0; i < len / line_bytes; i++)
    {               
        pos = 0;
        if (DispAddr != 0)
        {
            sprintf(&buf[pos], "%08X : ", addr);
            pos = 11;
        }        
        for (j = 0; j < line_bytes; j++)
        {
            ch = *data++;  
            addr++;
            
            buf[pos++] = BcdToChar(ch >> 4);
            buf[pos++] = BcdToChar(ch & 0x0F);
            buf[pos++] = ' ';
        }
        buf[pos++] = '\r'; 
        buf[pos++] = '\n'; 
        #if PRINT_TO_UDP == 1
            lua_udp_SendBuf((uint8_t *)buf, pos, LUA_UDP_PORT);
        #else    
            USBCom_SendBuf(1, (uint8_t *)buf, pos);
        #endif        
        bsp_Idle();
    }
    
    /* 打印不足一行的数据 */
    if (len % line_bytes)
    {
        pos = 0;
        for (j = 0; j < (len % line_bytes); j++)
        {
            ch = *data++;
            buf[pos++] = BcdToChar(ch >> 4);
            buf[pos++] = BcdToChar(ch & 0x0F);
            buf[pos++] = ' ';
        }
        buf[pos++] = '\r'; 
        buf[pos++] = '\n'; 
        #if PRINT_TO_UDP == 1
            lua_udp_SendBuf((uint8_t *)buf, pos, LUA_UDP_PORT);
        #else    
            USBCom_SendBuf(1, (uint8_t *)buf, pos);
        #endif         
    }
    
    return 1;
}

/*
*********************************************************************************************************
*    函 数 名: write_clock
*    功能说明: 写时钟
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int write_clock(lua_State* L)
{
    uint16_t year;
    uint8_t mon, day, hour, min, sec;
    
    if (lua_type(L, 1) == LUA_TNUMBER) /* 判断第1个参数 */
    {
        year = luaL_checknumber(L, 1);
    }
    if (lua_type(L, 2) == LUA_TNUMBER)
    {
        mon = luaL_checknumber(L, 2);
    }
    if (lua_type(L, 3) == LUA_TNUMBER)
    {
        day = luaL_checknumber(L, 3);
    }
    if (lua_type(L, 4) == LUA_TNUMBER)
    {
        hour = luaL_checknumber(L, 4);
    }
    if (lua_type(L, 5) == LUA_TNUMBER)
    {
        min = luaL_checknumber(L, 5);
    }
    if (lua_type(L, 6) == LUA_TNUMBER)
    {
        sec = luaL_checknumber(L, 6);
    }    
    
    RTC_WriteClock(year, mon, day, hour, min, sec);
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: read_clock
*    功能说明: 写时钟
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int read_clock(lua_State* L)
{
    static uint8_t rtc_buf[8];
    
    RTC_ReadClock();

    rtc_buf[0] = g_tRTC.Year >> 8;
    rtc_buf[1] = g_tRTC.Year;
    rtc_buf[2] = g_tRTC.Mon;
    rtc_buf[3] = g_tRTC.Day;
    rtc_buf[4] = g_tRTC.Hour;
    rtc_buf[5] = g_tRTC.Min;
    rtc_buf[6] = g_tRTC.Sec;
    rtc_buf[7] = g_tRTC.Week;
    
    lua_pushlstring(L, (const char *)rtc_buf, 8); 
    return 1;
}

/*
*********************************************************************************************************
*    函 数 名: get_runtime
*    功能说明: 获得CPU运行时间。ms单位
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int get_runtime(lua_State* L)
{
    int32_t runtime;
    
    runtime = bsp_GetRunTime();
    lua_pushnumber(L, runtime); 
    return 1;
}

/*
*********************************************************************************************************
*    函 数 名: check_runtime
*    功能说明: 判断时间长度
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int check_runtime(lua_State* L)
{
    int32_t lasttime;
    int32_t re;
    
    if (lua_type(L, 1) == LUA_TNUMBER) /* 判断第1个参数 */
    {
        lasttime = luaL_checknumber(L, 1);
    }

    re = bsp_CheckRunTime(lasttime);
    
    lua_pushnumber(L,     re); 
    return 1;
}

int l_my_print(lua_State* L) 
{
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) 
    {
        if (lua_isstring(L, i)) 
        {
            /* Pop the next arg using lua_tostring(L, i) and do your print */
            
            //lua_tostring(L, i)
        }
        else 
        {
            /* Do something with non-strings if you like */
        }
    }

    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: get_key
*    功能说明: 读取键值
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int get_key(lua_State* L)
{
    uint8_t key;
    
    key = bsp_GetKey();
    lua_pushnumber(L, key); 
    return 1;
}

/*
*********************************************************************************************************
*    函 数 名: put_key
*    功能说明: 模拟一个键值
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int put_key(lua_State* L)
{
    uint8_t key;
    
    if (lua_type(L, 1) == LUA_TNUMBER)  /* 判断第1个参数 */
    {
        key = luaL_checknumber(L, 1);
    }
    
    bsp_PutKey(key);
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: clear_key
*    功能说明: 清除按键缓冲区
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static int clear_key(lua_State* L)
{   
    bsp_ClearKey();
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: lua_RegisterFunc
*    功能说明: 注册lua可调用的c函数
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
static void lua_RegisterFunc(void)
{
    //将指定的函数注册为Lua的全局函数变量，其中第一个字符串参数为Lua代码
    //在调用C函数时使用的全局函数名，第二个参数为实际C函数的指针。
    lua_register(g_Lua, "beep", beep);
    lua_register(g_Lua, "delayus", delayus);
    lua_register(g_Lua, "delayms", delayms);
    lua_register(g_Lua, "print_hex", print_hex);
    lua_register(g_Lua, "write_clock", write_clock);
    lua_register(g_Lua, "read_clock", read_clock);
    lua_register(g_Lua, "get_runtime", get_runtime);
    lua_register(g_Lua, "check_runtime", check_runtime);
    lua_register(g_Lua, "get_key", get_key);
    lua_register(g_Lua, "put_key", put_key);
    lua_register(g_Lua, "clear_key", clear_key);
    
    /* 注册接口函数 */
    lua_gpio_RegisterFun();    
    lua_i2c_RegisterFun();
    lua_spi_RegisterFun();
    lua_tim_RegisterFun();
    lua_tcp_RegisterFun();
    lua_qspi_RegisterFun();
    lua_fatfs_RegisterFun();
    lua_swd_RegisterFun();   
    lua_swim_RegisterFun();  
    lua_adc_RegisterFun();
    lua_dac_RegisterFun();
    lua_reg_RegisterFun();
    lua_extio_RegisterFun();
    lua_uart_RegisterFun();
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
