# LLDB formatter script for `tr::utf8` iterators.

import lldb

def tr_utf8_iterator_summary(valobj: lldb.SBValue, _) -> str:
	address = valobj.GetChildMemberWithName("m_ptr").GetValueAsUnsigned()
	if address == 0:
		return "<null>"
	
	process = valobj.GetTarget().GetProcess()
	error = lldb.SBError()
	chars = []
	while not chars or chars[-1] >= 192:
		byte = process.ReadMemory(address, 1, error)
		if error.Fail():
			break
		chars.append(int(byte[0]))
		address += 1
	return f"'{bytes(chars).decode("utf-8", errors = "replace")}'" if chars else "<invalid>"

def tr_utf8_indexed_iterator_summary(valobj: lldb.SBValue, _) -> str:
	index = valobj.GetChildMemberWithName("m_index").GetValueAsSigned()
	address = valobj.GetChildMemberWithName("m_ptr").GetValueAsUnsigned()
	if address == 0:
		return f"<null> (index={index})"

	process = valobj.GetTarget().GetProcess()
	error = lldb.SBError()
	chars = []
	while not chars or chars[-1] >= 192:
		byte = process.ReadMemory(address, 1, error)
		if error.Fail():
			break
		chars.append(int(byte[0]))
		address += 1
	return f"'{bytes(chars).decode("utf-8")}' (index={index})" if chars else f"<invalid> (index={index})"

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type summary add -w tr --python-function utf8.tr_utf8_iterator_summary tr::utf8::iterator')
	debugger.HandleCommand('type summary add -w tr --python-function utf8.tr_utf8_indexed_iterator_summary tr::utf8::indexed_iterator')