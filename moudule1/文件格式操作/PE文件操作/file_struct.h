typedef struct _IMAGE_DOS_HEADER {   // DOS��.EXEͷ��
    WORD e_magic;       // ħ������
    WORD e_cblp;        // �ļ����ҳ���ֽ���
    WORD e_cp;          // �ļ�ҳ��
    WORD e_crlc;        // �ض���Ԫ�ظ���
    WORD e_cparhdr;     // ͷ���ߴ磬�Զ���Ϊ��λ
    WORD e_minalloc;    // �������С���Ӷ�
    WORD e_maxalloc;    // �������󸽼Ӷ�
    WORD e_ss;          // ��ʼ��SSֵ(���ƫ����)
    WORD e_sp;          // ��ʼ��SPֵ
    WORD e_csum;        // У���
    WORD e_ip;          // ��ʼ��IPֵ
    WORD e_cs;          // ��ʼ��CSֵ(���ƫ����)
    WORD e_lfarlc;      // �ط�����ļ���ַ
    WORD e_ovno;        // ���Ǻ�
    WORD e_res[4];      // ������
    WORD e_oemid;       // OEM��ʶ��(���e_oeminfo)
    WORD e_oeminfo;     // OEM��Ϣ
    WORD e_res2[10];    // ������
    LONG e_lfanew;      // ��exeͷ�����ļ���ַ
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;


typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD   VirtualAddress;
  DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
  
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES     16
typedef struct _IMAGE_OPTIONAL_HEADER {
  WORD    Magic;
  BYTE    MajorLinkerVersion;
  BYTE    MinorLinkerVersion;
  DWORD   SizeOfCode;
  DWORD   SizeOfInitializedData;
  DWORD   SizeOfUninitializedData;
  DWORD   AddressOfEntryPoint;
  DWORD   BaseOfCode;
  DWORD   BaseOfData;
  DWORD   ImageBase;
  DWORD   SectionAlignment;
  DWORD   FileAlignment;
  WORD    MajorOperatingSystemVersion;
  WORD    MinorOperatingSystemVersion;
  WORD    MajorImageVersion;
  WORD    MinorImageVersion;
  WORD    MajorSubsystemVersion;
  WORD    MinorSubsystemVersion;
  DWORD   Win32VersionValue;
  DWORD   SizeOfImage;
  DWORD   SizeOfHeaders;
  DWORD   CheckSum;
  WORD    Subsystem;
  WORD    DllCharacteristics;
  DWORD   SizeOfStackReserve;
  DWORD   SizeOfStackCommit;
  DWORD   SizeOfHeapReserve;
  DWORD   SizeOfHeapCommit;
  DWORD   LoaderFlags;
  DWORD   NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;


#define IMAGE_SIZEOF_FILE_HEADER 20
typedef struct _IMAGE_FILE_HEADER {
  WORD Machine;
  WORD NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD SizeOfOptionalHeader;
  WORD Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;


typedef struct _IMAGE_NT_HEADERS {
  DWORD Signature;
  IMAGE_FILE_HEADER FileHeader;
  IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;


#define IMAGE_SIZEOF_SHORT_NAME 8
typedef struct _IMAGE_SECTION_HEADER {
  BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
  union {
      DWORD   PhysicalAddress;
      DWORD   VirtualSize;
  } Misc;
  DWORD   VirtualAddress;
  DWORD   SizeOfRawData;
  DWORD   PointerToRawData;
  DWORD   PointerToRelocations;
  DWORD   PointerToLinenumbers;
  WORD    NumberOfRelocations;
  WORD    NumberOfLinenumbers;
  DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;


typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union {
        struct {
            DWORD NameOffset:31;
            DWORD NameIsString:1;
        };
        DWORD   Name;
        WORD    Id;
    };
    union {
        DWORD   OffsetToData;
        struct {
            DWORD   OffsetToDirectory:31;
            DWORD   DataIsDirectory:1;
        };
    };
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;


typedef struct _IMAGE_RESOURCE_DIRECTORY {
    DWORD Characteristics;
    DWORD TimeDateStamp;
    WORD MajorVersion;
    WORD MinorVersion;
    WORD NumberOfNamedEntries;
    WORD NumberOfIdEntries;
    IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;


typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
    WORD Length;
    WCHAR NameString[1];
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;


