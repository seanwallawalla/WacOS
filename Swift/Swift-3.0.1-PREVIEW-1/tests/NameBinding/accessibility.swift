// RUN: rm -rf %t && mkdir -p %t
// RUN: cp %s %t/main.swift

// RUN: %target-swift-frontend -parse -primary-file %t/main.swift %S/Inputs/accessibility_other.swift -module-name accessibility -enable-source-import -I %S/Inputs -sdk "" -enable-access-control -verify
// RUN: %target-swift-frontend -parse -primary-file %t/main.swift %S/Inputs/accessibility_other.swift -module-name accessibility -enable-source-import -I %S/Inputs -sdk "" -disable-access-control -D DEFINE_VAR_FOR_SCOPED_IMPORT -D ACCESS_DISABLED

// RUN: %target-swift-frontend -emit-module -o %t %S/Inputs/has_accessibility.swift -D DEFINE_VAR_FOR_SCOPED_IMPORT -enable-testing
// RUN: %target-swift-frontend -parse -primary-file %t/main.swift %S/Inputs/accessibility_other.swift -module-name accessibility -I %t -sdk "" -enable-access-control -verify
// RUN: %target-swift-frontend -parse -primary-file %t/main.swift %S/Inputs/accessibility_other.swift -module-name accessibility -I %t -sdk "" -disable-access-control -D ACCESS_DISABLED
// RUN: not %target-swift-frontend -parse -primary-file %t/main.swift %S/Inputs/accessibility_other.swift -module-name accessibility -I %t -sdk "" -D TESTABLE 2>&1 | %FileCheck -check-prefix=TESTABLE %s

#if TESTABLE
@testable import has_accessibility
#else
import has_accessibility
#endif

// This deliberately has the wrong import kind.
import var has_accessibility.zz // expected-error {{no such decl in module}}

func markUsed<T>(_ t: T) {}

markUsed(has_accessibility.x)
markUsed(has_accessibility.y) // expected-error {{module 'has_accessibility' has no member named 'y'}}
markUsed(has_accessibility.z) // expected-error {{module 'has_accessibility' has no member named 'z'}}
// TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
// TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
// TESTABLE: :[[@LINE-3]]:10: error: module 'has_accessibility' has no member named 'z'

markUsed(accessibility.a)
markUsed(accessibility.b)
markUsed(accessibility.c) // expected-error {{module 'accessibility' has no member named 'c'}}

markUsed(x)
markUsed(y) // expected-error {{use of unresolved identifier 'y'}}
markUsed(z) // expected-error {{use of unresolved identifier 'z'}}
// TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
// TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
// TESTABLE: :[[@LINE-3]]:10: error: use of unresolved identifier 'z'

markUsed(a)
markUsed(b)
markUsed(c) // expected-error {{use of unresolved identifier 'c'}}

Foo.x()
Foo.y() // expected-error {{'y' is inaccessible due to 'internal' protection level}}
Foo.z() // expected-error {{'z' is inaccessible due to 'private' protection level}}
// TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
// TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
// TESTABLE: :[[@LINE-3]]:{{[^:]+}}: error: 'z' is inaccessible due to 'private' protection level
Foo.a()
Foo.b()
Foo.c() // expected-error {{'c' is inaccessible due to 'private' protection level}}

_ = Foo() // expected-error {{'Foo' initializer is inaccessible due to 'internal' protection level}}
// TESTABLE-NOT: :[[@LINE-1]]:{{[^:]+}}:
_ = PrivateInit() // expected-error {{'PrivateInit' initializer is inaccessible due to 'private' protection level}}
// TESTABLE: :[[@LINE-1]]:{{[^:]+}}: error: 'PrivateInit' initializer is inaccessible due to 'private' protection level

var s = StructWithPrivateSetter()
s.x = 42 // expected-error {{cannot assign to property: 'x' setter is inaccessible}}

class Sub : Base {
  func test() {
    value = 4 // expected-error {{cannot assign to property: 'value' setter is inaccessible}}
    self.value = 4 // expected-error {{cannot assign to property: 'value' setter is inaccessible}}
    super.value = 4 // expected-error {{cannot assign to property: 'value' setter is inaccessible}}
    // TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
    // TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
    // TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:

    method() // expected-error {{use of unresolved identifier 'method'}}
    self.method() // expected-error {{'method' is inaccessible due to 'internal' protection level}}
    super.method() // expected-error {{'method' is inaccessible due to 'internal' protection level}}
    // TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
    // TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
    // TESTABLE-NOT: :[[@LINE-3]]:{{[^:]+}}:
  }
}

class ObservingOverrider : Base {
  override var value: Int { // expected-error {{cannot observe read-only property 'value'; it can't change}}
    willSet { markUsed(newValue) }
  }
}

class ReplacingOverrider : Base {
  override var value: Int {
    get { return super.value }
    set { super.value = newValue } // expected-error {{cannot assign to property: 'value' setter is inaccessible}}
  }
}

protocol MethodProto {
  func method() // expected-note * {{protocol requires function 'method()' with type '() -> ()'}}
}

extension OriginallyEmpty : MethodProto {}
// TESTABLE-NOT: :[[@LINE-1]]:{{[^:]+}}:
#if !ACCESS_DISABLED
extension HiddenMethod : MethodProto {} // expected-error {{type 'HiddenMethod' does not conform to protocol 'MethodProto'}}

extension Foo : MethodProto {} // expected-error {{type 'Foo' does not conform to protocol 'MethodProto'}}
#endif


protocol TypeProto {
  associatedtype TheType // expected-note * {{protocol requires nested type 'TheType'}}
}

extension OriginallyEmpty {}
#if !ACCESS_DISABLED
extension HiddenType : TypeProto {} // expected-error {{type 'HiddenType' does not conform to protocol 'TypeProto'}}
// TESTABLE-NOT: :[[@LINE-1]]:{{[^:]+}}:

extension Foo : TypeProto {} // expected-error {{type 'Foo' does not conform to protocol 'TypeProto'}}
#endif


#if !ACCESS_DISABLED
private func privateInBothFiles() {} // no-warning
private func privateInPrimaryFile() {} // expected-error {{invalid redeclaration}}
func privateInOtherFile() {} // expected-note {{previously declared here}}
#endif


#if !ACCESS_DISABLED
// rdar://problem/21408035
private class PrivateBox<T> { // expected-note 2 {{type declared here}}
  typealias ValueType = T
  typealias AlwaysFloat = Float
}

let boxUnboxInt: PrivateBox<Int>.ValueType = 0 // expected-error {{constant must be declared fileprivate because its type uses a fileprivate type}}
let boxFloat: PrivateBox<Int>.AlwaysFloat = 0 // expected-error {{constant must be declared fileprivate because its type uses a fileprivate type}}
#endif


#if !ACCESS_DISABLED
struct ConformerByTypeAlias : TypeProto {
  private typealias TheType = Int // expected-error {{type alias 'TheType' must be declared internal because it matches a requirement in internal protocol 'TypeProto'}} {{3-10=internal}}
}

struct ConformerByLocalType : TypeProto {
  private struct TheType {} // expected-error {{struct 'TheType' must be declared internal because it matches a requirement in internal protocol 'TypeProto'}} {{3-10=internal}}
}

private struct PrivateConformerByLocalType : TypeProto {
  struct TheType {} // okay
}

private struct PrivateConformerByLocalTypeBad : TypeProto {
  private struct TheType {} // expected-error {{struct 'TheType' must be as accessible as its enclosing type because it matches a requirement in protocol 'TypeProto'}} {{3-10=fileprivate}}
}
#endif

