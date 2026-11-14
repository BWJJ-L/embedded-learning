#include "i2c_eeprom.h"

EEPROM_TypeDef gEEPROMInstance = {0};

void I2C_eepromWaitReady(  uint8_t SlaveAddr)
{
    I2C_DisableInt();
    while (1)
    {
        if (I2C_MasterSendDataToSlave( SlaveAddr, NULL, 0) == I2C_NO_ERROR)
        {
            break;
        }
    }    
}

I2C_ErrorDef I2C_eepromWriteBytes(  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length)
{
    uint16_t addr = DataAddr;
    uint16_t len = Length, size;
    uint8_t SendBuff[EEPROM_PAGE_SIZE+2];
    uint8_t i, cnt;
    uint8_t bytesUntilPageBoundary;

    if (Length == 0)
    {
        return I2C_NO_ERROR;
    }

    while (len > 0)
    {   
        I2C_eepromWaitReady( SlaveAddr);
        
        bytesUntilPageBoundary = EEPROM_PAGE_SIZE - addr % EEPROM_PAGE_SIZE;
        
        cnt = len < bytesUntilPageBoundary ? len : bytesUntilPageBoundary;
        
        if (EEPROM_DATA_ADDR_WIDTH == 1)
        {
            SendBuff[0] = (uint8_t)addr;
            for (i=0; i < cnt; i++)
            {
                SendBuff[i+1] = *buffer;
                buffer++;
            }
            size = cnt + 1;           
        }
        else
        {
            SendBuff[0] = (uint8_t)addr;
            SendBuff[1] = (uint8_t)(addr>>8);
            for (i = 0; i< cnt; i++)
            {
                SendBuff[i+2] = *buffer;
                buffer++;
            }
            size = cnt + 2;  
        }
        len -= cnt;
        addr += cnt;
        
        I2C_MasterSendDataToSlave( SlaveAddr, SendBuff, size);        
    }
     return I2C_NO_ERROR;
}

I2C_ErrorDef I2C_eepromReadBytes(  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length)
{
    uint16_t addr = DataAddr;
    
    if (Length == 0)
    {
        return I2C_NO_ERROR;
    }
    
    I2C_eepromWaitReady( SlaveAddr);
    
    if (EEPROM_DATA_ADDR_WIDTH == 1)
    {
        I2C_MasterSendDataToSlave( SlaveAddr, (uint8_t *)(&addr), 1);
    }
    else
    {
        I2C_MasterSendDataToSlave( SlaveAddr, (uint8_t *)(&addr), 2);
    }
    
    return I2C_MasterRecDataFromSlave( SlaveAddr, buffer, Length);    
}

I2C_ErrorDef I2C_eepromWriteBytesInt(  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length)
{
    if (gEEPROMInstance.EE_State != EE_STATE_IDLE)
    {
        return I2C_BUSY;
    }
    if (Length == 0)
    {
        return I2C_NO_ERROR;
    }
    gEEPROMInstance.EE_DataAddr = DataAddr;
    gEEPROMInstance.EE_DeviceAddr = SlaveAddr;
    gEEPROMInstance.EE_SendBuffPtr = buffer;
    gEEPROMInstance.EE_SendLen  = Length;
    gEEPROMInstance.EE_State = EE_STATE_WRITE;
    
    I2C_ClearIrq();
    I2C_EnableInt();
    I2C_Cmd( ENABLE);
    I2C_GenerateSTART( ENABLE);  
    return I2C_BUSY;   
}

I2C_ErrorDef I2C_eepromReadBytesInt(  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length)
{
    if (gEEPROMInstance.EE_State != EE_STATE_IDLE)
    {
        return I2C_BUSY;
    }
    if (Length == 0)
    {
        return I2C_NO_ERROR;
    }
    gEEPROMInstance.EE_DataAddr = DataAddr;
    gEEPROMInstance.EE_DeviceAddr = SlaveAddr;
    gEEPROMInstance.EE_RecvBuffPtr = buffer;
    gEEPROMInstance.EE_RecvLen  = Length;
    gEEPROMInstance.EE_State = EE_STATE_READ;
    
    I2C_ClearIrq();
    I2C_EnableInt();
    I2C_Cmd( ENABLE);
    I2C_GenerateSTART( ENABLE);
    return I2C_BUSY;    
}

