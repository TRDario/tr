# LLDB formatter script for graphics objects.

import lldb

def tr_graphics_object_summary(valobj: lldb.SBValue, _):
	valid = valobj.EvaluateExpression("valid()").GetValueAsUnsigned()
	if not valid:
		return f"<invalid>"
	else:
		return f"label={valobj.EvaluateExpression("label()").GetSummary()}"

def __lldb_init_module(debugger, _):
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::compute_shader')
	debugger.HandleCommand('type summary add -w tr --regex --python-function graphics_object.tr_graphics_object_summary "^tr::dynamic_atlas<.+>$"')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::dynamic_index_buffer')
	debugger.HandleCommand('type summary add -w tr --regex --python-function graphics_object.tr_graphics_object_summary "^tr::dynamic_vertex_buffer<.+>$"')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::fragment_shader')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::framebuffer')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::owning_shader_pipeline')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::ping_pong_target')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::shader')
	debugger.HandleCommand('type summary add -w tr --regex --python-function graphics_object.tr_graphics_object_summary "^tr::shader_array<.+>$"')
	debugger.HandleCommand('type summary add -w tr --regex --python-function graphics_object.tr_graphics_object_summary "^tr::shader_buffer<.+>$"')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::shader_pipeline')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::static_index_buffer')
	debugger.HandleCommand('type summary add -w tr --regex --python-function graphics_object.tr_graphics_object_summary "^tr::static_vertex_buffer<.+>$"')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::texture')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::texture_target')
	debugger.HandleCommand('type summary add -w tr --regex --python-function graphics_object.tr_graphics_object_summary "^tr::uniform_buffer<.+>$"')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::untyped_dynamic_vertex_buffer')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::untyped_shader_buffer')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::untyped_static_vertex_buffer')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::untyped_uniform_buffer')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::vertex_format')
	debugger.HandleCommand('type summary add -w tr --python-function graphics_object.tr_graphics_object_summary tr::vertex_shader')