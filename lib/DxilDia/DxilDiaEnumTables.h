#pragma once

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

#include "dxc/Support/WinIncludes.h"
#include "dxc/DxilContainer/DxilContainer.h"
#include "dxc/DXIL/DxilShaderModel.h"
#include "dxc/DXIL/DxilMetadataHelper.h"
#include "dxc/DXIL/DxilModule.h"
#include "dxc/DXIL/DxilUtil.h"
#include "dxc/Support/Global.h"
#ifdef _WIN32
#include "dia2.h"
#endif

#include "dxc/dxcapi.internal.h"

#include "dxc/Support/Global.h"
#include "dxc/Support/Unicode.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MSFileSystem.h"
#include "dxc/Support/microcom.h"
#include "dxc/Support/FileIOHelper.h"
#include "dxc/Support/dxcapi.impl.h"
#include <algorithm>
#include <array>
#ifdef _WIN32
#include <comdef.h>
#endif

#include "DxilDia.h"
#include "DxilDiaTable.h"

namespace dxil_dia {

class Session;

class EnumTables : public IDiaEnumTables {
private:
  DXC_MICROCOM_TM_REF_FIELDS()
protected:
  CComPtr<Session> m_pSession;
  unsigned m_next;
public:
  DXC_MICROCOM_TM_ADDREF_RELEASE_IMPL()

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject) {
    return DoBasicQueryInterface<IDiaEnumTables>(this, iid, ppvObject);
  }

  EnumTables(IMalloc *pMalloc, Session *pSession)
      : m_pMalloc(pMalloc), m_pSession(pSession), m_dwRef(0), m_next(0) {
    m_tables.fill(nullptr);
  }

  STDMETHODIMP get__NewEnum(
    /* [retval][out] */ IUnknown **pRetVal) override { return ENotImpl(); }

  STDMETHODIMP get_Count(_Out_ LONG *pRetVal) override;

  STDMETHODIMP Item(
    /* [in] */ VARIANT index,
    /* [retval][out] */ IDiaTable **table) override;

  STDMETHODIMP Next(
    ULONG celt,
    IDiaTable **rgelt,
    ULONG *pceltFetched) override;

  STDMETHODIMP Skip(
    /* [in] */ ULONG celt) override { return ENotImpl(); }

  STDMETHODIMP Reset(void) override;

  STDMETHODIMP Clone(
    /* [out] */ IDiaEnumTables **ppenum) override { return ENotImpl(); }

  static HRESULT Create(Session *pSession,
                        IDiaEnumTables **ppEnumTables);
private:
  std::array<CComPtr<IDiaTable>, (int)Table::LastKind+1> m_tables;
};

}  // namespace dxil_dia
