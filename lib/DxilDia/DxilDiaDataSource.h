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

class DataSource : public IDiaDataSource {
private:
  DXC_MICROCOM_TM_REF_FIELDS()
  std::shared_ptr<llvm::Module> m_module;
  std::shared_ptr<llvm::LLVMContext> m_context;
  std::shared_ptr<llvm::DebugInfoFinder> m_finder;

public:
  DXC_MICROCOM_TM_ADDREF_RELEASE_IMPL()

  STDMETHODIMP QueryInterface(REFIID iid, void **ppvObject) {
    return DoBasicQueryInterface<IDiaDataSource>(this, iid, ppvObject);
  }

  DataSource(IMalloc *pMalloc);

  ~DataSource();

  STDMETHODIMP get_lastError(BSTR *pRetVal) override;

  STDMETHODIMP loadDataFromPdb(_In_ LPCOLESTR pdbPath) override { return ENotImpl(); }

  STDMETHODIMP loadAndValidateDataFromPdb(
    _In_ LPCOLESTR pdbPath,
    _In_ GUID *pcsig70,
    _In_ DWORD sig,
    _In_ DWORD age) override { return ENotImpl(); }

  STDMETHODIMP loadDataForExe(
    _In_ LPCOLESTR executable,
    _In_ LPCOLESTR searchPath,
    _In_ IUnknown *pCallback) override { return ENotImpl(); }

  STDMETHODIMP loadDataFromIStream(_In_ IStream *pIStream) override;

  STDMETHODIMP openSession(_COM_Outptr_ IDiaSession **ppSession) override;

  HRESULT STDMETHODCALLTYPE loadDataFromCodeViewInfo(
    _In_ LPCOLESTR executable,
    _In_ LPCOLESTR searchPath,
    _In_ DWORD cbCvInfo,
    _In_ BYTE *pbCvInfo,
    _In_ IUnknown *pCallback) override { return ENotImpl(); }

  HRESULT STDMETHODCALLTYPE loadDataFromMiscInfo(
    _In_ LPCOLESTR executable,
    _In_ LPCOLESTR searchPath,
    _In_ DWORD timeStampExe,
    _In_ DWORD timeStampDbg,
    _In_ DWORD sizeOfExe,
    _In_ DWORD cbMiscInfo,
    _In_ BYTE *pbMiscInfo,
    _In_ IUnknown *pCallback) override { return ENotImpl(); }
};

}  // namespace dxil_dia
