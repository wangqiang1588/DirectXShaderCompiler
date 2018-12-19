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

class InjectedSource : public IDiaInjectedSource {
private:
  DXC_MICROCOM_TM_REF_FIELDS()
  CComPtr<Session> m_pSession;
  DWORD m_index;

public:
  DXC_MICROCOM_TM_ADDREF_RELEASE_IMPL()
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject) {
    return DoBasicQueryInterface<IDiaInjectedSource>(this, iid, ppvObject);
  }

  InjectedSource(IMalloc *pMalloc, Session *pSession, DWORD index)
    : m_pMalloc(pMalloc), m_pSession(pSession), m_index(index) {}

  llvm::MDTuple *NameContent();
  llvm::StringRef Name();
  llvm::StringRef Content();

  STDMETHODIMP get_crc(
    /* [retval][out] */ DWORD *pRetVal) override { return ENotImpl(); }

  STDMETHODIMP get_length(_Out_ ULONGLONG *pRetVal) override;

  STDMETHODIMP get_filename(BSTR *pRetVal) override;

  STDMETHODIMP get_objectFilename(BSTR *pRetVal) override;

  STDMETHODIMP get_virtualFilename(BSTR *pRetVal) override;

  STDMETHODIMP get_sourceCompression(
    /* [retval][out] */ DWORD *pRetVal) override { return ENotImpl(); }

  STDMETHODIMP get_source(
    /* [in] */ DWORD cbData,
    /* [out] */ DWORD *pcbData,
    /* [size_is][out] */ BYTE *pbData) override;
};

class InjectedSourcesTable : public impl::TableBase<IDiaEnumInjectedSources,
                                                    IDiaInjectedSource> {
public:
  InjectedSourcesTable(IMalloc *pMalloc, Session *pSession);

  HRESULT GetItem(DWORD index, IDiaInjectedSource **ppItem) override;

  void Init(llvm::StringRef filename);

private:
  std::vector<unsigned> m_indexList;
};

}  // namespace dxil_dia
