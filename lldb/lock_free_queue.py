# LLDB formatter script for `tr::lock_free_queue`.

import lldb

def _read_atomic(value: lldb.SBValue, target: lldb.SBTarget) -> int:
	base_type = value.GetType().GetTemplateArgumentType(0)
	base_value = target.CreateValueFromAddress(value.GetName(), target.ResolveLoadAddress(value.GetLoadAddress()), base_type)
	return base_value.GetValueAsUnsigned()

def _get_occupied_slots(valobj: lldb.SBValue) -> list[int]:
	valobj = valobj.GetNonSyntheticValue()
	slots = valobj.GetChildMemberWithName("m_slots").GetNonSyntheticValue().GetChildAtIndex(0)
	capacity = slots.GetNumChildren()

	target = valobj.GetTarget()
	read_position = _read_atomic(valobj.GetChildMemberWithName("m_read_position"), target)
	write_position = _read_atomic(valobj.GetChildMemberWithName("m_write_position"), target)
	position_type = valobj.GetChildMemberWithName("m_read_position").GetType().GetTemplateArgumentType(0)
	mask = (1 << (position_type.GetByteSize() * 8)) - 1
	reserved_slot_count = min((write_position - read_position) & mask, capacity)

	occupied_slots = []
	for offset in range(reserved_slot_count):
		position = (read_position + offset) & mask
		slot_index = position & (capacity - 1)
		sequence = _read_atomic(slots.GetChildAtIndex(slot_index).GetChildMemberWithName("sequence"), target)
		if sequence == ((position + 1) & mask):
			occupied_slots.append(slot_index)
	return occupied_slots

def tr_lock_free_queue_summary(valobj: lldb.SBValue, _) -> str:
	return f"size={len(_get_occupied_slots(valobj))}"

class TrLockFreeQueueSyntheticProvider:
	def __init__(self, valobj: lldb.SBValue, _):
		self.valobj = valobj
		self.update()
	
	def update(self) -> bool:
		self.children = []
		valobj = self.valobj.GetNonSyntheticValue()
		element_type = valobj.GetType().GetTemplateArgumentType(0)
		occupied_slots = _get_occupied_slots(valobj)

		target = valobj.GetTarget()
		slots = valobj.GetChildMemberWithName("m_slots").GetNonSyntheticValue().GetChildAtIndex(0)
		for index, slot_index in zip(range(len(occupied_slots)), occupied_slots):
			element_address = slots.GetChildAtIndex(slot_index).GetChildMemberWithName("storage").GetLoadAddress()
			self.children.append(target.CreateValueFromAddress(f"[{index}]", target.ResolveLoadAddress(element_address), element_type))
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
	debugger.HandleCommand('type summary add -w tr --regex --python-function lock_free_queue.tr_lock_free_queue_summary "^tr::lock_free_queue<.+>$"')
	debugger.HandleCommand('type synthetic add -w tr --regex --python-class lock_free_queue.TrLockFreeQueueSyntheticProvider "^tr::lock_free_queue<.+>$"')