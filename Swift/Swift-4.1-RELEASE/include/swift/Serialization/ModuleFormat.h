//===--- ModuleFormat.h - The internals of serialized modules ---*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Contains various constants and helper types to deal with serialized
/// modules.
///
//===----------------------------------------------------------------------===//

#ifndef SWIFT_SERIALIZATION_MODULEFORMAT_H
#define SWIFT_SERIALIZATION_MODULEFORMAT_H

#include "swift/AST/Decl.h"
#include "swift/AST/Types.h"
#include "llvm/Bitcode/RecordLayout.h"
#include "llvm/Bitcode/BitCodes.h"
#include "llvm/ADT/PointerEmbeddedInt.h"

namespace swift {
namespace serialization {

using llvm::PointerEmbeddedInt;
using llvm::BCArray;
using llvm::BCBlob;
using llvm::BCFixed;
using llvm::BCGenericRecordLayout;
using llvm::BCRecordLayout;
using llvm::BCVBR;

/// Magic number for serialized module files.
const unsigned char MODULE_SIGNATURE[] = { 0xE2, 0x9C, 0xA8, 0x0E };

/// Magic number for serialized documentation files.
const unsigned char MODULE_DOC_SIGNATURE[] = { 0xE2, 0x9C, 0xA8, 0x07 };

/// Serialized module format major version number.
///
/// Always 0 for Swift 1.x - 4.x.
const uint16_t VERSION_MAJOR = 0;

/// Serialized module format minor version number.
///
/// When the format changes IN ANY WAY, this number should be incremented.
/// To ensure that two separate changes don't silently get merged into one
/// in source control, you should also update the comment to briefly
/// describe what change you made. The content of this comment isn't important;
/// it just ensures a conflict if two people change the module format.
const uint16_t VERSION_MINOR = 392; // Private discriminators for type xrefs

using DeclIDField = BCFixed<31>;

// TypeID must be the same as DeclID because it is stored in the same way.
using TypeID = DeclID;
using TypeIDField = DeclIDField;

using TypeIDWithBitField = BCFixed<32>;

// IdentifierID must be the same as DeclID because it is stored in the same way.
using IdentifierID = DeclID;
using IdentifierIDField = DeclIDField;

// DeclContextID must be the same as DeclID because it is stored in the same way.
using DeclContextID = DeclID;
using DeclContextIDField = DeclIDField;

// NormalConformanceID must be the same as DeclID because it is stored
// in the same way.
using NormalConformanceID = DeclID;
using NormalConformanceIDField = DeclIDField;

// GenericSignatureID must be the same as DeclID because it is stored in the
// same way.
using GenericSignatureID = DeclID;
using GenericSignatureIDField = DeclIDField;

// GenericEnvironmentID must be the same as DeclID because it is stored in the
// same way.
using GenericEnvironmentID = DeclID;
using GenericEnvironmentIDField = DeclIDField;

// ModuleID must be the same as IdentifierID because it is stored the same way.
using ModuleID = IdentifierID;
using ModuleIDField = IdentifierIDField;

// SILLayoutID must be the same as DeclID because it is stored in the same way.
using SILLayoutID = DeclID;
using SILLayoutIDField = DeclIDField;

using BitOffset = PointerEmbeddedInt<unsigned, 31>;
using BitOffsetField = BCFixed<31>;

// CharOffset must be the same as BitOffset because it is stored in the
// same way.
using CharOffset = BitOffset;
using CharOffsetField = BitOffsetField;

using FileSizeField = BCVBR<16>;
using FileModTimeField = BCVBR<16>;

enum class StorageKind : uint8_t {
  Stored, StoredWithTrivialAccessors, StoredWithObservers,
  InheritedWithObservers,
  Computed, ComputedWithMutableAddress,
  Addressed, AddressedWithTrivialAccessors, AddressedWithObservers,
};
using StorageKindField = BCFixed<4>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class StaticSpellingKind : uint8_t {
  None = 0,
  KeywordStatic,
  KeywordClass,
};
using StaticSpellingKindField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class FunctionTypeRepresentation : uint8_t {
  Swift = 0,
  Block,
  Thin,
  CFunctionPointer,
};
using FunctionTypeRepresentationField = BCFixed<4>;

enum class ForeignErrorConventionKind : uint8_t {
  ZeroResult,
  NonZeroResult,
  ZeroPreservedResult,
  NilResult,
  NonNilError,
};

using ForeignErrorConventionKindField = BCFixed<3>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class SILFunctionTypeRepresentation : uint8_t {
  Thick = 0,
  Block,
  Thin,
  CFunctionPointer,
  
  FirstSIL = 8,
  Method = FirstSIL,
  ObjCMethod,
  WitnessMethod,
  Closure,
};
using SILFunctionTypeRepresentationField = BCFixed<4>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class SILCoroutineKind : uint8_t {
  None = 0,
  YieldOnce = 1,
  YieldMany = 2,
};
using SILCoroutineKindField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum OperatorKind : uint8_t {
  Infix = 0,
  Prefix,
  Postfix,
  PrecedenceGroup,  // only for cross references
};
// This is currently required to have the same width as AccessorKindField.
using OperatorKindField = BCFixed<3>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum AccessorKind : uint8_t {
  Getter = 0,
  Setter,
  WillSet,
  DidSet,
  MaterializeForSet,
  Addressor,
  MutableAddressor,
};
using AccessorKindField = BCFixed<3>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum CtorInitializerKind : uint8_t {
  Designated = 0,
  Convenience = 1,
  Factory = 2,
  ConvenienceFactory = 3,
};
using CtorInitializerKindField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class VarDeclSpecifier : uint8_t {
  Let = 0,
  Var,
  InOut,
  Shared,
};
using VarDeclSpecifierField = BCFixed<2>;
  
// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class ParameterConvention : uint8_t {
  Indirect_In,
  Indirect_Inout,
  Indirect_InoutAliasable,
  Direct_Owned,
  Direct_Unowned,
  Direct_Guaranteed,
  Indirect_In_Guaranteed,
  Indirect_In_Constant,
};
using ParameterConventionField = BCFixed<4>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class ResultConvention : uint8_t {
  Indirect,
  Owned,
  Unowned,
  UnownedInnerPointer,
  Autoreleased,
};
using ResultConventionField = BCFixed<3>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum MetatypeRepresentation : uint8_t {
  MR_None, MR_Thin, MR_Thick, MR_ObjC
};
using MetatypeRepresentationField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class AddressorKind : uint8_t {
  NotAddressor, Unsafe, Owning, NativeOwning, NativePinning
};
using AddressorKindField = BCFixed<3>;
 
// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class SelfAccessKind : uint8_t {
  NonMutating = 0,
  Mutating,
  __Consuming,
};
using SelfAccessKindField = BCFixed<2>;
  
