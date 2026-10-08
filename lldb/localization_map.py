# LLDB formatter script for `tr::localization_map`.

import lldb

def _display_value(value: lldb.SBValue) -> str:
	summary = value.GetSummary()
	if summary is not None:
		return summary
	raw_value = value.GetValue()
	return raw_value if raw_value is not None else "<value>"

class TrLocalizationMapSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		self.children = []
		self.child_indices = {}
		valobj = self.valobj.GetNonSyntheticValue()
		keys = valobj.GetChildMemberWithName("m_keys").GetSyntheticValue()
		target = valobj.GetTarget()
		process = target.GetProcess()
		error = lldb.SBError()
		for index in range(keys.GetNumChildren()):
			key = keys.GetChildAtIndex(index)
			key_name = f'[{_display_value(key)}]'
			key_string_address = key.EvaluateExpression("(**this).m_ptr").GetValueAsUnsigned()
			key_string = process.ReadCStringFromMemory(key_string_address, 1024, error)
			self.children.append(target.CreateValueFromExpression(key_name, f"(const char*){key_string_address + len(key_string) + 1}"))
			self.child_indices[key_name] = index
		return True
	
	def has_children(self) -> bool:
		return bool(self.children)

	def num_children(self, max_children: int) -> int:
		return min(len(self.children), max_children)
	
	def get_child_index(self, name: str) -> int:
		return self.child_indices.get(name, -1)

	def get_child_at_index(self, index: int) -> lldb.SBValue | None:
		if 0 <= index < len(self.children):
			return self.children[index]
		return None
	
	def get_value(self) -> lldb.SBValue:
		return self.valobj

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type synthetic add -w tr --python-class localization_map.TrLocalizationMapSyntheticProvider tr::localization_map')