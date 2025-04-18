#pragma once

//-------------------------------------------------------------//
//   Lazy Import Resolving									   //
//															   //
//	Credits:												   //
//															   //
//		namazso - Explaining the concept in this neat post:	   //
//		https://www.unknowncheats.me/forum/1900827-post61.html //
//															   //
//		filsinger@github - FNV-1 Hash stuff:				   //
//      https://gist.github.com/filsinger/1255697/21762ea83a2d3c17561c8e6a29f44249a4626f9e
//															   //
//		Some Random Website - _LDR_MODULE Structure and		   //
//			explanation of the PEB Stuff  (Didn't find it)	   //
//															   //
//		Me(T0b1) - Implementation							   //
//-------------------------------------------------------------//

//Includes
#include <iostream>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winternl.h>
#include <intrin.h>

#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH 2
#define DLL_THREAD_DETACH 3
#define DLL_PROCESS_DETACH 0

#define M_PI 3.14159265358979323846
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308
#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F
#define INT_MAX 2147483647
#define INT_MIN (-2147483647 - 1)
#define INT32_MIN (-2147483647i32 - 1)

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int* PUINT;

typedef unsigned __int64   uintptr_t;
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned __int64   size_t;

typedef signed char        int_least8_t;
typedef short              int_least16_t;
typedef int                int_least32_t;
typedef long long          int_least64_t;
typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned int       uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char        int_fast8_t;
typedef int                int_fast16_t;
typedef int                int_fast32_t;
typedef long long          int_fast64_t;
typedef unsigned char      uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned int       uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;
typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef long long          intmax_t;
typedef unsigned long long uintmax_t;
typedef void* PVOID;
typedef unsigned long ULONG;

typedef char* PCHAR;

typedef uint64_t ULONG_PTR;

typedef struct _ImageFileHeader
{
    WORD Machine;
    WORD NumberOfSections;
    DWORD TimeDateStamp;
    DWORD PointerToSymbolTable;
    DWORD NumberOfSymbols;
    WORD SizeOfOptionalHeader;
    WORD Characteristics;
} ImageFileHeader, * PImageFileHeader;

struct ImageDataDirectory
{
    unsigned long virtualAddress;
    unsigned long size;
};

typedef struct _ImageOptionalHeader64
{
    WORD                 Magic;
    BYTE                 MajorLinkerVersion;
    BYTE                 MinorLinkerVersion;
    DWORD                SizeOfCode;
    DWORD                SizeOfInitializedData;
    DWORD                SizeOfUninitializedData;
    DWORD                AddressOfEntryPoint;
    DWORD                BaseOfCode;
    uintptr_t            ImageBase;
    DWORD                SectionAlignment;
    DWORD                FileAlignment;
    WORD                 MajorOperatingSystemVersion;
    WORD                 MinorOperatingSystemVersion;
    WORD                 MajorImageVersion;
    WORD                 MinorImageVersion;
    WORD                 MajorSubsystemVersion;
    WORD                 MinorSubsystemVersion;
    DWORD                Win32VersionValue;
    DWORD                SizeOfImage;
    DWORD                SizeOfHeaders;
    DWORD                CheckSum;
    WORD                 Subsystem;
    WORD                 DllCharacteristics;
    uintptr_t            SizeOfStackReserve;
    uintptr_t            SizeOfStackCommit;
    uintptr_t            SizeOfHeapReserve;
    uintptr_t            SizeOfHeapCommit;
    DWORD                LoaderFlags;
    DWORD                NumberOfRvaAndSizes;
    ImageDataDirectory datadirectory[16];
} ImageOptionalHeader64, * PImageOptionalHeader64;

//typedef struct _IMAGE_DOS_HEADER
//{
//    WORD e_magic;
//    WORD e_cblp;
//    WORD e_cp;
//    WORD e_crlc;
//    WORD e_cparhdr;
//    WORD e_minalloc;
//    WORD e_maxalloc;
//    WORD e_ss;
//    WORD e_sp;
//    WORD e_csum;
//    WORD e_ip;
//    WORD e_cs;
//    WORD e_lfarlc;
//    WORD e_ovno;
//    WORD e_res[4];
//    WORD e_oemid;
//    WORD e_oeminfo;
//    WORD e_res2[10];
//    long e_lfanew;
//} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;