/// Translates an operator DeclKind to a Serialization fixity, whose values are
/// guaranteed to be stable.
static inline OperatorKind getStableFixity(DeclKind kind) {
  switch (kind) {
  case DeclKind::PrefixOperator:
    return Prefix;
  case DeclKind::PostfixOperator:
    return Postfix;
  case DeclKind::InfixOperator:
    return Infix;
  default:
    llvm_unreachable("unknown operator fixity");
  }
}

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum GenericRequirementKind : uint8_t {
  Conformance = 0,
  SameType    = 1,
  Superclass  = 2,
  Layout = 3,
};
using GenericRequirementKindField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum LayoutRequirementKind : uint8_t {
  UnknownLayout = 0,
  TrivialOfExactSize = 1,
  TrivialOfAtMostSize = 2,
  Trivial = 3,
  RefCountedObject = 4,
  NativeRefCountedObject = 5,
  Class = 6,
  NativeClass = 7
};
using LayoutRequirementKindField = BCFixed<3>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum Associativity : uint8_t {
  NonAssociative = 0,
  LeftAssociative,
  RightAssociative
};
using AssociativityField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum Ownership : uint8_t {
  Strong = 0,
  Weak,
  Unowned,
  Unmanaged,
};
using OwnershipField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class DefaultArgumentKind : uint8_t {
  None = 0,
  Normal,
  File,
  Line,
  Column,
  Function,
  Inherited,
  DSOHandle,
  NilLiteral,
  EmptyArray,
  EmptyDictionary,
};
using DefaultArgumentField = BCFixed<4>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum LibraryKind : uint8_t {
  Library = 0,
  Framework
};
using LibraryKindField = BCFixed<1>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class AccessLevel : uint8_t {
  Private = 0,
  FilePrivate,
  Internal,
  Public,
  Open,
};
using AccessLevelField = BCFixed<3>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class OptionalTypeKind : uint8_t {
  None,
  Optional,
  ImplicitlyUnwrappedOptional
};
using OptionalTypeKindField = BCFixed<2>;

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class DeclNameKind: uint8_t {
  Normal,
  Subscript,
  Destructor
};

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum SpecialIdentifierID : uint8_t {
  /// Special IdentifierID value for the Builtin module.
  BUILTIN_MODULE_ID = 0,
  /// Special IdentifierID value for the current module.
  CURRENT_MODULE_ID,
  /// Special value for the module for imported Objective-C headers.
  OBJC_HEADER_MODULE_ID,
  /// Special value for the special subscript name
  SUBSCRIPT_ID,
  /// Special value for the special destructor name
  DESTRUCTOR_ID,

  /// The number of special Identifier IDs. This value should never be encoded;
  /// it should only be used to count the number of names above. As such, it
  /// is correct and necessary to add new values above this one.
  NUM_SPECIAL_IDS
};

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class EnumElementRawValueKind : uint8_t {
  /// No raw value serialized.
  None = 0,
  /// Integer literal.
  IntegerLiteral,
  /// TODO: Float, string, char, etc.
};

// These IDs must \em not be renumbered or reordered without incrementing
// VERSION_MAJOR.
enum class ResilienceExpansion : uint8_t {
  Minimal = 0,
  Maximal,
};

using EnumElementRawValueKindField = BCFixed<4>;

/// The various types of blocks that can occur within a serialized Swift
/// module.
///
/// These IDs must \em not be renumbered or reordered without incrementing
/// VERSION_MAJOR.
enum BlockID {
  /// The module block, which contains all of the other blocks (and in theory
  /// allows a single file to contain multiple modules).
  MODULE_BLOCK_ID = llvm::bitc::FIRST_APPLICATION_BLOCKID,

  /// The control block, which contains all of the information that needs to
  /// be validated prior to committing to loading the serialized module.
  ///
  /// \sa control_block
  CONTROL_BLOCK_ID,

  /// The input block, which contains all the files this module depends on.
  ///
  /// \sa input_block
  INPUT_BLOCK_ID,

  /// The "decls-and-types" block, which contains all of the declarations that
  /// come from this module.
  ///
  /// Types are also stored here, so that types that just wrap a Decl don't need
  /// a separate entry in the file.
  ///
  /// \sa decls_block
  DECLS_AND_TYPES_BLOCK_ID,

  /// The identifier block, which contains all of the strings used in
  /// identifiers in the module.
  ///
  /// Unlike other blocks in the file, all data within this block is completely
  /// opaque. Offsets into this block should point directly into the blob at a
  /// null-terminated UTF-8 string.
  IDENTIFIER_DATA_BLOCK_ID,

  /// The index block, which contains cross-referencing information for the
  /// module.
  ///
  /// \sa index_block
  INDEX_BLOCK_ID,

  /// The block for SIL functions.
  ///
  /// \sa sil_block
  SIL_BLOCK_ID,

  /// The index block for SIL functions.
  ///
  /// \sa sil_index_block
  SIL_INDEX_BLOCK_ID,

  /// A sub-block of the control block that contains configuration options
  /// needed to successfully load this module.
  ///
  /// \sa options_block
  OPTIONS_BLOCK_ID,

  /// The module documentation container block, which contains all other
  /// documentation blocks.
  MODULE_DOC_BLOCK_ID = 96,

  /// The comment block, which contains documentation comments.
  ///
  /// \sa comment_block
  COMMENT_BLOCK_ID,

  /// The declaration member-tables index block, a sub-blocb of the index block.
  ///
  /// \sa decl_member_tables_block
  DECL_MEMBER_TABLES_BLOCK_ID
};

/// The record types within the control block.
///
/// \sa CONTROL_BLOCK_ID
namespace control_block {
  // These IDs must \em not be renumbered or reordered without incrementing
  // VERSION_MAJOR.
  enum {
    METADATA = 1,
    MODULE_NAME,
    TARGET
  };

  using MetadataLayout = BCRecordLayout<
    METADATA, // ID
    BCFixed<16>, // Module format major version
    BCFixed<16>, // Module format minor version
    BCVBR<8>, // length of "short version string" in the blob
    BCVBR<8>, // length of "short compatibility version string" in the blob
    BCBlob // misc. version information
  >;

  using ModuleNameLayout = BCRecordLayout<
    MODULE_NAME,
    BCBlob
  >;

  using TargetLayout = BCRecordLayout<
    TARGET,
    BCBlob // LLVM triple
  >;
}

