#pragma once

#define BigShort( val )    WordSwap( val )
#define BigWord( val )     WordSwap( val )
#define BigLong( val )     DWordSwap( val )
#define BigDWord( val )    DWordSwap( val )
#define LittleShort( val ) ( val )
#define LittleWord( val )  ( val )
#define LittleLong( val )  ( val )
#define LittleDWord( val ) ( val )
#define SwapShort( val )   BigShort( val )
#define SwapWord( val )    BigWord( val )
#define SwapLong( val )    BigLong( val )
#define SwapDWord( val )   BigDWord( val )

#define CRC32_INIT_VALUE 0xFFFFFFFFUL
#define CRC32_XOR_VALUE  0xFFFFFFFFUL

#define NUM_BYTES 256

typedef unsigned long CRC32_t;

void CRC32_Init( CRC32_t* pulCRC );
void CRC32_ProcessBuffer( CRC32_t* pulCRC, const void* p, int len );
void CRC32_Final( CRC32_t* pulCRC );
CRC32_t CRC32_GetTableEntry( unsigned int slot );

inline CRC32_t CRC32_ProcessSingleBuffer( const void* p, int len )
{
	CRC32_t crc;

	CRC32_Init( &crc );
	CRC32_ProcessBuffer( &crc, p, len );
	CRC32_Final( &crc );

	return crc;
}
