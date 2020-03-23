#ifndef DXL_PROTOCOL_V1_H_
#define DXL_PROTOCOL_V1_H_

#include "DxlProtocolBase.h"

#include <stdint.h>
#include <stddef.h>


class DxlProtocolV1 : public DxlProtocolBase {
 private:
  enum Reg : uint8_t {
    kId = 2,
    kSize = 3,
    kIns = 4,
    kStatus = 4,
  };
 public:
  enum Ins : uint8_t {
    kBroadcastId = 0xfe,
    kPing = 0x01,
    kRead = 0x02,
    kWrite = 0x03,
    kSyncWrite = 0x83
  };
  DxlProtocolV1() { }
  uint8_t getRxStatusByte() override;
  bool beginRxRead() override;
  uint8_t getRxId() override;
  size_t getRxSize() override;
  size_t estimateRxSize() override;

  size_t finalizeTx() override;
  void initTxData(uint8_t id, uint8_t ins_code) override;
}; 

#endif  // DXL_PROTOCOL_V1_H_