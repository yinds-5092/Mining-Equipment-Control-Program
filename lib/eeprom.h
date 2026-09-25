/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。
技术支持QQ群二：915765308
******************************************/

#ifndef __EEPROM_H
#define __EEPROM_H

#include <STC8HX.h>

#define     EEPROM_STANDBY()       IAP_CMD = 0     //IAP空闲命令（禁止）
#define     EEPROM_IAP_READ()      IAP_CMD = 1     //IAP读出命令
#define     EEPROM_IAP_WRITE()     IAP_CMD = 2     //IAP写入命令
#define     EEPROM_IAP_ERASE()     IAP_CMD = 3     //IAP擦除命令

#define     EEPROM_IAP_ENABLE()    IAP_CONTR = EEPROM_IAP_EN; IAP_TPS = sys_clk / 1000000
#define     EEPROM_IAP_DISABLE()   IAP_CONTR = 0; IAP_CMD = 0; IAP_TRIG = 0; IAP_ADDRH = 0xff; IAP_ADDRL = 0xff

#define     EEPROM_IAP_EN          (1<<7)
#define     EEPROM_IAP_SWBS        (1<<6)
#define     EEPROM_IAP_SWRST       (1<<5)
#define     EEPROM_IAP_CMD_FAIL    (1<<4)

void eeprom_sector_erase(uint16 addr);//EEPROM擦除指定扇区

/* eeprom读数据函数 */
void eeprom_read(uint16 addr,uint8 *buf,uint16 length);//EEPROM读字节数据
int8 eeprom_read_one_char(uint16 addr);
void eeprom_read_char(uint16 addr,char *buf,uint16 length);
uint8 eeprom_read_one_uchar(uint16 addr);
void eeprom_read_uchar(uint16 addr,uint8_t *buf,uint16 length);
int16 eeprom_read_one_short(uint16 addr);
void eeprom_read_Short(uint16 addr,int16 *buf,uint16 length);
uint16 eeprom_read_one_ushort(uint16 addr);
void eeprom_read_ushort(uint16 addr,uint16 *buf,uint16 length);
int32 eeprom_read_one_int32(uint16 addr);
void eeprom_read_int32(uint16 addr,int32 *buf,uint16 length);
uint32 eeprom_read_one_uint32(uint16 addr);
void eeprom_read_uint32(uint16 addr,uint32 *buf,uint16 length);
// int64 eeprom_read_one_long64(uint16 addr);
// void eeprom_read_long64(uint16 addr,int64_t *buf,uint16 length);
// uint64 eeprom_read_one_ulong64(uint16 addr);
// void eeprom_read_ulong64(uint16 addr,uint64_t *buf,uint16 length);
float eeprom_read_one_float(uint16 addr);
void eeprom_read_float(uint16 addr,float *buf,uint16 length);
// double eeprom_read_one_double(uint16 addr);
// void eeprom_read_double(uint16 addr,double *buf,uint16 length);

/* eeprom写数据函数 */
uint8 eeprom_write(uint16 addr,uint8 *buf,uint8 length);//EEPROM写字节数据
uint8 eeprom_write_one_char(uint16 addr,char dat);
uint8 eeprom_write_char(uint16 addr,char *buf,uint16 length);
uint8 eeprom_write_one_uchar(uint16 addr,uint8 dat);
uint8 eeprom_write_uchar(uint16 addr,uint8 *buf,uint16 length);
uint8 eeprom_write_one_short(uint16 addr,int16 dat);
uint8 eeprom_write_short(uint16 addr,int16 *buf,uint16 length);
uint8 eeprom_write_one_ushort(uint16 addr,uint16 dat);
uint8 eeprom_write_ushort(uint16 addr,uint16 *buf,uint16 length);
uint8 eeprom_write_one_int32(uint16 addr,int32 dat);
uint8 eeprom_write_int32(uint16 addr,int32 *buf,uint16 length);
uint8 eeprom_write_one_uint32(uint16 addr,uint32 dat);
uint8 eeprom_write_uint32(uint16 addr,uint32 *buf,uint16 length);
// uint8 eeprom_write_one_long64(uint16 addr,int64 dat);
// uint8 eeprom_write_long64(uint16 addr,int64_t *buf,uint16 length);
// uint8 eeprom_write_one_ulong64(uint16 addr,uint64 dat);
// uint8 eeprom_write_ulong64(uint16 addr,uint64 *buf,uint16 length);
uint8 eeprom_write_one_float(uint16 addr,float dat);
uint8 eeprom_write_float(uint16 addr,float *buf,uint16 length);
// uint8 eeprom_write_one_double(uint16 addr,double dat);
// uint8 eeprom_write_double(uint16 addr,double *buf,uint16 length);

