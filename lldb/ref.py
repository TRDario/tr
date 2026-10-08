# LLDB formatter script for `tr::ref`.

import lldb

class TrRefSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		self.target_valobj = self.valobj.GetChildMemberWithName("m_base").Dereference()
		return True
	
	def has_children(self) -> bool:
		return self.target_valobj.MightHaveChildren()

	def num_children(self) -> int:
		return self.target_valobj.GetNumChildren()
	
	def get_child_index(self, name: str) -> int:
		return self.target_valobj.GetIndexOfChildWithName(name)

	def get_child_at_index(self, index: int) -> lldb.SBValue | None:
		return self.target_valobj.GetChildAtIndex(index)

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type synthetic add -w tr --regex --python-class ref.TrRefSyntheticProvider "^tr::ref<.+>$"')