//Helpers
#define toStr(x) #x
#define FNV(x) IFH::hash::fnv1a<uint32_t>::hash(x)

// Search all Modules until Function found
#define IFH1(fn) IFH::GetProcAddr<decltype(&fn), 0, FNV(toStr(fn)), IFH::hash::strlen_c(toStr(fn))>() 

// Search for function in specific Module
#define IFH2(mod, fn) IFH::GetProcAddr<decltype(&fn), FNV(mod), FNV(toStr(fn)), IFH::hash::strlen_c(toStr(fn))>() 

//Overloading
#define IFH_GET_MACRO(_1,_2,NAME,...) NAME
#define IFH_EXPAND(x) x
#define IFH(...) IFH_EXPAND(IFH_GET_MACRO(__VA_ARGS__, IFH2, IFH1)(__VA_ARGS__))

namespace IFH {

#if defined(_WIN64) //If your IDE/Compiler doesn't define this just use something else to differentiate 64 bit from 32 bit
#define IFH_64
#endif

    //String Hashing
    namespace hash
    {
        template <typename S> struct fnv_internal;
        template <typename S> struct fnv1a;

        template <> struct fnv_internal<uint32_t>
        {
            constexpr static uint32_t default_offset_basis = 0x811C9DC5;
            constexpr static uint32_t prime = 0x01000193;
        };

        template <> struct fnv1a<uint32_t> : public fnv_internal<uint32_t>
        {
            constexpr static inline uint32_t hash(char const* const aString, const uint32_t val = default_offset_basis)
            {
                return (aString[0] == '\0') ? val : hash(&aString[1], (val ^ uint32_t(aString[0])) * prime);
            }

            constexpr static inline uint32_t hash(wchar_t const* const aString, const uint32_t val = default_offset_basis)
            {
                return (aString[0] == L'\0') ? val : hash(&aString[1], (val ^ uint32_t(aString[0])) * prime);
            }
        };

        constexpr size_t strlen_c(const char* str) {
            return *str ? 1 + strlen_c(str + 1) : 0;
        }
    }


    typedef struct _LDR_MODULE {

        LIST_ENTRY              InLoadOrderModuleList;
        LIST_ENTRY              InMemoryOrderModuleList;
        LIST_ENTRY              InInitializationOrderModuleList;
        PVOID                   BaseAddress;
        PVOID                   EntryPoint;
        ULONG                   SizeOfImage;
        UNICODE_STRING          FullDllName;
        UNICODE_STRING          BaseDllName;
        ULONG                   Flags;
        SHORT                   LoadCount;
        SHORT                   TlsIndex;
        LIST_ENTRY              HashTableEntry;
        ULONG                   TimeDateStamp;

    } LDR_MODULE, * PLDR_MODULE;

    template <uint32_t hash>
    constexpr void* GetFn(ULONG64 base) {

        auto DOSH = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
        auto NTH = reinterpret_cast<PIMAGE_NT_HEADERS>((reinterpret_cast<PBYTE>(DOSH) + DOSH->e_lfanew));

        auto EATA = NTH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
        auto EAT = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(EATA + base);

        for (DWORD i = 0; i < EAT->NumberOfFunctions; i++) {
            PULONG FUNCNAME = 0;

            if (i >= EAT->NumberOfNames) {
                break; //Prevent crashes
            }

            FUNCNAME = reinterpret_cast<PULONG>(base + EAT->AddressOfNames + (sizeof(ULONG) * i));

            auto fName = reinterpret_cast<char*>(base + *FUNCNAME);

            if (!fName)
                continue;

            if (FNV(fName) == hash) {
                auto AONP = *reinterpret_cast<PWORD>(base + (EAT->AddressOfNameOrdinals + (i * sizeof(WORD))));
                auto AOF = reinterpret_cast<PULONG>(base + (EAT->AddressOfFunctions + (sizeof(ULONG) * AONP)));

                return reinterpret_cast<void*>(base + *AOF);
            }

        }

        return nullptr;

    }

