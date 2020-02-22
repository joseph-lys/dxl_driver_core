#ifndef DXL_WRITER_BASE_H_
#define DXL_WRITER_BASE_H_


#include <stdint.h>
#include <stddef.h>




class DxlWriterBase {
 protected:
  uint8_t* buffer_ = nullptr;
  size_t idx_ = 0;
  size_t writable_size_ = 0;
  size_t buffer_size_ = 0;
 public:
  /// Writes the Id and Instruction. Reset the write pointer to the start of writable data. 
  virtual void setInstruction(uint8_t id, uint8_t ins_code) = 0;
  
  /// Finalizes the writing of data. Returns the total number of bytes, including header and checksum.
  virtual size_t finalize() = 0;

  /// Setup reading from a new buffer location. 
  inline void setBuffer(uint8_t* buffer, int buffer_size);

  /// Write one byte and increment the write index. Returns true if success, returns false if error.
  inline bool writeByte(uint8_t);

  /// Write one word and increment the write index. Returns true if success, returns false if error
  inline bool writeWord(uint16_t); 
};


void DxlWriterBase::setBuffer(uint8_t* buffer, int buffer_size) {
  if (buffer) {
    buffer_ = buffer;
    buffer_size_ = buffer_size;
    writable_size_ = 0;
  }
};

bool DxlWriterBase::writeByte(uint8_t value) {
  bool success = false;
  if (idx_ < writable_size_) {
    buffer_[idx_++] = value;
    success = true;
  }
  return success;
}

bool DxlWriterBase::writeWord(uint16_t value) {
  bool success = false;
  if (idx_ < writable_size_ && (idx_ + 1) < writable_size_) {
    buffer_[idx_++] = static_cast<uint8_t>(value & 0xff);
    buffer_[idx_++] = static_cast<uint8_t>(value >> 8);
  }
  return success;
}


#endif  // DXL_WRITER_BASE_H_