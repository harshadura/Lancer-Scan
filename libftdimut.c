#include "libftdimut.h"

FT_STATUS ftdimut_setup() {
  FT_STATUS ftStatus; 
  unsigned char timer;

	ftStatus = FT_SetVIDPID(USB_VID, USB_PID);
  if(ftStatus != FT_OK) return ftStatus;
	ftStatus = FT_Open(0, &ftHandle);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_ResetDevice(ftHandle);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_SetBaudRate(ftHandle, 15625);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_SetTimeouts(ftHandle, 1000, 1000);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_GetLatencyTimer(ftHandle, &timer);
  if(ftStatus != FT_OK) return ftStatus;
  ftStatus = FT_SetLatencyTimer(ftHandle, 1);
  if(ftStatus != FT_OK) return ftStatus;

  return FT_OK;
}

FT_STATUS ftdimut_init() {
  FT_STATUS ftStatus; 
  unsigned char buf[4];
  unsigned int bytesRead = 0;

  if(ftdimut_getData(0x17) != 0) {
    return FT_OK;
  }

  printf("Sending 0x00 at 5 baud\n");

  printf("Break on......\n");
  ftStatus = FT_SetBreakOn(ftHandle);
  if(ftStatus != FT_OK) return ftStatus;
  usleep(1800 * 1000);
  printf("Break off......\n");
  ftStatus = FT_SetBreakOff(ftHandle);
  if(ftStatus != FT_OK) return ftStatus;

  ftStatus = FT_Read(ftHandle, buf, 4, &bytesRead);
  if(ftStatus != FT_OK) return ftStatus;

  if(bytesRead == 4) {
    return FT_OK;
  }
  return FT_OTHER_ERROR;
}

FT_STATUS ftdimut_close() {
	return FT_Close(ftHandle);
}

unsigned char ftdimut_getData(unsigned char request) {
  unsigned int bytesRead, bytesWrote;
  unsigned char buf[2];
  FT_Write(ftHandle, &request, 1, &bytesWrote);
  if(bytesWrote != 1) {
    return 0;
  }
  FT_Read(ftHandle, buf, 2, &bytesRead);
  if(bytesRead != 2) {
    return 0;
  }

  return buf[1];
}