    template <typename Fn = void*, unsigned int modHash, unsigned int hash, unsigned int len>
    constexpr Fn GetProcAddr() {

        static Fn fn = nullptr;
        if (fn)
            return fn;

        static PPEB peb = nullptr;

        if (!peb) {
#ifdef IFH_64
            PTEB tebPtr = reinterpret_cast<PTEB>(__readgsqword(0x30));
            peb = tebPtr->ProcessEnvironmentBlock;
#else
            peb = reinterpret_cast<PPEB>(__readfsdword(0x30));
#endif
        }

        if (!peb)
            return nullptr;

        auto ldr = peb->Ldr;
        auto p = static_cast<PLDR_MODULE>(ldr->Reserved2[1]);

        while (p->BaseAddress) {
            std::wstring fullname = p->FullDllName.Buffer;
            std::transform(fullname.begin(), fullname.end(), fullname.begin(), ::tolower);

            auto base = reinterpret_cast<ULONG64>(p->BaseAddress);
            std::wstring str = fullname.substr(fullname.length() - len + 1);

            if (modHash == 0 || modHash == FNV(str.c_str())) {
                if (fullname.substr(fullname.length() - 4) != L".exe") {
                    auto FN = GetFn<hash>(base);
                    if (FN) {
                        fn = static_cast<Fn>(FN);
                        return fn;
                    }
                }
            }

            p = reinterpret_cast<PLDR_MODULE>(p->InLoadOrderModuleList.Flink);
        }
        return nullptr;
    }

}

