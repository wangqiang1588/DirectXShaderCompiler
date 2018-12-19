#include "DxilDia.h"

#include <Windows.h>

#include "dxc/Support/Global.h"
#include "dxc/Support/Unicode.h"
#include "llvm/ADT/StringRef.h"

HRESULT dxil_dia::StringRefToBSTR(llvm::StringRef value, BSTR *pRetVal) {
  try {
    wchar_t *wide;
    size_t sideSize;
    if (!Unicode::UTF8BufferToUTF16Buffer(value.data(), value.size(), &wide,
      &sideSize))
      return E_FAIL;
    *pRetVal = SysAllocString(wide);
    delete[] wide;
  }
  CATCH_CPP_RETURN_HRESULT();
  return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Misc helpers.

HRESULT dxil_dia::ENotImpl() {
  return E_NOTIMPL;
}