/// The record types within the options block (a sub-block of the control
/// block).
///
/// \sa OPTIONS_BLOCK_ID
namespace options_block {
  // These IDs must \em not be renumbered or reordered without incrementing
  // VERSION_MAJOR.
  enum {
    SDK_PATH = 1,
    XCC,
    IS_SIB,
    IS_TESTABLE,
    RESILIENCE_STRATEGY
  };

  using SDKPathLayout = BCRecordLayout<
    SDK_PATH,
    BCBlob // path
  >;

  using XCCLayout = BCRecordLayout<
    XCC,
    BCBlob // -Xcc flag, as string
  >;

  using IsSIBLayout = BCRecordLayout<
    IS_SIB,
    BCFixed<1> // Is this an intermediate file?
  >;

  using IsTestableLayout = BCRecordLayout<
    IS_TESTABLE
  >;

  using ResilienceStrategyLayout = BCRecordLayout<
    RESILIENCE_STRATEGY,
    BCFixed<2>
  >;
}

/// The record types within the input block.
///
/// \sa INPUT_BLOCK_ID
namespace input_block {
  // These IDs must \em not be renumbered or reordered without incrementing
  // VERSION_MAJOR.
  enum {
    IMPORTED_MODULE = 1,
    LINK_LIBRARY,
    IMPORTED_HEADER,
    IMPORTED_HEADER_CONTENTS,
    MODULE_FLAGS,
    SEARCH_PATH
  };

  using ImportedModuleLayout = BCRecordLayout<
    IMPORTED_MODULE,
    BCFixed<1>, // exported?
    BCFixed<1>, // scoped?
    BCBlob // module name, with submodule path pieces separated by \0s.
           // If the 'scoped' flag is set, the final path piece is an access
           // path within the module.
  >;

  using LinkLibraryLayout = BCRecordLayout<
    LINK_LIBRARY,
    LibraryKindField, // kind
    BCFixed<1>, // forced?
    BCBlob // library name
  >;

  using ImportedHeaderLayout = BCRecordLayout<
    IMPORTED_HEADER,
    BCFixed<1>, // exported?
    FileSizeField, // file size (for validation)
    FileModTimeField, // file mtime (for validation)
    BCBlob // file path
  >;

  using ImportedHeaderContentsLayout = BCRecordLayout<
    IMPORTED_HEADER_CONTENTS,
    BCBlob
  >;

  using ModuleFlagsLayout = BCRecordLayout<
    MODULE_FLAGS,
    BCFixed<1> // has underlying module? [[UNUSED]]
  >;

  using SearchPathLayout = BCRecordLayout<
    SEARCH_PATH,
    BCFixed<1>, // framework?
    BCFixed<1>, // system?
    BCBlob      // path
  >;
}

/// The record types within the "decls-and-types" block.
///
/// \sa DECLS_AND_TYPES_BLOCK_ID
namespace decls_block {
  // These IDs must \em not be renumbered or reordered without incrementing
  // VERSION_MAJOR.
  enum RecordKind : uint8_t {
#define RECORD(Id) Id,
#define RECORD_VAL(Id, Value) Id = Value,
#include "swift/Serialization/DeclTypeRecordNodes.def"
  };

  using NameAliasTypeLayout = BCRecordLayout<
    NAME_ALIAS_TYPE,
    DeclIDField, // typealias decl
    TypeIDField  // canonical type (a fallback)
  >;

  using GenericTypeParamTypeLayout = BCRecordLayout<
    GENERIC_TYPE_PARAM_TYPE,
    DeclIDField, // generic type parameter decl or depth
    BCVBR<4>     // index + 1, or zero if we have a generic type parameter decl
  >;

  using DependentMemberTypeLayout = BCRecordLayout<
    DEPENDENT_MEMBER_TYPE,
    TypeIDField,      // base type
    DeclIDField       // associated type decl
  >;
  using NominalTypeLayout = BCRecordLayout<
    NOMINAL_TYPE,
    DeclIDField, // decl
    TypeIDField  // parent
  >;

  using ParenTypeLayout = BCRecordLayout<
    PAREN_TYPE,
    TypeIDField,        // inner type
    BCFixed<1>,         // vararg?
    BCFixed<1>,         // autoclosure?
    BCFixed<1>,         // escaping?
    BCFixed<1>,         // inout?
    BCFixed<1>          // shared?
  >;

  using TupleTypeLayout = BCRecordLayout<
    TUPLE_TYPE
  >;

  using TupleTypeEltLayout = BCRecordLayout<
    TUPLE_TYPE_ELT,
    IdentifierIDField,  // name
    TypeIDField,        // type
    BCFixed<1>,         // vararg?
    BCFixed<1>,         // autoclosure?
    BCFixed<1>,         // escaping?
    BCFixed<1>,         // inout?
    BCFixed<1>          // shared?
  >;

  using FunctionTypeLayout = BCRecordLayout<
    FUNCTION_TYPE,
    TypeIDField, // input
    TypeIDField, // output
    FunctionTypeRepresentationField, // representation
    BCFixed<1>,  // auto-closure?
    BCFixed<1>,  // noescape?
    BCFixed<1>   // throws?
  >;

  using MetatypeTypeLayout = BCRecordLayout<
    METATYPE_TYPE,
    TypeIDField,                       // instance type
    MetatypeRepresentationField        // representation
  >;

  using ExistentialMetatypeTypeLayout = BCRecordLayout<
    EXISTENTIAL_METATYPE_TYPE,
    TypeIDField,                       // instance type
    MetatypeRepresentationField        // representation
  >;

  using InOutTypeLayout = BCRecordLayout<
    INOUT_TYPE,
    TypeIDField // object type
  >;

  using ArchetypeTypeLayout = BCRecordLayout<
    ARCHETYPE_TYPE,
    GenericEnvironmentIDField, // generic environment
    TypeIDField                // interface type
  >;

  using OpenedExistentialTypeLayout = BCRecordLayout<
    OPENED_EXISTENTIAL_TYPE,
    TypeIDField         // the existential type
  >;

  using DynamicSelfTypeLayout = BCRecordLayout<
    DYNAMIC_SELF_TYPE,
    TypeIDField          // self type
  >;

  using ProtocolCompositionTypeLayout = BCRecordLayout<
    PROTOCOL_COMPOSITION_TYPE,
    BCFixed<1>,          // has AnyObject constraint
    BCArray<TypeIDField> // protocols
  >;

  using BoundGenericTypeLayout = BCRecordLayout<
    BOUND_GENERIC_TYPE,
    DeclIDField, // generic decl
    TypeIDField, // parent
    BCArray<TypeIDField> // generic arguments
  >;

