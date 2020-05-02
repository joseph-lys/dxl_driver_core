#include "DxlProtocolV1.h"


uint8_t calcChkSum(uint8_t* buffer, size_t size) {
  uint8_t x = 0;
  for (size_t i=0; i<size; i++) {
    x += buffer[i];
  }
  return ~x;
}

bool DxlProtocolV1::beginRxRead() {
  bool success = false;
  size_t size = 0;
  uint8_t chksum;

  rx_idx_ = 0;
  rx_readable_size_ = 0;

  if (rx_buf_ == nullptr) {
    // this is an error
  } else if (rx_buf_[0] != 0xff && rx_buf_[1] != 0xff) {
    // this is an error
  } else if (rx_buf_[2] != tx_buf_[2]) {
    // this is an error, mismatched response
  } else if (rx_buf_[2] == 0xff) {
    // this is an error
  } else {
    size = static_cast<size_t>(rx_buf_[Reg::kSize]);
    if (size < 1) {
      // this is an error, not enough bytes
    } else if (size <= rx_buf_size_ && (size + 4) <= rx_buf_size_) {
      chksum = calcChkSum(&rx_buf_[Reg::kId], size + 1);
      if (chksum == rx_buf_[size + 3]) {
        success = true;
        rx_idx_ = 5;
        rx_readable_size_ = size + 3;
      }
    }
  }
  return success;
}

uint8_t DxlProtocolV1::getRxId() {
  uint8_t value = 0xff;
  if (rx_readable_size_ >= 2) {
    value = rx_buf_[2];
  }
  return value;
}

size_t DxlProtocolV1::getRxSize() {
  uint8_t value = 0xff;
  if (rx_readable_size_ >= 2) {
    value = static_cast<size_t>(rx_buf_[4]);
  }
  return value;
}

uint8_t DxlProtocolV1::getRxStatusByte() {
  uint8_t value = 0xff;
  if (rx_readable_size_ >= 2) {
    value = rx_buf_[Reg::kStatus];
  }
  return value;
}

size_t DxlProtocolV1::estimateRxSize() {
  size_t est = 0;
  if (tx_writable_size_ != tx_idx_ || tx_writable_size_==0) {
    est = 0;
  } else if (tx_buf_[Reg::kId] == Ins::kBroadcastId) {
    est = 0;
  } else if (tx_buf_[Reg::kIns] == Ins::kSyncWrite) {
    est = 0;
  } else if (tx_buf_[Reg::kIns]==Ins::kPing) {
    est = 6;
  } else if (tx_buf_[Reg::kIns]==Ins::kWrite) {
    est = 6;
  } else if (tx_buf_[Reg::kIns]==Ins::kRead) {
    est = 6 + tx_buf_[6];
  }
  return est;
}


size_t DxlProtocolV1::finalizeTx() {
  uint8_t chksum = 0;
  if (tx_idx_ < 3) {
    // this is an error.
    tx_idx_ = 0;
    tx_writable_size_ = 0;
  } else if (tx_idx_ <= tx_writable_size_) {
    tx_buf_[Reg::kSize] = static_cast<uint8_t>(tx_idx_ - 3);
    chksum = calcChkSum(&tx_buf_[2], tx_idx_ - 2);
    tx_buf_[tx_idx_++] = chksum;
    tx_writable_size_ = tx_idx_;  // prevent further changes
  }
  return tx_idx_;
}

void DxlProtocolV1::initTxData(uint8_t id, uint8_t ins_code) {
  tx_buf_[0] = 0xff;
  tx_buf_[1] = 0xff;
  tx_buf_[Reg::kId] = id;
  tx_buf_[Reg::kIns] = ins_code;
  tx_idx_ = 5;
  tx_writable_size_ = tx_buf_size_ - 1;
}
