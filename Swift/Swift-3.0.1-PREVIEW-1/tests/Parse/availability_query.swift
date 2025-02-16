// RUN: %target-parse-verify-swift

// REQUIRES: OS=macosx

if #available(OSX 10.51, *) {
}

// Disallow use as an expression.
if (#available(OSX 10.51, *)) {}  // expected-error {{#available may only be used as condition of an 'if', 'guard'}}

let x = #available(OSX 10.51, *)  // expected-error {{#available may only be used as condition of}}

(#available(OSX 10.51, *) ? 1 : 0) // expected-error {{#available may only be used as condition of an}}

if !#available(OSX 10.52, *) { // expected-error {{#available may only be used as condition of an}}
}
if let _ = Optional(5), !#available(OSX 10.52, *) { // expected-error {{#available may only be used as condition}}
}

if #available(OSX 10.51, *) && #available(OSX 10.52, *) { // expected-error {{expected ',' joining parts of a multi-clause condition}} {{29-31=,}}
}


if #available { // expected-error {{expected availability condition}} expected-error {{braced block of statements is an unused closure}} expected-error {{statement cannot begin with a closure expression}} expected-note {{explicitly discard the result of the closure by assigning to '_'}} {{15-15=_ = }} expected-error {{expression resolves to an unused function}}
}

if #available( { // expected-error {{expected platform name}} expected-error {{expected ')'}} expected-note {{to match this opening '('}}
}

if #available() { // expected-error {{expected platform name}}
}

if #available(OSX { // expected-error {{expected version number}} expected-error {{expected ')'}} expected-note {{to match this opening '('}}
}

if #available(OSX) { // expected-error {{expected version number}}
}

if #available(OSX 10.51 { // expected-error {{expected ')'}} expected-note {{to match this opening '('}} expected-error {{must handle potential future platforms with '*'}} {{24-24=, *}}
}

if #available(iDishwasherOS 10.51) { // expected-error {{unrecognized platform name 'iDishwasherOS'}}
}

if #available(iDishwasherOS 10.51, *) { // expected-error {{unrecognized platform name 'iDishwasherOS'}}
}

if #available(OSX 10.51, OSX 10.52, *) {  // expected-error {{version for 'OSX' already specified}}
}

if #available(OSX 10.52) { }  // expected-error {{must handle potential future platforms with '*'}} {{24-24=, *}}

if #available(OSX 10.51, iOS 8.0) { }  // expected-error {{must handle potential future platforms with '*'}} {{33-33=, *}}

if #available(iOS 8.0, *) {
}

// Want to make sure we can parse this. Perhaps we should not let this validate, though.
if #available(*) {
}

if #available(* { // expected-error {{expected ')' in availability query}} expected-note {{to match this opening '('}}
}

// Multiple platforms
if #available(OSX 10.51, iOS 8.0, *) {
}


if #available(OSX 10.51, { // expected-error {{expected platform name}} // expected-error {{expected ')'}} expected-note {{to match this opening '('}}
}

if #available(OSX 10.51,) { // expected-error {{expected platform name}}
}

if #available(OSX 10.51, iOS { // expected-error {{expected version number}} // expected-error {{expected ')'}} expected-note {{to match this opening '('}}
}

if #available(OSX 10.51, iOS 8.0, iDishwasherOS 10.51) { // expected-error {{unrecognized platform name 'iDishwasherOS'}}
}

if #available(iDishwasherOS 10.51, OSX 10.51) { // expected-error {{unrecognized platform name 'iDishwasherOS'}}
}

if #available(OSX 10.51 || iOS 8.0) {// expected-error {{'||' cannot be used in an availability condition}}
}

// Emit Fix-It removing un-needed >=, for the moment.

if #available(OSX >= 10.51, *) { // expected-error {{version comparison not needed}} {{19-22=}}
}

// <rdar://problem/20904820> Following a "let" condition with #available is incorrectly rejected

// Bool then #available.
if 1 != 2, #available(iOS 8.0, *) {}

// Pattern then #available(iOS 8.0, *) {
if case 42 = 42, #available(iOS 8.0, *) {}
if let x = Optional(42), #available(iOS 8.0, *) {}

// Allow "macOS" as well.
if #available(macOS 10.51, *) {
}