  using BoundGenericSubstitutionLayout = BCRecordLayout<
    BOUND_GENERIC_SUBSTITUTION,
    TypeIDField,  // replacement
    BCVBR<5>
    // Trailed by protocol conformance info (if any)
  >;

  using GenericFunctionTypeLayout = BCRecordLayout<
    GENERIC_FUNCTION_TYPE,
    TypeIDField,         // input
    TypeIDField,         // output
    FunctionTypeRepresentationField, // representation
    BCFixed<1>,          // throws?
    GenericSignatureIDField // generic signture
  >;

  using SILFunctionTypeLayout = BCRecordLayout<
    SIL_FUNCTION_TYPE,
    SILCoroutineKindField, // coroutine kind
    ParameterConventionField, // callee convention
    SILFunctionTypeRepresentationField, // representation
    BCFixed<1>,            // pseudogeneric?
    BCFixed<1>,            // noescape?
    BCFixed<1>,            // error result?
    BCFixed<30>,           // number of parameters
    BCFixed<30>,           // number of yields
    BCFixed<30>,           // number of results
    GenericSignatureIDField, // generic signature
    BCArray<TypeIDField>   // parameter types/conventions, alternating
                           // followed by result types/conventions, alternating
                           // followed by error result type/convention
    // Optionally a protocol conformance (for witness_methods)
  >;
  
  using SILBlockStorageTypeLayout = BCRecordLayout<
    SIL_BLOCK_STORAGE_TYPE,
    TypeIDField            // capture type
  >;

  using SILLayoutLayout = BCRecordLayout<
    SIL_LAYOUT,
    GenericSignatureIDField,    // generic signature
    BCFixed<31>,                // number of fields
    BCArray<TypeIDWithBitField> // field types with mutability
  >;

  using SILBoxTypeLayout = BCRecordLayout<
    SIL_BOX_TYPE,
    SILLayoutIDField     // layout
                         // trailing substitutions
  >;

  template <unsigned Code>
  using SyntaxSugarTypeLayout = BCRecordLayout<
    Code,
    TypeIDField // element type
  >;

  using ArraySliceTypeLayout = SyntaxSugarTypeLayout<ARRAY_SLICE_TYPE>;
  using OptionalTypeLayout = SyntaxSugarTypeLayout<OPTIONAL_TYPE>;
  using ImplicitlyUnwrappedOptionalTypeLayout =
    SyntaxSugarTypeLayout<UNCHECKED_OPTIONAL_TYPE>;

  using DictionaryTypeLayout = BCRecordLayout<
    DICTIONARY_TYPE,
    TypeIDField, // key type
    TypeIDField  // value type
  >;

  using ReferenceStorageTypeLayout = BCRecordLayout<
    REFERENCE_STORAGE_TYPE,
    OwnershipField,  // ownership
    TypeIDField      // implementation type
  >;

  using UnboundGenericTypeLayout = BCRecordLayout<
    UNBOUND_GENERIC_TYPE,
    DeclIDField, // generic decl
    TypeIDField  // parent
  >;

  using TypeAliasLayout = BCRecordLayout<
    TYPE_ALIAS_DECL,
    IdentifierIDField, // name
    DeclContextIDField,// context decl
    TypeIDField, // underlying type
    TypeIDField, // interface type (no longer used)
    BCFixed<1>,  // implicit flag
    GenericEnvironmentIDField, // generic environment
    AccessLevelField, // access level
    BCArray<TypeIDField> // dependency types
    // Trailed by generic parameters (if any).
  >;

  using GenericTypeParamDeclLayout = BCRecordLayout<
    GENERIC_TYPE_PARAM_DECL,
    IdentifierIDField,  // name
    DeclContextIDField, // context decl
    BCFixed<1>,         // implicit flag
    BCVBR<4>,           // depth
    BCVBR<4>            // index
  >;

  using AssociatedTypeDeclLayout = BCRecordLayout<
    ASSOCIATED_TYPE_DECL,
    IdentifierIDField,   // name
    DeclContextIDField,  // context decl
    TypeIDField,         // default definition
    BCFixed<1>,          // implicit flag
    BCArray<DeclIDField> // overridden associated types
  >;

  using StructLayout = BCRecordLayout<
    STRUCT_DECL,
    IdentifierIDField,      // name
    DeclContextIDField,     // context decl
    BCFixed<1>,             // implicit flag
    GenericEnvironmentIDField, // generic environment
    AccessLevelField, // access level
    BCVBR<4>,               // number of conformances
    BCArray<TypeIDField>    // inherited types
    // Trailed by the generic parameters (if any), the members record, and
    // finally conformance info (if any).
  >;

  using EnumLayout = BCRecordLayout<
    ENUM_DECL,
    IdentifierIDField,      // name
    DeclContextIDField,     // context decl
    BCFixed<1>,             // implicit flag
    GenericEnvironmentIDField, // generic environment
    TypeIDField,            // raw type
    AccessLevelField, // access level
    BCVBR<4>,               // number of conformances
    BCVBR<4>,               // number of inherited types
    BCArray<TypeIDField>    // inherited types, followed by dependency types
    // Trailed by the generic parameters (if any), the members record, and
    // finally conformance info (if any).
  >;

  using ClassLayout = BCRecordLayout<
    CLASS_DECL,
    IdentifierIDField, // name
    DeclContextIDField,// context decl
    BCFixed<1>,        // implicit?
    BCFixed<1>,        // explicitly objc?
    BCFixed<1>,        // requires stored property initial values
    GenericEnvironmentIDField, // generic environment
    TypeIDField,       // superclass
    AccessLevelField, // access level
    BCVBR<4>,               // number of conformances
    BCArray<TypeIDField>    // inherited types
    // Trailed by the generic parameters (if any), the members record, and
    // finally conformance info (if any).
  >;

  using ProtocolLayout = BCRecordLayout<
    PROTOCOL_DECL,
    IdentifierIDField,      // name
    DeclContextIDField,     // context decl
    BCFixed<1>,             // implicit flag
    BCFixed<1>,             // class-bounded?
    BCFixed<1>,             // objc?
    BCFixed<1>,             // existential-type-supported?
    GenericEnvironmentIDField, // generic environment
    AccessLevelField, // access level
    BCArray<DeclIDField>    // inherited types
    // Trailed by the generic parameters (if any), the members record, and
    // the default witness table record
  >;

  /// A default witness table for a protocol.
  using DefaultWitnessTableLayout = BCRecordLayout<
    DEFAULT_WITNESS_TABLE,
    BCArray<DeclIDField>
    // An array of requirement / witness pairs
  >;

