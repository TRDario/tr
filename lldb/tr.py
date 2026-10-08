# LLDB tr formatter script loader.

import lldb
import os
import sys

def __lldb_init_module(debugger: lldb.SBDebugger, _):
	directory = os.path.dirname(os.path.abspath(__file__))
	if directory not in sys.path:
		sys.path.insert(0, directory)

	files = [
		"atlas_entries",
		"boost_unordered",
		"graphics_object",
		"inplace_string",
		"inplace_vector",
		"localization_map",
		"lock_free_queue",
		"opt_ref",
		"ref",
		"string_pool",
		"utf8"
	]
	for file in files:
		debugger.HandleCommand(f'command script import "{os.path.join(directory, f"{file}.py")}"')
	debugger.HandleCommand(f'command source "{os.path.join(directory, "commands")}"')
	print("Successfully loaded tr LLDB formatters.")