//========================================================================
// 描述: 禁止EEPROM.
// 参数: none.
// 返回: none.
//========================================================================
void eeprom_disable(void)        //禁止访问EEPROM
{
    IAP_CONTR = 0;          //关闭 IAP 功能
    IAP_CMD = 0;            //清除命令寄存器
    IAP_TRIG = 0;           //清除触发寄存器
    IAP_ADDRH = 0xff;       //将地址设置到非 IAP 区域
    IAP_ADDRL = 0xff;
}

//========================================================================
// 描述: 触发EEPROM操作.
// 参数: none.
// 返回: none.
//========================================================================
void eeprom_trig(void)
{
    F0 = EA;    //保存全局中断
    EA = 0;     //禁止中断, 避免触发命令无效
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                    //先送5AH，再送A5H到IAP触发寄存器，每次都需要如此
                                        //送完A5H后，IAP命令立即被触发启动
                                        //CPU等待IAP完成后，才会继续执行程序。
    _nop_();
    _nop_();
    EA = F0;    //恢复全局中断
}

//========================================================================
// 描述: 擦除一个扇区.
// 参数: addr:  要擦除的EEPROM的扇区中的一个字节地址.
// 返回: none.
//========================================================================
void eeprom_sector_erase(uint16 addr)
{
    EEPROM_IAP_ENABLE();    //设置等待时间，允许IAP操作，送一次就够
    EEPROM_IAP_ERASE();     //宏调用, 送扇区擦除命令，命令不需改变时，不需重新送命令
                            //只有扇区擦除，没有字节擦除，512字节/扇区。
                            //扇区中任意一个字节地址都是扇区地址。
    IAP_ADDRH = addr / 256;       //送扇区地址高字节（地址需要改变时才需重新送地址）
    IAP_ADDRL = addr % 256;       //送扇区地址低字节
    eeprom_trig();      //触发EEPROM操作
    eeprom_disable();   //禁止EEPROM操作
}

//========================================================================
// 描述: 读N个字节函数.
// 参数: addr:  要读出的EEPROM的首地址.
//       buf: 要读出数据的指针.
//       length:      要读出的长度
// 返回: none.
//========================================================================
void eeprom_read(uint16 addr,uint8 *buf,uint16 length)
{
    EEPROM_IAP_ENABLE();                           //设置等待时间，允许IAP操作，送一次就够
    EEPROM_IAP_READ();                             //送字节读命令，命令不需改变时，不需重新送命令
    do
    {
        IAP_ADDRH = addr / 256;       //送地址高字节（地址需要改变时才需重新送地址）
        IAP_ADDRL = addr % 256;       //送地址低字节
        eeprom_trig();                      //触发EEPROM操作
        *buf = IAP_DATA;            //读出的数据送往
        addr++;
        buf++;
    }while(--length);
    eeprom_disable();
}

//========================================================================
// 描述: 读取一个有符号8位数据.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
int8 eeprom_read_one_char(uint16 addr)
{
    int8 temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;
}

//========================================================================
// 描述: 读取多个有符号8位数据.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_char(uint16 addr,char *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(char));
}

//========================================================================
// 描述: 读取一个无符号8位数据.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
uint8 eeprom_read_one_uchar(uint16 addr)
{
    uint8 temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;   
}

