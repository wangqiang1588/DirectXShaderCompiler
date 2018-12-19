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

namespace dxil_dia {

class Session;

namespace Table {
enum class Kind {
  Symbols,
  SourceFiles,
  LineNumbers,
  Sections,
  SegmentMap,
  InjectedSource,
  FrameData,
  InputAssemblyFile
};
static constexpr Kind FirstKind = Kind::Symbols;
static constexpr Kind LastKind = Kind::InputAssemblyFile;

HRESULT Create(
    /* [in] */ Session *pSession,
    /* [in] */ Kind kind,
    /* [out] */ IDiaTable **ppTable);
}  // namespace Table

namespace impl {

template<typename T, typename TItem>
class TableBase : public IDiaTable, public T {
protected:
  static constexpr LPCWSTR TableNames[] = {
    L"Symbols",
    L"SourceFiles",
    L"LineNumbers",
    L"Sections",
    L"SegmentMap",
    L"InjectedSource",
    L"FrameData",
    L"InputAssemblyFiles"
  };

  DXC_MICROCOM_TM_REF_FIELDS()
  CComPtr<Session> m_pSession;
  unsigned m_next;
  unsigned m_count;
  Table::Kind m_kind;
public:
  DXC_MICROCOM_TM_ADDREF_RELEASE_IMPL()

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject) {
    return DoBasicQueryInterface<IDiaTable, T, IEnumUnknown>(this, iid, ppvObject);
  }

  TableBase(IMalloc *pMalloc, Session *pSession, Table::Kind kind) {
    m_pMalloc = pMalloc;
    m_pSession = pSession;
    m_kind = kind;
    m_next = 0;
    m_count = 0;
  }

  // IEnumUnknown implementation.
  STDMETHODIMP Next(
    _In_  ULONG celt,
    _Out_writes_to_(celt, *pceltFetched)  IUnknown **rgelt,
    _Out_opt_  ULONG *pceltFetched) override {
    DxcThreadMalloc TM(m_pMalloc);
    ULONG fetched = 0;
    while (fetched < celt && m_next < m_count) {
      HRESULT hr = Item(m_next, &rgelt[fetched]);
      if (FAILED(hr)) {
        return hr; // TODO: this leaks prior tables.
      }
      ++m_next, ++fetched;
    }
    if (pceltFetched != nullptr)
      *pceltFetched = fetched;
    return (fetched == celt) ? S_OK : S_FALSE;
  }

  STDMETHODIMP Skip(ULONG celt) override {
    if (celt + m_next <= m_count) {
      m_next += celt;
      return S_OK;
    }
    return S_FALSE;
  }

  STDMETHODIMP Reset(void) override {
    m_next = 0;
    return S_OK;
  }

  STDMETHODIMP Clone(IEnumUnknown **ppenum) override {
    return ENotImpl();
  }

  // IDiaTable implementation.
  STDMETHODIMP get__NewEnum(IUnknown **pRetVal) override {
    return ENotImpl();
  }

  STDMETHODIMP get_name(BSTR *pRetVal) override {
    *pRetVal = SysAllocString(TableNames[(unsigned)m_kind]);
    return (*pRetVal) ? S_OK : E_OUTOFMEMORY;
  }

  STDMETHODIMP get_Count(_Out_ LONG *pRetVal) override {
    *pRetVal = m_count;
    return S_OK;
  }

  STDMETHODIMP Item(DWORD index, _COM_Outptr_ IUnknown **table) override {
    if (index >= m_count)
      return E_INVALIDARG;
    return GetItem(index, (TItem **)table);
  }

  // T implementation (partial).
  STDMETHODIMP Clone(_COM_Outptr_ T **ppenum) override {
    *ppenum = nullptr;
    return ENotImpl();
  }
  STDMETHODIMP Next(
    /* [in] */ ULONG celt,
    /* [out] */ TItem **rgelt,
    /* [out] */ ULONG *pceltFetched) override {
    DxcThreadMalloc TM(m_pMalloc);
    ULONG fetched = 0;
    while (fetched < celt && m_next < m_count) {
      HRESULT hr = GetItem(m_next, &rgelt[fetched]);
      if (FAILED(hr)) {
        return hr; // TODO: this leaks prior items.
      }
      ++m_next, ++fetched;
    }
    if (pceltFetched != nullptr)
      *pceltFetched = fetched;
    return (fetched == celt) ? S_OK : S_FALSE;
  }
  STDMETHODIMP Item(
    /* [in] */ DWORD index,
    /* [retval][out] */ TItem **ppItem) override {
    DxcThreadMalloc TM(m_pMalloc);
    if (index >= m_count)
      return E_INVALIDARG;
    return GetItem(index, ppItem);
  }

  virtual HRESULT GetItem(DWORD index, TItem **ppItem) {
    UNREFERENCED_PARAMETER(index);
    *ppItem = nullptr;
    return ENotImpl();
  }
};
}  // namespace impl
}  // namespace dxil_dia
