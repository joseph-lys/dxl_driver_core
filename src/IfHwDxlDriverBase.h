#ifndef IF_HW_DXL_DRIVER_BASE_H_
#define IF_HW_DXL_DRIVER_BASE_H_

#include <stdint.h>
#include <stddef.h>


/// Abstract class for DxlDriver to interface with Hardware
class IfHwDxlDriverBase {
 public:
  virtual ~IfHwDxlDriverBase() { }
  
  /// check if there are any available data on the receive buffer
  virtual size_t available() = 0;  

  /// check if there are any available data on the receive buffer
  virtual bool txIsDone() = 0;

  /// check if there are any available data on the receive buffer
  virtual bool isTimeout() = 0;

  /// read one byte from the buffer
  virtual uint8_t read() = 0;

  /// starts a transmission. [expected_reply_size] is used to estimate the timeout
  virtual void beginTransmission(uint8_t* tx_buf, size_t tx_buf_size, size_t expected_reply_size) = 0;
};


#endif  // IF_HW_DXL_DRIVER_BASE_H_