//========================================================================
// 描述: 读取多个无符号8位数据.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_uchar(uint16 addr,uint8_t *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(uint8_t));
}

//========================================================================
// 描述: 读取一个有符号16位数据.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
int16 eeprom_read_one_short(uint16 addr)
{
    int16 temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;     
}

//========================================================================
// 描述: 读取多个有符号16位数据.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_short(uint16 addr,int16 *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(int16));
}

//========================================================================
// 描述: 读取一个无符号16位数据.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
uint16 eeprom_read_one_ushort(uint16 addr)
{
    uint16 temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;
}

//========================================================================
// 描述: 读取多个无符号16位数据.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_ushort(uint16 addr,uint16 *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(uint16)); 
}

//========================================================================
// 描述: 读取一个有符号32位数据.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
int32 eeprom_read_one_int32(uint16 addr)
{
    int32 temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;
}

//========================================================================
// 描述: 读取多个有符号32位数据.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_int32(uint16 addr,int32 *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(int32)); 
}

//========================================================================
// 描述: 读取一个无符号32位数据.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
uint32 eeprom_read_one_uint32(uint16 addr)
{
    uint32 temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;   
}

//========================================================================
// 描述: 读取多个无符号32位数据.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_uint32(uint16 addr,uint32 *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(uint32));
}

//========================================================================
// 描述: 读取一个单精度浮点数.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
float eeprom_read_one_float(uint16 addr)
{
    float temp;
    eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
    return temp;  
}

//========================================================================
// 描述: 读取多个单精度浮点数.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
void eeprom_read_float(uint16 addr,float *buf,uint16 length)
{
    eeprom_read(addr,(uint8 *)buf,length*sizeof(float));
}

//========================================================================
// 描述: 读取一个双精度浮点数.
// 参数: addr:要读入的地址
// 返回: 读出的数据.
//========================================================================
// double eeprom_read_one_double(uint16 addr)
// {
//     double temp;
//     eeprom_read(addr,(uint8 *)&temp,sizeof(temp));
//     return temp; 
// }

//========================================================================
// 描述: 读取多个双精度浮点数.
// 参数: addr:要读入的地址;buf:读出的数据指针;length:要读的长度.
// 返回: none.
//========================================================================
// void eeprom_read_double(uint16 addr,double *buf,uint16 length)
// {
//     eeprom_read(addr,(uint8 *)buf,length);
// }

//========================================================================
// 描述: 写N个字节函数.
// 参数: addr:  要写入的EEPROM的首地址.
//       buf: 要写入数据的指针.
//       length:      要写入的长度
// 返回: 0: 写入正确.  1: 写入长度为0错误.  2: 写入数据错误.
//========================================================================
uint8 eeprom_write(uint16 addr,uint8 *buf,uint8 length)
{
    uint8  i;
    uint16 j;
    uint8  *p;
    
    if(length == 0) return 1;   //长度为0错误

    EEPROM_IAP_ENABLE();                       //设置等待时间，允许IAP操作，送一次就够
    i = length;
    j = addr;
    p = buf;
    EEPROM_IAP_WRITE();                            //宏调用, 送字节写命令
    do
    {
        IAP_ADDRH = addr / 256;       //送地址高字节（地址需要改变时才需重新送地址）
        IAP_ADDRL = addr % 256;       //送地址低字节
        IAP_DATA  = *buf;           //送数据到IAP_DATA，只有数据改变时才需重新送
        eeprom_trig();                      //触发EEPROM操作
        addr++;                       //下一个地址
        buf++;                      //下一个数据
    }while(--length);                       //直到结束

    addr = j;
    length = i;
    buf = p;
    i = 0;
    EEPROM_IAP_READ();                             //读N个字节并比较
    do
    {
        IAP_ADDRH = addr / 256;       //送地址高字节（地址需要改变时才需重新送地址）
        IAP_ADDRL = addr % 256;       //送地址低字节
        eeprom_trig();                      //触发EEPROM操作
        if(*buf != IAP_DATA)        //读出的数据与源数据比较
        {
            i = 2;
            break;
        }
        addr++;
        buf++;
    }while(--length);

    eeprom_disable();
    return i;
}

