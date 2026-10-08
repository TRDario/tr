# LLDB formatter script for `boost::unordered` containers.

import lldb
import re

def boost_unordered_summary(valobj: lldb.SBValue, _) -> str:
	valobj = valobj.GetNonSyntheticValue()
	size = valobj.GetChildMemberWithName("table_").GetChildMemberWithName("size_ctrl").GetChildMemberWithName("size")
	return f"size={size.GetValueAsUnsigned()}"

def _display_value(value: lldb.SBValue) -> str:
	summary = value.GetSummary()
	if summary is not None:
		return summary
	raw_value = value.GetValue()
	return raw_value if raw_value is not None else "<value>"

class BoostUnorderedSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()

	def update(self) -> bool:
		self.children = []
		self.child_indices = {}
		
		container_match = re.compile(r"^boost::unordered::unordered_(flat|node)_(map|set)<").match(self.valobj.GetTypeName())
		storage_type, container_type = container_match.groups()

		table = self.valobj.GetChildMemberWithName("table_")
		arrays = table.GetChildMemberWithName("arrays");
		size = table.GetChildMemberWithName("size_ctrl").GetChildMemberWithName("size").GetValueAsUnsigned()
		if size == 0:
			return True
		groups_size_mask = arrays.GetChildMemberWithName("groups_size_mask").GetValueAsUnsigned()
		groups = arrays.GetChildMemberWithName("groups_")
		elements = arrays.GetChildMemberWithName("elements_")
		group_address = groups.GetValueAsUnsigned()
		group_type = groups.GetType().GetPointeeType()
		group_size = group_type.GetByteSize()
		elements_address = elements.GetValueAsUnsigned()
		element_type = elements.GetType().GetPointeeType()
		element_size = element_type.GetByteSize()

		target = self.valobj.GetTarget()
		process = target.GetProcess()
		index = 0
		for group_index in range(groups_size_mask + 1):
			if index == size:
				return True
			
			error = lldb.SBError()
			control_bytes = process.ReadMemory(group_address + group_index * group_size, 15, error)
			if error.Fail():
				break

			for slot_index, control in enumerate(control_bytes):
				if index == size:
					return True
				if control == 0:
					continue
				
				element_address = elements_address + (group_index * 15 + slot_index) * element_size
				name = f"[{index}]"
				element = target.CreateValueFromAddress(name, target.ResolveLoadAddress(element_address), element_type)

				if storage_type == "node":
					node_pointer = element.GetChildMemberWithName("p")
					element_address = node_pointer.GetValueAsUnsigned()
					if not element_address:
						continue
					element = target.CreateValueFromAddress(name, target.ResolveLoadAddress(element_address), node_pointer.GetType().GetPointeeType())
				
				if container_type == "map":
					name = f"[{_display_value(element.GetChildMemberWithName("first"))}]"
					element = element.GetChildMemberWithName("second").Clone(name)

				self.children.append(element)
				self.child_indices[name] = index
				index += 1
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
	for container in ("unordered_flat_map", "unordered_flat_set", "unordered_node_map", "unordered_node_set"):
		debugger.HandleCommand(f'type summary add -w tr --expand --regex --python-function boost_unordered.boost_unordered_summary "^boost::unordered::{container}<.+>$"')
		debugger.HandleCommand(f'type synthetic add -w tr --regex --python-class boost_unordered.BoostUnorderedSyntheticProvider "^boost::unordered::{container}<.+>$"')