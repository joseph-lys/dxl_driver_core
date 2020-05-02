#ifndef DXL_PROTOCOL_BASE_H_
#define DXL_PROTOCOL_BASE_H_


#include <stdint.h>
#include <stddef.h>


class DxlProtocolBase {
protected:
  uint8_t* rx_buf_ = nullptr;
  size_t rx_idx_ = 0;
  size_t rx_readable_size_ = 0;
  size_t rx_buf_size_ = 0;

  uint8_t* tx_buf_ = nullptr;
  size_t tx_idx_ = 0;
  size_t tx_writable_size_ = 0;
  size_t tx_buf_size_ = 0;

 public:
  /// Reset the read pointer to the start of readable data.
  /// Returns true if data is valid, else return false
  /// Use this to validate the data on the read buffer, no further checking is done on other functions.
  /// User should also check the StatusByte to check for device specific errors
  virtual bool beginRxRead() = 0;

  /// Get the reply data's source Id, does not increment the read index
  virtual uint8_t getRxId() = 0;

  /// Get the reply data's size, does not increment the read index
  virtual size_t getRxSize() = 0;

  /// Get the reply status byte.
  virtual uint8_t getRxStatusByte() = 0;

  /// Setup reading from a new buffer location. >Does NOT validate the data.
  inline void setRxBuffer(uint8_t* buffer, int buffer_size);

  /// get one byte and increment the read index, returns 0xff if error
  inline uint8_t readRxByte();

  /// get one byte and increment the read index, return 0xffff if error
  inline uint16_t readRxWord(); 
  
  /// estimate the number or expected Rx bytes based on the Tx Data
  virtual size_t estimateRxSize() = 0;
  
  /// Writes the Id and Instruction. Reset the write pointer to the start of writable data. 
  virtual void initTxData(uint8_t id, uint8_t ins_code) = 0;
  
  /// Finalizes the writing of data. Returns the total number of bytes, including header and checksum.
  virtual size_t finalizeTx() = 0;

  /// Setup reading from a new buffer location. 
  inline void setTxBuffer(uint8_t* buffer, int buffer_size);

  /// Write one byte and increment the write index. Returns true if success, returns false if error.
  inline bool writeTxByte(uint8_t);

  /// Write one word and increment the write index. Returns true if success, returns false if error
  inline bool writeTxWord(uint16_t); 
};


void DxlProtocolBase::setRxBuffer(uint8_t* buffer, int buffer_size) {
  if (buffer) {
    rx_buf_ = buffer;
    rx_buf_size_ = buffer_size;
    rx_readable_size_ = 0;
  }
}

uint8_t DxlProtocolBase::readRxByte() {
  uint8_t value = 0xff;
  if (rx_idx_ < rx_readable_size_) {
    value = rx_buf_[rx_idx_++];
  }
  return value;
}

uint16_t DxlProtocolBase::readRxWord() {
  uint16_t value = 0xffff;
  if (rx_idx_ < rx_readable_size_ && (rx_idx_ + 1) < rx_readable_size_) {
    value = static_cast<uint16_t>(rx_buf_[rx_idx_++]);
    value |= static_cast<uint16_t>(rx_buf_[rx_idx_++]) << 8; 
  }
  return value;
}

void DxlProtocolBase::setTxBuffer(uint8_t* buffer, int buffer_size) {
  if (buffer) {
    tx_buf_ = buffer;
    tx_buf_size_ = buffer_size;
    tx_writable_size_ = 0;
  }
};

bool DxlProtocolBase::writeTxByte(uint8_t value) {
  bool success = false;
  if (tx_idx_ < tx_writable_size_) {
    tx_buf_[tx_idx_++] = value;
    success = true;
  }
  return success;
}

bool DxlProtocolBase::writeTxWord(uint16_t value) {
  bool success = false;
  if (tx_idx_ < tx_writable_size_ && (tx_idx_ + 1) < tx_writable_size_) {
    tx_buf_[tx_idx_++] = static_cast<uint8_t>(value & 0xff);
    tx_buf_[tx_idx_++] = static_cast<uint8_t>(value >> 8);
    success = true;
  }
  return success;
}

#endif  // DXL_PROTOCOL_BASE_H_
