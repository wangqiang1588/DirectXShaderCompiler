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

class SourceFile : public IDiaSourceFile {
private:
  DXC_MICROCOM_TM_REF_FIELDS()
  CComPtr<Session> m_pSession;
  DWORD m_index;

public:
  DXC_MICROCOM_TM_ADDREF_RELEASE_IMPL()
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject) {
    return DoBasicQueryInterface<IDiaSourceFile>(this, iid, ppvObject);
  }

  SourceFile(IMalloc *pMalloc, Session *pSession, DWORD index);

  llvm::MDTuple *NameContent() const;

  llvm::StringRef Name() const;

  STDMETHODIMP get_uniqueId(
    /* [retval][out] */ DWORD *pRetVal) override;

  STDMETHODIMP get_fileName(
    /* [retval][out] */ BSTR *pRetVal) override;

  STDMETHODIMP get_checksumType(
    /* [retval][out] */ DWORD *pRetVal) override { return ENotImpl(); }

  STDMETHODIMP get_compilands(
    /* [retval][out] */ IDiaEnumSymbols **pRetVal) override { return ENotImpl(); }

  STDMETHODIMP get_checksum(
    /* [in] */ DWORD cbData,
    /* [out] */ DWORD *pcbData,
    /* [size_is][out] */ BYTE *pbData) override { return ENotImpl(); }
};

class SourceFilesTable : public impl::TableBase<IDiaEnumSourceFiles, IDiaSourceFile> {
public:
  SourceFilesTable(IMalloc *pMalloc, Session *pSession);

  HRESULT GetItem(DWORD index, IDiaSourceFile **ppItem) override;

private:
  std::vector<CComPtr<IDiaSourceFile>> m_items;
};

}  // namespace dxil_dia
