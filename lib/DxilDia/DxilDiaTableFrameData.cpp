#include "DxilDiaTableFrameData.h"

#include "DxilDiaSession.h"

dxil_dia::FrameDataTable::FrameDataTable(IMalloc *pMalloc, Session *pSession)
  : impl::TableBase<IDiaEnumFrameData, IDiaFrameData>(pMalloc, pSession, Table::Kind::FrameData) {
}

HRESULT dxil_dia::FrameDataTable::GetItem(DWORD index, IDiaFrameData **ppItem) {
  *ppItem = nullptr;
  return E_FAIL;
}