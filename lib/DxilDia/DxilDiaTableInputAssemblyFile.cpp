#include "DxilDiaTableInputAssemblyFile.h"

#include "DxilDiaSession.h"

dxil_dia::InputAssemblyFilesTable::InputAssemblyFilesTable(IMalloc *pMalloc, Session *pSession)
  : impl::TableBase<IDiaEnumInputAssemblyFiles, IDiaInputAssemblyFile>(pMalloc, pSession, Table::Kind::InputAssemblyFile) {

}

HRESULT dxil_dia::InputAssemblyFilesTable::GetItem(DWORD index, IDiaInputAssemblyFile **ppItem) {
  *ppItem = nullptr;
  return E_FAIL;
}