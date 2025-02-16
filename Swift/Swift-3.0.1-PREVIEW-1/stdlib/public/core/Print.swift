//===--- Print.swift ------------------------------------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

/// Writes the textual representations of the given items into the standard
/// output.
///
/// You can pass zero or more items to the `print(_:separator:terminator:)`
/// function. The textual representation for each item is the same as that
/// obtained by calling `String(item)`. The following example prints a string,
/// a closed range of integers, and a group of floating-point values to
/// standard output:
///
///     print("One two three four five")
///     // Prints "One two three four five"
///
///     print(1...5)
///     // Prints "1...5"
///
///     print(1.0, 2.0, 3.0, 4.0, 5.0)
///     // Prints "1.0 2.0 3.0 4.0 5.0"
///
/// To print the items separated by something other than a space, pass a string
/// as `separator`.
///
///     print(1.0, 2.0, 3.0, 4.0, 5.0, separator: " ... ")
///     // Prints "1.0 ... 2.0 ... 3.0 ... 4.0 ... 5.0"
///
/// The output from each call to `print(_:separator:terminator:)` includes a
/// newline by default. To print the items without a trailing newline, pass an
/// empty string as `terminator`.
///
///     for n in 1...5 {
///         print(n, terminator: "")
///     }
///     // Prints "12345"
///
/// - Parameters:
///   - items: Zero or more items to print.
///   - separator: A string to print between each item. The default is a single
///     space (`" "`).
///   - terminator: The string to print after all items have been printed. The
///     default is a newline (`"\n"`).
///
/// - SeeAlso: `debugPrint(_:separator:terminator:)`, `TextOutputStreamable`,
///   `CustomStringConvertible`, `CustomDebugStringConvertible`
@inline(never)
@_semantics("stdlib_binary_only")
public func print(
  _ items: Any...,
  separator: String = " ",
  terminator: String = "\n"
) {
  if let hook = _playgroundPrintHook {
    var output = _TeeStream(left: "", right: _Stdout())
    _print(
      items, separator: separator, terminator: terminator, to: &output)
    hook(output.left)
  }
  else {
    var output = _Stdout()
    _print(
      items, separator: separator, terminator: terminator, to: &output)
  }
}

/// Writes the textual representations of the given items most suitable for
/// debugging into the standard output.
///
/// You can pass zero or more items to the
/// `debugPrint(_:separator:terminator:)` function. The textual representation
/// for each item is the same as that obtained by calling
/// `String(reflecting: item)`. The following example prints the debugging
/// representation of a string, a closed range of integers, and a group of
/// floating-point values to standard output:
///
///     debugPrint("One two three four five")
///     // Prints "One two three four five"
///
///     debugPrint(1...5)
///     // Prints "CountableClosedRange(1...5)"
///
///     debugPrint(1.0, 2.0, 3.0, 4.0, 5.0)
///     // Prints "1.0 2.0 3.0 4.0 5.0"
///
/// To print the items separated by something other than a space, pass a string
/// as `separator`.
///
///     debugPrint(1.0, 2.0, 3.0, 4.0, 5.0, separator: " ... ")
///     // Prints "1.0 ... 2.0 ... 3.0 ... 4.0 ... 5.0"
///
/// The output from each call to `debugPrint(_:separator:terminator:)` includes
/// a newline by default. To print the items without a trailing newline, pass
/// an empty string as `terminator`.
///
///     for n in 1...5 {
///         debugPrint(n, terminator: "")
///     }
///     // Prints "12345"
///
/// - Parameters:
///   - items: Zero or more items to print.
///   - separator: A string to print between each item. The default is a single
///     space (`" "`).
///   - terminator: The string to print after all items have been printed. The
///     default is a newline (`"\n"`).
///
/// - SeeAlso: `print(_:separator:terminator:)`, `TextOutputStreamable`,
///   `CustomStringConvertible`, `CustomDebugStringConvertible`
@inline(never)
@_semantics("stdlib_binary_only")
public func debugPrint(
  _ items: Any...,
  separator: String = " ",
  terminator: String = "\n") {
  if let hook = _playgroundPrintHook {
    var output = _TeeStream(left: "", right: _Stdout())
    _debugPrint(
      items, separator: separator, terminator: terminator, to: &output)
    hook(output.left)
  }
  else {
    var output = _Stdout()
    _debugPrint(
      items, separator: separator, terminator: terminator, to: &output)
  }
}

/// Writes the textual representations of the given items into the given output
/// stream.
///
/// You can pass zero or more items to the `print(_:separator:terminator:to:)`
/// function. The textual representation for each item is the same as that
/// obtained by calling `String(item)`. The following example prints a closed
/// range of integers to a string:
///
///     var range = "My range: "
///     print(1...5, to: &range)
///     // range == "My range: 1...5\n"
///
/// To print the items separated by something other than a space, pass a string
/// as `separator`.
///
///     var separated = ""
///     print(1.0, 2.0, 3.0, 4.0, 5.0, separator: " ... ", to: &separated)
///     // separated == "1.0 ... 2.0 ... 3.0 ... 4.0 ... 5.0\n"
///
/// The output from each call to `print(_:separator:terminator:to:)` includes a
/// newline by default. To print the items without a trailing newline, pass an
/// empty string as `terminator`.
///
///     var numbers = ""
///     for n in 1...5 {
///         print(n, terminator: "", to: &numbers)
///     }
///     // numbers == "12345"
///
/// - Parameters:
///   - items: Zero or more items to print.
///   - separator: A string to print between each item. The default is a single
///     space (`" "`).
///   - terminator: The string to print after all items have been printed. The
///     default is a newline (`"\n"`).
///   - output: An output stream to receive the text representation of each
///     item.
///
/// - SeeAlso: `print(_:separator:terminator:)`,
///   `debugPrint(_:separator:terminator:to:)`, `TextOutputStream`,
///   `TextOutputStreamable`, `CustomStringConvertible`, `CustomDebugStringConvertible`
@inline(__always)
public func print<Target : TextOutputStream>(
  _ items: Any...,
  separator: String = " ",
  terminator: String = "\n",
  to output: inout Target
) {
  _print(items, separator: separator, terminator: terminator, to: &output)
}

