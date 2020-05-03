#ifndef DXL_DRIVER_H_
#define DXL_DRIVER_H_

#include "DxlProtocolBase.h"
#include "IfHwDxlDriverBase.h"

class DxlDriver {
 public:
  enum Status : int {
    kErrorInvalidTransmitData = -3,
    kErrorInvalidReceiveData = -2,
    kErrorTimeout = -1,
    kDone = 0,
    kTransmitting = 1,
    kReceiving = 2,
  };
 
 private:
  bool has_valid_rx_ = false;
  Status status_ = Status::kDone;
  uint8_t* rx_buf_ = nullptr;
  uint8_t* tx_buf_ = nullptr;
  size_t rx_buf_size_ = 0;
  size_t tx_buf_size_ = 0;
  size_t expected_rx_size_ = 0;
  DxlProtocolBase* protocol_ = nullptr;
  IfHwDxlDriverBase* driver_ = nullptr;
  
  DxlDriver() { }   // private default constructor
  /// Parameterized Constructor
  explicit DxlDriver(DxlProtocolBase* protocol, size_t rx_size, size_t tx_size, IfHwDxlDriverBase& driver);
  
 public:
  /// Static templated method to create a protocol to attach to driver.
  template <typename Protocol, size_t RxSize, size_t TxSize, typename Driver>
  static DxlDriver create(Driver& driver) {
    DxlProtocolBase* protocol = new Protocol();  // created driver will own the protocol instance
    return DxlDriver(protocol, RxSize, TxSize, static_cast<IfHwDxlDriverBase&>(driver));
  }

  DxlDriver(const DxlDriver& other) = delete;  // no copy constructor
  DxlDriver& operator=(const DxlDriver&) = delete;  // no copy
  DxlDriver(DxlDriver&&) = default;  // default move constructor
  DxlDriver& operator=(DxlDriver&&) = default;  // default copy constructor

  /// Destructor
  ~DxlDriver();

  /// Get the reply status byte, does not increment the read index
  inline uint8_t getRxStatusByte();
  
  /// Get the reply data's source Id, does not increment the read index
  inline uint8_t getRxId();

  /// Get one byte and increment the read index, returns 0xff if error
  inline uint8_t readRxByte();

  /// Get one byte and increment the read index, return 0xffff if error
  inline uint16_t readRxWord(); 

  /// Writes the Id and Instruction. Reset the write pointer to the start of writable data. 
  inline void setTxIns(uint8_t id, uint8_t ins_code);
  
  /// Write one byte and increment the write index. Returns true if success, returns false if error.
  inline bool writeTxByte(uint8_t);

  /// Write one word and increment the write index. Returns true if success, returns false if error
  inline bool writeTxWord(uint16_t); 

  /// Start transmission
  Status beginTransmission();
  
  /// poll for transmission status
  Status poll();
};

uint8_t DxlDriver::getRxStatusByte() {
  return protocol_->getRxStatusByte();
}

uint8_t DxlDriver::getRxId() {
  return protocol_->getRxId();
}

uint8_t DxlDriver::readRxByte() {
  return protocol_->readRxByte();
}

uint16_t DxlDriver::readRxWord() {
  return protocol_->readRxWord();
}

void DxlDriver::setTxIns(uint8_t id, uint8_t ins_code) {
  protocol_->initTxData(id, ins_code);
}

bool DxlDriver::writeTxByte(uint8_t value) {
  return protocol_->writeTxByte(value);
}

bool DxlDriver::writeTxWord(uint16_t value) {
  return protocol_->writeTxWord(value);
}

#endif  // DXL_DRIVER_H_