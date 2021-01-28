#pragma once
#include "h2codez.h"
#include "H2Tool.h"

struct lightmap_quality_setting
{
	const wchar_t *name;
	uint32_t secondary_monte_carlo_setting;
	uint32_t main_monte_carlo_setting;
	uint32_t is_draft;
	uint32_t proton_count;
	uint32_t is_direct_only;
	float    unk7;
	uint32_t is_checkboard;
};
CHECK_STRUCT_SIZE(lightmap_quality_setting, 0x20);

// allows distributing lightmapping over multiple computers
extern const s_tool_command lightmaps_slave;

// starts slave and forks once we get to rasterizing
extern const s_tool_command lightmaps_slave_fork;

// merges together the distributed lightmap
extern const s_tool_command lightmaps_master;

// runs multiple lighermappers and then merges the resulting data
extern const s_tool_command lightmaps_local_mp;

// Fix lightmaps extracted using the tag extractor
extern const s_tool_command fix_extraced_lightmap;
