#include "Netvars.h"
#include "../SDK/SDK.h"
#include "../SDK/Misc/RecV.h"

class d_variant;
class recv_table;
class recv_prop;
class c_recv_proxy_data;

using recv_var_proxy_fn = void(*)(const c_recv_proxy_data* data, void* struct_ptr, void* out_ptr);
using array_length_recv_proxy_fn = void(*)(void* struct_ptr, int object_id, int current_array_length);
using data_table_recv_var_proxy_fn = void(*)(const recv_prop* prop, void** out_ptr, void* data_ptr, int object_id);

enum send_prop_type
{
	_int = 0,
	_float,
	_vec,
	_vec_xy,
	_string,
	_array,
	_data_table,
	_int_64,
};
class d_variant
{
public:
	union
	{
		float m_float;
		long m_int;
		char* m_string;
		void* m_data;
		float m_vector[3];
		__int64 m_int64;
	};
	send_prop_type type;
};
class c_recv_proxy_data
{
public:
	const recv_prop* recv_prop;
	d_variant value;
	int element_index;
	int object_id;
};
class recv_prop
{
public:
	char* prop_name;
	send_prop_type prop_type;
	int prop_flags;
	int buffer_size;
	int is_inside_of_array;
	const void* extra_data_ptr;
	recv_prop* array_prop;
	array_length_recv_proxy_fn array_length_proxy;
	recv_var_proxy_fn proxy_fn;
	data_table_recv_var_proxy_fn data_table_proxy_fn;
	recv_table* data_table;
	int offset;
	int element_stride;
	int elements_count;
	const char* parent_array_prop_name;
};
class recv_table
{
public:
	recv_prop* props;
	int props_count;
	void* decoder_ptr;
	char* table_name;
	bool is_initialized;
	bool is_in_main_list;
};

namespace NetvarManager
{
	using netvar_key_value_map = std::unordered_map< uint32_t, uintptr_t >;
	using netvar_table_map = std::unordered_map< uint32_t, netvar_key_value_map >;
	void initialize_props(netvar_table_map& table_map);

	uintptr_t GetNetVar(const uint32_t table,
		const uint32_t prop)
	{
		static netvar_table_map map = {};
		if (map.empty())
			initialize_props(map);

		if (map.find(table) == map.end())
			return 0;

		netvar_key_value_map& table_map = map.at(table);
		if (table_map.find(prop) == table_map.end())
			return 0;

		return table_map.at(prop);
	}

	void AddPropsFromTable(netvar_table_map& table_map, const uint32_t table_name_hash, const std::string& table_name, recv_table* table, const bool dump_vars, std::map< std::string, std::map< uintptr_t, std::string > >& var_dump, const size_t child_offset = 0)
	{
		for (auto i = 0; i < table->props_count; ++i)
		{
			auto& prop = table->props[i];

			if (prop.data_table && prop.elements_count > 0)
			{
				if (std::string(prop.prop_name).substr(0, 1) == std::string("0"))
					continue;

				AddPropsFromTable(table_map, table_name_hash, table_name, prop.data_table, dump_vars, var_dump, prop.offset + child_offset);
			}

			auto name = std::string(prop.prop_name);

			if (name.substr(0, 1) != "m" /*&& name.substr( 0, 1 ) != "b"*/)
				continue;

			const auto name_hash = Fnv::hash(prop.prop_name);
			const auto offset = uintptr_t(prop.offset) + child_offset;

			table_map[table_name_hash][name_hash] = offset;

			if (dump_vars)
				var_dump[table_name][offset] = prop.prop_name;
		}
	}

	void initialize_props(netvar_table_map& table_map)
	{
		const auto dump_vars = true;  //true if netvar dump

		std::map< std::string, std::map< uintptr_t, std::string > > var_dump;
		for (auto client_class = Interfaces::g_CHLClient->GetClientClasses();
			client_class;
			client_class = client_class->m_pNext)
		{
			const auto table = reinterpret_cast<recv_table*>(client_class->m_pRecvTable);
			const auto table_name = table->table_name;
			const auto table_name_hash = Fnv::hash(table_name);

			if (table == nullptr)
				continue;

			AddPropsFromTable(table_map, table_name_hash, table_name, table, dump_vars, var_dump);
		}
	}
}