/**
 * @brief  I2C EEPROM读写中断函数
 *
 */

void I2C_eepromHandleIRQ(void)
{
    uint8_t u8State = I2C_GetState();
    static uint16_t cnt;
    static uint8_t IsDataAddrTransferCplt = 0;
    
    switch (u8State)
    {       
        case 0x08:    //发送完START信号
        case 0x10:    //发送完重复起始信号
            I2C_GenerateSTART( DISABLE);            
            if (gEEPROMInstance.EE_State == EE_STATE_WRITE)
            {
                I2C_Send7bitAddress( gEEPROMInstance.EE_DeviceAddr, I2C_Direction_Transmitter);
                cnt = gEEPROMInstance.EE_SendLen;                
            }
            if (gEEPROMInstance.EE_State == EE_STATE_READ)
            {
                if (IsDataAddrTransferCplt == 2)
                {
                    // 读数据的地址已发送完毕
                    I2C_Send7bitAddress( gEEPROMInstance.EE_DeviceAddr, I2C_Direction_Receiver);
                    cnt = gEEPROMInstance.EE_RecvLen;
                    IsDataAddrTransferCplt = 0;
                }
                else
                {
                    I2C_Send7bitAddress( gEEPROMInstance.EE_DeviceAddr, I2C_Direction_Transmitter);
                    IsDataAddrTransferCplt = 0;
                }
            }             
            break;
        // 发送
        case 0x18:
            I2C_SendData( (uint8_t)gEEPROMInstance.EE_DataAddr);               
            break;        
        case 0x28:
            if (EEPROM_DATA_ADDR_WIDTH != 1)
            {
                if (IsDataAddrTransferCplt == 0)   // 发高位地址
                {
                    I2C_SendData( (uint8_t)(gEEPROMInstance.EE_DataAddr>>8));
                    IsDataAddrTransferCplt = 1;                                 
                    break;
                }
            }            
            IsDataAddrTransferCplt = 1;
            if (gEEPROMInstance.EE_State == EE_STATE_WRITE)
            {
                if (cnt > 0)
                {
                    I2C_SendData( *gEEPROMInstance.EE_SendBuffPtr);
                    gEEPROMInstance.EE_SendBuffPtr++;
                    cnt--;
                }            
                else
                {
                    I2C_GenerateSTOP();
                    IsDataAddrTransferCplt = 0;
                    gEEPROMInstance.EE_State = EE_STATE_IDLE;
                }                
            }
            if (gEEPROMInstance.EE_State == EE_STATE_READ)
            {
                if (IsDataAddrTransferCplt == 1)
                {
                    //重发起始信号，切换到读                    
                    I2C_GenerateSTART( ENABLE); 
                    IsDataAddrTransferCplt = 2;                    
                }
            }
            break;
        case 0x20:    //已发送 SLA+W，已接收 NACK            
        case 0x30:    // 已发送 I2C_DR 中的数据，已接收 NACK
        case 0x38:    // 主机在发送 SLA+W 阶段或者发送数据阶段丢失仲裁
        case 0x48:    // 已发送 SLA+R，已接收 NACK
        case 0x58:    // 已接收数据字节，NACK 已返回
            I2C_GenerateSTOP();
            IsDataAddrTransferCplt = 0;
            *gEEPROMInstance.EE_RecvBuffPtr =  I2C_ReceiveData();
            cnt--;
            if (cnt == 0) 
            {
                gEEPROMInstance.EE_State = EE_STATE_IDLE;
            }
            else
            {
                gEEPROMInstance.EE_State = EE_STATE_FAIL;
            }                
            break;

        // 接收
        case 0x40:    //已发送 SLA+R，已接收 ACK
            I2C_AcknowledgeConfig( ENABLE);                 
            break;
        case 0x50:    // 已接收数据字节，ACK 已返回
            *gEEPROMInstance.EE_RecvBuffPtr =  I2C_ReceiveData();
            gEEPROMInstance.EE_RecvBuffPtr++;
            cnt--;
            if (cnt == 1)
            {                
                I2C_AcknowledgeConfig( DISABLE);
            }                   
            break;  
    }
    I2C_ClearIrq();
}

