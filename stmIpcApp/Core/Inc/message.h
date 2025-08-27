//********************************* Message ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : Typedefs are included.
// Note    : None
//
//*****************************************************************************
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//******************************* Include Files *******************************
#include "common.h"

//******************************* Global Types ********************************
typedef struct REQUEST
{
    uint32 ulUID;
    uint8 ucCMD;
    uint32 ulData;

}REQUEST;

typedef struct ACK
{
    uint32 ulUID;
    uint8 ucCMD;
    uint8 ucState;
    uint32 ulData;

}ACK;
//***************************** Global Constants ******************************

//***************************** Global Variables ******************************

//**************************** Forward Declarations ***************************

//*********************** Inline Method Implementations ***********************

#endif // _MESSAGE_H_
// EOF