  using ConstructorLayout = BCRecordLayout<
    CONSTRUCTOR_DECL,
    DeclContextIDField, // context decl
    OptionalTypeKindField,  // failability
    BCFixed<1>,  // implicit?
    BCFixed<1>,  // objc?
    BCFixed<1>,  // stub implementation?
    BCFixed<1>,  // throws?
    CtorInitializerKindField,  // initializer kind
    GenericEnvironmentIDField, // generic environment
    TypeIDField, // interface type
    DeclIDField, // overridden decl
    AccessLevelField, // access level
    BCFixed<1>,   // requires a new vtable slot
    BCFixed<1>,   // default argument resilience expansion
    BCFixed<1>,   // 'required' but overridden is not (used for recovery)
    BCVBR<5>,     // number of parameter name components
    BCArray<IdentifierIDField> // name components,
                               // followed by TypeID dependencies
    // Trailed by its generic parameters, if any, followed by the parameter
    // patterns.
  >;

  using VarLayout = BCRecordLayout<
    VAR_DECL,
    IdentifierIDField, // name
    DeclContextIDField,  // context decl
    BCFixed<1>,   // implicit?
    BCFixed<1>,   // explicitly objc?
    BCFixed<1>,   // static?
    VarDeclSpecifierField,   // specifier
    BCFixed<1>,   // HasNonPatternBindingInit?
    BCFixed<1>,   // is getter mutating?
    BCFixed<1>,   // is setter mutating?
    StorageKindField,   // StorageKind
    TypeIDField,  // interface type
    DeclIDField,  // getter
    DeclIDField,  // setter
    DeclIDField,  // materializeForSet
    DeclIDField,  // addressor
    DeclIDField,  // mutableAddressor
    DeclIDField,  // willset
    DeclIDField,  // didset
    DeclIDField,  // overridden decl
    AccessLevelField, // access level
    AccessLevelField, // setter access, if applicable
    BCArray<TypeIDField> // dependencies
  >;

  using ParamLayout = BCRecordLayout<
    PARAM_DECL,
    IdentifierIDField, // argument name
    IdentifierIDField, // parameter name
    DeclContextIDField,  // context decl
    VarDeclSpecifierField,   // specifier
    TypeIDField  // interface type
  >;

  using FuncLayout = BCRecordLayout<
    FUNC_DECL,
    DeclContextIDField,  // context decl
    BCFixed<1>,   // implicit?
    BCFixed<1>,   // is 'static' or 'class'?
    StaticSpellingKindField, // spelling of 'static' or 'class'
    BCFixed<1>,   // explicitly objc?
    SelfAccessKindField,   // self access kind
    BCFixed<1>,   // has dynamic self?
    BCFixed<1>,   // has forced static dispatch?
    BCFixed<1>,   // throws?
    GenericEnvironmentIDField, // generic environment
    TypeIDField,  // interface type
    DeclIDField,  // operator decl
    DeclIDField,  // overridden function
    DeclIDField,  // AccessorStorageDecl
    BCVBR<5>,     // 0 for a simple name, otherwise the number of parameter name
                  // components plus one
    AddressorKindField, // addressor kind
    AccessLevelField, // access level
    BCFixed<1>,   // requires a new vtable slot
    BCFixed<1>,   // default argument resilience expansion
    BCArray<IdentifierIDField> // name components,
                               // followed by TypeID dependencies
    // The record is trailed by:
    // - its _silgen_name, if any
    // - its generic parameters, if any
    // - body parameter patterns
  >;

  using PatternBindingLayout = BCRecordLayout<
    PATTERN_BINDING_DECL,
    DeclContextIDField, // context decl
    BCFixed<1>,  // implicit flag
    BCFixed<1>,  // static?
    StaticSpellingKindField, // spelling of 'static' or 'class'
    BCVBR<3>,    // numpatterns
    BCArray<DeclContextIDField> // init contexts
    // The patterns and decl-contexts trail the record.
  >;

  template <unsigned Code>
  using UnaryOperatorLayout = BCRecordLayout<
    Code, // ID field
    IdentifierIDField, // name
    DeclContextIDField // context decl
  >;

  using PrefixOperatorLayout = UnaryOperatorLayout<PREFIX_OPERATOR_DECL>;
  using PostfixOperatorLayout = UnaryOperatorLayout<POSTFIX_OPERATOR_DECL>;

  using InfixOperatorLayout = BCRecordLayout<
    INFIX_OPERATOR_DECL,
    IdentifierIDField, // name
    DeclContextIDField,// context decl
    DeclIDField        // precedence group
  >;

  using PrecedenceGroupLayout = BCRecordLayout<
    PRECEDENCE_GROUP_DECL,
    IdentifierIDField, // name
    DeclContextIDField,// context decl
    AssociativityField,// associativity
    BCFixed<1>,        // assignment
    BCVBR<2>,          // numHigherThan
    BCArray<DeclIDField> // higherThan, followed by lowerThan
  >;

  using EnumElementLayout = BCRecordLayout<
    ENUM_ELEMENT_DECL,
    IdentifierIDField, // name
    DeclContextIDField,// context decl
    TypeIDField, // interface type
    BCFixed<1>,  // has argument type?
    BCFixed<1>,  // implicit?
    EnumElementRawValueKindField,  // raw value kind
    BCFixed<1>,  // negative raw value?
    BCBlob       // raw value
  >;

  using SubscriptLayout = BCRecordLayout<
    SUBSCRIPT_DECL,
    DeclContextIDField, // context decl
    BCFixed<1>,  // implicit?
    BCFixed<1>,  // objc?
    BCFixed<1>,   // is getter mutating?
    BCFixed<1>,   // is setter mutating?
    StorageKindField,   // StorageKind
    GenericEnvironmentIDField, // generic environment
    TypeIDField, // interface type
    DeclIDField, // getter
    DeclIDField, // setter
    DeclIDField, // materializeForSet
    DeclIDField, // addressor
    DeclIDField, // mutableAddressor
    DeclIDField, // willSet
    DeclIDField, // didSet
    DeclIDField, // overridden decl
    AccessLevelField, // access level
    AccessLevelField, // setter access, if applicable
    BCVBR<5>,    // number of parameter name components
    BCArray<IdentifierIDField> // name components,
                               // followed by TypeID dependencies
    // Trailed by:
    // - generic parameters, if any
    // - the indices pattern
  >;

