#include "DxilDiaTableSections.h"

#include "DxilDiaSession.h"

dxil_dia::SectionsTable::SectionsTable(IMalloc *pMalloc, Session *pSession)
  : impl::TableBase<IDiaEnumSectionContribs, IDiaSectionContrib>(pMalloc, pSession, Table::Kind::Sections) {
}

HRESULT dxil_dia::SectionsTable::GetItem(DWORD index, IDiaSectionContrib **ppItem) {
  *ppItem = nullptr;
  return E_FAIL;
}
