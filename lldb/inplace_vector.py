# LLDB formatter script for `tr::inplace_vector`.

import lldb

def tr_inplace_vector_summary(valobj: lldb.SBValue, _) -> str:
	return f'size={valobj.GetNonSyntheticValue().GetChildMemberWithName("m_size").GetValueAsUnsigned()}'

class TrInplaceVectorSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		self.children = []

		size = self.valobj.GetChildMemberWithName("m_size").GetValueAsUnsigned()
		if size == 0:
			return True

		element_type = self.valobj.GetType().GetTemplateArgumentType(0)
		element_size = element_type.GetByteSize()
		base_address = self.valobj.GetChildMemberWithName("m_buffer").GetLoadAddress()
		target = self.valobj.GetTarget()
		for index in range(size):
			address = base_address + index * element_size
			self.children.append(target.CreateValueFromAddress(f"[{index}]", target.ResolveLoadAddress(address), element_type))
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
	debugger.HandleCommand('type summary add -w tr --regex --python-function inplace_vector.tr_inplace_vector_summary "^tr::inplace_vector<.+>$"')
	debugger.HandleCommand('type synthetic add -w tr --regex --python-class inplace_vector.TrInplaceVectorSyntheticProvider "^tr::inplace_vector<.+>$"')