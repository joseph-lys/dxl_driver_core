#include "DxlProtocolV1.h"


uint8_t calcChkSum(uint8_t* buffer, size_t size) {
  uint8_t x = 0;
  for (size_t i=0; i<size; i++) {
    x += buffer[i];
  }
  return ~x;
}

DxlReaderV1::DxlReaderV1() 
: DxlReaderBase() {}

uint8_t DxlReaderV1::beginRead() {
  uint8_t status = 0x80;  // custom error
  size_t size = 0;
  uint8_t chksum;

  idx_ = 0;
  readable_size_ = 0;

  if (buffer_ == nullptr) {
    // this is an error
  } else if (buffer_[0] != 0xff && buffer_[1] != 0xff) {
    // this is an error
  } else if (buffer_[2] == 0xff) {
    // this is an error
  } else {
    size = static_cast<size_t>(buffer_[3]);
    if (size <= buffer_size_ && (size + 4) <= buffer_size_) {
      chksum = calcChkSum(&buffer_[2], size);
      if (chksum == buffer_[size + 2]) {
        status = buffer_[4];
          idx_ = 5;
          readable_size_ = size + 3;
      }
    }
  }
  return status;
}

uint8_t DxlReaderV1::getId() {
  return buffer_[2];
}

size_t DxlReaderV1::getSize() {
  return static_cast<size_t>(buffer_[4]);
}


size_t DxlWriterV1::finalize() {
  uint8_t chksum = 0;
  if (idx_ < 3) {
    // this is an error.
    idx_ = 0;
    writable_size_ = 0;
  } else if (idx_ < writable_size_) {
    buffer_[3] = static_cast<uint8_t>(idx_ - 3);
    chksum = calcChkSum(&buffer_[2], idx_ - 2);
    buffer_[idx_++] = chksum;
    writable_size_ = idx_;  // prevent further changes
  }
  return idx_;
}

void DxlWriterV1::setInstruction(uint8_t id, uint8_t ins_code) {
  buffer_[2] = id;
  buffer_[4] = ins_code;
  idx_ = 5;
  writable_size_ = buffer_size_ - 1;
}