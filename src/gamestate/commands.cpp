#include "commands.hpp"
#include "system_state.hpp"
#include "game_scene.hpp"

namespace command {

bool is_console_command(command_type t) {
	return uint8_t(t) == 255;
}

void add_to_command_queue(sys::state& state, command_data& p) {
	assert(command::can_perform_command(state, p));
	bool b = state.incoming_commands.try_push(p);
}




bool can_perform_command(sys::state& state, command_data& c) {
	switch(c.header.type) {
	case command_type::invalid:
		assert(false && "Invalid command received"); // invalid command
		break;
	//case command_type::change_nat_focus:
	//{
	//	auto& data = c.get_payload<command::national_focus_data>();
	//	return can_set_national_focus(state, source, data.target_state, data.focus);
	//}
	}
	return false;
}

bool execute_command(sys::state& state, command_data& c) {
	if(!can_perform_command(state, c))
		return false;
	state.tick_start_counter.fetch_add(1, std::memory_order::seq_cst);
	
	switch(c.header.type) {
	case command_type::invalid:
	{
		assert(false && "Invalid command received"); // invalid command
		break;
	}
	//case command_type::change_nat_focus:
	//{
	//	auto& data = c.get_payload<command::national_focus_data>();
	//	execute_set_national_focus(state, source_nation, data.target_state, data.focus);
	//	break;
	//}
	}
	state.tick_end_counter.fetch_add(1, std::memory_order::seq_cst);
	return true;
}


void execute_pending_commands(sys::state& state) {
	auto* c = state.incoming_commands.front();
	bool command_executed = false;
	while(c) {
		command_executed = true;
		execute_command(state, *c);
		state.incoming_commands.pop();
		c = state.incoming_commands.front();
	}

	if(command_executed) {
		state.game_state_updated.store(true, std::memory_order::release);
	}
}

} // namespace command
