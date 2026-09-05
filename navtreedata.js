/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "tr", "index.html", [
    [ "Configuration and Building", "md_pages_2building.html", [
      [ "Tooling", "md_pages_2building.html#autotoc_md1", null ],
      [ "Modules", "md_pages_2building.html#autotoc_md2", [
        [ "Utility module", "md_pages_2building.html#autotoc_md3", null ],
        [ "Audio module", "md_pages_2building.html#autotoc_md4", null ],
        [ "System and graphics module", "md_pages_2building.html#autotoc_md5", null ],
        [ "ImGui integration module", "md_pages_2building.html#autotoc_md6", null ]
      ] ],
      [ "Compile definitions", "md_pages_2building.html#autotoc_md7", null ],
      [ "Documentation", "md_pages_2building.html#autotoc_md8", null ]
    ] ],
    [ "CMake Library", "md_pages_2cmake.html", [
      [ "Definitions", "md_pages_2cmake.html#autotoc_md10", [
        [ "<span class=\"tt\">TR_HAS_GCC</span>, <span class=\"tt\">TR_HAS_CLANG</span>, <span class=\"tt\">TR_HAS_CLANG_CL</span>, <span class=\"tt\">TR_HAS_MSVC</span>", "md_pages_2cmake.html#autotoc_md11", null ],
        [ "<span class=\"tt\">TR_DO_NOTHING</span>", "md_pages_2cmake.html#autotoc_md12", null ]
      ] ],
      [ "Functions", "md_pages_2cmake.html#autotoc_md13", [
        [ "<span class=\"tt\">tr_generate_embeddable_string(TARGET, INPUT_FILE, OUTPUT_FILE, VARIABLE_NAME)</span>", "md_pages_2cmake.html#autotoc_md14", null ],
        [ "<span class=\"tt\">tr_generate_embeddable_binary(TARGET, INPUT_FILE, OUTPUT_FILE, VARIABLE_NAME)</span>", "md_pages_2cmake.html#autotoc_md15", null ],
        [ "<span class=\"tt\">tr_git_hash_setup()</span>", "md_pages_2cmake.html#autotoc_md16", null ],
        [ "<span class=\"tt\">tr_target_template(TARGET)</span>", "md_pages_2cmake.html#autotoc_md17", null ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", "namespacemembers_dup" ],
        [ "Functions", "namespacemembers_func.html", "namespacemembers_func" ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"alignment_8hpp.html",
"classtr_1_1audio__source.html#a0ec6886d84dc19803f8d65b016637988",
"classtr_1_1basic__renderer.html#af1e5f2bd873dc9daca497e3befb2792d",
"classtr_1_1bitmap_1_1iterator.html#a50472af9a64a00c7c7e6446310cb48bc",
"classtr_1_1dyn__atlas.html#aa0d274dce1125b3694cfc44e50a27ee0",
"classtr_1_1graphics__context.html#a23c270da8a15a9ea782723c70eff0404",
"classtr_1_1omstream.html#a7bba20d589b17f5f1e2b79f7c54f4a23",
"classtr_1_1scan__state.html#a3aacae5315b54820574618da2e8f04e8",
"classtr_1_1static__string.html#a08c0247f1291aa4f09452ad389b29379",
"classtr_1_1sub__bitmap.html#a52ef190e540f1591939a5924d3138555",
"classtr_1_1vertex__format.html#a07cc01bd28ed9b2fa3ed529c9f0b4add",
"concepttr_1_1volatile__qualified.html",
"namespacetr.html#a1ea67ac4cd166f033972b19f49e81323",
"namespacetr.html#ac1babae6e9367fc90585a8791dadc746ac87d0f704bc006892960956b017dbdc0",
"namespacetr_1_1literals_1_1angle__literals.html#a47e5023a5af2ba15ec678c8845272974",
"structtr_1_1blend__mode.html",
"structtr_1_1rectangle.html#a638f36394d275ed28be3690d77bb0161",
"structtr_1_1scan__chord.html#ab7934b582c91919092752a453e817406"
];

const SYNCONMSG = 'click to disable panel synchronization';
const SYNCOFFMSG = 'click to enable panel synchronization';
const LISTOFALLMEMBERS = 'List of all members';