  using ExtensionLayout = BCRecordLayout<
    EXTENSION_DECL,
    TypeIDField, // base type
    DeclContextIDField, // context decl
    BCFixed<1>,  // implicit flag
    GenericEnvironmentIDField,  // generic environment
    BCVBR<4>,    // # of protocol conformances
    BCVBR<4>,    // number of inherited types
    BCArray<TypeIDField> // inherited types, followed by TypeID dependencies
    // Trailed by the generic parameter lists, members record, and then
    // conformance info (if any).
  >;

  using DestructorLayout = BCRecordLayout<
    DESTRUCTOR_DECL,
    DeclContextIDField, // context decl
    BCFixed<1>,  // implicit?
    BCFixed<1>,  // objc?
    GenericEnvironmentIDField, // generic environment
    TypeIDField  // interface type
    // Trailed by a pattern for self.
  >;

  using ParameterListLayout = BCRecordLayout<
    PARAMETERLIST,
    BCVBR<5>    // numparams
  >;
  
  using ParameterListEltLayout = BCRecordLayout<
    PARAMETERLIST_ELT,
    DeclIDField,           // ParamDecl
    BCFixed<1>,            // isVariadic?
    DefaultArgumentField   // default argument
    >;

  using ParenPatternLayout = BCRecordLayout<
    PAREN_PATTERN,
    BCFixed<1> // implicit?
    // The sub-pattern trails the record.
  >;

  using TuplePatternLayout = BCRecordLayout<
    TUPLE_PATTERN,
    TypeIDField, // type
    BCVBR<5>,    // arity
    BCFixed<1>   // implicit?
    // The elements trail the record.
  >;

  using TuplePatternEltLayout = BCRecordLayout<
    TUPLE_PATTERN_ELT,
    IdentifierIDField     // label
    // The element pattern trails the record.
  >;

  using NamedPatternLayout = BCRecordLayout<
    NAMED_PATTERN,
    DeclIDField, // associated VarDecl
    TypeIDField, // type
    BCFixed<1>   // implicit?
  >;

  using AnyPatternLayout = BCRecordLayout<
    ANY_PATTERN,
    TypeIDField, // type
    BCFixed<1>   // implicit?
    // FIXME: is the type necessary?
  >;

  using TypedPatternLayout = BCRecordLayout<
    TYPED_PATTERN,
    TypeIDField, // associated type
    BCFixed<1>   // implicit?
    // The sub-pattern trails the record.
  >;

  using VarPatternLayout = BCRecordLayout<
    VAR_PATTERN,
    BCFixed<1>, // isLet?
    BCFixed<1>  // implicit?
    // The sub-pattern trails the record.
  >;

  using GenericParamListLayout = BCRecordLayout<
    GENERIC_PARAM_LIST
    // The actual parameters and requirements trail the record.
  >;

  using GenericParamLayout = BCRecordLayout<
    GENERIC_PARAM,
    DeclIDField // Typealias
  >;

  using GenericSignatureLayout = BCRecordLayout<
    GENERIC_SIGNATURE,
    BCArray<TypeIDField>         // generic parameter types
  >;

  using SILGenericEnvironmentLayout = BCRecordLayout<
    SIL_GENERIC_ENVIRONMENT,
    BCArray<TypeIDField>         // (generic parameter name, sugared interface
                                 //  type) pairs
  >;

  using GenericRequirementLayout = BCRecordLayout<
    GENERIC_REQUIREMENT,
    GenericRequirementKindField, // requirement kind
    TypeIDField,                 // types involved (two for conformance,
    TypeIDField                  // same-type; one for value witness marker)
  >;

  using LayoutRequirementLayout = BCRecordLayout<
    LAYOUT_REQUIREMENT,
    LayoutRequirementKindField,  // requirement kind
    TypeIDField,                 // type being constrained
    BCFixed<24>,                 // size
    BCFixed<32>                  // alignment
  >;

  /// Specifies the private discriminator string for a private declaration. This
  /// identifies the declaration's original source file in some opaque way.
  using PrivateDiscriminatorLayout = BCRecordLayout<
    PRIVATE_DISCRIMINATOR,
    IdentifierIDField  // discriminator string, as an identifier
  >;

  using LocalDiscriminatorLayout = BCRecordLayout<
    LOCAL_DISCRIMINATOR,
    BCVBR<2> // context-scoped discriminator counter
  >;

  /// A placeholder for lack of concrete conformance information.
  using AbstractProtocolConformanceLayout = BCRecordLayout<
    ABSTRACT_PROTOCOL_CONFORMANCE,
    DeclIDField // the protocol
  >;

  using NormalProtocolConformanceLayout = BCRecordLayout<
    NORMAL_PROTOCOL_CONFORMANCE,
    DeclIDField, // the protocol
    DeclContextIDField, // the decl that provided this conformance
    BCVBR<5>, // type mapping count
    BCVBR<5>, // value mapping count
    BCVBR<5>, // requirement signature conformance count
    BCArray<DeclIDField>
    // The array contains type witnesses, then value witnesses.
    // Requirement signature conformances follow, then the substitution records
    // for the associated types.
  >;

  using SpecializedProtocolConformanceLayout = BCRecordLayout<
    SPECIALIZED_PROTOCOL_CONFORMANCE,
    TypeIDField,         // conforming type
    BCVBR<5>             // # of substitutions for the conformance
    // followed by substitution records for the conformance
  >;

  using InheritedProtocolConformanceLayout = BCRecordLayout<
    INHERITED_PROTOCOL_CONFORMANCE,
    TypeIDField // the conforming type
  >;

  // Refers to a normal protocol conformance in the given module via its id.
  using NormalProtocolConformanceIdLayout = BCRecordLayout<
    NORMAL_PROTOCOL_CONFORMANCE_ID,
    NormalConformanceIDField // the normal conformance ID
  >;

  using ProtocolConformanceXrefLayout = BCRecordLayout<
    PROTOCOL_CONFORMANCE_XREF,
    DeclIDField, // the protocol being conformed to
    DeclIDField, // the nominal type of the conformance
    ModuleIDField // the module in which the conformance can be found
  >;

  using MembersLayout = BCRecordLayout<
    MEMBERS,
    BCArray<DeclIDField>
  >;

  using XRefLayout = BCRecordLayout<
    XREF,
    ModuleIDField,  // base module ID
    BCVBR<4>        // xref path length (cannot be 0)
  >;

  using XRefTypePathPieceLayout = BCRecordLayout<
    XREF_TYPE_PATH_PIECE,
    IdentifierIDField, // name
    IdentifierIDField, // private discriminator
    BCFixed<1>         // restrict to protocol extension
  >;

  using XRefValuePathPieceLayout = BCRecordLayout<
    XREF_VALUE_PATH_PIECE,
    TypeIDField,       // type
    IdentifierIDField, // name
    BCFixed<1>,        // restrict to protocol extension
    BCFixed<1>         // static?
  >;

