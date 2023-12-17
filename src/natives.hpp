/*
# natives.hpp

Contains all the `PAWN_NATIVE_DECL` for native function declarations.
*/

#ifndef PROJECTNAME_NATIVES_H
#define PROJECTNAME_NATIVES_H

#include <string>

#include <amx/amx2.h>
#include "common.hpp"
#include "impl.hpp"

namespace Natives {
	cell PCMD_COMMAND(AMX* amx, cell* params);
}

#endif
