// RUN: rm -rf %t
// RUN: mkdir -p %t
// RUN: %target-swiftc_driver -emit-library -emit-module -emit-module-path %t/empty.swiftmodule -module-name empty -module-link-name empty %S/empty.swift
// RUN: %target-swiftc_driver -c %s -I %t -o %t/import_experimental.o
// RUN: llvm-ar cr %t/import_experimental.a %t/import_experimental.o
// RUN: %target-swift-autolink-extract %t/import_experimental.a -o - | %FileCheck --check-prefix CHECK-%target-object-format %s

// REQUIRES: autolink-extract

// CHECK-elf-DAG: -lswiftCore
// CHECK-elf-DAG: -lempty

import empty
