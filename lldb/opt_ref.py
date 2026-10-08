# LLDB formatter script for `tr::opt_ref`.

import lldb

def tr_opt_ref_summary(valobj: lldb.SBValue, _) -> str:
	has_value = valobj.EvaluateExpression("has_value()").GetValueAsUnsigned()
	return valobj.GetSummary() if has_value else "<empty>"

class TrOptRefSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		m_base = self.valobj.GetChildMemberWithName("m_base")
		if m_base.GetValueAsUnsigned(0) != 0:
			self.target_valobj = m_base.Dereference()
		else:
			self.target_valobj = None
		return True
	
	def has_children(self) -> bool:
		return self.target_valobj.MightHaveChildren() if self.target_valobj is not None else False

	def num_children(self) -> int:
		return self.target_valobj.GetNumChildren() if self.target_valobj is not None else 0
	
	def get_child_index(self, name: str) -> int:
		return self.target_valobj.GetIndexOfChildWithName(name) if self.target_valobj is not None else -1

	def get_child_at_index(self, index: int) -> lldb.SBValue | None:
		return self.target_valobj.GetChildAtIndex(index) if self.target_valobj is not None else None

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type summary add -w tr --regex --python-function opt_ref.tr_opt_ref_summary "^tr::opt_ref<.+>$"')
	debugger.HandleCommand('type synthetic add -w tr --regex --python-class opt_ref.TrOptRefSyntheticProvider "^tr::opt_ref<.+>$"')