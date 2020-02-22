#ifndef DXL_READER_BASE_H_
#define DXL_READER_BASE_H_


#include <stdint.h>
#include <stddef.h>


class DxlReaderBase {
protected:
  uint8_t* buffer_ = nullptr;
  size_t idx_ = 0;
  size_t readable_size_ = 0;
  size_t buffer_size_ = 0;
 public:
  /// Reset the read pointer to the start of readable data.
  /// Returns "STATUS" flag of the dynamixel, if there is any error, return 0x80
  /// Use this to validate the data on the read buffer, no further checking is done on other functions.
  virtual uint8_t beginRead() = 0;

  /// Get the reply data's source Id, does not increment the read index
  virtual uint8_t getId() = 0;

  /// Get the reply data's size, does not increment the read index
  virtual size_t getSize() = 0;

  /// Setup reading from a new buffer location. Does NOT validate the data.
  inline void setBuffer(uint8_t* buffer, int buffer_size);

  /// get one byte and increment the read index, returns 0xff if error
  inline uint8_t readByte();

  /// get one byte and increment the read index, return 0xffff if error
  inline uint16_t readWord(); 
};


void DxlReaderBase::setBuffer(uint8_t* buffer, int buffer_size) {
  if (buffer) {
    buffer_ = buffer;
    buffer_size_ = buffer_size;
    readable_size_ = 0;
  }
}

uint8_t DxlReaderBase::readByte() {
  uint8_t value = 0xff;
  if (idx_ < readable_size_) {
    value = buffer_[idx_++];
  }
  return value;
}

uint16_t DxlReaderBase::readWord() {
  uint16_t value = 0xffff;
  if (idx_ < readable_size_ && (idx_ + 1) < readable_size_) {
    value = static_cast<uint16_t>(buffer_[idx_++]);
    value |= static_cast<uint16_t>(buffer_[idx_++]) << 8; 
  }
  return value;
}


#endif  // DXL_READER_BASE_H_