//========================================================================
// 描述: 写入一个有符号8位数据.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_char(uint16 addr,char dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));    
}

//========================================================================
// 描述: 写入多个有符号8位数据.
// 参数: addr:要写入的地址
//       buf:要写入的数据指针
//       length:要写入的数据长度  
// 返回: none.
//========================================================================
uint8 eeprom_write_char(uint16 addr,char *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length);
}

//========================================================================
// 描述: 写入一个无符号8位数据.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_uchar(uint16 addr,uint8 dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
}

//========================================================================
// 描述: 写入多个无符号8位数据.
// 参数: addr:要写入的地址
//       buf:要写入的数据指针
//       length:要写入的数据长度  
// 返回: none.
//========================================================================
uint8 eeprom_write_uchar(uint16 addr,uint8 *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length);
}

//========================================================================
// 描述: 写入一个有符号16位数据.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_short(uint16 addr,int16 dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
}

//========================================================================
// 描述: 写入多个有符号16位数据.
// 参数: addr:要写入的地址
//       buf:要写入的数据指针
//       length:要写入的数据长度  
// 返回: none.
//========================================================================
uint8 eeprom_write_short(uint16 addr,int16 *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length*sizeof(int16));
}

//========================================================================
// 描述: 写入一个无符号16位数据.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_ushort(uint16 addr,uint16 dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
}

//========================================================================
// 描述: 写入多个无符号16位数据.
// 参数: addr:要写入的地址
//       buf:要写入的数据指针
//       length:要写入的数据长度  
// 返回: none.
//========================================================================
uint8 eeprom_write_ushort(uint16 addr,uint16 *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length*sizeof(uint16));
}

//========================================================================
// 描述: 写入一个有符号32位数据.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_int32(uint16 addr,int32 dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
}

//========================================================================
// 描述: 写入多个有符号32位数据.
// 参数: addr:要写入的地址
//       buf:要写入的数据指针
//       length:要写入的数据长度  
// 返回: none.
//========================================================================
uint8 eeprom_write_int32(uint16 addr,int32 *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length*sizeof(int32));
}

//========================================================================
// 描述: 写入一个无符号32位数据.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_uint32(uint16 addr,uint32 dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
}

//========================================================================
// 描述: 写入多个无符号32位数据.
// 参数: addr:要写入的地址
//       buf:要写入的数据指针
//       length:要写入的数据长度  
// 返回: none.
//========================================================================
uint8 eeprom_write_uint32(uint16 addr,uint32 *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length*sizeof(uint32));
}

//========================================================================
// 描述: 写入一个单精度浮点数.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_one_float(uint16 addr,float dat)
{
    return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
}

//========================================================================
// 描述: 写入多个单精度浮点数.
// 参数: addr:要写入的地址
//       dat:要写入的数据
//       length:要写入的数据长度  
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
uint8 eeprom_write_float(uint16 addr,float *buf,uint16 length)
{
    return eeprom_write(addr,(uint8 *)buf,length*sizeof(float));
}

//========================================================================
// 描述: 写入一个双精度浮点数.
// 参数: addr:要写入的地址
//       dat:要写入的数据
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
// uint8 eeprom_write_one_double(uint16 addr,double dat)
// {
//     return eeprom_write(addr,(uint8 *)&dat,sizeof(dat));  
// }

//========================================================================
// 描述: 写入多个双精度浮点数.
// 参数: addr:要写入的地址
//       dat:要写入的数据
//       length:要写入的数据长度  
// 返回:  0: 写入正确,  1: 写入长度为0错误,  2: 写入数据错误.
//========================================================================
// uint8 eeprom_write_double(uint16 addr,double *buf,uint16 length)
// {
//     return eeprom_write(addr,(uint8 *)buf,length*sizeof(double));
// }

#endif