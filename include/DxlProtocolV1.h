#ifndef DXL_PROTOCOL_V1_H_
#define DXL_PROTOCOL_V1_H_

#include "DxlReaderBase.h"
#include "DxlWriterBase.h"

#include <stddef.h>


class DxlReaderV1 : public DxlReaderBase {
 public:
  DxlReaderV1();
  uint8_t beginRead() override;
  uint8_t getId() override;
  size_t getSize() override;
};

class DxlWriterV1 : public DxlWriterBase {
 public:
  DxlWriterV1();
  size_t finalize() override;
  void setInstruction(uint8_t id, uint8_t ins_code) override;
};

namespace dxl_v1 {
  enum Ins : uint8_t {
    broadcast = 0xfe,
    ping = 0x01,
    read = 0x02,
    write = 0x03,
    sync_write = 0x83
  };
}  // namespace dxl_v1

#endif  // DXL_PROTOCOL_V1_H_