  using XRefInitializerPathPieceLayout = BCRecordLayout<
    XREF_INITIALIZER_PATH_PIECE,
    TypeIDField,             // type
    BCFixed<1>,              // restrict to protocol extension
    CtorInitializerKindField // initializer kind
  >;

  using XRefExtensionPathPieceLayout = BCRecordLayout<
    XREF_EXTENSION_PATH_PIECE,
    ModuleIDField,       // module ID
    GenericSignatureIDField  // for a constrained extension,
                             // the generic signature
  >;

  using XRefOperatorOrAccessorPathPieceLayout = BCRecordLayout<
    XREF_OPERATOR_OR_ACCESSOR_PATH_PIECE,
    IdentifierIDField, // name
    AccessorKindField  // accessor kind OR operator fixity
  >;
  static_assert(std::is_same<AccessorKindField, OperatorKindField>::value,
                "accessor kinds and operator kinds are not compatible");

  using XRefGenericParamPathPieceLayout = BCRecordLayout<
    XREF_GENERIC_PARAM_PATH_PIECE,
    BCVBR<5> // index
  >;

  using SILGenNameDeclAttrLayout = BCRecordLayout<
    SILGenName_DECL_ATTR,
    BCFixed<1>, // implicit flag
    BCBlob      // _silgen_name
  >;

  using CDeclDeclAttrLayout = BCRecordLayout<
    CDecl_DECL_ATTR,
    BCFixed<1>, // implicit flag
    BCBlob      // _silgen_name
  >;

  
  using AlignmentDeclAttrLayout = BCRecordLayout<
    Alignment_DECL_ATTR,
    BCFixed<1>, // implicit flag
    BCFixed<31> // alignment
  >;
  
  using SwiftNativeObjCRuntimeBaseDeclAttrLayout = BCRecordLayout<
    SwiftNativeObjCRuntimeBase_DECL_ATTR,
    BCFixed<1>, // implicit flag
    IdentifierIDField // name
  >;

  using SemanticsDeclAttrLayout = BCRecordLayout<
    Semantics_DECL_ATTR,
    BCFixed<1>, // implicit flag
    BCBlob      // semantics value
  >;

  using EffectsDeclAttrLayout = BCRecordLayout<
    Effects_DECL_ATTR,
    BCFixed<2>  // modref value
  >;

  using DeclContextLayout = BCRecordLayout<
    DECL_CONTEXT,
    // If this DeclContext is a local context, this is an
    // index into the local decl context table.
    // If this DeclContext is a Decl (and not a DeclContext
    // *at all*, this is an index into the decl table.
    DeclContextIDField,
    BCFixed<1> // is a decl
  >;

  using ForeignErrorConventionLayout = BCRecordLayout<
    FOREIGN_ERROR_CONVENTION,
    ForeignErrorConventionKindField,  // kind
    BCFixed<1>,                       // owned
    BCFixed<1>,                       // replaced
    BCVBR<4>,                         // error parameter index
    TypeIDField,                      // error parameter type
    TypeIDField                       // result type
  >;

  using AbstractClosureExprLayout = BCRecordLayout<
    ABSTRACT_CLOSURE_EXPR_CONTEXT,
    TypeIDField, // type
    BCFixed<1>, // implicit
    BCVBR<4>, // discriminator
    DeclContextIDField // parent context decl
  >;

  using TopLevelCodeDeclContextLayout = BCRecordLayout<
    TOP_LEVEL_CODE_DECL_CONTEXT,
    DeclContextIDField // parent context decl
  >;

  using PatternBindingInitializerLayout = BCRecordLayout<
    PATTERN_BINDING_INITIALIZER_CONTEXT,
    DeclIDField, // parent pattern binding decl
    BCVBR<3>     // binding index in the pattern binding decl
  >;

  using DefaultArgumentInitializerLayout = BCRecordLayout<
    DEFAULT_ARGUMENT_INITIALIZER_CONTEXT,
    DeclContextIDField, // parent context decl
    BCVBR<3> // parameter index
  >;

  // Stub layouts, unused.
  using OwnershipDeclAttrLayout = BCRecordLayout<Ownership_DECL_ATTR>;
  using RawDocCommentDeclAttrLayout = BCRecordLayout<RawDocComment_DECL_ATTR>;
  using AccessControlDeclAttrLayout = BCRecordLayout<AccessControl_DECL_ATTR>;
  using SetterAccessDeclAttrLayout = BCRecordLayout<SetterAccess_DECL_ATTR>;
  using ObjCBridgedDeclAttrLayout = BCRecordLayout<ObjCBridged_DECL_ATTR>;
  using SynthesizedProtocolDeclAttrLayout
    = BCRecordLayout<SynthesizedProtocol_DECL_ATTR>;
  using ImplementsDeclAttrLayout = BCRecordLayout<Implements_DECL_ATTR>;
  using ObjCRuntimeNameDeclAttrLayout
    = BCRecordLayout<ObjCRuntimeName_DECL_ATTR>;
  using RestatedObjCConformanceDeclAttrLayout
    = BCRecordLayout<RestatedObjCConformance_DECL_ATTR>;

  using InlineDeclAttrLayout = BCRecordLayout<
    Inline_DECL_ATTR,
    BCFixed<2>  // inline value
  >;

  using OptimizeDeclAttrLayout = BCRecordLayout<
    Optimize_DECL_ATTR,
    BCFixed<2>  // optimize value
  >;

  // Encodes a VersionTuple:
  //
  //  Major
  //  Minor
  //  Subminor
  //  HasMinor
  //  HasSubminor
#define BC_AVAIL_TUPLE\
    BCVBR<5>,\
    BCVBR<5>,\
    BCVBR<4>,\
    BCFixed<1>,\
    BCFixed<1>

  using AvailableDeclAttrLayout = BCRecordLayout<
    Available_DECL_ATTR,
    BCFixed<1>, // implicit flag
    BCFixed<1>, // is unconditionally unavailable?
    BCFixed<1>, // is unconditionally deprecated?
    BC_AVAIL_TUPLE, // Introduced
    BC_AVAIL_TUPLE, // Deprecated
    BC_AVAIL_TUPLE, // Obsoleted
    BCVBR<5>,   // platform
    BCVBR<5>,   // number of bytes in message string
    BCVBR<5>,   // number of bytes in rename string
    BCBlob      // platform, followed by message
  >;

#undef BC_AVAIL_TUPLE

