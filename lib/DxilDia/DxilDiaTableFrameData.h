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

class FrameDataTable : public impl::TableBase<IDiaEnumFrameData, IDiaFrameData> {
public:
  FrameDataTable(IMalloc *pMalloc, Session *pSession);

  // HLSL inlines functions for a program, so no data to return.
  STDMETHODIMP frameByRVA(
    /* [in] */ DWORD relativeVirtualAddress,
    /* [retval][out] */ IDiaFrameData **frame) override { return ENotImpl(); }

  STDMETHODIMP frameByVA(
    /* [in] */ ULONGLONG virtualAddress,
    /* [retval][out] */ IDiaFrameData **frame) override { return ENotImpl(); }

  HRESULT GetItem(DWORD index, IDiaFrameData **ppItem) override;
};

}  // namespace dxil_dia
