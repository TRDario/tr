# LLDB formatter script for `tr::inplace_string`.

import lldb

def tr_inplace_string_summary(valobj: lldb.SBValue, _) -> str:
	size = valobj.GetChildMemberWithName("m_size").GetValueAsUnsigned()
	chars = b''
	if size > 0:
		process = valobj.GetTarget().GetProcess()
		address = valobj.GetChildMemberWithName("m_buffer").GetLoadAddress().GetValueAsUnsigned()
		error = lldb.SBError()
		chars = process.ReadMemory(address, size, error)
		if error.Fail():
			return "<invalid>"
	return f'"{chars.decode("utf-8")}"'

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type summary add -w tr --regex --python-function inplace_string.tr_inplace_string_summary "^tr::inplace_string<.+>$"')