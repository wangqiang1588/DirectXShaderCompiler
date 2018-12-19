#pragma once
#include "dxc/Support/WinIncludes.h"

#include "dxc/Support/Global.h"
#include "dxc/Support/Unicode.h"
#include "llvm/ADT/StringRef.h"

namespace dxil_dia {

// Single program, single compiland allows for some simplifications.
static constexpr DWORD HlslProgramId = 1;
static constexpr DWORD HlslCompilandId = 2;
static constexpr DWORD HlslCompilandDetailsId = 3;
static constexpr DWORD HlslCompilandEnvFlagsId = 4;
static constexpr DWORD HlslCompilandEnvTargetId = 5;
static constexpr DWORD HlslCompilandEnvEntryId = 6;
static constexpr DWORD HlslCompilandEnvDefinesId = 7;
static constexpr DWORD HlslCompilandEnvArgumentsId = 8;

HRESULT ENotImpl();
HRESULT StringRefToBSTR(llvm::StringRef value, BSTR *pRetVal);
}  // namespace dxil_dia
