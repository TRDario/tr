# LLDB formatter script for `tr::string_pool`.

import lldb

def _string_pool_children(valobj: lldb.SBValue) -> list[lldb.SBValue]:
	valobj = valobj.GetNonSyntheticValue()
	target = valobj.GetTarget()
	data = valobj.GetChildMemberWithName("m_data").GetSyntheticValue()
	size = data.GetNumChildren()
	if size == 0:
		return []
	
	base_address = data.GetChildAtIndex(0).GetLoadAddress()
	error = lldb.SBError()
	buffer = target.GetProcess().ReadMemory(base_address, size, error)
	if error.Fail():
		return []
	
	char_type = target.FindFirstType("char")
	children = []
	begin = 0
	for index, char in enumerate(buffer):
		if char == 0:
			string_address = target.ResolveLoadAddress(base_address + begin)
			string_type = char_type.GetArrayType(index - begin + 1)
			children.append(target.CreateValueFromAddress(f"[{len(children)}]", string_address, string_type))
			begin = index + 1
	if begin < size:
		string_address = target.ResolveLoadAddress(base_address + begin)
		string_type = char_type.GetArrayType(size - begin)
		children.append(target.CreateValueFromAddress(f"[{len(children)}]", string_address, string_type))
	return children

def tr_string_pool_summary(valobj: lldb.SBValue, _) -> str:
	return f"size={len(_string_pool_children(valobj))}"

def tr_string_pool_iterator_summary(valobj: lldb.SBValue, _):
	return valobj.EvaluateExpression("**this").GetSummary()

class TrStringPoolSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		self.children = _string_pool_children(self.valobj)
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
	debugger.HandleCommand('type summary add -w tr --python-function string_pool.tr_string_pool_summary tr::string_pool')
	debugger.HandleCommand('type summary add -w tr --python-function string_pool.tr_string_pool_iterator_summary tr::string_pool::iterator')
	debugger.HandleCommand('type synthetic add -w tr --python-class string_pool.TrStringPoolSyntheticProvider tr::string_pool')