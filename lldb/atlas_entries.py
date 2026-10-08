# LLDB formatter script for `tr::atlas_entries`.

import lldb

class TrAtlasEntriesSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		self.target_valobj = self.valobj.GetChildMemberWithName("m_entries").GetSyntheticValue()
		return True
	
	def has_children(self) -> bool:
		return self.target_valobj.MightHaveChildren()

	def num_children(self, max_children: int) -> int:
		return min(self.target_valobj.GetNumChildren(), max_children)
	
	def get_child_index(self, name: str) -> int:
		return self.target_valobj.GetIndexOfChildWithName(name)

	def get_child_at_index(self, index: int) -> lldb.SBValue | None:
		if 0 <= index < self.target_valobj.GetNumChildren():
			return self.target_valobj.GetChildAtIndex(index)
		return None

	def get_value(self) -> lldb.SBValue:
		return self.valobj

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type synthetic add -w tr --regex --python-class atlas_entries.TrAtlasEntriesSyntheticProvider "^tr::atlas_entries<.+>$"')