/*
 * Copyright 2018-2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 // documentation is available at https://github.com/JustasMasiulis/lazy_importer

#ifndef LAZY_IMPORTER_HPP
#define LAZY_IMPORTER_HPP

#define lazy(name) \
    ::li::detail::lazy_function<::li::detail::khash(#name), decltype(&name)>()

#define LI_FN_DEF(name) ::li::detail::lazy_function<::li::detail::khash(#name), name>()

#define LI_MODULE(name) ::li::detail::lazy_module<::li::detail::khash(name)>()

// NOTE only std::forward is used from this header.
// If there is a need to eliminate this dependency the function itself is very small.

#include <utility>
#include <cstddef>
#include <intrin.h>

#ifndef LAZY_IMPORTER_NO_FORCEINLINE
#if defined(_MSC_VER)
#define LAZY_IMPORTER_FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define LAZY_IMPORTER_FORCEINLINE inline __attribute__((__always_inline__))
#else
#define LAZY_IMPORTER_FORCEINLINE inline
#endif
#else
#define LAZY_IMPORTER_FORCEINLINE inline
#endif

#ifdef LAZY_IMPORTER_CASE_INSENSITIVE
#define LAZY_IMPORTER_TOLOWER(c) (c >= 'A' && c <= 'Z' ? (c | (1 << 5)) : c)
#else
#define LAZY_IMPORTER_TOLOWER(c) (c)
#endif

namespace li {
    namespace detail {

        template<class First, class Second>
        struct pair {
            First  first;
            Second second;
        };

        namespace win {

            struct LIST_ENTRY_T {
                const char* Flink;
                const char* Blink;
            };

            struct UNICODE_STRING_T {
                unsigned short Length;
                unsigned short MaximumLength;
                wchar_t* Buffer;
            };

            struct PEB_LDR_DATA_T {
                unsigned long Length;
                unsigned long Initialized;
                const char* SsHandle;
                LIST_ENTRY_T  InLoadOrderModuleList;
            };

            struct PEB_T {
                unsigned char   Reserved1[2];
                unsigned char   BeingDebugged;
                unsigned char   Reserved2[1];
                const char* Reserved3[2];
                PEB_LDR_DATA_T* Ldr;
            };

            struct LDR_DATA_TABLE_ENTRY_T {
                LIST_ENTRY_T InLoadOrderLinks;
                LIST_ENTRY_T InMemoryOrderLinks;
                LIST_ENTRY_T InInitializationOrderLinks;
                const char* DllBase;
                const char* EntryPoint;
                union {
                    unsigned long SizeOfImage;
                    const char* _dummy;
                };
                UNICODE_STRING_T FullDllName;
                UNICODE_STRING_T BaseDllName;

                LAZY_IMPORTER_FORCEINLINE const LDR_DATA_TABLE_ENTRY_T*
                    load_order_next() const noexcept
                {
                    return reinterpret_cast<const LDR_DATA_TABLE_ENTRY_T*>(
                        InLoadOrderLinks.Flink);
                }
            };

            struct IMAGE_DOS_HEADER { // DOS .EXE header
                unsigned short e_magic; // Magic number
                unsigned short e_cblp; // Bytes on last page of file
                unsigned short e_cp; // Pages in file
                unsigned short e_crlc; // Relocations
                unsigned short e_cparhdr; // Size of header in paragraphs
                unsigned short e_minalloc; // Minimum extra paragraphs needed
                unsigned short e_maxalloc; // Maximum extra paragraphs needed
                unsigned short e_ss; // Initial (relative) SS value
                unsigned short e_sp; // Initial SP value
                unsigned short e_csum; // Checksum
                unsigned short e_ip; // Initial IP value
                unsigned short e_cs; // Initial (relative) CS value
                unsigned short e_lfarlc; // File address of relocation table
                unsigned short e_ovno; // Overlay number
                unsigned short e_res[4]; // Reserved words
                unsigned short e_oemid; // OEM identifier (for e_oeminfo)
                unsigned short e_oeminfo; // OEM information; e_oemid specific
                unsigned short e_res2[10]; // Reserved words
                long           e_lfanew; // File address of new exe header
            };

            struct IMAGE_FILE_HEADER {
                unsigned short Machine;
                unsigned short NumberOfSections;
                unsigned long  TimeDateStamp;
                unsigned long  PointerToSymbolTable;
                unsigned long  NumberOfSymbols;
                unsigned short SizeOfOptionalHeader;
                unsigned short Characteristics;
            };

            struct IMAGE_EXPORT_DIRECTORY {
                unsigned long  Characteristics;
                unsigned long  TimeDateStamp;
                unsigned short MajorVersion;
                unsigned short MinorVersion;
                unsigned long  Name;
                unsigned long  Base;
                unsigned long  NumberOfFunctions;
                unsigned long  NumberOfNames;
                unsigned long  AddressOfFunctions; // RVA from base of image
                unsigned long  AddressOfNames; // RVA from base of image
                unsigned long  AddressOfNameOrdinals; // RVA from base of image
            };

            struct IMAGE_DATA_DIRECTORY {
                unsigned long VirtualAddress;
                unsigned long Size;
            };

            struct IMAGE_OPTIONAL_HEADER64 {
                unsigned short       Magic;
                unsigned char        MajorLinkerVersion;
                unsigned char        MinorLinkerVersion;
                unsigned long        SizeOfCode;
                unsigned long        SizeOfInitializedData;
                unsigned long        SizeOfUninitializedData;
                unsigned long        AddressOfEntryPoint;
                unsigned long        BaseOfCode;
                unsigned long long   ImageBase;
                unsigned long        SectionAlignment;
                unsigned long        FileAlignment;
                unsigned short       MajorOperatingSystemVersion;
                unsigned short       MinorOperatingSystemVersion;
                unsigned short       MajorImageVersion;
                unsigned short       MinorImageVersion;
                unsigned short       MajorSubsystemVersion;
                unsigned short       MinorSubsystemVersion;
                unsigned long        Win32VersionValue;
                unsigned long        SizeOfImage;
                unsigned long        SizeOfHeaders;
                unsigned long        CheckSum;
                unsigned short       Subsystem;
                unsigned short       DllCharacteristics;
                unsigned long long   SizeOfStackReserve;
                unsigned long long   SizeOfStackCommit;
                unsigned long long   SizeOfHeapReserve;
                unsigned long long   SizeOfHeapCommit;
                unsigned long        LoaderFlags;
                unsigned long        NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory[16];
            };

            struct IMAGE_OPTIONAL_HEADER32 {
                unsigned short       Magic;
                unsigned char        MajorLinkerVersion;
                unsigned char        MinorLinkerVersion;
                unsigned long        SizeOfCode;
                unsigned long        SizeOfInitializedData;
                unsigned long        SizeOfUninitializedData;
                unsigned long        AddressOfEntryPoint;
                unsigned long        BaseOfCode;
                unsigned long        BaseOfData;
                unsigned long        ImageBase;
                unsigned long        SectionAlignment;
                unsigned long        FileAlignment;
                unsigned short       MajorOperatingSystemVersion;
                unsigned short       MinorOperatingSystemVersion;
                unsigned short       MajorImageVersion;
                unsigned short       MinorImageVersion;
                unsigned short       MajorSubsystemVersion;
                unsigned short       MinorSubsystemVersion;
                unsigned long        Win32VersionValue;
                unsigned long        SizeOfImage;
                unsigned long        SizeOfHeaders;
                unsigned long        CheckSum;
                unsigned short       Subsystem;
                unsigned short       DllCharacteristics;
                unsigned long        SizeOfStackReserve;
                unsigned long        SizeOfStackCommit;
                unsigned long        SizeOfHeapReserve;
                unsigned long        SizeOfHeapCommit;
                unsigned long        LoaderFlags;
                unsigned long        NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory[16];
            };

            struct IMAGE_NT_HEADERS {
                unsigned long     Signature;
                IMAGE_FILE_HEADER FileHeader;
#ifdef _WIN64
                IMAGE_OPTIONAL_HEADER64 OptionalHeader;
#else
                IMAGE_OPTIONAL_HEADER32 OptionalHeader;
#endif
            };

        } // namespace win

        // hashing stuff
        struct hash_t {
            using value_type = unsigned long;
            constexpr static value_type         offset = 2166136261;
            constexpr static value_type         prime = 16777619;
            constexpr static unsigned long long prime64 = prime;

            LAZY_IMPORTER_FORCEINLINE constexpr static value_type single(value_type value,
                char c) noexcept
            {
                return static_cast<hash_t::value_type>(
                    (value ^ LAZY_IMPORTER_TOLOWER(c)) *
                    static_cast<unsigned long long>(prime));
            }
        };

        template<class CharT = char>
        LAZY_IMPORTER_FORCEINLINE constexpr hash_t::value_type
            khash(const CharT* str, hash_t::value_type value = hash_t::offset) noexcept
        {
            return (*str ? khash(str + 1, hash_t::single(value, *str)) : value);
        }

        template<class CharT = char>
        LAZY_IMPORTER_FORCEINLINE hash_t::value_type hash(const CharT* str) noexcept
        {
            hash_t::value_type value = hash_t::offset;

            for (;;) {
                char c = *str++;
                if (!c)
                    return value;
                value = hash_t::single(value, c);
            }
        }

        LAZY_IMPORTER_FORCEINLINE hash_t::value_type hash(
            const win::UNICODE_STRING_T& str) noexcept
        {
            auto       first = str.Buffer;
            const auto last = first + (str.Length / sizeof(wchar_t));
            auto       value = hash_t::offset;
            for (; first != last; ++first)
                value = hash_t::single(value, static_cast<char>(*first));

            return value;
        }

        LAZY_IMPORTER_FORCEINLINE pair<hash_t::value_type, hash_t::value_type> hash_forwarded(
            const char* str) noexcept
        {
            pair<hash_t::value_type, hash_t::value_type> module_and_function{
                hash_t::offset, hash_t::offset
            };

            for (; *str != '.'; ++str)
                module_and_function.first = hash_t::single(module_and_function.first, *str);

            ++str;

            for (; *str; ++str)
                module_and_function.second = hash_t::single(module_and_function.second, *str);

            return module_and_function;
        }


        // some helper functions
        LAZY_IMPORTER_FORCEINLINE const win::PEB_T* peb() noexcept
        {
#if defined(_M_X64) || defined(__amd64__)
            return reinterpret_cast<const win::PEB_T*>(__readgsqword(0x60));
#elif defined(_M_IX86) || defined(__i386__)
            return reinterpret_cast<const win::PEB_T*>(__readfsdword(0x30));
#elif defined(_M_ARM) || defined(__arm__)
            return *reinterpret_cast<const win::PEB_T**>(_MoveFromCoprocessor(15, 0, 13, 0, 2) + 0x30);
#elif defined(_M_ARM64) || defined(__aarch64__)
            return *reinterpret_cast<const win::PEB_T**>(__getReg(18) + 0x60);
#elif defined(_M_IA64) || defined(__ia64__)
            return *reinterpret_cast<const win::PEB_T**>(static_cast<char*>(_rdteb()) + 0x60);
#else
#error Unsupported platform. Open an issue and I'll probably add support.
#endif
        }

        LAZY_IMPORTER_FORCEINLINE const win::PEB_LDR_DATA_T* ldr()
        {
            return reinterpret_cast<const win::PEB_LDR_DATA_T*>(peb()->Ldr);
        }

        LAZY_IMPORTER_FORCEINLINE const win::IMAGE_NT_HEADERS* nt_headers(
            const char* base) noexcept
        {
            return reinterpret_cast<const win::IMAGE_NT_HEADERS*>(
                base + reinterpret_cast<const win::IMAGE_DOS_HEADER*>(base)->e_lfanew);
        }

        LAZY_IMPORTER_FORCEINLINE const win::IMAGE_EXPORT_DIRECTORY* image_export_dir(
            const char* base) noexcept
        {
            return reinterpret_cast<const win::IMAGE_EXPORT_DIRECTORY*>(
                base + nt_headers(base)->OptionalHeader.DataDirectory->VirtualAddress);
        }

        LAZY_IMPORTER_FORCEINLINE const win::LDR_DATA_TABLE_ENTRY_T* ldr_data_entry() noexcept
        {
            return reinterpret_cast<const win::LDR_DATA_TABLE_ENTRY_T*>(
                ldr()->InLoadOrderModuleList.Flink);
        }

        struct exports_directory {
            const char* _base;
            const win::IMAGE_EXPORT_DIRECTORY* _ied;
            unsigned long                      _ied_size;

        public:
            using size_type = unsigned long;

            LAZY_IMPORTER_FORCEINLINE
                exports_directory(const char* base) noexcept : _base(base)
            {
                const auto ied_data_dir = nt_headers(base)->OptionalHeader.DataDirectory[0];
                _ied = reinterpret_cast<const win::IMAGE_EXPORT_DIRECTORY*>(
                    base + ied_data_dir.VirtualAddress);
                _ied_size = ied_data_dir.Size;
            }

            LAZY_IMPORTER_FORCEINLINE explicit operator bool() const noexcept
            {
                return reinterpret_cast<const char*>(_ied) != _base;
            }

            LAZY_IMPORTER_FORCEINLINE size_type size() const noexcept
            {
                return _ied->NumberOfNames;
            }

            LAZY_IMPORTER_FORCEINLINE const char* base() const noexcept { return _base; }
            LAZY_IMPORTER_FORCEINLINE const win::IMAGE_EXPORT_DIRECTORY* ied() const noexcept
            {
                return _ied;
            }

            LAZY_IMPORTER_FORCEINLINE const char* name(size_type index) const noexcept
            {
                return reinterpret_cast<const char*>(
                    _base + reinterpret_cast<const unsigned long*>(
                        _base + _ied->AddressOfNames)[index]);
            }

            LAZY_IMPORTER_FORCEINLINE const char* address(size_type index) const noexcept
            {
                const auto* const rva_table =
                    reinterpret_cast<const unsigned long*>(_base + _ied->AddressOfFunctions);

                const auto* const ord_table = reinterpret_cast<const unsigned short*>(
                    _base + _ied->AddressOfNameOrdinals);

                return _base + rva_table[ord_table[index]];
            }

            LAZY_IMPORTER_FORCEINLINE bool is_forwarded(
                const char* export_address) const noexcept
            {
                const auto ui_ied = reinterpret_cast<const char*>(_ied);
                return (export_address > ui_ied && export_address < ui_ied + _ied_size);
            }
        };

        struct safe_module_enumerator {
            using value_type = const detail::win::LDR_DATA_TABLE_ENTRY_T;
            value_type* value;
            value_type* head;

            LAZY_IMPORTER_FORCEINLINE safe_module_enumerator() noexcept
                : safe_module_enumerator(ldr_data_entry())
            {
            }

            LAZY_IMPORTER_FORCEINLINE
                safe_module_enumerator(const detail::win::LDR_DATA_TABLE_ENTRY_T* ldr) noexcept
                : value(ldr->load_order_next()), head(value)
            {
            }

            LAZY_IMPORTER_FORCEINLINE void reset() noexcept
            {
                value = head->load_order_next();
            }

            LAZY_IMPORTER_FORCEINLINE bool next() noexcept
            {
                value = value->load_order_next();

                return value != head && value->DllBase;
            }
        };

        struct unsafe_module_enumerator {
            using value_type = const detail::win::LDR_DATA_TABLE_ENTRY_T*;
            value_type value;

            LAZY_IMPORTER_FORCEINLINE unsafe_module_enumerator() noexcept
                : value(ldr_data_entry())
            {
            }

            LAZY_IMPORTER_FORCEINLINE void reset() noexcept { value = ldr_data_entry(); }

            LAZY_IMPORTER_FORCEINLINE bool next() noexcept
            {
                value = value->load_order_next();
                return true;
            }
        };

        // provides the cached functions which use Derive classes methods
        template<class Derived, class DefaultType = void*>
        class lazy_base {
        protected:
            // This function is needed because every templated function
            // with different args has its own static buffer
            LAZY_IMPORTER_FORCEINLINE static void*& _cache() noexcept
            {
                static void* value = nullptr;
                return value;
            }

        public:
            template<class T = DefaultType>
            LAZY_IMPORTER_FORCEINLINE static T safe() noexcept
            {
                return Derived::template get<T, safe_module_enumerator>();
            }

            template<class T = DefaultType, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static T cached() noexcept
            {
                auto& cached = _cache();
                if (!cached)
                    cached = Derived::template get<void*, Enum>();

                return (T)(cached);
            }

            template<class T = DefaultType>
            LAZY_IMPORTER_FORCEINLINE static T safe_cached() noexcept
            {
                return cached<T, safe_module_enumerator>();
            }
        };

        template<hash_t::value_type Hash>
        struct lazy_module : lazy_base<lazy_module<Hash>> {
            template<class T = void*, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static T get() noexcept
            {
                Enum e;
                do {
                    if (hash(e.value->BaseDllName) == Hash)
                        return (T)(e.value->DllBase);
                } while (e.next());
                return {};
            }

            template<class T = void*, class Ldr>
            LAZY_IMPORTER_FORCEINLINE static T in(Ldr ldr) noexcept
            {
                safe_module_enumerator e((const detail::win::LDR_DATA_TABLE_ENTRY_T*)(ldr));
                do {
                    if (hash(e.value->BaseDllName) == Hash)
                        return (T)(e.value->DllBase);
                } while (e.next());
                return {};
            }

            template<class T = void*, class Ldr>
            LAZY_IMPORTER_FORCEINLINE static T in_cached(Ldr ldr) noexcept
            {
                auto& cached = lazy_base<lazy_module<Hash>>::_cache();
                if (!cached)
                    cached = in(ldr);

                return (T)(cached);
            }
        };

        template<hash_t::value_type Hash, class T>
        struct lazy_function : lazy_base<lazy_function<Hash, T>, T> {
            using base_type = lazy_base<lazy_function<Hash, T>, T>;

            template<class... Args>
            LAZY_IMPORTER_FORCEINLINE decltype(auto) operator()(Args&&... args) const
            {
#ifndef LAZY_IMPORTER_CACHE_OPERATOR_PARENS
                return get()(std::forward<Args>(args)...);
#else
                return this->cached()(std::forward<Args>(args)...);
#endif
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F get() noexcept
            {
                // for backwards compatability.
                // Before 2.0 it was only possible to resolve forwarded exports when
                // this macro was enabled
#ifdef LAZY_IMPORTER_RESOLVE_FORWARDED_EXPORTS
                return forwarded<F, Enum>();
#else

                Enum e;

                do {
#ifdef LAZY_IMPORTER_HARDENED_MODULE_CHECKS
                    if (!e.value->DllBase || !e.value->FullDllName.Length)
                        continue;
#endif

                    const exports_directory exports(e.value->DllBase);

                    if (exports) {
                        auto export_index = exports.size();
                        while (export_index--)
                            if (hash(exports.name(export_index)) == Hash)
                                return (F)(exports.address(export_index));
                    }
                } while (e.next());
                return {};
#endif
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F forwarded() noexcept
            {
                detail::win::UNICODE_STRING_T name;
                hash_t::value_type            module_hash = 0;
                auto                          function_hash = Hash;

                Enum e;
                do {
                    name = e.value->BaseDllName;
                    name.Length -= 8; // get rid of .dll extension

                    if (!module_hash || hash(name) == module_hash) {
                        const exports_directory exports(e.value->DllBase);

                        if (exports) {
                            auto export_index = exports.size();
                            while (export_index--)
                                if (hash(exports.name(export_index)) == function_hash) {
                                    const auto addr = exports.address(export_index);

                                    if (exports.is_forwarded(addr)) {
                                        auto hashes = hash_forwarded(
                                            reinterpret_cast<const char*>(addr));

                                        function_hash = hashes.second;
                                        module_hash = hashes.first;

                                        e.reset();
                                        break;
                                    }
                                    return (F)(addr);
                                }
                        }
                    }
                } while (e.next());
                return {};
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_safe() noexcept
            {
                return forwarded<F, safe_module_enumerator>();
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_cached() noexcept
            {
                auto& value = base_type::_cache();
                if (!value)
                    value = forwarded<void*, Enum>();
                return (F)(value);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_safe_cached() noexcept
            {
                return forwarded_cached<F, safe_module_enumerator>();
            }

            template<class F = T, bool IsSafe = false, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in(Module m) noexcept
            {
                if (IsSafe && !m)
                    return {};

                const exports_directory exports((const char*)(m));
                if (IsSafe && !exports)
                    return {};

                for (unsigned long i{};; ++i) {
                    if (IsSafe && i == exports.size())
                        break;

                    if (hash(exports.name(i)) == Hash)
                        return (F)(exports.address(i));
                }
                return {};
            }

            template<class F = T, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_safe(Module m) noexcept
            {
                return in<F, true>(m);
            }

            template<class F = T, bool IsSafe = false, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_cached(Module m) noexcept
            {
                auto& value = base_type::_cache();
                if (!value)
                    value = in<void*, IsSafe>(m);
                return (F)(value);
            }

            template<class F = T, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_safe_cached(Module m) noexcept
            {
                return in_cached<F, true>(m);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt() noexcept
            {
                return in<F>(ldr_data_entry()->load_order_next()->DllBase);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_safe() noexcept
            {
                return in_safe<F>(ldr_data_entry()->load_order_next()->DllBase);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_cached() noexcept
            {
                return in_cached<F>(ldr_data_entry()->load_order_next()->DllBase);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_safe_cached() noexcept
            {
                return in_safe_cached<F>(ldr_data_entry()->load_order_next()->DllBase);
            }
        };

    }
} // namespace li::detail

#endif // include guard

typedef struct {
    uint32_t hashname;
    uint64_t address;
} cache_t;

std::vector<cache_t> api_cache;

constexpr __forceinline uint32_t hash(const char* str)
{
    uint32_t hash = 5381, c = 0;
    while (c = *str++)
        hash = hash * 33 + c;
    return hash;
}

__forceinline uint64_t find_export_by_hash(uint32_t hashname)
{
    for (cache_t cache : api_cache)
        if (cache.hashname == hashname)
            return cache.address;

    PPEB peb = (PPEB)((PTEB)NtCurrentTeb())->ProcessEnvironmentBlock;
    PLIST_ENTRY head = &peb->Ldr->InMemoryOrderModuleList;

    for (PLIST_ENTRY current = head->Flink; current != head; current = current->Flink)
    {
        PLDR_DATA_TABLE_ENTRY data_table = CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        uint64_t image_base = (uint64_t)data_table->DllBase;

        uint32_t export_rva = ((PIMAGE_NT_HEADERS64)(image_base + ((PIMAGE_DOS_HEADER)image_base)->e_lfanew))->OptionalHeader.DataDirectory[0].VirtualAddress;
        if (!export_rva)
            continue;

        IMAGE_EXPORT_DIRECTORY* exports = (IMAGE_EXPORT_DIRECTORY*)(image_base + export_rva);

        for (uint32_t i = 0; i < exports->NumberOfNames; ++i)
            if (hash((char*)(image_base + ((uint32_t*)(image_base + exports->AddressOfNames))[i])) == hashname)
            {
                uint64_t addr = image_base + ((uint32_t*)(image_base + exports->AddressOfFunctions))[((uint16_t*)(image_base + exports->AddressOfNameOrdinals))[i]];

                cache_t temp;
                temp.hashname = hashname;
                temp.address = addr;

                api_cache.push_back(temp);

                return addr;
            }

    }
}

#define CALL(x, ...) ((decltype(&x))([]() { constexpr uint32_t hashfunc = hash(#x); return find_export_by_hash(hashfunc); }()))(__VA_ARGS__)