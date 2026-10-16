# LLDB formatter script for `tr::basic_zstring_view_zspan`.

import sys
import lldb

def tr_basic_zstring_view_zspan_summary(valobj: lldb.SBValue, _) -> str:
	address = valobj.GetChildMemberWithName("m_ptr").GetValueAsUnsigned()
	if address == 0:
		return "size=0"

	size = 0
	process = valobj.GetTarget().GetProcess()
	error = lldb.SBError()
	string_address = int.from_bytes(process.ReadMemory(address + size * 8, 8, error), byteorder=sys.byteorder)
	while not error.Fail() and string_address != 0:
		size += 1
		string_address = int.from_bytes(process.ReadMemory(address + size * 8, 8, error), byteorder=sys.byteorder)
	return f"size={size}"

class TrBasicZstringViewZspanSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()

	def update(self) -> bool:
		self.children = []
		address = self.valobj.GetChildMemberWithName("m_ptr").GetValueAsUnsigned()
		if address == 0:
			return True

		target = self.valobj.GetTarget()
		process = target.GetProcess()
		error = lldb.SBError()
		string_address = int.from_bytes(process.ReadMemory(address, 8, error), byteorder=sys.byteorder)
		while not error.Fail() and string_address != 0:
			self.children.append(target.CreateValueFromExpression(f"[{len(self.children)}]", f"(const char*){string_address}"))
			string_address = int.from_bytes(process.ReadMemory(address + len(self.children) * 8, 8, error), byteorder=sys.byteorder)
		return True

	def has_children(self) -> bool:
		return bool(self.children)

	def num_children(self, max_children: int) -> int:
		return min(len(self.children), max_children)

	def get_child_index(self, name: str) -> int:
		return -1

	def get_child_at_index(self, index: int) -> lldb.SBValue | None:
		if 0 <= index < len(self.children):
			return self.children[index]
		return None

	def get_value(self) -> lldb.SBValue:
		return self.valobj

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type summary add -w tr --regex --python-function basic_zstring_view_zspan.tr_basic_zstring_view_zspan_summary "^tr::basic_zstring_view_zspan<.+>$"')
	debugger.HandleCommand('type synthetic add -w tr --regex --python-class basic_zstring_view_zspan.TrBasicZstringViewZspanSyntheticProvider "^tr::basic_zstring_view_zspan<.+>$"')