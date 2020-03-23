#include "DxlDriver.h"


DxlDriver::DxlDriver(
    DxlProtocolBase* protocol, 
    size_t rx_size, 
    size_t tx_size, 
    IfHwDxlDriverBase& driver) : DxlDriver() {
  protocol_ = protocol;
  rx_buf_ = new uint8_t[rx_size];
  tx_buf_ = new uint8_t[rx_size];
  rx_buf_size_ = rx_size;
  tx_buf_size_ = rx_size;
  driver_ = &driver;
  protocol_->setRxBuffer(rx_buf_, rx_buf_size_);
  protocol_->setTxBuffer(tx_buf_, tx_buf_size_);
} 

DxlDriver::Status DxlDriver::beginTransmission() {
  size_t i;
  size_t tx_size = protocol_->finalizeTx();
  expected_rx_size_ = protocol_->estimateRxSize();

  has_valid_rx_ = false;
  if (tx_size > 0) {
    driver_->beginTransmission(tx_buf_, tx_size, rx_buf_, rx_buf_size_, expected_rx_size_);
    status_ = Status::kTransmitting;
  } else {
    status_ = Status::kErrorInvalidTransmitData;
  }
  return status_;
}

DxlDriver::Status DxlDriver::poll() {
  size_t i;
  if(status_ == Status::kTransmitting) {
    if (driver_->txIsDone()) {
      status_ = kReceiving;
    }
  }
  if(status_ == Status::kReceiving && expected_rx_size_ > 0)
    if (driver_->available() >= expected_rx_size_) {
      // more bytes to rx buffer
      for (i=0; i<expected_rx_size_; i++) {
        rx_buf_[i] = driver_->read();
      }
      // burn extra characters
      while (driver_->available()) {
        driver_->read();
      }
      // check if valid
      if (protocol_->beginRxRead()) {
        status_ = kDone;
      } else {
      status_ = kErrorInvalidReceiveData;
    }
  }
  if (status_ == Status::kReceiving) {
    if (driver_->isTimeout()) {
      if (expected_rx_size_ == 0) {  
        // there was no expected response. so consider done!
        status_ = kDone;
      } else {
        // try to read data that was received.
        i = 0;
        while (driver_->available() && i < rx_buf_size_) {
          rx_buf_[i] = driver_->read();
        }
        if (protocol_->beginRxRead()) {
          status_ = kDone;
        } else {
          status_ = kErrorTimeout;
        }
      }
    }
  }
  return status_;
}