  using ObjCDeclAttrLayout = BCRecordLayout<
    ObjC_DECL_ATTR,
    BCFixed<1>, // implicit flag
    BCFixed<1>, // Swift 3 inferred
    BCFixed<1>, // implicit name flag
    BCVBR<4>,   // # of arguments (+1) or zero if no name
    BCArray<IdentifierIDField>
  >;

  using SpecializeDeclAttrLayout = BCRecordLayout<
    Specialize_DECL_ATTR,
    BCFixed<1>, // exported flag
    BCFixed<1> // specialization kind
  >;

#define SIMPLE_DECL_ATTR(X, CLASS, ...) \
  using CLASS##DeclAttrLayout = BCRecordLayout< \
    CLASS##_DECL_ATTR, \
    BCFixed<1> /* implicit flag */ \
  >;
#include "swift/AST/Attr.def"

}

/// Returns the encoding kind for the given decl.
///
/// Note that this does not work for all encodable decls, only those designed
/// to be stored in a hash table.
static inline decls_block::RecordKind getKindForTable(const Decl *D) {
  using namespace decls_block;

  switch (D->getKind()) {
  case DeclKind::TypeAlias:
    return decls_block::TYPE_ALIAS_DECL;
  case DeclKind::Enum:
    return decls_block::ENUM_DECL;
  case DeclKind::Struct:
    return decls_block::STRUCT_DECL;
  case DeclKind::Class:
    return decls_block::CLASS_DECL;
  case DeclKind::Protocol:
    return decls_block::PROTOCOL_DECL;

  case DeclKind::Func:
    return decls_block::FUNC_DECL;
  case DeclKind::Var:
    return decls_block::VAR_DECL;
  case DeclKind::Param:
    return decls_block::PARAM_DECL;

  case DeclKind::Subscript:
    return decls_block::SUBSCRIPT_DECL;
  case DeclKind::Constructor:
    return decls_block::CONSTRUCTOR_DECL;
  case DeclKind::Destructor:
    return decls_block::DESTRUCTOR_DECL;

  default:
    llvm_unreachable("cannot store this kind of decl in a hash table");
  }
}

/// The record types within the identifier block.
///
/// \sa IDENTIFIER_BLOCK_ID
namespace identifier_block {
  enum {
    IDENTIFIER_DATA = 1
  };

  using IdentifierDataLayout = BCRecordLayout<IDENTIFIER_DATA, BCBlob>;
};

/// The record types within the index block.
///
/// \sa INDEX_BLOCK_ID
namespace index_block {
  // These IDs must \em not be renumbered or reordered without incrementing
  // VERSION_MAJOR.
  enum RecordKind {
    TYPE_OFFSETS = 1,
    DECL_OFFSETS,
    IDENTIFIER_OFFSETS,
    TOP_LEVEL_DECLS,
    OPERATORS,
    EXTENSIONS,
    CLASS_MEMBERS_FOR_DYNAMIC_LOOKUP,
    OPERATOR_METHODS,

    /// The Objective-C method index, which contains a mapping from
    /// Objective-C selectors to the methods/initializers/properties/etc. that
    /// produce Objective-C methods.
    OBJC_METHODS,

    ENTRY_POINT,
    LOCAL_DECL_CONTEXT_OFFSETS,
    DECL_CONTEXT_OFFSETS,
    LOCAL_TYPE_DECLS,
    GENERIC_ENVIRONMENT_OFFSETS,
    NORMAL_CONFORMANCE_OFFSETS,
    SIL_LAYOUT_OFFSETS,

    PRECEDENCE_GROUPS,
    NESTED_TYPE_DECLS,
    DECL_MEMBER_NAMES,

    GENERIC_SIGNATURE_OFFSETS,
    LastRecordKind = GENERIC_SIGNATURE_OFFSETS,
  };
  
  constexpr const unsigned RecordIDFieldWidth = 5;
  static_assert(LastRecordKind < (1 << RecordIDFieldWidth),
                "not enough bits for all record kinds");
  using RecordIDField = BCFixed<RecordIDFieldWidth>;

  using OffsetsLayout = BCGenericRecordLayout<
    RecordIDField, // record ID
    BCArray<BitOffsetField>
  >;

  using DeclListLayout = BCGenericRecordLayout<
    RecordIDField, // record ID
    BCVBR<16>,  // table offset within the blob (see below)
    BCBlob  // map from identifier strings to decl kinds / decl IDs
  >;

  using GroupNamesLayout = BCGenericRecordLayout<
    RecordIDField, // record ID
    BCBlob       // actual names
  >;

  using ExtensionTableLayout = BCRecordLayout<
    EXTENSIONS, // record ID
    BCVBR<16>,  // table offset within the blob (see below)
    BCBlob  // map from identifier strings to decl kinds / decl IDs
  >;

  using ObjCMethodTableLayout = BCRecordLayout<
    OBJC_METHODS,  // record ID
    BCVBR<16>,     // table offset within the blob (see below)
    BCBlob         // map from Objective-C selectors to methods with that selector
  >;

  using NestedTypeDeclsLayout = BCRecordLayout<
    NESTED_TYPE_DECLS, // record ID
    BCVBR<16>,  // table offset within the blob (see below)
    BCBlob  // map from identifier strings to decl kinds / decl IDs
  >;

  using DeclMemberNamesLayout = BCRecordLayout<
    DECL_MEMBER_NAMES, // record ID
    BCVBR<16>,  // table offset within the blob (see below)
    BCBlob  // map from member DeclBaseNames to offsets of DECL_MEMBERS records
  >;

  using EntryPointLayout = BCRecordLayout<
    ENTRY_POINT,
    DeclIDField  // the ID of the main class; 0 if there was a main source file
  >;
}

/// \sa DECL_MEMBER_TABLES_BLOCK_ID
namespace decl_member_tables_block {
  enum RecordKind {
    DECL_MEMBERS = 1,
  };

  using DeclMembersLayout = BCRecordLayout<
    DECL_MEMBERS, // record ID
    BCVBR<16>,  // table offset within the blob (see below)
    BCBlob  // maps from DeclIDs to DeclID vectors
  >;

}

/// \sa COMMENT_BLOCK_ID
namespace comment_block {
  enum RecordKind {
    DECL_COMMENTS = 1,
    GROUP_NAMES = 2,
  };

  using DeclCommentListLayout = BCRecordLayout<
    DECL_COMMENTS, // record ID
    BCVBR<16>,     // table offset within the blob (see below)
    BCBlob         // map from Decl IDs to comments
  >;

  using GroupNamesLayout = BCRecordLayout<
    GROUP_NAMES,    // record ID
    BCBlob          // actual names
  >;

} // namespace comment_block

} // end namespace serialization
} // end namespace swift

#endif