/// Writes the textual representations of the given items most suitable for
/// debugging into the given output stream.
///
/// You can pass zero or more items to the
/// `debugPrint(_:separator:terminator:to:)` function. The textual
/// representation for each item is the same as that obtained by calling
/// `String(reflecting: item)`. The following example prints a closed range of
/// integers to a string:
///
///     var range = "My range: "
///     debugPrint(1...5, to: &range)
///     // range == "My range: CountableClosedRange(1...5)\n"
///
/// To print the items separated by something other than a space, pass a string
/// as `separator`.
///
///     var separated = ""
///     debugPrint(1.0, 2.0, 3.0, 4.0, 5.0, separator: " ... ", to: &separated)
///     // separated == "1.0 ... 2.0 ... 3.0 ... 4.0 ... 5.0\n"
///
/// The output from each call to `debugPrint(_:separator:terminator:to:)`
/// includes a newline by default. To print the items without a trailing
/// newline, pass an empty string as `terminator`.
///
///     var numbers = ""
///     for n in 1...5 {
///         debugPrint(n, terminator: "", to: &numbers)
///     }
///     // numbers == "12345"
///
/// - Parameters:
///   - items: Zero or more items to print.
///   - separator: A string to print between each item. The default is a single
///     space (`" "`).
///   - terminator: The string to print after all items have been printed. The
///     default is a newline (`"\n"`).
///   - output: An output stream to receive the text representation of each
///     item.
///
/// - SeeAlso: `debugPrint(_:separator:terminator:)`,
///   `print(_:separator:terminator:to:)`, `TextOutputStream`, `TextOutputStreamable`,
///   `CustomStringConvertible`, `CustomDebugStringConvertible`
@inline(__always)
public func debugPrint<Target : TextOutputStream>(
  _ items: Any...,
  separator: String = " ",
  terminator: String = "\n",
  to output: inout Target
) {
  _debugPrint(
    items, separator: separator, terminator: terminator, to: &output)
}

@_versioned
@inline(never)
@_semantics("stdlib_binary_only")
internal func _print<Target : TextOutputStream>(
  _ items: [Any],
  separator: String = " ",
  terminator: String = "\n",
  to output: inout Target
) {
  var prefix = ""
  output._lock()
  defer { output._unlock() }
  for item in items {
    output.write(prefix)
    _print_unlocked(item, &output)
    prefix = separator
  }
  output.write(terminator)
}

@_versioned
@inline(never)
@_semantics("stdlib_binary_only")
internal func _debugPrint<Target : TextOutputStream>(
  _ items: [Any],
  separator: String = " ",
  terminator: String = "\n",
  to output: inout Target
) {
  var prefix = ""
  output._lock()
  defer { output._unlock() }
  for item in items {
    output.write(prefix)
    _debugPrint_unlocked(item, &output)
    prefix = separator
  }
  output.write(terminator)
}

//===----------------------------------------------------------------------===//
//===--- Migration Aids ---------------------------------------------------===//

@available(*, unavailable, renamed: "print(_:separator:terminator:to:)")
public func print<Target : TextOutputStream>(
  _ items: Any...,
  separator: String = "",
  terminator: String = "",
  toStream output: inout Target
) {}

@available(*, unavailable, renamed: "debugPrint(_:separator:terminator:to:)")
public func debugPrint<Target : TextOutputStream>(
  _ items: Any...,
  separator: String = "",
  terminator: String = "",
  toStream output: inout Target
) {}

@available(*, unavailable, message: "Please use 'terminator: \"\"' instead of 'appendNewline: false': 'print((...), terminator: \"\")'")
public func print<T>(_: T, appendNewline: Bool = true) {}
@available(*, unavailable, message: "Please use 'terminator: \"\"' instead of 'appendNewline: false': 'debugPrint((...), terminator: \"\")'")
public func debugPrint<T>(_: T, appendNewline: Bool = true) {}


//===--- FIXME: Not working due to <rdar://22101775> ----------------------===//
@available(*, unavailable, message: "Please use the 'to' label for the target stream: 'print((...), to: &...)'")
public func print<T>(_: T, _: inout TextOutputStream) {}
@available(*, unavailable, message: "Please use the 'to' label for the target stream: 'debugPrint((...), to: &...))'")
public func debugPrint<T>(_: T, _: inout TextOutputStream) {}

@available(*, unavailable, message: "Please use 'terminator: \"\"' instead of 'appendNewline: false' and use the 'to' label for the target stream: 'print((...), terminator: \"\", to: &...)'")
public func print<T>(_: T, _: inout TextOutputStream, appendNewline: Bool = true) {}
@available(*, unavailable, message: "Please use 'terminator: \"\"' instead of 'appendNewline: false' and use the 'to' label for the target stream: 'debugPrint((...), terminator: \"\", to: &...)'")
public func debugPrint<T>(
  _: T, _: inout TextOutputStream, appendNewline: Bool = true
) {}
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
